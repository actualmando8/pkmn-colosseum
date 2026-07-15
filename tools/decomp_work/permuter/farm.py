#!/usr/bin/env python3
"""farm.py — Windows-native decomp-permuter farm supervisor.

Runs N independent SINGLE-THREADED decomp-permuter processes, one per work
unit.  This deliberately avoids decomp-permuter's -j flag: its multiprocessing
is fork-only and does not work on native Windows (the reason the previous farm
was pushed into WSL, where wibo/9p/VM overhead capped it at ~8 effective
workers).  One process per unit needs no fork and scales to physical cores.

Layout (all under FARM_ROOT, default = directory containing this script):
  decomp-permuter/   patched permuter checkout
  units/<fn>/        work units (base.c, target.o, compile.bat, settings.toml)
  units/manifest.json  generation manifest (queue order preserved)
  state/state.json   progress, survives reboot
  state/status.json  heartbeat for the Mac-side poller
  outbox/<fn>/       WINS: <fn>.c + summary.json  (pulled by the Mac)
  outbox/nearwins/   near-miss candidates worth human review
  logs/<fn>.log      per-unit permuter output

Wins are detected two ways: the permuter exits after --stop-on-zero writing
output-0-*/, or a nonzero personal best lands below --nearwin-threshold.

The unit manifest is reloaded between worker assignments. This lets the Mac
prepend reviewed near-miss work units atomically without stopping active
permuters or waiting for a full farm restart.

The supervisor holds the machine awake via SetThreadExecutionState while at
least one worker is running (no admin needed); sleep resumes when it stops.

Usage:  py -3 farm.py [--workers 12] [--budget 5400] [--rounds 3]
        py -3 farm.py --status        (print status.json)
"""

import argparse
import ctypes
import json
import os
import re
import subprocess
import sys
import time
from pathlib import Path

FARM_ROOT = Path(__file__).resolve().parent
PERMUTER = FARM_ROOT / "decomp-permuter" / "permuter.py"
UNITS = FARM_ROOT / "units"
STATE_DIR = FARM_ROOT / "state"
OUTBOX = FARM_ROOT / "outbox"
LOGS = FARM_ROOT / "logs"

ES_CONTINUOUS = 0x80000000
ES_SYSTEM_REQUIRED = 0x00000001

RE_BASE_SCORE = re.compile(r"base score = (\d+)")
RE_NEW_BEST = re.compile(r"found new best score! \((\d+) vs (\d+)\)")


def keep_awake(active: bool) -> None:
    if os.name != "nt":
        return
    flags = ES_CONTINUOUS | (ES_SYSTEM_REQUIRED if active else 0)
    ctypes.windll.kernel32.SetThreadExecutionState(flags)


def load_json(path: Path, default):
    try:
        return json.loads(path.read_text())
    except (OSError, ValueError):
        return default


def atomic_write(path: Path, data: str) -> None:
    tmp = path.with_suffix(".tmp")
    tmp.write_text(data)
    os.replace(tmp, path)


def load_manifest_queue(units: Path):
    manifest = load_json(units / "manifest.json", [])
    metas = {m["fn"]: m for m in manifest if m.get("status") == "ok"}
    queue_order = [
        m["fn"] for m in manifest
        if m.get("status") == "ok"
        and (units / m["fn"] / "base.c").is_file()
    ]
    return metas, queue_order


def prune_stale_results(state: dict, queue_order: list[str]) -> list[str]:
    """Drop persisted results for work units no longer in the manifest."""
    admitted = set(queue_order)
    stale = sorted(set(state["done"]) - admitted)
    for fn in stale:
        del state["done"][fn]
    return stale


class Worker:
    def __init__(self, fn: str, unit_dir: Path, budget: float):
        self.fn = fn
        self.unit_dir = unit_dir
        self.budget = budget
        self.started = time.time()
        self.base_score = None
        self.best_score = None
        self.log_path = LOGS / f"{fn}.log"
        self.log_start = self.log_path.stat().st_size if self.log_path.exists() else 0
        self.log = open(self.log_path, "a", encoding="utf-8", errors="replace")
        self.log.write(f"\n===== farm start {time.strftime('%Y-%m-%dT%H:%M:%S')} "
                       f"budget={budget}s =====\n")
        self.log.flush()
        flags = 0
        if os.name == "nt":
            flags = subprocess.CREATE_NEW_PROCESS_GROUP | 0x08000000  # NO_WINDOW
        # Redirect the worker's temp dir into the farm tree: the permuter
        # writes every candidate .c/.o through tempfile, and %TEMP% is not
        # covered by the Defender exclusion — real-time scanning of those
        # files is exactly the compile-loop tax that capped the old farm.
        env = dict(os.environ)
        tmp = FARM_ROOT / "tmp" / fn
        tmp.mkdir(parents=True, exist_ok=True)
        for var in ("TEMP", "TMP", "TMPDIR"):
            env[var] = str(tmp)
        self.tmp_dir = tmp
        self.proc = subprocess.Popen(
            [sys.executable, str(PERMUTER), str(unit_dir),
             "--best-only", "--stop-on-zero", "--stack-diffs"],
            stdout=self.log, stderr=subprocess.STDOUT,
            cwd=str(unit_dir), creationflags=flags, env=env)

    def poll(self):
        return self.proc.poll()

    def expired(self) -> bool:
        return time.time() - self.started > self.budget

    def stop(self) -> None:
        if self.proc.poll() is None:
            self.proc.terminate()
            try:
                self.proc.wait(timeout=15)
            except subprocess.TimeoutExpired:
                self.proc.kill()
        self.log.close()
        # clear leftover candidate temp files
        import shutil
        shutil.rmtree(self.tmp_dir, ignore_errors=True)

    def read_scores(self) -> None:
        try:
            with self.log_path.open("rb") as stream:
                stream.seek(self.log_start)
                text = stream.read().decode("utf-8", errors="replace")
        except OSError:
            return
        m = RE_BASE_SCORE.search(text)
        if m:
            self.base_score = int(m.group(1))
            if self.best_score is None:
                self.best_score = self.base_score
        for m in RE_NEW_BEST.finditer(text):
            s = int(m.group(1))
            if self.best_score is None or s < self.best_score:
                self.best_score = s


def harvest(w: Worker, meta: dict, nearwin_threshold: int):
    """Collect output-*/source.c candidates. Returns (won, best_score)."""
    w.read_scores()
    outputs = []
    for d in sorted(w.unit_dir.glob("output-*")):
        m = re.match(r"output-(\d+)-\d+", d.name)
        if m and (d / "source.c").is_file():
            outputs.append((int(m.group(1)), d))
    outputs.sort(key=lambda t: t[0])
    won = False
    best = w.best_score
    if outputs:
        score, best_dir = outputs[0]
        best = min(score, best if best is not None else score)
        dest = OUTBOX / w.fn if score == 0 else OUTBOX / "nearwins" / f"{w.fn}_score{score}"
        if score == 0 or score <= nearwin_threshold:
            dest.mkdir(parents=True, exist_ok=True)
            src_c = (best_dir / "source.c").read_text(encoding="utf-8", errors="replace")
            (dest / f"{w.fn}.c").write_text(src_c, encoding="utf-8")
            summary = {
                "fn": w.fn,
                "unit": meta.get("unit"),
                "addr": meta.get("addr"),
                "pct": meta.get("pct"),
                "size": meta.get("size"),
                "mw_version": meta.get("mw_version"),
                "cflags": meta.get("cflags"),
                "base_score": w.base_score,
                "score": score,
                "diff": f"permuter difflib score {score} (0 = asm-exact vs dtk target.o, "
                        f"--stack-diffs strict)",
                "won": score == 0,
                "ts": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
                "machine": "windows-native-farm",
            }
            (dest / "summary.json").write_text(json.dumps(summary, indent=1))
        won = score == 0
    return won, best


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--workers", type=int, default=12)
    ap.add_argument("--budget", type=float, default=5400,
                    help="seconds per unit per round")
    ap.add_argument("--rounds", type=int, default=99,
                    help="passes over the unfinished queue")
    ap.add_argument("--nearwin-threshold", type=int, default=25)
    ap.add_argument("--status", action="store_true")
    args = ap.parse_args()

    STATE_DIR.mkdir(exist_ok=True)
    OUTBOX.mkdir(exist_ok=True)
    (OUTBOX / "nearwins").mkdir(exist_ok=True)
    LOGS.mkdir(exist_ok=True)

    status_path = STATE_DIR / "status.json"
    state_path = STATE_DIR / "state.json"

    if args.status:
        print(json.dumps(load_json(status_path, {}), indent=1))
        return

    # single-instance lock (best effort)
    lock = STATE_DIR / "farm.pid"
    if lock.exists():
        try:
            oldpid = int(lock.read_text())
            if os.name == "nt":
                out = subprocess.run(
                    ["tasklist", "/fi", f"PID eq {oldpid}", "/fo", "csv", "/nh"],
                    capture_output=True, text=True).stdout
                if str(oldpid) in out and "python" in out.lower():
                    print(f"farm already running (pid {oldpid}); exiting")
                    return
        except (ValueError, OSError):
            pass
    lock.write_text(str(os.getpid()))

    metas, queue_order = load_manifest_queue(UNITS)

    state = load_json(state_path, {"done": {}, "round": 0})
    pruned = prune_stale_results(state, queue_order)
    if pruned:
        atomic_write(state_path, json.dumps(state, indent=1))
        print(f"startup: pruned {len(pruned)} stale results")

    def runnable(rnd):
        for fn in queue_order:
            d = state["done"].get(fn, {})
            if d.get("won"):
                continue
            if d.get("bad"):
                continue
            if d.get("rounds", 0) > rnd:
                continue
            yield fn

    workers = {}
    round_no = state.get("round", 0)
    print(f"farm: {len(queue_order)} units, {args.workers} workers, "
          f"budget {args.budget}s, starting round {round_no}")

    try:
        while round_no < args.rounds:
            # The injector atomically replaces manifest.json after copying the
            # complete unit directory. Preserve metadata for active workers,
            # then adopt the latest queue order before selecting free work.
            loaded_metas, loaded_order = load_manifest_queue(UNITS)
            metas.update(loaded_metas)
            if loaded_order != queue_order:
                queue_order = loaded_order
                pruned = prune_stale_results(state, queue_order)
                if pruned:
                    atomic_write(state_path, json.dumps(state, indent=1))
                print(f"manifest reload: {len(queue_order)} units, "
                      f"pruned {len(pruned)} stale results")

            pending = [fn for fn in runnable(round_no) if fn not in workers]
            if not pending and not workers:
                round_no += 1
                state["round"] = round_no
                remaining = [fn for fn in runnable(round_no)]
                if not remaining:
                    print("queue exhausted — all units won or bad")
                    break
                continue

            # top-up workers
            while pending and len(workers) < args.workers:
                fn = pending.pop(0)
                try:
                    workers[fn] = Worker(fn, UNITS / fn, args.budget)
                    print(f"start {fn}")
                except OSError as e:
                    state["done"].setdefault(fn, {})["bad"] = f"spawn: {e}"

            time.sleep(20)
            keep_awake(bool(workers))

            for fn in list(workers):
                w = workers[fn]
                rc = w.poll()
                if rc is None and not w.expired():
                    continue
                w.stop()
                won, best = harvest(w, metas.get(fn, {}), args.nearwin_threshold)
                d = state["done"].setdefault(fn, {})
                d["rounds"] = d.get("rounds", 0) + 1
                d["best"] = best if best is not None else d.get("best")
                d["base"] = w.base_score
                if won:
                    d["won"] = True
                    print(f"WIN  {fn}")
                elif rc not in (None, 0) and w.base_score is None:
                    d["bad"] = f"permuter exited rc={rc} before base score"
                    print(f"BAD  {fn} rc={rc}")
                else:
                    print(f"stop {fn} best={best}")
                del workers[fn]
                atomic_write(state_path, json.dumps(state, indent=1))

            wins = sum(1 for v in state["done"].values() if v.get("won"))
            bad = sum(1 for v in state["done"].values() if v.get("bad"))
            status = {
                "alive": True,
                "ts": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
                "pid": os.getpid(),
                "workers": len(workers),
                "target_workers": args.workers,
                "round": round_no,
                "units": len(queue_order),
                "wins": wins,
                "bad": bad,
                "active": {fn: {"base": w.base_score, "best": w.best_score,
                                "elapsed": int(time.time() - w.started)}
                           for fn, w in workers.items()},
            }
            for w in workers.values():
                w.read_scores()
            atomic_write(status_path, json.dumps(status, indent=1))
    finally:
        for w in workers.values():
            w.stop()
        keep_awake(False)
        st = load_json(status_path, {})
        st["alive"] = False
        atomic_write(status_path, json.dumps(st, indent=1))
        try:
            lock.unlink()
        except OSError:
            pass


if __name__ == "__main__":
    main()
