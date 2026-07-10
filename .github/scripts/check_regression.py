#!/usr/bin/env python3
"""Fail if any function's objdiff fuzzy_match_percent regressed base->head.
usage: check_regression.py <base_report.json> <head_report.json>"""
import json, sys

def fmap(path):
    r = json.load(open(path)); m = {}
    for u in r.get("units", []):
        fns = list(u.get("functions", []))
        for s in u.get("sections", []):
            fns += s.get("functions", [])
        for f in fns:
            n = f.get("name")
            if n:
                m[n] = float(f.get("fuzzy_match_percent", 0) or 0)
    return m

base, head = fmap(sys.argv[1]), fmap(sys.argv[2])
regs = [(n, base[n], head.get(n, 0.0)) for n in base if head.get(n, base[n]) < base[n] - 1e-6]
gains = [n for n in head if head[n] >= 100 > base.get(n, 0)]
if regs:
    print(f"::error::{len(regs)} function(s) regressed:")
    for n, b, h in sorted(regs, key=lambda x: x[1] - x[2], reverse=True):
        print(f"::error::  {n}: {b:.2f}% -> {h:.2f}%")
    print(f"\n(+{len(gains)} new 100% matches, but regressions block merge)")
    sys.exit(1)
print(f"No regressions. +{len(gains)} new byte-exact matches.")
