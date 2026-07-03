#!/usr/bin/env python3
"""Rank TUs by XD sequence-alignment port viability (two-gate check).

Naming an unnamed TU by porting XD names via address-order alignment only
works when the TU did NOT diverge between the games AND our splitter did
not bucket fragments of several XD source files into it. Two gates:

  Gate 1 (size-parity): every already-named anchor shared with XD has the
    same byte size. Necessary but NOT sufficient — a bucketed TU can still
    pass it (each anchor individually matches its own XD namesake).

  Gate 2 (gap-parity + order): the anchors are monotonic in XD address
    order AND the byte-gaps between adjacent anchors match ours within a
    tolerance. This is what actually proves contiguity: if our TU is XD's
    same contiguous source unit, the inter-anchor gaps are identical.

A TU passing BOTH gates with a large unnamed pool is a cheap mass-port
candidate (align by size, port XD names). A TU failing gate 2 (mono=False
or low gap-fraction) was bucketed by the address splitter from multiple XD
sources — port function-by-function by body skeleton instead, or fix the
split first. Learned the hard way: the coarse size-parity scan over-ranked
gs_pcbox/gs_scene/gs_party_access (1/125 portable) until gate 2 was added.

Usage: tools/port_candidates.py [--min-unnamed N] [--tol 0x10]
"""
import argparse
import bisect
import re
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def load(p):
    out = []
    pat = re.compile(
        r"(\w+) = \.text:(0x[0-9A-Fa-f]+); // type:function size:(0x[0-9A-Fa-f]+)")
    for line in open(p):
        m = pat.match(line)
        if m:
            out.append((int(m.group(2), 16), m.group(1), int(m.group(3), 16)))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--min-unnamed", type=int, default=10)
    ap.add_argument("--tol", type=lambda x: int(x, 0), default=0x10)
    ap.add_argument("--symbols", default=str(ROOT / "config/GC6E01/symbols.txt"))
    ap.add_argument("--xd", default=str(
        ROOT / "tools/symbolmap/xd_ref/GXXE01/config/GXXE01/symbols.txt"))
    ap.add_argument("--splits", default=str(ROOT / "config/GC6E01/splits.txt"))
    a = ap.parse_args()

    cs = load(a.symbols)
    xs = {n: (addr, s) for addr, n, s in load(a.xd)}

    splits, cur = [], None
    for line in open(a.splits):
        m = re.match(r"^(\S+\.c):", line)
        if m:
            cur = m.group(1)
            continue
        m = re.search(r"\.text\s+start:(0x[0-9A-Fa-f]+) end:(0x[0-9A-Fa-f]+)", line)
        if m and cur:
            splits.append((int(m.group(1), 16), int(m.group(2), 16), cur))
    splits.sort()
    starts = [s[0] for s in splits]

    def tu(addr):
        i = bisect.bisect_right(starts, addr) - 1
        if 0 <= i < len(splits) and splits[i][0] <= addr < splits[i][1]:
            return splits[i][2]
        return "?"

    anchors = defaultdict(list)
    unnamed = defaultdict(int)
    for addr, name, size in cs:
        t = tu(addr)
        if name.startswith("fn_"):
            unnamed[t] += 1
        elif not name.startswith("lbl_") and name in xs:
            anchors[t].append((addr, name, size, xs[name][0]))

    rows = []
    for t, anch in anchors.items():
        if t == "?" or len(anch) < 4 or unnamed[t] < a.min_unnamed:
            continue
        anch.sort()
        xa = [x[3] for x in anch]
        mono = all(xa[i + 1] > xa[i] for i in range(len(xa) - 1))
        gok = gtot = 0
        size_ok = sum(1 for _, n, s, _ in anch if s == xs[n][1])
        for i in range(len(anch) - 1):
            og = anch[i + 1][0] - anch[i][0]
            xg = anch[i + 1][3] - anch[i][3]
            gtot += 1
            if abs(og - xg) <= a.tol:
                gok += 1
        gapfrac = gok / gtot if gtot else 0
        passes = mono and gapfrac >= 0.8 and size_ok == len(anch)
        rows.append((passes, gapfrac, len(anch), unnamed[t], mono,
                     f"{gok}/{gtot}", f"{size_ok}/{len(anch)}", t))

    rows.sort(key=lambda r: (-r[0], -r[3]))
    print(f"{'PASS':>4} {'gap':>5} {'anch':>4} {'unnamed':>7} {'mono':>5} "
          f"{'gaps':>7} {'sizes':>7}  TU")
    for p, gf, na, un, mono, g, sz, t in rows:
        print(f"{'YES' if p else 'no':>4} {gf * 100:4.0f}% {na:>4} {un:>7} "
              f"{str(mono):>5} {g:>7} {sz:>7}  {t}")
    n_pass = sum(1 for r in rows if r[0])
    print(f"\n{n_pass} TUs pass both gates (cheap mass-port candidates); "
          f"{len(rows) - n_pass} fail (bucketed/diverged — per-fn only)")


if __name__ == "__main__":
    main()
