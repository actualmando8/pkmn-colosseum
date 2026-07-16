#!/usr/bin/env python3
"""Re-admit only NOWIN targets that set a new permuter-score record.

The first completed pass must beat its base score. Later passes must beat the
best score previously admitted. State is independent of terminal .status
files so results and partials remain intact.
"""

import json
import os
import re
from pathlib import Path

BASE = Path(os.environ.get("FARM_BASE", "/storage/finetune/pkmn-colosseum-2026"))
FARM = BASE / "farm"
STATE = FARM / "state"
HISTORY = STATE / "retry_best.json"


def load_history():
    try:
        return json.loads(HISTORY.read_text())
    except (OSError, ValueError):
        return {}


def atomic_write(path, data):
    tmp = path.with_suffix(".tmp")
    tmp.write_text(json.dumps(data, indent=1, sort_keys=True) + "\n")
    os.replace(tmp, path)


def queue_names():
    names = set()
    for line in (FARM / "queue.tsv").read_text().splitlines():
        if not line or line.startswith("#"):
            continue
        parts = line.split("\t")
        if len(parts) >= 4:
            names.add(parts[3])
    return names


def main():
    queued = queue_names()
    history = load_history()
    admitted = []
    considered = 0

    for status_path in sorted(STATE.glob("*.status")):
        first = status_path.read_text(errors="replace").splitlines()[:1]
        if not first or not first[0].startswith("NOWIN "):
            continue
        fn = status_path.name.removesuffix(".status")
        if fn not in queued:
            continue
        considered += 1
        best_match = re.search(r"\bbest=(-?\d+)\b", first[0])
        base_match = re.search(r"\bbase=(-?\d+)\b", first[0])
        if not best_match or not base_match:
            continue
        best = int(best_match.group(1))
        base = int(base_match.group(1))
        previous = int(history.get(fn, base))
        if best < previous:
            history[fn] = best
            status_path.unlink()
            admitted.append(fn)

    atomic_write(HISTORY, history)
    detail = STATE / "retry_last.json"
    atomic_write(detail, {
        "considered": considered,
        "admitted": admitted,
        "admitted_count": len(admitted),
    })
    print(len(admitted))


if __name__ == "__main__":
    main()
