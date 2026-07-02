#!/usr/bin/env python3
"""extract_unit_flags.py — build the per-unit compile manifest for the 3090 farm.

Merges objdiff.json (unit name -> source/target paths) with build.ninja
(base object -> mw_version + exact cflags + rule) and emits
build/permuter_units_3090.json:

  { "<unit>": { "src": "src/game/pokemon.c",
                "target_o": "build/GC6E01/obj/game/pokemon.o",
                "mw_version": "GC/1.3",
                "sjis": true,
                "cflags": "<exact flags string from build.ninja>" } }

Run on the Mac from the repo root. READ-ONLY except for the output JSON.
"""
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
OUT = os.path.join(ROOT, "build", "permuter_units_3090.json")


def parse_ninja(path):
    """Return {base_obj_path: {"rule":..., "src":..., "mw_version":..., "cflags":...}}"""
    # join $-continuations
    raw = open(path, encoding="utf-8").read()
    lines = []
    for ln in raw.split("\n"):
        if lines and lines[-1].endswith("$"):
            lines[-1] = lines[-1][:-1] + ln.lstrip()
        else:
            lines.append(ln)
    edges = {}
    cur = None
    for ln in lines:
        m = re.match(r"^build (build/GC6E01/src/\S+\.o): (mwcc\S*) (\S+\.c)", ln)
        if m:
            cur = {"rule": m.group(2), "src": m.group(3)}
            edges[m.group(1)] = cur
            continue
        if ln.startswith("build "):
            cur = None
            continue
        if cur is not None:
            vm = re.match(r"^\s+(\w+) = (.*)$", ln)
            if vm:
                cur[vm.group(1)] = vm.group(2)
    return edges


def main():
    edges = parse_ninja(os.path.join(ROOT, "build.ninja"))
    units = json.load(open(os.path.join(ROOT, "objdiff.json")))["units"]
    out = {}
    for u in units:
        base = u.get("base_path")
        meta = u.get("metadata") or {}
        src = meta.get("source_path")
        if not base or not src or base not in edges:
            continue
        e = edges[base]
        out[u["name"]] = {
            "src": src,
            "target_o": u["target_path"],
            "base_o": base,
            "mw_version": e.get("mw_version", "GC/1.3"),
            "sjis": "sjis" in e.get("rule", ""),
            "cflags": e.get("cflags", ""),
        }
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(out, f, indent=1, sort_keys=True)
    print(f"wrote {OUT}: {len(out)} units")
    # sanity: every unit in the queue must be present
    qpath = os.path.join(ROOT, "build", "permuter_queue_3090.tsv")
    missing = set()
    if os.path.exists(qpath):
        for ln in open(qpath):
            if ln.startswith("#") or not ln.strip():
                continue
            unit = ln.rstrip("\n").split("\t")[-1]
            if unit not in out:
                missing.add(unit)
    if missing:
        print("MISSING queue units:", ", ".join(sorted(missing)), file=sys.stderr)
        sys.exit(1)
    print("all queue units covered")


if __name__ == "__main__":
    main()
