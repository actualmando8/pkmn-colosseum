#!/usr/bin/env python3
"""Find functions whose blocks are emitted in a different order than retail.

MWCC lays switch-case bodies and if/else arms out in source order. When our
source has an arm in the wrong position the whole body is displaced, which
costs far more fuzzy match than any register-level difference. On the PDA
campaign this was repeatedly the single highest-yield fix in a large function
(fn_8003F464 76->86, fn_8004A7A8 81->85, fn_800499BC 85->86).

Signature: the multiset of `bl` targets matches retail exactly, but the
sequence does not. Same calls, different order => a block moved. Call targets
are normalised to addresses so symbol-naming differences do not register.

A clean run means only that no *call-bearing* block is misplaced; an arm with
no calls in it is invisible to this check.

usage: find_reordered_blocks.py [min_fuzzy] [min_size]
"""
import collections
import difflib
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')
os.chdir(ROOT)
MIN_FUZZY = float(sys.argv[1]) if len(sys.argv) > 1 else 50.0
MIN_SIZE = int(sys.argv[2]) if len(sys.argv) > 2 else 200

sym = {}
for line in open('config/GC6E01/symbols.txt'):
    m = re.match(r'(\S+) = \.text:0x([0-9A-Fa-f]{8});', line)
    if m:
        sym[m.group(1)] = int(m.group(2), 16)
rev = {v: k for k, v in sym.items()}


def addr(name):
    m = re.fullmatch(r'fn_([0-9A-Fa-f]{8})', name)
    return int(m.group(1), 16) if m else sym.get(name, ('NAME', name))


report = json.load(open('build/GC6E01/report.json'))
units = []
for u in report['units']:
    fns = list(u.get('functions', []))
    for s in u.get('sections', []):
        fns += s.get('functions', [])
    sel = [(f['name'], f.get('fuzzy_match_percent', 0.0), int(f['size']))
           for f in fns
           if MIN_FUZZY <= f.get('fuzzy_match_percent', 0.0) < 99.5
           and int(f['size']) >= MIN_SIZE]
    if sel:
        units.append((u['name'], sel))


def scan(item):
    unit, sel = item
    try:
        out = subprocess.run(
            ['build/tools/objdiff-cli', 'diff', '-p', '.', '-u', unit,
             '-o', '-', '--format', 'json'],
            capture_output=True, text=True, timeout=120).stdout
        d = json.loads(out)
        idx = {s: {x['name']: x for x in d[s]['symbols']}
               for s in ('left', 'right')}
    except Exception:
        return []
    rows = []
    for fn, pct, size in sel:
        left, right = idx['left'].get(fn), idx['right'].get(fn)
        if not left or not right:
            continue

        def seq(symbol):
            out = []
            for i in symbol['instructions']:
                t = i.get('instruction', {}).get('formatted', '')
                if t.startswith('bl '):
                    out.append(addr(t.split()[1]))
            return out

        a, b = seq(left), seq(right)
        if a == b or collections.Counter(a) != collections.Counter(b):
            continue
        # how far is the displacement?
        sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
        moved = sum(max(i2 - i1, j2 - j1)
                    for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != 'equal')
        gap = size * (100 - pct) / 100
        frac = moved / max(len(a), 1)
        rows.append((gap * frac, gap, pct, size, fn, unit, moved, len(a)))
    return rows


rows = []
with ThreadPoolExecutor(max_workers=8) as ex:
    for out in ex.map(scan, units):
        rows.extend(out)
# Rank by expected value, not raw gap: a 4000-byte gap with 2 of 330 calls
# displaced is almost entirely some other problem, and reordering wins little.
rows.sort(reverse=True)
print('%d functions with retail-matching call multiset but different order'
      % len(rows))
print('%8s %8s %8s %6s  %-40s %s'
      % ('est', 'gap', 'fuzzy', 'size', 'fn', 'displaced'))
for est, gap, pct, size, fn, unit, moved, ncalls in rows:
    print('%8.0f %8.0f %7.2f%% %6d  %-40s %3d of %3d calls'
          % (est, gap, pct, size, fn, moved, ncalls))
