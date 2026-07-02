#!/usr/bin/env python3
"""harvest.py — score & harvest a finished permuter run for one function.

usage: harvest.py <fn> <unit> [worker]

Reads $FARM/dirs/<fn>/ (permuter output-<score>-* dirs + logs/run_<fn>.log).
On a score-0 candidate:
  - recompiles it with the unit's exact compile.sh
  - re-scores with objdiff-cli (independent scorer; must report 100% match)
  - writes $FARM/results/<fn>/{source.c, diff.txt, summary.json}
Non-zero best candidates are stashed under results/_partials/<fn>/ so partial
progress is preserved. Terminal state written to $FARM/state/<fn>.status.
"""
import glob
import json
import os
import re
import subprocess
import sys
import time

BASE = os.environ.get("FARM_BASE", "/storage/finetune/pkmn-colosseum-2026")
FARM = os.path.join(BASE, "farm")
OBJDIFF = os.path.join(BASE, "tools", "objdiff-cli")


def objdiff_pct(target_o, cand_o, fn):
    """Return objdiff match percent for fn, or None."""
    try:
        r = subprocess.run(
            [OBJDIFF, "diff", "-1", target_o, "-2", cand_o, "-o", "-",
             "--format", "json", fn],
            capture_output=True, text=True, timeout=120)
        data = json.loads(r.stdout)
    except Exception:
        return None

    # walk the report for the symbol's match_percent
    def walk(o):
        if isinstance(o, dict):
            name = o.get("name") or o.get("symbol_name") or o.get("demangled_name")
            if name == fn:
                for k in ("match_percent", "matchPercent", "fuzzy_match_percent"):
                    if k in o:
                        return float(o[k])
            for v in o.values():
                got = walk(v)
                if got is not None:
                    return got
        elif isinstance(o, list):
            for v in o:
                got = walk(v)
                if got is not None:
                    return got
        return None

    return walk(data)


def main():
    fn, unit = sys.argv[1], sys.argv[2]
    worker = sys.argv[3] if len(sys.argv) > 3 else "?"
    d = os.path.join(FARM, "dirs", fn)
    runlog = os.path.join(FARM, "logs", f"run_{fn}.log")
    state = os.path.join(FARM, "state", fn + ".status")
    os.makedirs(os.path.join(FARM, "state"), exist_ok=True)
    log = ""
    if os.path.exists(runlog):
        log = open(runlog, errors="replace").read()
    mbase = re.search(r"base score = (-?\d+)", log)
    base_score = int(mbase.group(1)) if mbase else None

    outs = []
    for od in glob.glob(os.path.join(d, "output-*")):
        m = re.match(r"output-(-?\d+)", os.path.basename(od))
        sp = os.path.join(od, "source.c")
        if m and os.path.exists(sp):
            outs.append((int(m.group(1)), sp))
    outs.sort()
    best_score, best_src = outs[0] if outs else (None, None)
    if best_score is None and base_score == 0:
        best_score, best_src = 0, os.path.join(d, "base.c")

    if best_score == 0 and best_src:
        # independent verification: recompile + objdiff score
        cand_o = os.path.join(d, "cand_verify.o")
        r = subprocess.run(["bash", os.path.join(d, "compile.sh"),
                            best_src, "-o", cand_o], capture_output=True, text=True)
        pct = None
        if r.returncode == 0 and os.path.exists(cand_o):
            pct = objdiff_pct(os.path.join(d, "target.o"), cand_o, fn)
        rd = os.path.join(FARM, "results", fn)
        os.makedirs(rd, exist_ok=True)
        subprocess.run(["cp", "-f", best_src, os.path.join(rd, "source.c")])
        diff = subprocess.run(["diff", "-u", os.path.join(d, "base.c"), best_src],
                              capture_output=True, text=True).stdout
        open(os.path.join(rd, "diff.txt"), "w").write(diff)
        summary = {
            "fn": fn, "unit": unit, "worker": worker,
            "permuter_score": 0, "base_score": base_score,
            "achieved_pct": pct,
            "objdiff_confirmed": pct is not None and pct >= 100.0,
            "recompile_ok": r.returncode == 0,
            "created": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
            "source_diff": diff[:20000],
        }
        with open(os.path.join(rd, "summary.json"), "w") as f:
            json.dump(summary, f, indent=1)
        status = "WIN" if summary["objdiff_confirmed"] else "WIN_UNCONFIRMED"
        with open(state, "w") as f:
            f.write(f"{status} {worker} {int(time.time())} pct={pct}\n")
        print(f"{status} {fn} pct={pct}")
    else:
        if best_src:
            pd = os.path.join(FARM, "results", "_partials", fn)
            os.makedirs(pd, exist_ok=True)
            subprocess.run(["cp", "-f", best_src, os.path.join(pd, "best.c")])
            with open(os.path.join(pd, "info.json"), "w") as f:
                json.dump({"fn": fn, "unit": unit, "best_score": best_score,
                           "base_score": base_score}, f)
        with open(state, "w") as f:
            f.write(f"NOWIN {worker} {int(time.time())} best={best_score} base={base_score}\n")
        print(f"NOWIN {fn} best={best_score} base={base_score}")


if __name__ == "__main__":
    main()
