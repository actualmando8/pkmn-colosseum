#!/usr/bin/env python3
"""refill_queue.py — the daily-gains loop. Pull the closest WINNABLE near-misses
from the current objdiff report and write them as the permuter swarm queue.

A "winnable" near-miss is a function whose ACTIVE source branch is real C (NOT an
asm-wrapper — those score 99.x% from reloc/pool artifacts but have no C to
permute) and whose fuzzy match is in [min,max). The closest ones (99.9%+) are
typically 1-3 instructions off — exactly where the annealer has a gradient and
the LLM agents can hand-fix. (Contrast the reg-alloc / structural WALLS, which
no source change reaches — see triage_gate.py / coloring_oracle.py.)

usage:
  python3 refill_queue.py [--min 95] [--max 99.999] [--n 14] [--min-bytes 160]
  python3 refill_queue.py --list        # just print, don't write the queue
Writes .omc/permuter_queue.json (grind2.py reads it). Run with WSL python3."""
import json, os, re, sys
from pathlib import Path

REPO = Path("/mnt/c/Users/douglaswhittingham/pkmn-colosseum")
REPORT = REPO / "report.json"
QUEUE = REPO / ".omc" / "permuter_queue.json"
SRC = REPO / "src"


def opt(flag, default, cast=float):
    if flag in sys.argv:
        return cast(sys.argv[sys.argv.index(flag) + 1])
    return default


def classify_sources():
    """fn -> source path, restricted to functions whose ACTIVE (#if-live) branch
    is a real C definition (not an `asm` wrapper)."""
    real, asmw, fnfile = set(), set(), {}
    for c in SRC.rglob("*.c"):
        try:
            lines = c.read_text(errors="replace").splitlines()
        except OSError:
            continue
        stack = [True]
        for l in lines:
            s = l.strip()
            if s.startswith("#if"):
                stack.append(stack[-1] and s[3:].strip() != "0")
            elif s.startswith("#else"):
                if len(stack) > 1:
                    stack[-1] = (not stack[-1]) and stack[-2]
            elif s.startswith("#endif"):
                if len(stack) > 1:
                    stack.pop()
            elif stack[-1]:
                m = re.match(r"^\s*asm\s+\w+\s+(fn_[0-9A-Fa-f]+)\s*\(", l)
                if m:
                    asmw.add(m.group(1))
                m2 = re.match(r"^\s*(?:static\s+)?[A-Za-z_][\w \*]*\b(fn_[0-9A-Fa-f]{8})\s*\(", l)
                if m2:
                    real.add(m2.group(1))
                    fnfile.setdefault(m2.group(1), str(c.relative_to(REPO)).replace("\\", "/"))
    return real, asmw, fnfile


def main():
    lo = opt("--min", 95.0)
    hi = opt("--max", 99.999)
    n = opt("--n", 14, int)
    min_bytes = opt("--min-bytes", 160, int)
    list_only = "--list" in sys.argv

    rep = json.load(open(REPORT))
    fns = {}
    for u in rep["units"]:
        for f in u.get("functions") or []:
            fm = f.get("fuzzy_match_percent")
            if fm is not None and lo <= fm < hi:
                fns[f["name"]] = {"fuzzy": fm, "size": int(f.get("size", 0))}

    real, asmw, fnfile = classify_sources()
    cand = [(name, d, fnfile[name]) for name, d in fns.items()
            if name in real and name not in asmw and name in fnfile and d["size"] >= min_bytes]
    cand.sort(key=lambda x: -x[1]["fuzzy"])

    print(f"{len(cand)} winnable real-C near-misses in [{lo},{hi})%, >={min_bytes}B — closest first:")
    for name, d, f in cand[:n]:
        print(f"  {name}  {d['fuzzy']:6.2f}%  {d['size']:>5}B  {f}")
    if list_only:
        return
    queue = [[name, f] for name, d, f in cand[:n]]
    os.makedirs(QUEUE.parent, exist_ok=True)
    json.dump(queue, open(QUEUE, "w"), indent=1)
    print(f"\nwrote {QUEUE} with {len(queue)} targets — restart grind2.py to grind them")


if __name__ == "__main__":
    main()
