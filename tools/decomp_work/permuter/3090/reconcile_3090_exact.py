#!/usr/bin/env python3
"""Remove live-tree exact functions from the inactive 3090 farm queue."""

import json
import os
import sys
from pathlib import Path


base = Path(os.environ.get("FARM_BASE", "/storage/finetune/pkmn-colosseum-2026"))
farm = base / "farm"
exact = {line.strip() for line in sys.stdin if line.strip()}
queue = farm / "queue.tsv"
lines = queue.read_text(encoding="utf-8").splitlines()
kept = []
removed = []
deferred = []
for line in lines:
    if not line or line.startswith("#"):
        kept.append(line)
        continue
    fields = line.split("\t")
    name = fields[3]
    if name not in exact:
        kept.append(line)
        continue
    state_path = farm / "state" / f"{name}.status"
    state = state_path.read_text(errors="replace") if state_path.exists() else ""
    if state.startswith("CLAIMED"):
        kept.append(line)
        deferred.append(name)
    else:
        removed.append(name)
tmp = queue.with_suffix(".tsv.tmp")
tmp.write_text("\n".join(kept) + "\n", encoding="utf-8")
os.replace(tmp, queue)
print(json.dumps({
    "removed": sorted(set(removed)),
    "deferred_active": sorted(set(deferred)),
    "remaining": len([x for x in kept if x and not x.startswith("#")]),
}))
