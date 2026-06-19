#!/usr/bin/env python3
"""gen_bucket_queue.py — bucket-by-bucket queue. Picks the HIGHEST-priority bucket
that still has unattempted fns and writes build/wall_queue.txt from ONLY that
bucket, so the fleet completes one bucket before moving to the next. Auto-advances
when a bucket is fully attempted. Run before each auto_rebatch pass.

Priority (value-descending): NEARWALL -> STRUCT -> ASM -> LOW
"""
import json, os, sys
from collections import defaultdict
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
LED = os.path.join(ROOT, "build", "wall_ledger.json")
QUEUE = os.path.join(ROOT, "build", "wall_queue.txt")
ASSIGNED = os.path.join(ROOT, "build", "wall_assigned.txt")
BAD = ("effect_util", "hsd_", "ui_core", "fsys_file", "gs_material", "pokemon", "gs_pokemon_summary")
PRIORITY = ["NEARWALL", "STRUCT", "ASM", "LOW"]
# per-bucket minimum match% (skip the truly-hopeless within a bucket)
MINPCT = {"NEARWALL": 0.0, "STRUCT": 0.0, "ASM": 0.0, "LOW": 40.0}


def fresh_by_file(led, bucket):
    bf = defaultdict(list)
    for fn, v in led.items():
        if v["attempted"] or v["bucket"] != bucket:
            continue
        if any(b in v["file"] for b in BAD):
            continue
        if v["pct"] < MINPCT.get(bucket, 0.0):
            continue
        src = "src/" + v["file"] + ".c"
        if os.path.exists(os.path.join(ROOT, src)):
            bf[src].append((v["pct"], fn))
    return bf


# Need enough DISTINCT files to feed every lane (band locks per-file = one lane per
# file). With ~14 lanes a single concentrated bucket (e.g. STRUCT in 2 files) would
# starve most lanes, so we fill the queue with the current bucket's files FIRST and
# overflow into the next bucket(s) — in priority order — only as far as needed.
MIN_FILES = int(os.environ.get("BUCKET_MIN_FILES", "16"))


def main():
    led = json.load(open(LED))
    active = None
    lines = []
    seen = set()
    total_fresh = 0
    for bucket in PRIORITY:
        bf = fresh_by_file(led, bucket)
        nfn = sum(len(v) for v in bf.values())
        if nfn == 0:
            continue
        if active is None:
            active = bucket
        total_fresh += nfn
        for src, fns in sorted(bf.items(), key=lambda kv: (-max(f[0] for f in kv[1]), -len(kv[1]))):
            if src in seen:
                continue
            seen.add(src)
            names = [fn for _, fn in sorted(fns, reverse=True)][:6]
            lines.append(src + " " + " ".join(names))
        if len(lines) >= MIN_FILES:
            break   # enough files to feed the lanes; current bucket is prioritized at the top
    if not active:
        open(QUEUE, "w").write("")
        print("ALL-BUCKETS-COMPLETE")
        return
    open(QUEUE, "w").write("\n".join(lines) + "\n")
    marker = os.path.join(ROOT, "build", ".active_bucket")
    prev = open(marker).read().strip() if os.path.exists(marker) else ""
    if prev != active:
        open(ASSIGNED, "w").write("")
        open(marker, "w").write(active)
    print(f"ACTIVE-BUCKET={active} files={len(lines)} (>=current bucket first, overflow as needed)")


if __name__ == "__main__":
    main()
