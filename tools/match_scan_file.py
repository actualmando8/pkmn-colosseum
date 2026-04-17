#!/usr/bin/env python3
"""Scan match percentage for functions in any src/game/<file>.c.

Usage:
  python3 tools/match_scan_file.py <file_stem> fn_XXXXX [fn_YYYYY ...]

Example:
  python3 tools/match_scan_file.py gs_event_exec fn_800141BC fn_80014234
"""
import subprocess
import json
import os
import sys
from pathlib import Path

os.chdir(Path(__file__).resolve().parent.parent)

CLI = r'tools\objdiff-cli.exe'
TARGET = r'build\GC6E01\obj\auto_01_800055E0_text.o'

if len(sys.argv) < 3:
    print(__doc__)
    sys.exit(2)

stem = sys.argv[1]
BASE = fr'build\GC6E01\base\game\{stem}.o'
syms = sys.argv[2:]

for s in syms:
    r = subprocess.run(
        [CLI, 'diff', '-1', TARGET, '-2', BASE, '-o', '-',
         '--format', 'json', '-c', 'ppc.calculatePoolRelocations=false', s],
        capture_output=True, text=True, shell=False
    )
    if r.returncode == 0 and r.stdout.strip():
        try:
            d = json.loads(r.stdout)
            pct = None
            for side in ('right', 'left'):
                for sym in d.get(side, {}).get('symbols', []):
                    if sym.get('name') == s and sym.get('kind') == 'SYMBOL_FUNCTION':
                        mp = sym.get('match_percent')
                        if mp is not None:
                            pct = mp
                            break
                if pct is not None:
                    break
            if pct is None:
                print(f'  NO-MATCH {s}')
            else:
                print(f'{pct:6.1f}%  {s}')
        except Exception as e:
            print(f'  ERR {s}: {e} :: {r.stdout[:200]}')
    else:
        print(f'  FAIL {s}: rc={r.returncode} stderr={r.stderr[:120]}')
