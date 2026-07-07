#!/usr/bin/env python3
from __future__ import annotations

import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PROPOSALS = ROOT / "config/GC6E01/symbolmap/hsd_proposals.json"
SYMBOLS = ROOT / "config/GC6E01/symbols.txt"

def main() -> int:
    proposals = json.loads(PROPOSALS.read_text())
    symbols = SYMBOLS.read_text()

    applied = []
    missing = []

    for p in proposals:
        fn = p["fn"]
        name = p["proposedName"]

        if re.search(rf"^{re.escape(name)}\s*=", symbols, re.M):
            applied.append(p)
        elif re.search(rf"^{re.escape(fn)}\s*=", symbols, re.M):
            missing.append(p)

    print(f"applied: {len(applied)}")
    print(f"missing: {len(missing)}")
    print()

    for p in missing:
        print(f'{p["confidence"]} {p["fn"]} {p["addr"]} -> {p["proposedName"]}  [{p["unit"]}]')

    return 0

if __name__ == "__main__":
    raise SystemExit(main())
