#!/usr/bin/env python3
from __future__ import annotations

import subprocess
import sys

CMDS = [
    ["ninja", "all_source", "build/GC6E01/report.json"],
    ["python3", "configure.py", "progress"],
    ["python3", "tools/update_readme_progress.py", "--check"],
    ["python3", "tools/check_object_map_freeze.py"],
    ["git", "diff", "--check"],
]

def run(cmd: list[str]) -> None:
    print("\n$", " ".join(cmd))
    subprocess.run(cmd, check=True)

def main() -> int:
    try:
        for cmd in CMDS:
            run(cmd)
    except subprocess.CalledProcessError as e:
        print(f"\nFAILED: {' '.join(e.cmd)}", file=sys.stderr)
        return e.returncode
    print("\nOK: progress/report/freeze/diff checks passed")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
