#!/usr/bin/env python3
"""Batch-flip `#if 1 / asm void fn_X / #include ... / #endif` blocks to `#if 0`
when there's a real C stub in the #else branch.

Usage:
  python3 tools/batch_flip_stubs.py <file_stem>

For each candidate asm wrapper in src/game/<stem>.c:
  1. Check that the block has a non-TODO, non-register-scraped #else stub.
  2. Flip `#if 1` to `#if 0`.
  3. Run `python3 tools/compile_check.py src/game/<stem>.c`.
  4. If compile passes, run `python3 tools/match_scan_file.py <stem> fn_X`.
     If match >= 50%, keep the flip and continue.
     Else revert.
  5. If compile fails, revert and continue.

Produces a summary at end.
"""
import os
import re
import subprocess
import sys
from pathlib import Path

os.chdir(Path(__file__).resolve().parent.parent)

if len(sys.argv) < 2:
    print(__doc__)
    sys.exit(2)

stem = sys.argv[1]
path = Path(f'src/game/{stem}.c')
if not path.exists():
    print(f'No such file: {path}')
    sys.exit(1)

def load(): return path.read_bytes().decode('latin-1')
def save(text): path.write_bytes(text.encode('latin-1'))

def find_candidate_blocks(text):
    # Match `#if 1\n asm void fn_X(void) { #include "..." } #else ... #endif`
    pattern = re.compile(
        r'#if 1\s*\n'
        r'(asm void (fn_\w+)\(void\) \{\s*\n#include "[^"]+"\s*\n\}\s*\n)'
        r'#else\s*\n'
        r'(.*?)'
        r'#endif',
        re.DOTALL)
    out = []
    for m in pattern.finditer(text):
        asm_body = m.group(1)
        fn = m.group(2)
        else_body = m.group(3)
        stripped = else_body.strip()
        if 'TODO' in stripped: continue
        if not stripped: continue
        if 'sp[0x' in stripped: continue      # pseudo-code scraps
        if 'ctr_fn' in stripped: continue     # more scraps
        out.append((m.start(), m.end(), fn, m.group(0)))
    return out

def run_compile():
    r = subprocess.run(['python3', 'tools/compile_check.py', str(path)],
                       capture_output=True, text=True)
    return r.returncode == 0

def run_match(fn):
    r = subprocess.run(['python3', 'tools/match_scan_file.py', stem, fn],
                       capture_output=True, text=True)
    m = re.search(r'(\d+\.\d+)%', r.stdout)
    return float(m.group(1)) if m else 0.0

# Process one at a time, greedy-keep
text = load()
blocks = find_candidate_blocks(text)
print(f'Found {len(blocks)} candidate stubs')

kept = []
reverted = []
for i, (start, end, fn, original_block) in enumerate(blocks):
    text = load()
    # Find the block fresh (positions may have shifted if we kept earlier flips)
    pattern = re.compile(
        r'#if 1\s*\n'
        r'(asm void ' + re.escape(fn) + r'\(void\) \{\s*\n#include "[^"]+"\s*\n\}\s*\n)'
        r'#else\s*\n'
        r'(.*?)'
        r'#endif',
        re.DOTALL)
    m = pattern.search(text)
    if not m:
        reverted.append((fn, 'already flipped or not found'))
        continue

    flipped_text = pattern.sub(
        lambda m: '#if 0\n' + m.group(1) + '#else\n' + m.group(2) + '#endif',
        text, count=1)
    save(flipped_text)

    # Remove stale .o so compile is fresh
    try:
        os.remove(f'build/GC6E01/base/game/{stem}.o')
    except FileNotFoundError:
        pass

    if not run_compile():
        save(text)  # revert
        reverted.append((fn, 'compile failed'))
        print(f'  [{i+1}/{len(blocks)}] {fn}  REVERT (compile failed)')
        continue

    pct = run_match(fn)
    if pct >= 50.0:
        kept.append((fn, pct))
        print(f'  [{i+1}/{len(blocks)}] {fn}  KEEP {pct:.1f}%')
    else:
        save(text)  # revert
        reverted.append((fn, f'match only {pct:.1f}%'))
        print(f'  [{i+1}/{len(blocks)}] {fn}  REVERT ({pct:.1f}%)')

print()
print(f'=== Summary ===')
print(f'Kept: {len(kept)}')
for fn, pct in kept:
    print(f'  {pct:5.1f}%  {fn}')
print(f'Reverted: {len(reverted)}')
for fn, reason in reverted:
    print(f'  {fn}: {reason}')
