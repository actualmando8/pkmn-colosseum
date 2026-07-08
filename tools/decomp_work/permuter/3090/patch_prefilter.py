#!/usr/bin/env python3
"""patch_prefilter.py <permuter_root> — add the env-gated Phase-3 pre-filter
hook to a decomp-permuter clone (idempotent). Assumes src/prefilter.py already
copied in. Edits src/permuter.py only.
"""
import os
import sys

MARK = "prefilter-instrumentation"


def patch(path, anchor, insert, name):
    src = open(path, encoding="utf-8").read()
    if insert.strip() in src:
        print(f"  {name}: already applied")
        return
    if anchor not in src:
        sys.exit(f"FATAL: anchor not found for {name}")
    src = src.replace(anchor, anchor + insert, 1)
    open(path, "w", encoding="utf-8").write(src)
    print(f"  {name}: applied")


def main():
    perm = os.path.join(sys.argv[1], "src", "permuter.py")

    # 1. construct prefilter right after the trainlog logger in __init__
    patch(
        perm,
        "        self._trainlog = TrainLogger.create(\n"
        "            self.fn_name, self.base_score, self.base_source\n"
        "        )\n",
        f"        # {MARK}: env-gated Phase-3 compile pre-filter (None when off)\n"
        "        from .prefilter import PreFilter\n"
        "        self._prefilter = PreFilter.create(\n"
        "            self.fn_name, self.base_score, self.base_source\n"
        "        )\n",
        "permuter.py __init__ prefilter",
    )

    # 2. decision branch: featurize+predict BEFORE compile; skip compile on reject
    patch(
        perm,
        "        old_score = self._score_for_source.get(source_hash)\n"
        "        if old_score is not None:\n"
        "            result = CandidateResult(score=old_score, hash=None, source=cand_source)\n"
        "        else:\n",
        "",  # placeholder; real replace below
        "noop",
    ) if False else None

    src = open(perm, encoding="utf-8").read()
    if f"{MARK}: decision" not in src:
        old = (
            "        old_score = self._score_for_source.get(source_hash)\n"
            "        if old_score is not None:\n"
            "            result = CandidateResult(score=old_score, hash=None, source=cand_source)\n"
            "        else:\n"
        )
        new = (
            "        old_score = self._score_for_source.get(source_hash)\n"
            f"        _pf = self._prefilter  # {MARK}: decision\n"
            "        _pf_skip = False\n"
            "        if old_score is None and _pf is not None:\n"
            "            _pf_skip = (\n"
            "                _pf.pre(\n"
            "                    cand_source,\n"
            "                    getattr(self._cur_cand.randomizer, \"applied_methods\", None),\n"
            "                )\n"
            "                == \"skip\"\n"
            "            )\n"
            "        if old_score is not None:\n"
            "            result = CandidateResult(score=old_score, hash=None, source=cand_source)\n"
            "        elif _pf_skip:\n"
            "            # prefilter rejected: skip compile, synthesize a non-improving result\n"
            "            result = CandidateResult(score=self.base_score, hash=None, source=cand_source)\n"
            "        else:\n"
        )
        if old not in src:
            sys.exit("FATAL: decision anchor not found")
        src = src.replace(old, new, 1)
        open(perm, "w", encoding="utf-8").write(src)
        print("  permuter.py decision branch: applied")
    else:
        print("  permuter.py decision branch: already applied")

    # 3. after scoring a compiled candidate, notify the prefilter of the outcome
    patch(
        perm,
        "            result = self._cur_cand.score(self.scorer, o_file)\n"
        "            profiler.add_stat(Profiler.StatType.score, timer.tick())\n",
        f"            if _pf is not None:  # {MARK}: record compiled outcome\n"
        "                _pf.post(result.score)\n",
        "permuter.py post-score hook",
    )
    print("prefilter patch complete")


if __name__ == "__main__":
    main()
