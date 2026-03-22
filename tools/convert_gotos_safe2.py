#!/usr/bin/env python3
"""
Safe goto converter - preserves brace balance by checking after each conversion.
Handles:
1. Single-ref forward conditional goto -> if/else wrapping
2. Single-ref forward unconditional goto after } -> else block
3. Single-ref backward conditional goto -> do-while loop
4. Multi-ref forward gotos (cond+uncond) -> do{...}while(0) + break
5. goto-to-next-label removal
6. Unreferenced label removal
"""
import re
import sys
import os


def invert_cond(cond):
    cond = cond.strip()
    if cond.startswith('(') and cond.endswith(')'):
        inner = cond[1:-1]
        depth = 0
        balanced = True
        for c in inner:
            if c == '(':
                depth += 1
            elif c == ')':
                depth -= 1
                if depth < 0:
                    balanced = False
                    break
        if balanced and depth == 0:
            inv = invert_cond(inner)
            if inv is not None:
                return '(' + inv + ')'
    if cond.startswith('!'):
        return cond[1:]
    if ' && ' in cond and ' || ' not in cond:
        parts = cond.split(' && ')
        inv = [invert_cond(p.strip()) for p in parts]
        if None not in inv:
            return ' || '.join(inv)
    if ' || ' in cond and ' && ' not in cond:
        parts = cond.split(' || ')
        inv = [invert_cond(p.strip()) for p in parts]
        if None not in inv:
            return ' && '.join(inv)
    for old, new in [(' == ', ' != '), (' != ', ' == '), (' < ', ' >= '),
                     (' >= ', ' < '), (' > ', ' <= '), (' <= ', ' > ')]:
        if old in cond and cond.count(old.strip()) == 1:
            return cond.replace(old, new, 1)
    return None


def build_indices(lines):
    label_pos = {}
    label_refs = {}
    label_sources = {}
    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m:
            label_pos[m.group(1)] = i
    for i, line in enumerate(lines):
        for m in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', line):
            lbl = m.group(1)
            label_refs[lbl] = label_refs.get(lbl, 0) + 1
            label_sources.setdefault(lbl, []).append(i)
    return label_pos, label_refs, label_sources


def get_indent(line):
    return len(line) - len(line.lstrip())


def brace_balance(lines, start=None, end=None):
    if start is None:
        start = 0
    if end is None:
        end = len(lines)
    depth = 0
    for i in range(start, end):
        depth += lines[i].count('{') - lines[i].count('}')
    return depth


def safe_convert(lines, convert_func):
    """Run a conversion. Revert if brace balance changes."""
    orig_balance = brace_balance(lines)
    snapshot = [l for l in lines]
    changes = convert_func(lines)
    new_balance = brace_balance(lines)
    if new_balance != orig_balance:
        for i in range(max(len(lines), len(snapshot))):
            if i < len(snapshot):
                if i < len(lines):
                    lines[i] = snapshot[i]
                else:
                    lines.append(snapshot[i])
            elif i < len(lines):
                pass
        while len(lines) > len(snapshot):
            lines.pop()
        while len(lines) < len(snapshot):
            lines.append(snapshot[len(lines)])
        return 0
    return changes


def convert_goto_to_next(lines):
    """Remove goto when label is the next non-empty line."""
    changes = 0
    label_pos, label_refs, label_sources = build_indices(lines)
    for i, line in enumerate(lines):
        m = re.match(r'^(\s*)goto\s+(L_[0-9A-Fa-f]+)\s*;\s*$', line)
        if not m:
            continue
        lbl = m.group(2)
        if lbl not in label_pos:
            continue
        nxt = i + 1
        while nxt < len(lines) and not lines[nxt].strip():
            nxt += 1
        if nxt == label_pos[lbl]:
            lines[i] = ''
            changes += 1
            label_pos, label_refs, label_sources = build_indices(lines)
    return changes


def check_range_safe(lines, src, dst, lbl, label_pos, label_sources):
    """Check if the range [src+1, dst) is safe to wrap."""
    for i in range(src + 1, dst):
        lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[i])
        if lm:
            inner = lm.group(1)
            for s in label_sources.get(inner, []):
                if s < src or s >= dst:
                    return False
    for i in range(src + 1, dst):
        for gm in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[i]):
            t = gm.group(1)
            if t == lbl:
                continue
            if t in label_pos:
                tp = label_pos[t]
                if tp <= src or tp > dst:
                    return False
    return True


def convert_single_fwd_cond(lines):
    """if (cond) goto L; ... L: -> if (!cond) { ... }"""
    changes = 0
    label_pos, label_refs, label_sources = build_indices(lines)

    for lbl in sorted(label_pos.keys(), key=lambda l: label_pos[l]):
        if label_refs.get(lbl, 0) != 1 or lbl not in label_sources:
            continue
        src = label_sources[lbl][0]
        dst = label_pos[lbl]
        if src >= dst:
            continue

        stripped = lines[src].strip()
        m = re.match(r'^if\s*\((.+)\)\s*goto\s+' + re.escape(lbl) + r'\s*;\s*$', stripped)
        if not m:
            continue

        cond = m.group(1)
        inv = invert_cond(cond)
        if inv is None:
            continue

        bal = brace_balance(lines, src + 1, dst)
        if bal != 0:
            continue

        if not check_range_safe(lines, src, dst, lbl, label_pos, label_sources):
            continue

        has_code = any(
            lines[i].strip() and not re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[i])
            for i in range(src + 1, dst)
        )

        if not has_code:
            lines[src] = ''
            lines[dst] = ''
        else:
            indent = ' ' * get_indent(lines[src])
            lines[src] = indent + 'if (' + inv + ') {\n'
            for i in range(src + 1, dst):
                if lines[i].strip():
                    lines[i] = '    ' + lines[i]
            lines[dst] = indent + '}\n'

        changes += 1
        label_pos, label_refs, label_sources = build_indices(lines)

    return changes


def convert_single_fwd_uncond(lines):
    """goto L; code... L: after a } -> else { code... }"""
    changes = 0
    label_pos, label_refs, label_sources = build_indices(lines)

    for lbl in list(label_pos.keys()):
        if label_refs.get(lbl, 0) != 1 or lbl not in label_sources:
            continue
        src = label_sources[lbl][0]
        dst = label_pos[lbl]
        if src >= dst:
            continue

        stripped = lines[src].strip()
        if not re.match(r'^goto\s+' + re.escape(lbl) + r'\s*;\s*$', stripped):
            continue

        bal = brace_balance(lines, src + 1, dst)
        if bal != 0:
            continue

        if not check_range_safe(lines, src, dst, lbl, label_pos, label_sources):
            continue

        prev = src - 1
        while prev >= 0 and not lines[prev].strip():
            prev -= 1

        if prev >= 0 and lines[prev].strip() == '}':
            indent = ' ' * get_indent(lines[src])
            lines[src] = indent + 'else {\n'
            for i in range(src + 1, dst):
                if lines[i].strip():
                    lines[i] = '    ' + lines[i]
            lines[dst] = indent + '}\n'
            changes += 1
            label_pos, label_refs, label_sources = build_indices(lines)

    return changes


def convert_single_bwd_cond(lines):
    """if (cond) goto L; where L is before -> do { ... } while(cond);"""
    changes = 0
    label_pos, label_refs, label_sources = build_indices(lines)

    for lbl in list(label_pos.keys()):
        if label_refs.get(lbl, 0) != 1 or lbl not in label_sources:
            continue
        src = label_sources[lbl][0]
        dst = label_pos[lbl]
        if src <= dst:
            continue

        stripped = lines[src].strip()
        m = re.match(r'^if\s*\((.+)\)\s*goto\s+' + re.escape(lbl) + r'\s*;\s*$', stripped)
        if not m:
            continue
        cond = m.group(1)

        bal = brace_balance(lines, dst + 1, src)
        if bal != 0:
            continue

        # Check safety (using dst as the "src" boundary and src as the "dst" boundary)
        ok = True
        for i in range(dst + 1, src):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[i])
            if lm:
                inner = lm.group(1)
                for s in label_sources.get(inner, []):
                    if s < dst or s > src:
                        ok = False
                        break
                if not ok:
                    break
        if not ok:
            continue
        for i in range(dst + 1, src):
            for gm in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[i]):
                t = gm.group(1)
                if t == lbl:
                    continue
                if t in label_pos:
                    tp = label_pos[t]
                    if tp < dst or tp > src:
                        ok = False
                        break
            if not ok:
                break
        if not ok:
            continue

        indent = ' ' * get_indent(lines[dst])
        lines[dst] = indent + 'do {\n'
        for i in range(dst + 1, src):
            if lines[i].strip():
                lines[i] = '    ' + lines[i]
        lines[src] = indent + '} while (' + cond + ');\n'
        changes += 1
        label_pos, label_refs, label_sources = build_indices(lines)

    return changes


def convert_multi_fwd_do_while(lines):
    """Multi-ref forward gotos (cond+uncond) -> do{...}while(0) + break"""
    changes = 0
    label_pos, label_refs, label_sources = build_indices(lines)

    sorted_labels = sorted(
        [(lbl, label_pos[lbl]) for lbl in label_refs
         if label_refs[lbl] >= 2 and lbl in label_pos],
        key=lambda x: -x[1]
    )

    for lbl, dst in sorted_labels:
        sources = label_sources.get(lbl, [])
        refs = label_refs.get(lbl, 0)
        if refs < 2:
            continue
        if not all(s < dst for s in sources):
            continue

        goto_info = []
        ok = True
        for s in sources:
            stripped = lines[s].strip()
            m_c = re.match(r'^if\s*\((.+)\)\s*goto\s+' + re.escape(lbl) + r'\s*;\s*$', stripped)
            m_u = re.match(r'^goto\s+' + re.escape(lbl) + r'\s*;\s*$', stripped)
            if m_c:
                goto_info.append((s, 'cond', m_c.group(1)))
            elif m_u:
                goto_info.append((s, 'uncond', None))
            else:
                ok = False
                break
        if not ok:
            continue

        first = min(sources)
        bal = brace_balance(lines, first, dst)
        if bal != 0:
            continue

        # No external label refs
        for i in range(first, dst):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[i])
            if lm:
                inner = lm.group(1)
                if inner == lbl:
                    continue
                for s in label_sources.get(inner, []):
                    if s < first or s >= dst:
                        ok = False
                        break
                if not ok:
                    break
        if not ok:
            continue

        # No escape gotos
        for i in range(first, dst):
            for gm in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[i]):
                t = gm.group(1)
                if t == lbl:
                    continue
                if t in label_pos:
                    tp = label_pos[t]
                    if tp < first or tp > dst:
                        ok = False
                        break
            if not ok:
                break
        if not ok:
            continue

        indent = ' ' * get_indent(lines[first])
        for s, gtype, cond in goto_info:
            si = ' ' * get_indent(lines[s])
            if gtype == 'cond':
                lines[s] = si + 'if (' + cond + ') break;\n'
            else:
                lines[s] = si + 'break;\n'

        lines[first] = indent + 'do {\n' + lines[first]
        lines[dst] = indent + '} while (0);\n'
        changes += refs
        label_pos, label_refs, label_sources = build_indices(lines)

    return changes


def remove_unref_labels(lines):
    label_pos, label_refs, _ = build_indices(lines)
    changes = 0
    for lbl, pos in label_pos.items():
        if label_refs.get(lbl, 0) == 0:
            lines[pos] = ''
            changes += 1
    return changes


def process_file(filepath):
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()

    initial_gotos = len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\b', content))
    if initial_gotos == 0:
        return 0

    print(f'Processing {filepath}: {initial_gotos} gotos')
    lines = content.split('\n')

    for iteration in range(10):
        c = 0
        c += safe_convert(lines, convert_goto_to_next)
        c += safe_convert(lines, convert_single_fwd_cond)
        c += safe_convert(lines, convert_single_fwd_uncond)
        c += safe_convert(lines, convert_single_bwd_cond)
        c += safe_convert(lines, convert_multi_fwd_do_while)
        c += remove_unref_labels(lines)
        if c == 0:
            break
        print(f'  Iteration {iteration+1}: {c} changes')

    content = '\n'.join(lines)
    content = re.sub(r'\n{4,}', '\n\n\n', content)

    final_gotos = len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\b', content))
    removed = initial_gotos - final_gotos
    print(f'  Result: {initial_gotos} -> {final_gotos} gotos ({removed} removed)')

    with open(filepath, 'w', encoding='utf-8', errors='replace') as f:
        f.write(content)
    return removed


def main():
    files = sys.argv[1:]
    if not files:
        print("Usage: convert_gotos_safe2.py <file.c> [file2.c ...]")
        sys.exit(1)
    total = 0
    for f in files:
        if os.path.exists(f):
            total += process_file(f)
        else:
            print(f'File not found: {f}')
    print(f'\nTotal gotos removed: {total}')


if __name__ == '__main__':
    main()
