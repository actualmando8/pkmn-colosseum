#!/usr/bin/env python3
"""update_report.py <fn ...> — mark the given functions 100% in report.json.

Called by auto_gate right after a win commits, so report.json (and therefore the
wall_ledger / bucket queues built from it) immediately reflects the function as
DONE instead of re-offering it for hours until the dashboard's periodic regen.
Atomic write so it never collides with a concurrent dashboard regen.
"""
import json, os, sys, tempfile

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
REPORT = os.path.join(ROOT, "report.json")


def main():
    fns = set(sys.argv[1:])
    if not fns or not os.path.exists(REPORT):
        return
    try:
        d = json.load(open(REPORT, encoding="utf-8"))
    except Exception:
        return
    n = 0
    for u in d.get("units", []):
        bumped = False
        for f in u.get("functions", []):
            if f.get("name") in fns and float(f.get("fuzzy_match_percent", 0) or 0) < 99.95:
                f["fuzzy_match_percent"] = 100.0
                n += 1
                bumped = True
        # keep the unit's matched_functions roughly honest so dashboard counts track
        if bumped:
            m = u.get("measures")
            if isinstance(m, dict) and "matched_functions" in m:
                done = sum(1 for f in u.get("functions", [])
                           if float(f.get("fuzzy_match_percent", 0) or 0) >= 99.95)
                m["matched_functions"] = max(int(m.get("matched_functions", 0)), done)
    if not n:
        return
    fd, tmp = tempfile.mkstemp(dir=ROOT, suffix=".tmp")
    try:
        with os.fdopen(fd, "w", encoding="utf-8") as fh:
            json.dump(d, fh)
        os.replace(tmp, REPORT)
        print(f"report.json: marked {n} fn(s) 100%")
    except Exception:
        try:
            os.unlink(tmp)
        except OSError:
            pass


if __name__ == "__main__":
    main()
