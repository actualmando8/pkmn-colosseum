#!/usr/bin/env python3
"""Cluster the Colosseum pool into high-similarity template families.

Usage:
  python3 tools/decomp_work/simindex/families.py [--min-size 3]
         [--unmatched-only] [--min-insns 4] [--threshold 0.5] [--top 30]

Families are ranked by sweep payoff = member_count * avg_fn_size.
With --unmatched-only, only functions with fuzzy_match_percent < 100
are clustered (the actionable "solve one, sweep N" pool).
"""

import argparse
import os
import sys
from collections import defaultdict

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import simindex_lib as lib


class DSU:
    def __init__(self, n):
        self.p = list(range(n))

    def find(self, x):
        while self.p[x] != x:
            self.p[x] = self.p[self.p[x]]
            x = self.p[x]
        return x

    def union(self, a, b):
        ra, rb = self.find(a), self.find(b)
        if ra != rb:
            self.p[rb] = ra


def build_families(index, pool_ids, threshold=0.45, rows=2, max_bucket=64):
    """Cluster via representative-verified union-find.

    Candidate pairs come from short (2-row) LSH bands for recall. A pair of
    clusters is merged only if their *representatives* pass verification
    (SequenceMatcher on full normalized sequences x size-ratio prior), which
    keeps every family cohesive around one shape and prevents transitive
    drift into mega-clusters. Template families are similar, not identical:
    WazaHit-style members sit at seq 0.5-0.8, hence the 0.45 default."""
    import difflib

    fns = index["functions"]
    sigs = index["sig_matrix"]
    id2local = {fid: i for i, fid in enumerate(pool_ids)}
    dsu = DSU(len(pool_ids))
    rep = {i: fid for i, fid in enumerate(pool_ids)}   # root(local) -> fn id
    csize = {i: 1 for i in range(len(pool_ids))}

    n_bands = lib.N_PERMS // rows
    buckets = defaultdict(list)
    for fid in pool_ids:
        s = sigs[fid]
        for b in range(n_bands):
            buckets[(b, s[b * rows:(b + 1) * rows].tobytes())].append(fid)

    def verify(a, b):
        fa, fb = fns[a], fns[b]
        size_prior = (min(fa["n_insns"], fb["n_insns"])
                      / max(fa["n_insns"], fb["n_insns"])) ** 0.25
        if size_prior < 0.5:
            return False
        # near-identical shortcut
        if float((sigs[a] == sigs[b]).mean()) >= 0.9:
            return True
        sm = difflib.SequenceMatcher(None, fa["norm"], fb["norm"])
        if sm.real_quick_ratio() * size_prior < threshold:
            return False
        if sm.quick_ratio() * size_prior < threshold:
            return False
        return sm.ratio() * size_prior >= threshold

    seen = set()
    for members in buckets.values():
        if len(members) < 2:
            continue
        if len(members) > max_bucket:
            members = members[:max_bucket]
        anchor = members[0]
        cand = ([(anchor, m) for m in members[1:]]
                + list(zip(members, members[1:])))
        for a, b in cand:
            ra, rb = dsu.find(id2local[a]), dsu.find(id2local[b])
            if ra == rb:
                continue
            pair = (rep[ra], rep[rb]) if rep[ra] < rep[rb] else (rep[rb], rep[ra])
            if pair in seen:
                continue
            seen.add(pair)
            if verify(rep[ra], rep[rb]):
                dsu.union(ra, rb)
                root = dsu.find(ra)
                big = ra if csize[ra] >= csize[rb] else rb
                csize[root] = csize[ra] + csize[rb]
                rep[root] = rep[big]

    groups = defaultdict(list)
    for fid in pool_ids:
        groups[dsu.find(id2local[fid])].append(fid)
    return [g for g in groups.values() if len(g) > 1]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--min-size", type=int, default=3,
                    help="minimum family member count")
    ap.add_argument("--unmatched-only", action="store_true")
    ap.add_argument("--min-insns", type=int, default=4,
                    help="skip functions smaller than this many instructions")
    ap.add_argument("--threshold", type=float, default=0.45,
                    help="seq-similarity x size-prior vs cluster representative")
    ap.add_argument("--top", type=int, default=30)
    ap.add_argument("--index", default=lib.INDEX_PATH)
    args = ap.parse_args()

    index = lib.load_index(args.index)
    fns = index["functions"]
    pool_ids = [f["id"] for f in fns
                if f["corpus"] == "colo"
                and f["n_insns"] >= args.min_insns
                and (not args.unmatched_only or (f["fuzzy"] or 0) < 100.0)]
    print("pool: %d colo functions (unmatched_only=%s)" % (
        len(pool_ids), args.unmatched_only))

    fams = build_families(index, pool_ids, threshold=args.threshold)
    fams = [g for g in fams if len(g) >= args.min_size]
    scored = []
    for g in fams:
        avg_size = sum(fns[i]["size"] for i in g) / len(g)
        scored.append((len(g) * avg_size, g, avg_size))
    scored.sort(key=lambda s: -s[0])

    print("%d families (>= %d members)\n" % (len(scored), args.min_size))
    for rank, (payoff, g, avg_size) in enumerate(scored[:args.top], 1):
        g = sorted(g, key=lambda i: fns[i]["addr"])
        n_unm = sum(1 for i in g if (fns[i]["fuzzy"] or 0) < 100.0)
        # a shape-family can span units; summarize the true distribution
        # (labeling by the lowest-address member alone misdirected a sweep)
        unit_counts = {}
        for i in g:
            unit_counts[fns[i]["unit"]] = unit_counts.get(fns[i]["unit"], 0) + 1
        unit_summary = " ".join(
            "%s(%d)" % (u, c)
            for u, c in sorted(unit_counts.items(), key=lambda kv: -kv[1])[:4])
        if len(unit_counts) > 4:
            unit_summary += " +%d more units" % (len(unit_counts) - 4)
        print("#%-3d payoff=%-9.0f members=%-4d avg_size=%-6.0f unmatched=%d  units: %s"
              % (rank, payoff, len(g), avg_size, n_unm, unit_summary))
        for i in g[:8]:
            f = fns[i]
            fuzzy = "%.1f%%" % f["fuzzy"] if f["fuzzy"] is not None else "-"
            print("      0x%08X %-45s sz=%-5d %s %s" % (
                f["addr"], f["name"], f["size"], fuzzy, f["unit"]))
        if len(g) > 8:
            print("      ... +%d more" % (len(g) - 8))
        print()


if __name__ == "__main__":
    main()
