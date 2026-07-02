#!/usr/bin/env python3
"""claim.py — atomically claim the next pending queue entry (run under flock).

Reads $FARM/queue.tsv in file order (tier 1 first, as shipped from the Mac).
State per fn lives in $FARM/state/<fn>.status; first line is:
  CLAIMED <worker> <epoch>   |   WIN ...   |   NOWIN ...   |   FAIL_* ...

A CLAIMED entry older than STALE_S (default budget*2) is presumed orphaned
(reboot / kill) and is re-claimable. Prints "fn<TAB>unit" or nothing.
"""
import os
import sys
import time

BASE = os.environ.get("FARM_BASE", "/storage/finetune/pkmn-colosseum-2026")
FARM = os.path.join(BASE, "farm")
STATE = os.path.join(FARM, "state")
STALE_S = int(os.environ.get("STALE_S", os.environ.get("BUDGET", "10800"))) * 2

worker = sys.argv[1] if len(sys.argv) > 1 else "?"
os.makedirs(STATE, exist_ok=True)
now = int(time.time())

for ln in open(os.path.join(FARM, "queue.tsv"), encoding="utf-8"):
    ln = ln.rstrip("\n")
    if not ln or ln.startswith("#"):
        continue
    parts = ln.split("\t")
    if len(parts) < 6:
        continue
    tier, pct, size, fn, addr, unit = parts[:6]
    sf = os.path.join(STATE, fn + ".status")
    if os.path.exists(sf):
        first = open(sf).readline().split()
        if first and first[0] == "CLAIMED":
            try:
                age = now - int(first[2])
            except (IndexError, ValueError):
                age = 0
            if age < STALE_S:
                continue  # actively being worked
            # stale claim -> fall through and re-claim
        else:
            continue  # terminal state
    with open(sf, "w") as f:
        f.write(f"CLAIMED {worker} {now}\n")
    sys.stdout.write(f"{fn}\t{unit}\n")
    break
