#!/usr/bin/env python3
"""Pass 6 goto converter - handles self-contained forward gotos and multi-ref do-while patterns."""
import re, copy, sys

def count_gotos(lines):
    return sum(len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\b', l)) for l in lines)

def build_indices(lines):
    lp, rc, ls = {}, {}, {}
    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m: lp[m.group(1)] = i
    for i, line in enumerate(lines):
        for m in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', line):
            lbl = m.group(1)
            rc[lbl] = rc.get(lbl, 0) + 1
            ls.setdefault(lbl, []).append(i)
    return lp, rc, ls

def verify_balance(lines):
    d = 0
    for i, l in enumerate(lines):
        d += l.count('{') - l.count('}')
        if d < 0: return False, i
    return d == 0, -1

def invert_cond(cond):
    cond = cond.strip()
    if ' && ' in cond and ' || ' not in cond:
        parts = cond.split(' && ')
        inv = [invert_cond(p.strip()) for p in parts]
        if None in inv: return None
        return ' || '.join(inv)
    if ' || ' in cond and ' && ' not in cond:
        parts = cond.split(' || ')
        inv = [invert_cond(p.strip()) for p in parts]
        if None in inv: return None
        return ' && '.join(inv)
    for old, new in [(' == ', ' != '), (' != ', ' == '), (' < ', ' >= '),
                     (' >= ', ' < '), (' > ', ' <= '), (' <= ', ' > ')]:
        if old in cond:
            count = sum(cond.count(op) for op in [' == ', ' != ', ' < ', ' >= ', ' > ', ' <= '])
            if count == 1:
                return cond.replace(old, new, 1)
    if cond.startswith('!'):
        return cond[1:]
    return None

def pass_self_contained_forward(lines):
    """Handle forward gotos where ALL labels and gotos in skipped region are self-contained."""
    changed = True
    total = 0
    while changed:
        changed = False
        lp, rc, ls = build_indices(lines)
        for i, line in enumerate(lines):
            m = re.match(r'^(\s+)if\s*\((.+?)\)\s*goto\s+(L_[0-9A-Fa-f]+)\s*;', line)
            if not m: continue
            indent, cond, label = m.group(1), m.group(2), m.group(3)
            if label not in lp: continue
            li = lp[label]
            if li <= i or li - i > 200: continue

            sk = lines[i+1:li]
            nb = sum(l.count('{') - l.count('}') for l in sk)
            if nb != 0: continue

            self_contained = True
            for k in range(i+1, li):
                gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
                if gm:
                    target = gm.group(1)
                    if target not in lp:
                        self_contained = False; break
                    tp = lp[target]
                    if not (i < tp <= li):
                        self_contained = False; break
                lm = re.match(r'\s*(L_[0-9A-Fa-f]+)\s*:', lines[k])
                if lm:
                    lab = lm.group(1)
                    if lab in ls:
                        for src in ls[lab]:
                            if not (i < src < li):
                                self_contained = False; break
                    if not self_contained: break
            if not self_contained: continue

            inv = invert_cond(cond)
            if inv is None: continue

            nl = [indent + 'if (' + inv + ') {\n']
            for s in sk:
                nl.append(('    ' + s) if s.strip() else s)
            nl.append(indent + '}\n')
            if rc[label] == 1:
                lines[i:li+1] = nl
            else:
                lines[i:li] = nl
            total += 1; changed = True; break
    return total

def pass_multi_ref_dowhile(lines):
    """Multi-ref forward gotos with all unconditional: wrap in do-while(0), break."""
    changed = True
    total = 0
    while changed:
        changed = False
        lp, rc, ls = build_indices(lines)
        for label in sorted(lp.keys(), key=lambda l: lp[l]):
            if label not in ls: continue
            sources = ls[label]
            if len(sources) < 2: continue
            label_pos = lp[label]
            if not all(s < label_pos for s in sources): continue

            all_uncond = True
            for s in sources:
                stripped = lines[s].strip()
                if not re.match(r'^goto\s+' + re.escape(label) + r'\s*;\s*$', stripped):
                    all_uncond = False; break
            if not all_uncond: continue

            first = min(sources)
            segment = lines[first:label_pos]
            nb = sum(l.count('{') - l.count('}') for l in segment)
            if nb != 0: continue

            ext = False
            for other_lbl, other_pos in lp.items():
                if other_lbl == label: continue
                if first <= other_pos < label_pos:
                    if other_lbl in ls:
                        for src in ls[other_lbl]:
                            if src < first or src >= label_pos:
                                ext = True; break
                if ext: break
            if ext: continue

            indent = re.match(r'^(\s*)', lines[first]).group(1)
            lines.insert(first, indent + 'do {\n')
            label_pos += 1
            new_sources = [s + 1 for s in sources]
            for s in new_sources:
                lines[s] = re.sub(r'goto\s+' + re.escape(label) + r'\s*;', 'break;', lines[s])
            lines[label_pos] = indent + '} while (0);\n'
            total += len(sources)
            changed = True; break
    return total

def pass_uncond_sole_forward(lines):
    """Single-ref unconditional forward goto: remove goto + label if only } between."""
    changed = True
    total = 0
    while changed:
        changed = False
        lp, rc, ls = build_indices(lines)
        for i, line in enumerate(lines):
            m = re.match(r'^(\s+)goto\s+(L_[0-9A-Fa-f]+)\s*;\s*$', line)
            if not m: continue
            label = m.group(2)
            if label not in lp or rc.get(label, 0) != 1: continue
            li = lp[label]
            if li <= i: continue
            ok = all(lines[k].strip() in ('', '}') for k in range(i+1, li))
            if not ok: continue
            lines[i] = ''
            lines[li] = ''
            total += 1; changed = True; break
    return total

def pass_backward_loop(lines):
    """Backward goto loop with single ref."""
    changed = True
    total = 0
    while changed:
        changed = False
        lp, rc, ls = build_indices(lines)
        for i, line in enumerate(lines):
            m = re.match(r'^(\s+)goto\s+(L_[0-9A-Fa-f]+)\s*;\s*$', line)
            if not m: continue
            indent, label = m.group(1), m.group(2)
            if label not in lp: continue
            li = lp[label]
            if li >= i or rc[label] != 1: continue
            body = lines[li+1:i]
            nb = sum(l.count('{') - l.count('}') for l in body)
            hl = any(re.match(r'\s*L_[0-9A-Fa-f]+\s*:', l) for l in body)
            if nb != 0 or hl: continue
            li_ind = re.match(r'^(\s*)', lines[li]).group(1)
            lines[li] = li_ind + 'while (1) {\n'
            lines[i] = indent + '}\n'
            total += 1; changed = True; break
    return total

def pass_cond_forward_simple(lines):
    """Simple conditional forward skip: if(c) goto L; code; L:"""
    changed = True
    total = 0
    while changed:
        changed = False
        lp, rc, ls = build_indices(lines)
        for i, line in enumerate(lines):
            m = re.match(r'^(\s+)if\s*\((.+?)\)\s*goto\s+(L_[0-9A-Fa-f]+)\s*;', line)
            if not m: continue
            indent, cond, label = m.group(1), m.group(2), m.group(3)
            if label not in lp: continue
            li = lp[label]
            if li <= i or li - i > 100: continue
            sk = lines[i+1:li]
            nb = sum(l.count('{') - l.count('}') for l in sk)
            hl = any(re.match(r'\s*L_[0-9A-Fa-f]+\s*:', l) for l in sk)
            hg = any(re.search(r'\bgoto\s', l) for l in sk)
            if hl or hg or nb != 0: continue
            inv = invert_cond(cond)
            if inv is None: continue
            nl = [indent + 'if (' + inv + ') {\n']
            for s in sk:
                nl.append(('    ' + s) if s.strip() else s)
            nl.append(indent + '}\n')
            if rc[label] == 1: lines[i:li+1] = nl
            else: lines[i:li] = nl
            total += 1; changed = True; break
    return total

files = sys.argv[1:]
grand = 0
for fname in files:
    with open(fname, 'rb') as f:
        raw = f.read()
    has_crlf = b'\r\n' in raw
    text = raw.decode('utf-8', errors='replace')
    if has_crlf:
        text = text.replace('\r\n', '\n')
    lines = text.split('\n')

    orig = count_gotos(lines)
    backup = copy.deepcopy(lines)

    for _ in range(200):
        t = (pass_cond_forward_simple(lines) +
             pass_uncond_sole_forward(lines) +
             pass_backward_loop(lines) +
             pass_self_contained_forward(lines) +
             pass_multi_ref_dowhile(lines))
        if t == 0: break

    final = count_gotos(lines)
    ok, bad = verify_balance(lines)
    if not ok:
        print(f'  BRACE IMBALANCE at line {bad+1}! Reverting {fname}.')
        lines = backup; final = orig; removed = 0
    else:
        out = '\n'.join(lines)
        if has_crlf:
            out = out.replace('\n', '\r\n')
        with open(fname, 'wb') as f:
            f.write(out.encode('utf-8'))
        removed = orig - final

    grand += removed
    if removed > 0:
        print(f'  {fname}: {orig} -> {final} ({removed} removed)')

print(f'Total removed: {grand}')
