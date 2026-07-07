"""trainlog.py — per-candidate JSONL training-data logger for decomp-permuter.

Deployed into the permuter clone as src/trainlog.py (Phase 1 of the learned
compiler-surrogate project). Every FRESHLY-COMPILED candidate emits one record;
cache-hit re-encounters of an identical source are skipped (duplicate rows).

Activation: entirely env-driven; without PERM_TRAINLOG_DIR set the permuter
behaves exactly as upstream (logger not constructed).
  PERM_TRAINLOG_DIR     root dir; records go to <dir>/<fn>/<worker>.jsonl
  PERM_TRAINLOG_WORKER  worker tag (default: pid<pid>)
  PERM_TRAINLOG_SAMPLE  log every Nth candidate (default 1 = all)

Record schema (one JSON object per line):
  fn         target function name
  ts         unix epoch (int, seconds)
  iter       1-based fresh-compile counter within this permuter run
  score      permuter scorer output (0 = byte-exact; 1e9 = compile fail)
  base_score score of the unmutated base source for this run
  seed       [perm_seed, rng_seed] — with base.c + weights this reproduces
             the candidate deterministically
  passes     cumulative randomization-pass names applied to this candidate
             lineage (last 64; null if unavailable)
  win_start  0-based line offset in the canonicalized base source where the
             diff window begins (common prefix/suffix trimmed before diffing)
  diff       unified diff (n=2 context) of candidate vs base source, both in
             pycparser-reprint canonical form, within the trimmed window

Performance: buffered writes (flush every 200 records / 256 KiB), no fsync;
common prefix/suffix trimming keeps difflib cost O(changed region), so
whole-TU work dirs don't pay a full-file diff per iteration. Rotation at
64 MiB with background gzip.
"""
import atexit
import difflib
import json
import os
import signal
import subprocess
import sys
import time
from typing import List, Optional, Sequence, Tuple


class TrainLogger:
    MAX_FILE_BYTES = 64 * 1024 * 1024
    FLUSH_RECORDS = 50
    FLUSH_BYTES = 256 * 1024

    @staticmethod
    def create(fn_name: str, base_score: int, base_source: str) -> Optional["TrainLogger"]:
        root = os.environ.get("PERM_TRAINLOG_DIR")
        if not root:
            return None
        try:
            return TrainLogger(root, fn_name, base_score, base_source)
        except OSError:
            return None

    def __init__(self, root: str, fn_name: str, base_score: int, base_source: str) -> None:
        self.fn = fn_name
        self.base_score = base_score
        self.base_lines = base_source.split("\n")
        self.sample = max(1, int(os.environ.get("PERM_TRAINLOG_SAMPLE", "1")))
        worker = os.environ.get("PERM_TRAINLOG_WORKER", f"pid{os.getpid()}")
        d = os.path.join(root, fn_name)
        os.makedirs(d, exist_ok=True)
        self.path = os.path.join(d, f"{worker}.jsonl")
        self._fh = open(self.path, "a", encoding="utf-8")
        self._written = self._fh.tell()
        self._buf: List[str] = []
        self._buf_bytes = 0
        self._iter = 0
        # Don't lose the tail buffer: flush on normal exit AND on the SIGTERM
        # that `timeout` sends at budget expiry (python's default SIGTERM
        # action skips atexit). Chain nothing — upstream installs no handler.
        atexit.register(self.close)
        try:
            signal.signal(signal.SIGTERM, self._on_sigterm)
        except (ValueError, OSError):
            pass  # non-main thread or unsupported: atexit still covers exits

    def _on_sigterm(self, signum, frame) -> None:
        self.close()
        sys.exit(143)

    # ---- diffing ------------------------------------------------------------
    def _window_diff(self, cand_lines: Sequence[str]) -> Tuple[int, str]:
        a, b = self.base_lines, cand_lines
        na, nb = len(a), len(b)
        lo = 0
        hi_max = min(na, nb)
        while lo < hi_max and a[lo] == b[lo]:
            lo += 1
        ta, tb = na, nb
        while ta > lo and tb > lo and a[ta - 1] == b[tb - 1]:
            ta -= 1
            tb -= 1
        if lo == ta and lo == tb:
            return lo, ""  # identical
        # small context margin so unified_diff hunks stay well-formed
        s = max(0, lo - 2)
        diff = difflib.unified_diff(
            a[s:ta + 2 if ta + 2 <= na else na],
            b[s:tb + 2 if tb + 2 <= nb else nb],
            n=2, lineterm="",
        )
        return s, "\n".join(diff)

    # ---- logging ------------------------------------------------------------
    def log(
        self,
        cand_source: str,
        score: int,
        seed: Optional[Tuple[int, int]],
        passes: Optional[Sequence[str]],
    ) -> None:
        self._iter += 1
        if self._iter % self.sample:
            return
        win_start, diff = self._window_diff(cand_source.split("\n"))
        rec = {
            "fn": self.fn,
            "ts": int(time.time()),
            "iter": self._iter,
            "score": score,
            "base_score": self.base_score,
            "seed": list(seed) if seed else None,
            "passes": list(passes)[-64:] if passes else None,
            "win_start": win_start,
            "diff": diff,
        }
        line = json.dumps(rec, separators=(",", ":"))
        self._buf.append(line)
        self._buf_bytes += len(line) + 1
        if len(self._buf) >= self.FLUSH_RECORDS or self._buf_bytes >= self.FLUSH_BYTES:
            self.flush()

    def flush(self) -> None:
        if not self._buf:
            return
        data = "\n".join(self._buf) + "\n"
        self._buf = []
        self._buf_bytes = 0
        try:
            self._fh.write(data)
            self._written += len(data)
        except OSError:
            return
        if self._written >= self.MAX_FILE_BYTES:
            self._rotate()

    def _rotate(self) -> None:
        try:
            self._fh.close()
            rotated = f"{self.path}.{int(time.time())}"
            os.rename(self.path, rotated)
            subprocess.Popen(
                ["gzip", "-f", rotated],
                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
            )
            self._fh = open(self.path, "a", encoding="utf-8")
            self._written = 0
        except OSError:
            pass

    def close(self) -> None:
        try:
            self.flush()
            self._fh.close()
        except OSError:
            pass
