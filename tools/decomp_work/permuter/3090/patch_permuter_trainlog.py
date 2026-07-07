#!/usr/bin/env python3
"""patch_permuter_trainlog.py — apply the trainlog instrumentation to a
decomp-permuter clone (idempotent; safe to re-run after upstream pulls).

usage: patch_permuter_trainlog.py /path/to/decomp-permuter

Edits:
  src/permuter.py   : import TrainLogger; construct in __init__; emit one
                      record per freshly-compiled candidate in _eval_candidate.
  src/randomizer.py : record applied randomization-pass names (mutation-class
                      labels) on the Randomizer as `applied_methods`.
trainlog.py itself must be copied to src/trainlog.py separately (deploy step).
"""
import os
import re
import sys

MARK = "trainlog-instrumentation"


def patch(path, anchor, insert, name):
    src = open(path, encoding="utf-8").read()
    if MARK in src and insert.strip() in src:
        print(f"  {name}: already applied")
        return
    if anchor not in src:
        sys.exit(f"FATAL: anchor not found for {name} in {path}")
    src = src.replace(anchor, anchor + insert, 1)
    open(path, "w", encoding="utf-8").write(src)
    print(f"  {name}: applied")


def main():
    root = sys.argv[1]
    perm = os.path.join(root, "src", "permuter.py")
    rand = os.path.join(root, "src", "randomizer.py")

    # 1. import
    patch(
        perm,
        "import hashlib\n",
        f"from .trainlog import TrainLogger  # {MARK}\n",
        "permuter.py import",
    )

    # 2. construct logger after base scoring in __init__
    patch(
        perm,
        "        self.best_score = self.base_score\n",
        f"        # {MARK}: env-gated per-candidate JSONL logger (None when off)\n"
        "        self._trainlog = TrainLogger.create(\n"
        "            self.fn_name, self.base_score, self.base_source\n"
        "        )\n",
        "permuter.py __init__ hook",
    )

    # 3. emit record for freshly compiled candidates in _eval_candidate
    patch(
        perm,
        "            if len(self._score_for_source) < 100000:  # prevent unbounded memory usage\n"
        "                self._score_for_source[source_hash] = result.score\n",
        f"\n            # {MARK}: log fresh compiles only (cache hits = duplicate rows)\n"
        "            if self._trainlog is not None:\n"
        "                self._trainlog.log(\n"
        "                    cand_source,\n"
        "                    result.score,\n"
        "                    self._cur_seed,\n"
        "                    getattr(self._cur_cand.randomizer, \"applied_methods\", None),\n"
        "                )\n",
        "permuter.py _eval_candidate hook",
    )

    # 4. randomizer: track applied pass names
    patch(
        rand,
        "        self.methods = [\n"
        "            (method, randomization_weights[method.__name__])\n"
        "            for method in RANDOMIZATION_PASSES\n"
        "        ]\n",
        f"        self.applied_methods = []  # {MARK}: mutation-class labels\n",
        "randomizer.py __init__ hook",
    )
    # insert BETWEEN the successful method(...) call and the break
    patch(
        rand,
        "            try:\n"
        "                method(fn, ast, indices, region, self.random)\n",
        f"                # {MARK}\n"
        "                self.applied_methods.append(method.__name__)\n"
        "                if len(self.applied_methods) > 64:\n"
        "                    del self.applied_methods[:-64]\n",
        "randomizer.py randomize hook",
    )
    print("patch complete")


if __name__ == "__main__":
    main()
