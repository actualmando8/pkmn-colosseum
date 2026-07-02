#!/usr/bin/env python3
"""Query the asm-similarity index for the most similar functions.

Usage:
  python3 tools/decomp_work/simindex/query.py <addr-or-name> [-k 5]
         [--corpus colo|xd|ext|all] [--matched-only]

--matched-only restricts results to functions known to have byte-exact C:
Colosseum functions with fuzzy_match_percent == 100, or any 'ext' corpus
function (external-repo matched fns are byte-exact by construction).
"""

import argparse
import difflib
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import simindex_lib as lib


def top_similar(index, qid, k=5, corpus="all", matched_only=False,
                n_candidates=400):
    fns = index["functions"]
    q = fns[qid]
    agree = lib.sig_agreement(index, qid)
    order = np.argsort(-agree)
    results = []
    checked = 0
    for fid in order:
        fid = int(fid)
        if fid == qid:
            continue
        f = fns[fid]
        if corpus != "all" and f["corpus"] != corpus:
            continue
        if matched_only and not (
                f["corpus"] == "ext"
                or (f["corpus"] == "colo" and (f["fuzzy"] or 0) >= 100.0)):
            continue
        checked += 1
        if checked > n_candidates:
            break
        ratio = difflib.SequenceMatcher(None, q["norm"], f["norm"]).ratio()
        size_prior = (min(q["n_insns"], f["n_insns"])
                      / max(q["n_insns"], f["n_insns"])) ** 0.25
        results.append((ratio * size_prior, ratio, agree[fid], f))
    results.sort(key=lambda r: -r[0])
    return results[:k]


def fmt(f):
    fuzzy = "%.1f%%" % f["fuzzy"] if f["fuzzy"] is not None else "-"
    src = f["src_path"] or ""
    label = f["corpus"]
    if f["corpus"] == "ext" and f.get("game"):
        label = "ext:" + f["game"]
        src = "%s [%s]" % (src, f["unit"] or "")
    addr = f["addr"] if f["addr"] is not None else 0
    return "%-14s 0x%08X %-50s sz=%-5d match=%-7s %s" % (
        label, addr, f["name"], f["size"], fuzzy, src)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("query", help="address (0x...) or function name")
    ap.add_argument("-k", type=int, default=5)
    ap.add_argument("--corpus", choices=["colo", "xd", "ext", "all"], default="all")
    ap.add_argument("--matched-only", action="store_true")
    ap.add_argument("--index", default=lib.INDEX_PATH)
    args = ap.parse_args()

    index = lib.load_index(args.index)
    qid = lib.resolve(index, args.query)
    if qid is None:
        sys.exit("not found in index: %s" % args.query)
    q = index["functions"][qid]
    print("query: " + fmt(q))
    print("-" * 110)
    for score, ratio, jacc, f in top_similar(
            index, qid, k=args.k, corpus=args.corpus,
            matched_only=args.matched_only):
        print("score=%.3f seq=%.3f mh=%.2f  %s" % (score, ratio, jacc, fmt(f)))


if __name__ == "__main__":
    main()
