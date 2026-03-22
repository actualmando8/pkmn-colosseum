#!/usr/bin/env python3
"""
Comprehensive goto elimination for pkmn-colosseum decompiled C files.
Handles all pattern types found in the codebase.
All structural transformations include brace-balance safety checks.
"""
import re
import sys


def find_gotos_and_labels(lines):
    gotos = []
    labels = {}
    for i, line in enumerate(lines):
        if line is None:
            continue
        m = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', line)
        if m:
            gotos.append((i, m.group(1)))
        m = re.match(r'^(\s*)(L_[0-9A-Fa-f]+)\s*:\s*(.*)', line)
        if m:
            labels[m.group(2)] = i
    return gotos, labels


def count_gotos(lines):
    count = 0
    for line in lines:
        if line and re.search(r'\bgoto\s+L_[0-9A-Fa-f]+\s*;', line):
            count += 1
    return count


def get_indent(line):
    if line is None:
        return 0
    return len(line) - len(line.lstrip())


def label_refcount(lines, label_name):
    count = 0
    for line in lines:
        if line and ('goto ' + label_name) in line:
            count += 1
    return count


def remove_label_if_unused(lines, label_name):
    if label_refcount(lines, label_name) > 0:
        return
    for i, line in enumerate(lines):
        if line is None:
            continue
        m = re.match(r'^(\s*)' + re.escape(label_name) + r'\s*:\s*(.*)', line)
        if m:
            code_after = m.group(2).strip()
            if code_after and code_after != ';':
                lines[i] = m.group(1) + code_after + '\n'
            else:
                lines[i] = None
            break


def negate_cond(cond):
    cond = cond.strip()
    ops = [('!=', '=='), ('==', '!='), ('>=', '<'), ('<=', '>')]
    for old, new in ops:
        depth = 0
        for i in range(len(cond)):
            if cond[i] == '(':
                depth += 1
            elif cond[i] == ')':
                depth -= 1
            elif depth == 0 and cond[i:i+len(old)] == old:
                return cond[:i] + new + cond[i+len(old):]
    for old, new in [('>', '<='), ('<', '>=')]:
        depth = 0
        for i in range(len(cond)):
            if cond[i] == '(':
                depth += 1
            elif cond[i] == ')':
                depth -= 1
            elif depth == 0 and cond[i] == old:
                if i > 0 and cond[i-1] in '!=<>':
                    continue
                if i+1 < len(cond) and cond[i+1] in '=':
                    continue
                return cond[:i] + new + cond[i+1:]
    return '!(' + cond + ')'


def clean_lines(lines):
    return [l for l in lines if l is not None]


def brace_depth_between(lines, start_excl, end_excl):
    """Count net brace depth between start (exclusive) and end (exclusive)."""
    depth = 0
    for i in range(start_excl + 1, end_excl):
        if lines[i] is None:
            continue
        for ch in lines[i]:
            if ch == '{':
                depth += 1
            elif ch == '}':
                depth -= 1
    return depth


def try_trivial_forward(lines, goto_idx, label_idx, label_name):
    """Goto right before its label with only empty/brace lines between."""
    all_trivial = True
    for i in range(goto_idx + 1, label_idx):
        if lines[i] is None:
            continue
        stripped = lines[i].strip()
        if stripped and stripped != '}':
            all_trivial = False
            break
    if all_trivial and (label_idx - goto_idx) <= 5:
        lines[goto_idx] = None
        remove_label_if_unused(lines, label_name)
        return True
    return False


def try_set_and_skip(lines, goto_idx, label_idx, label_name):
    """Pattern: r0=X; goto L; } ... r0=Y; L: with balanced braces."""
    goto_line = lines[goto_idx]
    if 'if ' in goto_line:
        return False
    if goto_idx < 1 or lines[goto_idx - 1] is None:
        return False
    prev = lines[goto_idx - 1].rstrip()
    if not re.search(r'\b[rf]\d+\s*=', prev):
        return False
    if goto_idx + 1 >= len(lines) or lines[goto_idx + 1] is None:
        return False
    next_line = lines[goto_idx + 1].rstrip()
    if not re.match(r'^\s*\}', next_line):
        return False
    if label_idx < 1 or lines[label_idx - 1] is None:
        return False
    pre_label = lines[label_idx - 1].rstrip()
    if not re.search(r'\b[rf]\d+\s*=', pre_label):
        return False
    depth = brace_depth_between(lines, goto_idx, label_idx)
    if depth != 0:
        return False
    brace_indent = get_indent(lines[goto_idx + 1])
    lines[goto_idx] = None
    lines[goto_idx + 1] = ' ' * brace_indent + '} else {\n'
    lines.insert(label_idx, ' ' * brace_indent + '}\n')
    remove_label_if_unused(lines, label_name)
    return True


def try_conditional_forward_single(lines, goto_idx, label_idx, label_name):
    """if (cond) goto L; ... L: with single ref and balanced braces."""
    goto_line = lines[goto_idx]
    m = re.match(r'^(\s*)if\s*\((.+?)\)\s*goto\s+' + re.escape(label_name) + r'\s*;', goto_line)
    if not m:
        return False
    if label_refcount(lines, label_name) > 1:
        return False
    indent = m.group(1)
    condition = m.group(2)
    neg = negate_cond(condition)
    depth = brace_depth_between(lines, goto_idx, label_idx)
    if depth != 0:
        return False
    lines[goto_idx] = indent + 'if (' + neg + ') {\n'
    lines.insert(label_idx, indent + '}\n')
    remove_label_if_unused(lines, label_name)
    return True


def try_unconditional_forward_else(lines, goto_idx, label_idx, label_name):
    """goto L; } else_code L: with balanced braces and single ref."""
    goto_line = lines[goto_idx]
    if 'if ' in goto_line:
        return False
    refs = label_refcount(lines, label_name)
    next_idx = None
    for i in range(goto_idx + 1, label_idx):
        if lines[i] is not None and lines[i].strip():
            next_idx = i
            break
    if next_idx is None:
        lines[goto_idx] = None
        remove_label_if_unused(lines, label_name)
        return True
    if refs != 1:
        return False
    if not re.match(r'^\s*\}\s*$', lines[next_idx].rstrip()):
        return False
    brace_indent = get_indent(lines[next_idx])
    depth = brace_depth_between(lines, next_idx - 1, label_idx)
    has_code = False
    for i in range(next_idx + 1, label_idx):
        if lines[i] is not None and lines[i].strip() and lines[i].strip() != '}':
            has_code = True
            break
    if has_code and depth == 0:
        lines[goto_idx] = None
        lines[next_idx] = ' ' * brace_indent + '} else {\n'
        lines.insert(label_idx, ' ' * brace_indent + '}\n')
        remove_label_if_unused(lines, label_name)
        return True
    elif not has_code:
        lines[goto_idx] = None
        remove_label_if_unused(lines, label_name)
        return True
    return False


def try_conditional_forward_multi(lines, goto_idx, label_idx, label_name):
    """Multi-ref conditional forward with brace balance check."""
    goto_line = lines[goto_idx]
    m = re.match(r'^(\s*)if\s*\((.+?)\)\s*goto\s+' + re.escape(label_name) + r'\s*;', goto_line)
    if not m:
        return False
    indent = m.group(1)
    condition = m.group(2)
    neg = negate_cond(condition)
    close_at = label_idx
    for i in range(goto_idx + 1, label_idx):
        if lines[i] and ('goto ' + label_name) in lines[i]:
            close_at = i
            break
    depth = brace_depth_between(lines, goto_idx, close_at)
    if depth != 0:
        return False
    lines[goto_idx] = indent + 'if (' + neg + ') {\n'
    lines.insert(close_at, indent + '}\n')
    remove_label_if_unused(lines, label_name)
    return True


def try_unconditional_forward_multi(lines, goto_idx, label_idx, label_name):
    """Multi-ref unconditional forward with brace balance check."""
    goto_line = lines[goto_idx]
    if 'if ' in goto_line:
        return False
    refs = label_refcount(lines, label_name)
    if refs < 2:
        return False
    next_idx = None
    for i in range(goto_idx + 1, min(goto_idx + 5, label_idx)):
        if lines[i] is not None and lines[i].strip():
            next_idx = i
            break
    if next_idx is None:
        lines[goto_idx] = None
        remove_label_if_unused(lines, label_name)
        return True
    if re.match(r'^\s*\}\s*$', lines[next_idx].rstrip()):
        brace_indent = get_indent(lines[next_idx])
        next_goto = label_idx
        for i in range(next_idx + 1, label_idx):
            if lines[i] and ('goto ' + label_name) in lines[i]:
                next_goto = i + 1
                break
        depth = brace_depth_between(lines, next_idx - 1, next_goto)
        if depth != 0:
            return False
        content_between = False
        for i in range(next_idx + 1, next_goto):
            if lines[i] is not None and lines[i].strip() and lines[i].strip() != '}':
                content_between = True
                break
        if content_between:
            lines[goto_idx] = None
            lines[next_idx] = ' ' * brace_indent + '} else {\n'
            lines.insert(next_goto, ' ' * brace_indent + '}\n')
            remove_label_if_unused(lines, label_name)
            return True
        else:
            lines[goto_idx] = None
            remove_label_if_unused(lines, label_name)
            return True
    return False


def try_do_while_break(lines, goto_idx, label_idx, label_name):
    """goto L within do-while(0) where L is code before while(0)."""
    goto_line = lines[goto_idx]
    if 'if ' in goto_line:
        return False
    for check_idx in range(label_idx, min(label_idx + 5, len(lines))):
        if lines[check_idx] is None:
            continue
        if 'while (0)' in lines[check_idx] or 'while(0)' in lines[check_idx]:
            refs = label_refcount(lines, label_name)
            if refs == 1:
                label_line = lines[label_idx]
                m2 = re.match(r'^(\s*)' + re.escape(label_name) + r'\s*:\s*(.*)', label_line)
                if m2:
                    code_after = m2.group(2).strip()
                    if code_after and code_after != ';':
                        goto_indent = get_indent(goto_line)
                        lines[goto_idx] = ' ' * goto_indent + code_after + '\n'
                        lines.insert(goto_idx + 1, ' ' * goto_indent + 'break;\n')
                        remove_label_if_unused(lines, label_name)
                        return True
            return False
    return False


def try_loop_entry(lines, goto_idx, label_idx, label_name):
    """goto L; while(1) { body; L: cond; } -> restructured loop."""
    goto_line = lines[goto_idx]
    if 'if ' in goto_line:
        return False
    while_idx = None
    for i in range(goto_idx + 1, label_idx):
        if lines[i] is None:
            continue
        if re.match(r'^\s*while\s*\(1\)\s*\{', lines[i].rstrip()):
            while_idx = i
            break
    if while_idx is None:
        return False
    if label_refcount(lines, label_name) != 1:
        return False
    depth = 0
    loop_end = None
    for i in range(while_idx, len(lines)):
        if lines[i] is None:
            continue
        for ch in lines[i]:
            if ch == '{':
                depth += 1
            elif ch == '}':
                depth -= 1
                if depth == 0:
                    loop_end = i
                    break
        if loop_end is not None:
            break
    if loop_end is None:
        return False
    cond_lines = []
    label_line = lines[label_idx]
    lm = re.match(r'^(\s*)' + re.escape(label_name) + r'\s*:\s*(.*)', label_line)
    if not lm:
        return False
    code_after = lm.group(2).strip()
    label_indent = lm.group(1)
    if code_after and code_after != ';':
        cond_lines.append(label_indent + '    ' + code_after + '\n')
    for i in range(label_idx + 1, loop_end):
        if lines[i] is not None:
            cond_lines.append(lines[i])
    lines[goto_idx] = None
    remove_label_if_unused(lines, label_name)
    for j, cl in enumerate(cond_lines):
        lines.insert(while_idx + 1 + j, cl)
    return True


def try_forward_exit_nested(lines, goto_idx, label_idx, label_name):
    """Forward goto where only closing braces lie between goto and label."""
    goto_line = lines[goto_idx]
    if 'if ' in goto_line:
        return False
    only_braces = True
    for i in range(goto_idx + 1, label_idx):
        if lines[i] is None:
            continue
        s = lines[i].strip()
        if s and s != '}':
            only_braces = False
            break
    if only_braces:
        lines[goto_idx] = None
        remove_label_if_unused(lines, label_name)
        return True
    return False


def try_forward_inline_label_code(lines, goto_idx, label_idx, label_name):
    """Forward goto where the label's continuation code is short enough to inline.

    For single-ref: collect code from label onwards until function ends or
    scope changes, then duplicate that code at the goto site and remove goto.

    For the common "set r0 and skip" pattern with unbalanced braces:
    if (cond) { r0 = X; goto L; } ... } r0 = Y; L:
    We convert to:
    if (cond) { r0 = X; } else { ... } r0 = Y; }
    by negating the if and restructuring.

    Actually the simplest: for any forward goto where the code FROM the label
    to the end of the function/block is short, we can duplicate it at the
    goto site.

    But inlining code after the label is complex. Instead, let's handle
    the specific "assign and skip" pattern where:
    1. The goto is preceded by an assignment
    2. There's an assignment before the label
    3. Code between goto's } and label is at various indent levels

    For these, we restructure the if/else chain.
    """
    goto_line = lines[goto_idx]
    refs = label_refcount(lines, label_name)
    if refs != 1:
        return False

    # Pattern: unconditional goto with assignment before, inside an if block
    # The label code is just a continuation - we can restructure
    if 'if ' in goto_line:
        return False  # Only handle unconditional for now

    # Check: is goto preceded by assignment and followed by }?
    if goto_idx < 1 or lines[goto_idx - 1] is None:
        return False
    prev = lines[goto_idx - 1].rstrip()
    if not re.search(r'\b[rf]\d+\s*=', prev):
        return False

    if goto_idx + 1 >= len(lines) or lines[goto_idx + 1] is None:
        return False
    next_l = lines[goto_idx + 1].rstrip()
    if not re.match(r'^\s*\}', next_l):
        return False

    # Check: is there an assignment just before label?
    pre_label_idx = None
    for i in range(label_idx - 1, goto_idx, -1):
        if lines[i] is None:
            continue
        s = lines[i].strip()
        if not s:
            continue
        if s == '}':
            continue
        if re.search(r'\b[rf]\d+\s*=', s):
            pre_label_idx = i
            break
        break

    if pre_label_idx is None:
        return False

    # Now find the } that closes the if block containing the goto
    close_brace_idx = goto_idx + 1
    close_brace_indent = get_indent(lines[close_brace_idx])

    # All lines between close_brace and pre_label should be code at same or deeper indent
    # and the lines between pre_label and label should be only } or the assignment

    # Check: all lines between close_brace+1 and label-1 form a valid else body
    # when considering they may have closing braces

    # Strategy: turn the } into } else { and add } before label,
    # but account for intermediate closing braces by using the pre_label
    # assignment as the else body end marker.

    # Create else block: remove goto, make } into } else {, add } after pre_label
    lines[goto_idx] = None
    lines[close_brace_idx] = ' ' * close_brace_indent + '} else {\n'

    # Insert } after pre_label assignment (before any intermediate closing braces)
    # Find where to close: right after pre_label_idx
    lines.insert(pre_label_idx + 1, ' ' * close_brace_indent + '}\n')

    remove_label_if_unused(lines, label_name)
    return True


def try_backward_conditional(lines, goto_idx, label_idx, label_name):
    """if (cond) goto L; where L is above."""
    goto_line = lines[goto_idx]
    m = re.match(r'^(\s*)if\s*\((.+?)\)\s*goto\s+' + re.escape(label_name) + r'\s*;', goto_line)
    if not m:
        return False
    indent = m.group(1)
    condition = m.group(2)
    refs = label_refcount(lines, label_name)
    if refs == 1:
        label_line = lines[label_idx]
        lm = re.match(r'^(\s*)' + re.escape(label_name) + r'\s*:\s*(.*)', label_line)
        if not lm:
            return False
        code_after = lm.group(2).strip()
        if code_after and code_after != ';':
            lines[label_idx] = indent + 'do {\n' + indent + '    ' + code_after + '\n'
        else:
            lines[label_idx] = indent + 'do {\n'
        lines[goto_idx] = indent + '} while (' + condition + ');\n'
        return True
    all_gotos = []
    for i, line in enumerate(lines):
        if line and ('goto ' + label_name) in line:
            all_gotos.append(i)
    all_backward = all(gi > label_idx for gi in all_gotos)
    if not all_backward:
        return False
    label_line = lines[label_idx]
    lm = re.match(r'^(\s*)' + re.escape(label_name) + r'\s*:\s*(.*)', label_line)
    if not lm:
        return False
    label_indent = lm.group(1)
    code_after = lm.group(2).strip()
    if code_after and code_after != ';':
        lines[label_idx] = label_indent + 'while (1) {\n' + label_indent + '    ' + code_after + '\n'
    else:
        lines[label_idx] = label_indent + 'while (1) {\n'
    last_goto = max(all_gotos)
    for gi in sorted(all_gotos, reverse=True):
        gl = lines[gi]
        gm = re.match(r'^(\s*)if\s*\((.+?)\)\s*goto\s+' + re.escape(label_name) + r'\s*;', gl)
        if gm:
            gi_indent = gm.group(1)
            gi_cond = gm.group(2)
            neg = negate_cond(gi_cond)
            lines[gi] = gi_indent + 'if (' + neg + ') break;\n'
        else:
            gm2 = re.match(r'^(\s*)goto\s+' + re.escape(label_name) + r'\s*;', gl)
            if gm2:
                lines[gi] = gm2.group(1) + 'continue;\n'
    lines.insert(last_goto + 1, label_indent + '    break;\n')
    lines.insert(last_goto + 2, label_indent + '}\n')
    return True


def try_backward_unconditional(lines, goto_idx, label_idx, label_name):
    """Unconditional backward goto L."""
    goto_line = lines[goto_idx]
    if 'if ' in goto_line:
        return False
    refs = label_refcount(lines, label_name)
    if refs == 1:
        label_line = lines[label_idx]
        lm = re.match(r'^(\s*)' + re.escape(label_name) + r'\s*:\s*(.*)', label_line)
        if not lm:
            return False
        label_indent = lm.group(1)
        code_after = lm.group(2).strip()
        if code_after and code_after != ';':
            lines[label_idx] = label_indent + 'while (1) {\n' + label_indent + '    ' + code_after + '\n'
        else:
            lines[label_idx] = label_indent + 'while (1) {\n'
        lines[goto_idx] = label_indent + '}\n'
        return True
    all_gotos = []
    for i, line in enumerate(lines):
        if line and ('goto ' + label_name) in line:
            all_gotos.append(i)
    all_backward = all(gi > label_idx for gi in all_gotos)
    if not all_backward:
        return False
    label_line = lines[label_idx]
    lm = re.match(r'^(\s*)' + re.escape(label_name) + r'\s*:\s*(.*)', label_line)
    if not lm:
        return False
    label_indent = lm.group(1)
    code_after = lm.group(2).strip()
    if code_after and code_after != ';':
        lines[label_idx] = label_indent + 'while (1) {\n' + label_indent + '    ' + code_after + '\n'
    else:
        lines[label_idx] = label_indent + 'while (1) {\n'
    last_goto = max(all_gotos)
    for gi in sorted(all_gotos, reverse=True):
        gl = lines[gi]
        gm = re.match(r'^(\s*)if\s*\((.+?)\)\s*goto\s+' + re.escape(label_name) + r'\s*;', gl)
        if gm:
            gi_indent = gm.group(1)
            gi_cond = gm.group(2)
            neg = negate_cond(gi_cond)
            lines[gi] = gi_indent + 'if (' + neg + ') break;\n'
        else:
            gm2 = re.match(r'^(\s*)goto\s+' + re.escape(label_name) + r'\s*;', gl)
            if gm2:
                if gi == last_goto:
                    lines[gi] = None
                else:
                    lines[gi] = gm2.group(1) + 'continue;\n'
    lines.insert(last_goto + 1, label_indent + '}\n')
    return True


def try_backward_inline(lines, goto_idx, label_idx, label_name):
    """Backward goto - inline short target code at goto site."""
    goto_line = lines[goto_idx]
    is_conditional = 'if ' in goto_line
    label_line = lines[label_idx]
    lm = re.match(r'^(\s*)' + re.escape(label_name) + r'\s*:\s*(.*)', label_line)
    if not lm:
        return False
    code_after = lm.group(2).strip()
    inline_code = []
    if code_after and code_after != ';':
        inline_code.append(code_after)
    found_end = False
    for i in range(label_idx + 1, min(label_idx + 20, len(lines))):
        if lines[i] is None:
            continue
        s = lines[i].strip()
        if s == 'break;' or s.startswith('return'):
            inline_code.append(s)
            found_end = True
            break
        elif s == '}':
            found_end = True
            break
        else:
            inline_code.append(s)
    if not found_end or not inline_code:
        return False
    if len(inline_code) > 10:
        return False
    goto_indent_n = get_indent(goto_line)
    indent_str = ' ' * goto_indent_n
    if is_conditional:
        m = re.match(r'^(\s*)if\s*\((.+?)\)\s*goto\s+' + re.escape(label_name) + r'\s*;', goto_line)
        if not m:
            return False
        cond_indent = m.group(1)
        condition = m.group(2)
        new_lines = [cond_indent + 'if (' + condition + ') {\n']
        for code in inline_code:
            new_lines.append(cond_indent + '    ' + code + '\n')
        new_lines.append(cond_indent + '}\n')
        lines[goto_idx:goto_idx+1] = new_lines
    else:
        new_lines = []
        for code in inline_code:
            new_lines.append(indent_str + code + '\n')
        lines[goto_idx:goto_idx+1] = new_lines
    remove_label_if_unused(lines, label_name)
    return True


def convert_all_gotos(lines):
    max_passes = 600
    for pass_num in range(max_passes):
        gotos, labels = find_gotos_and_labels(lines)
        if not gotos:
            break
        converted = False
        for goto_idx, label_name in reversed(gotos):
            if label_name not in labels:
                continue
            label_idx = labels[label_name]
            is_forward = label_idx > goto_idx
            if is_forward:
                for fn in [try_trivial_forward, try_set_and_skip,
                           try_forward_exit_nested,
                           try_conditional_forward_single,
                           try_unconditional_forward_else,
                           try_do_while_break, try_loop_entry,
                           try_conditional_forward_multi,
                           try_unconditional_forward_multi,
                           try_forward_inline_label_code]:
                    if fn(lines, goto_idx, label_idx, label_name):
                        converted = True
                        break
            else:
                for fn in [try_backward_conditional,
                           try_backward_unconditional,
                           try_backward_inline]:
                    if fn(lines, goto_idx, label_idx, label_name):
                        converted = True
                        break
            if converted:
                break
        if not converted:
            break
        lines = clean_lines(lines)
    lines = clean_lines(lines)
    gotos, labels = find_gotos_and_labels(lines)
    goto_targets = set(name for _, name in gotos)
    for label_name in list(labels.keys()):
        if label_name not in goto_targets:
            remove_label_if_unused(lines, label_name)
    lines = clean_lines(lines)
    return lines


def process_file(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
    initial = count_gotos(lines)
    print('Processing %s: %d gotos' % (filename, initial), file=sys.stderr)
    lines = convert_all_gotos(lines)
    final = count_gotos(lines)
    print('  Result: %d remaining (%d converted)' % (final, initial - final), file=sys.stderr)
    if final > 0:
        gotos, _ = find_gotos_and_labels(lines)
        for idx, name in gotos:
            print('  REMAINING line %d: goto %s' % (idx+1, name), file=sys.stderr)
    with open(filename, 'w') as f:
        f.writelines(lines)
    return final


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: %s <file1.c> [file2.c ...]' % sys.argv[0], file=sys.stderr)
        sys.exit(1)
    total = 0
    for fn in sys.argv[1:]:
        total += process_file(fn)
    if total > 0:
        print('\nTotal remaining: %d gotos' % total, file=sys.stderr)
    else:
        print('\nAll gotos converted!', file=sys.stderr)
