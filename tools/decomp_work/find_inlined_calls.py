#!/usr/bin/env python3
"""Find functions where retail CALLS something we inlined away.

MWCC auto-inlines a same-TU function that is defined before the call site and
is small enough to qualify. When retail kept a real `bl`, our version silently
grows the callee's body inline and loses a lot of fuzzy match -- fn_8003A520
was 65% -> 91% and fn_8004C120 69% -> 90% from exactly this.

Signature: comparing the multiset of `bl` targets between retail and ours,
retail calls something we never call AND we call a cluster of things retail
does not. Call targets are normalised to addresses first, so `menuOpen` and
`fn_8010264C` do not read as a difference.

The fix is usually a block-scope `extern` of the callee inside the caller,
which defeats CW's auto-inline for that call site only.

Candidates are further filtered to the ones MWCC could actually have inlined:
the callee must be defined in the SAME source file, BEFORE the caller. Without
that filter roughly three quarters of the hits are impossible cases -- a
cross-file callee, or one defined after the call site -- where the missing call
means something else entirely (usually that we simply have not written it).

usage: find_inlined_calls.py [min_fuzzy] [min_size]
"""
import collections
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..')
os.chdir(ROOT)
MIN_FUZZY = float(sys.argv[1]) if len(sys.argv) > 1 else 55.0
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

        def calls(symbol):
            c = collections.Counter()
            for i in symbol['instructions']:
                text = i.get('instruction', {}).get('formatted', '')
                if text.startswith('bl '):
                    c[addr(text.split()[1])] += 1
            return c

        missing = calls(left) - calls(right)
        extra = calls(right) - calls(left)
        if missing and extra:
            rows.append((size * (100 - pct) / 100, pct, size, fn, unit,
                         dict(missing), sum(extra.values())))
    return rows


rows = []
with ThreadPoolExecutor(max_workers=8) as ex:
    for out in ex.map(scan, units):
        rows.extend(out)
rows.sort(reverse=True)

# Keep only what CW could actually have inlined: callee defined in the same
# file, ahead of the caller.
SRCS = [os.path.join(r, f) for r, _, fs in os.walk('src')
        for f in fs if f.endswith('.c')]


def _defre(n):
    return re.compile(r'^[A-Za-z_][A-Za-z0-9_ \*]*?\b' + re.escape(n) +
                      r'\s*\([^;]*?\)\s*\{', re.M | re.S)


def _locate(n):
    rx = _defre(n)
    for path in SRCS:
        text = open(path, errors='ignore').read()
        m = rx.search(text)
        if m and not text[m.start():m.start() + 7].startswith('extern'):
            return path, m.start()
    return None, None


def _inlinable(caller, missing):
    cpath, coff = _locate(caller)
    if cpath is None:
        return False
    for a in missing:
        name = nm(a)
        path, off = _locate(name)
        if path == cpath and off is not None and off < coff:
            return True
    return False


def nm(a):
    return a[1] if isinstance(a, tuple) else rev.get(a, 'fn_%08X' % a)


rows = [r for r in rows if _inlinable(r[3], r[5])]
print('%d inlinable candidates (fuzzy >= %.0f, size >= %d)'
      % (len(rows), MIN_FUZZY, MIN_SIZE))
for gap, pct, size, fn, unit, missing, n_extra in rows:
    miss = ', '.join('%s x%d' % (nm(a), c) for a, c in list(missing.items())[:4])
    print('gap%7.0f %7.2f%% %6d  %-36s extra=%3d  missing: %s'
          % (gap, pct, size, fn, n_extra, miss))
