#!/usr/bin/env python3
"""
Goto converter v3: Targeted at remaining hard patterns.
Wraps multi-ref goto targets in do{...}while(0) and converts gotos to break.
Only processes when ALL gotos to a label can be safely converted.
"""
import re
import sys
import os

def build_indices(lines):
    label_pos = {}
    label_refcount = {}
    label_sources = {}
    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m:
            label_pos[m.group(1)] = i
    for i, line in enumerate(lines):
        for m in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', line):
            lbl = m.group(1)
            label_refcount[lbl] = label_refcount.get(lbl, 0) + 1
            if lbl not in label_sources:
                label_sources[lbl] = []
            label_sources[lbl].append(i)
    return label_pos, label_refcount, label_sources

def find_functions(lines):
    """Find function boundaries (start line, end line)."""
    functions = []
    depth = 0
    func_start = None
    for i, line in enumerate(lines):
        if depth == 0 and '{' in line:
            # Look back for function signature
            for j in range(i, max(i-5, -1), -1):
                m = re.match(r'^(void|u\d+|s\d+|u8|s8|u16|s16|u32|s32|float|int|char|struct)\s', lines[j])
                if m:
                    func_start = j
                    break
            if func_start is None:
                func_start = i
        depth += line.count('{') - line.count('}')
        if depth == 0 and func_start is not None:
            functions.append((func_start, i))
            func_start = None
    return functions

def convert_do_while_wrap(lines):
    """Wrap high-ref goto targets in do{...}while(0) within each function."""
    changes = 0
    lp, rc, ls = build_indices(lines)
    functions = find_functions(lines)

    for func_start, func_end in functions:
        # Find labels within this function with high refcount
        func_labels = {}
        for lbl, pos in lp.items():
            if func_start <= pos <= func_end and rc.get(lbl, 0) >= 3:
                func_labels[lbl] = pos

        for end_label, end_pos in sorted(func_labels.items(), key=lambda x: -rc.get(x[0], 0)):
            sources = ls.get(end_label, [])
            # All sources must be within the function
            if not all(func_start <= s <= func_end for s in sources):
                continue
            # All sources must be before end_pos
            if not all(s < end_pos for s in sources):
                continue
            # All sources must be goto statements
            all_gotos = True
            for src in sources:
                stripped = lines[src].strip()
                if not (re.match(r'^goto ' + re.escape(end_label) + r';$', stripped) or
                        re.match(r'^if \(.+\) goto ' + re.escape(end_label) + r';$', stripped)):
                    all_gotos = False
                    break
            if not all_gotos:
                continue

            first_src = min(sources)

            # Find wrap point: walk backward from first_src, tracking brace depth
            # We need wrap_start to be at the SAME brace depth as end_pos
            end_indent = len(lines[end_pos]) - len(lines[end_pos].lstrip()) if lines[end_pos].strip() else 4

            # Compute brace depth at end_pos relative to first_src
            depth_at_first = 0
            for k in range(first_src, end_pos):
                depth_at_first += lines[k].count('{') - lines[k].count('}')

            # The do-while must wrap code at the same brace depth
            # Walk backward, tracking depth, find where depth becomes 0
            wrap_start = first_src
            depth = 0
            for k in range(first_src - 1, max(func_start, first_src - 500), -1):
                depth -= lines[k + 1].count('{') - lines[k + 1].count('}')
                stripped = lines[k].strip()
                if not stripped:
                    continue
                # Only stop at lines that are at same or lower indent as end_pos
                # AND where brace depth is 0 (same block level)
                line_indent = len(lines[k]) - len(lines[k].lstrip())
                if line_indent <= end_indent and depth <= 0:
                    wrap_start = k + 1
                    break

            # CRITICAL: Check that ALL gotos in the range [wrap_start, end_pos)
            # target ONLY labels within [wrap_start, end_pos] or the end_label itself
            has_escape = False
            for k in range(wrap_start, end_pos):
                for gm in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k]):
                    target = gm.group(1)
                    if target == end_label:
                        continue  # This is what we're converting
                    if target in lp:
                        t = lp[target]
                        if t < wrap_start or t > end_pos:
                            has_escape = True
                            break
                if has_escape:
                    break
            if has_escape:
                continue

            # Check that ALL labels in the range are only referenced from within the range
            has_external_ref = False
            for k in range(wrap_start, end_pos):
                ml = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
                if ml:
                    inner_lbl = ml.group(1)
                    for src in ls.get(inner_lbl, []):
                        if src < wrap_start or src >= end_pos:
                            has_external_ref = True
                            break
                    if has_external_ref:
                        break
            if has_external_ref:
                continue

            # Also verify the end_label itself is ONLY referenced from within the range
            for src in sources:
                if src < wrap_start or src >= end_pos:
                    has_external_ref = True
                    break
            if has_external_ref:
                continue

            # Verify brace balance from wrap_start to end_pos
            brace_depth = 0
            brace_ok = True
            for k in range(wrap_start, end_pos):
                brace_depth += lines[k].count('{') - lines[k].count('}')
                if brace_depth < 0:
                    brace_ok = False
                    break
            if not brace_ok or brace_depth != 0:
                continue

            # All checks passed - apply transformation
            indent = ' ' * end_indent

            # FIRST: Convert all gotos to break (before modifying lines)
            for src in sources:
                stripped = lines[src].strip()
                src_indent = ' ' * (len(lines[src]) - len(lines[src].lstrip()))
                m_uncond = re.match(r'^goto ' + re.escape(end_label) + r';$', stripped)
                m_cond = re.match(r'^if \((.+)\) goto ' + re.escape(end_label) + r';$', stripped)
                if m_uncond:
                    lines[src] = src_indent + 'break;'
                elif m_cond:
                    lines[src] = src_indent + 'if (' + m_cond.group(1) + ') break;'
                changes += 1

            # Insert do { before wrap_start
            lines[wrap_start] = indent + 'do {\n' + lines[wrap_start]

            # Replace end label with } while(0);
            lines[end_pos] = indent + '} while (0);'

            # Note: NOT indenting body to avoid breaking existing multi-line constructs

            # Rebuild indices since we changed lines
            lp, rc, ls = build_indices(lines)

    return changes


def remove_unreferenced_labels(lines):
    lp, rc, ls = build_indices(lines)
    changes = 0
    for label, pos in lp.items():
        if rc.get(label, 0) <= 0:
            m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[pos])
            if m:
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

    for iteration in range(5):
        c = convert_do_while_wrap(lines)
        c += remove_unreferenced_labels(lines)
        if c == 0:
            break

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
        print("Usage: convert_gotos_v3.py <file.c> [file2.c ...]")
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
