#!/usr/bin/env python3
"""Run safe passes (no goto_in_if_else) + multi-ref else on specific files."""
import re, os, sys

sys.path.insert(0, 'tools')
from convert_gotos_safe import (
    build_indices, count_gotos, find_function_ranges, verify_brace_balance,
    pass_goto_next, pass_or_chain, pass_skip_return, pass_goto_return,
    pass_assign_goto_return, pass_skip_terminating_block, pass_if_else,
    pass_cascaded_if_else, pass_fwd_single_skip, pass_multi_ref_consecutive,
    pass_multi_ref_skip, pass_while_loop, pass_do_while, pass_break_from_loop,
    pass_continue_in_loop, pass_uncond_forward_deadcode, pass_remove_unused_labels
)

def pass_multi_else(lines, lp, rc, ls):
    changes = 0
    for label in sorted(lp.keys(), key=lambda x: lp[x]):
        pos = lp[label]
        sources = sorted(ls.get(label, []))
        if not sources:
            continue
        for si in range(len(sources) - 1, -1, -1):
            src = sources[si]
            m = re.match(r'^(\s+)goto ' + re.escape(label) + r';$', lines[src])
            if not m:
                continue
            if src >= pos:
                continue
            close_brace_idx = None
            for k in range(src + 1, min(src + 3, len(lines))):
                s = lines[k].strip()
                if s:
                    if s == '}':
                        close_brace_idx = k
                    break
            if close_brace_idx is None:
                continue
            brace_indent = re.match(r'^(\s*)', lines[close_brace_idx]).group(1)
            depth = 0
            open_brace_line = None
            for k in range(close_brace_idx, -1, -1):
                depth += lines[k].count('}') - lines[k].count('{')
                if depth <= 0:
                    open_brace_line = k
                    break
            if open_brace_line is None:
                continue
            open_line_stripped = lines[open_brace_line].strip()
            if not (re.match(r'^if\s*\(', open_line_stripped) or
                    re.match(r'^}\s*else\s+if\s*\(', open_line_stripped) or
                    re.match(r'^}\s*else\s*\{', open_line_stripped)):
                continue
            next_after_brace = None
            for k in range(close_brace_idx + 1, min(close_brace_idx + 3, len(lines))):
                s = lines[k].strip()
                if s:
                    next_after_brace = s
                    break
            if next_after_brace and next_after_brace.startswith('} else'):
                continue
            else_lines = []
            has_label_inside = False
            has_bad_goto = False
            brace_depth = 0
            for k in range(close_brace_idx + 1, pos):
                s = lines[k].strip()
                if not s:
                    continue
                brace_depth += lines[k].count('{') - lines[k].count('}')
                ml = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', s)
                if ml:
                    lbl_name = ml.group(1)
                    for ss in ls.get(lbl_name, []):
                        if ss < src or ss >= pos:
                            has_label_inside = True
                            break
                gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', s)
                if gm and gm.group(1) != label:
                    t_lbl = gm.group(1)
                    if t_lbl in lp and lp[t_lbl] < src:
                        has_bad_goto = True
                else_lines.append(k)
            if has_label_inside or has_bad_goto or brace_depth != 0:
                continue
            if len(else_lines) == 0 or len(else_lines) > 200:
                continue
            lines[src] = ''
            lines[close_brace_idx] = brace_indent + '} else {'
            for k in else_lines:
                lines[k] = '    ' + lines[k]
            for k in range(close_brace_idx + 1, pos):
                if k not in else_lines and not lines[k].strip():
                    lines[k] = ''
            rc[label] = rc.get(label, 0) - 1
            if rc.get(label, 0) <= 0:
                lines[pos] = brace_indent + '}'
            else:
                lines[pos] = brace_indent + '}\n' + lines[pos]
            changes += 1
            break
    return changes

passes_safe = [
    ('goto_next', pass_goto_next),
    ('or_chain', pass_or_chain),
    ('skip_return', pass_skip_return),
    ('goto_return', pass_goto_return),
    ('assign_goto_return', pass_assign_goto_return),
    ('skip_terminating_block', pass_skip_terminating_block),
    ('if_else', pass_if_else),
    ('cascaded_if_else', pass_cascaded_if_else),
    ('fwd_single_skip', pass_fwd_single_skip),
    ('multi_ref_consecutive', pass_multi_ref_consecutive),
    ('multi_ref_skip', pass_multi_ref_skip),
    ('while_loop', pass_while_loop),
    ('do_while', pass_do_while),
    ('break_from_loop', pass_break_from_loop),
    ('continue_in_loop', pass_continue_in_loop),
    ('uncond_forward_deadcode', pass_uncond_forward_deadcode),
    ('remove_unused_labels', pass_remove_unused_labels),
]

total_removed = 0
for path in sys.argv[1:]:
    path = path.replace('\\', '/')
    with open(path, 'rb') as f:
        raw = f.read()
    has_crlf = b'\r\n' in raw
    content = raw.decode('utf-8', errors='replace')
    initial = len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\b', content))
    if initial == 0:
        continue
    lines = content.replace('\r\n', '\n').split('\n')
    original = lines[:]
    prev = initial
    for iteration in range(30):
        for name, func in passes_safe:
            lp, rc, ls_map = build_indices(lines)
            func(lines, lp, rc, ls_map)
        current = count_gotos(lines)
        if current >= prev:
            break
        prev = current
    for iteration in range(50):
        lp, rc, ls_map = build_indices(lines)
        c = pass_multi_else(lines, lp, rc, ls_map)
        lp2, rc2, ls2 = build_indices(lines)
        for label, p in list(lp2.items()):
            if rc2.get(label, 0) <= 0:
                m = re.match(r'^(\s*)(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[p])
                if m:
                    lines[p] = ''
                    c += 1
        if c == 0:
            break
    func_ranges = find_function_ranges(lines)
    ok, bad = verify_brace_balance(lines, func_ranges)
    if not ok:
        print('  REVERT %s: brace imbalance' % path)
        continue
    new_content = '\n'.join(lines)
    new_content = re.sub(r'\n{4,}', '\n\n\n', new_content)
    final = len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\b', new_content))
    removed = initial - final
    if removed <= 0:
        print('%s: no change (%d gotos)' % (path, initial))
        continue
    if has_crlf:
        new_content = new_content.replace('\n', '\r\n')
    with open(path, 'wb') as f:
        f.write(new_content.encode('utf-8'))
    print('%s: %d -> %d (%d removed)' % (path, initial, final, removed))
    total_removed += removed
print('Total: %d removed' % total_removed)
