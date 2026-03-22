#!/usr/bin/env python3
"""
Second-pass goto eliminator for convergence patterns.

Handles the case where 'goto L' is the last meaningful statement
inside an if/else block, and L is reachable by normal fallthrough
after the enclosing block(s) close.

This is the dominant remaining pattern:
    if (cond) {
        ...
        goto L;    <-- remove: falls through to L after }
    }
    ... more code ...
    L: ;

The goto is NOT redundant in general (there's code between } and L).
But if the goto is INSIDE an if-block and L is AFTER the if-block,
it's semantically equivalent to: the if-block body doesn't execute
the code after } (because it gotos past it).

Strategy: Convert to if-else chains:
    if (cond) {
        ...
    } else {
        ... more code ...
    }
"""
import re
import sys


def count_gotos(text):
    return len(re.findall(r'\bgoto\s+\w+\s*;', text))


def label_uses(lines, label):
    pat = 'goto ' + label
    return sum(1 for l in lines if pat in l)


def is_label(line):
    s = line.strip()
    m = re.match(r'^(\w+)\s*:\s*;?\s*$', s)
    if m and not s.startswith('default') and not s.startswith('case'):
        return m.group(1)
    return None


def find_label(lines, label, start=0, end=None):
    if end is None:
        end = len(lines)
    t = label + ':'
    for i in range(start, end):
        s = lines[i].strip()
        if s == t or s == t + ' ;' or s.startswith(t + ' '):
            return i
    return -1


def extract_goto(line):
    s = line.strip()
    m = re.match(r'if\s*\((.+?)\)\s+goto\s+(\w+)\s*;$', s)
    if m:
        return (m.group(1), m.group(2))
    m = re.match(r'goto\s+(\w+)\s*;$', s)
    if m:
        return (None, m.group(1))
    return None


def has_live_label(lines, start, end):
    """Check if any label in lines[start:end] is targeted by goto from outside."""
    for i in range(start, end):
        lbl = is_label(lines[i])
        if lbl:
            for j, l in enumerate(lines):
                if (j < start or j >= end) and f'goto {lbl}' in l:
                    return True
    return False


def get_indent(line):
    return line[:len(line) - len(line.lstrip())] if line.strip() else ''


def indent_block(block, extra='    '):
    return [extra + l if l.strip() else l for l in block]


def negate(cond):
    cond = cond.strip()
    if cond.startswith('!(') and cond.endswith(')'):
        return cond[2:-1]
    ops = {'!=': '==', '==': '!=', '>=': '<', '<=': '>', '>': '<=', '<': '>='}
    m = re.search(r'(.*?)\s*(!=|==|>=|<=|>(?!=)|<(?!=))\s*(.*)', cond)
    if m:
        lhs, op, rhs = m.groups()
        if op in ops:
            return f'{lhs} {ops[op]} {rhs}'
    return f'!({cond})'


def find_functions(lines):
    """Find function boundaries."""
    funcs = []
    i = 0
    while i < len(lines):
        if re.match(
            r'^(void|u32|s32|u16|s16|u8|s8|f32|f64|int|void\s*\*|static\s+\w+|\w+\s*\*)\s+\w+\s*\(',
            lines[i],
        ):
            if lines[i].rstrip().endswith(';'):
                i += 1
                continue
            j = i
            while j < len(lines) and '{' not in lines[j]:
                if lines[j].rstrip().endswith(';') and j > i:
                    break
                j += 1
            if j >= len(lines):
                i += 1
                continue
            depth = 0
            for k in range(j, len(lines)):
                depth += lines[k].count('{') - lines[k].count('}')
                if depth == 0:
                    funcs.append((i, k))
                    i = k + 1
                    break
            else:
                i += 1
        else:
            i += 1
    return funcs


def process_func(func_lines):
    """Convert forward-goto-to-convergence-label patterns.

    Pattern 1: goto L is last stmt before }, L is after the }
    Convert by wrapping the code between } and L in an else block.

    Pattern 2: if (cond) goto L; <code> L: ; where code doesn't terminate
    Convert to if (!cond) { <code> }
    """
    lines = list(func_lines)
    orig = count_gotos('\n'.join(lines))
    if orig == 0:
        return lines, 0

    # Iterative passes
    prev = orig + 1
    rounds = 0
    while count_gotos('\n'.join(lines)) > 0 and count_gotos('\n'.join(lines)) < prev and rounds < 50:
        prev = count_gotos('\n'.join(lines))
        rounds += 1
        did_something = False

        # Pass A: goto L as last statement before }, where L is forward
        # and the code between } and L has no external label targets.
        # Convert: remove goto, wrap code-between-close-and-label in else.
        i = 0
        while i < len(lines):
            g = extract_goto(lines[i])
            if g and g[0] is None:
                lbl = g[1]
                # Check next non-empty is }
                j = i + 1
                while j < len(lines) and not lines[j].strip():
                    j += 1
                if j < len(lines) and lines[j].strip() == '}':
                    li = find_label(lines, lbl, j + 1)
                    if li > j:
                        code_between = lines[j + 1:li]
                        if not has_live_label(lines, j + 1, li):
                            ind = get_indent(lines[j])
                            lines[i] = ''
                            lines[j] = ind + '} else {'
                            lines[li:li] = [ind + '}']
                            for ci in range(j + 1, li):
                                if lines[ci].strip():
                                    lines[ci] = '    ' + lines[ci]
                            if label_uses(lines, lbl) == 0:
                                new_li = find_label(lines, lbl)
                                if new_li >= 0:
                                    lines[new_li] = ''
                            did_something = True
                            break
            i += 1

        if did_something:
            continue

        # Pass B: if (cond) goto L; <code> L: where L is multi-use
        # and code doesn't have external label targets.
        # Convert to if (!cond) { code }
        i = 0
        while i < len(lines):
            g = extract_goto(lines[i])
            if g and g[0] is not None:
                cond, lbl = g
                li = find_label(lines, lbl, i + 1)
                if li > i:
                    code = lines[i + 1:li]
                    if not has_live_label(lines, i + 1, li):
                        # Check code doesn't have gotos that jump backward
                        # or to labels before i (which would be unsafe)
                        safe = True
                        for cl in code:
                            cg = extract_goto(cl)
                            if cg:
                                _, clbl = cg
                                cli = find_label(lines, clbl)
                                if cli >= 0 and cli < i:
                                    safe = False
                                    break
                        if safe:
                            ind = get_indent(lines[i])
                            neg = negate(cond)
                            new = [f'{ind}if ({neg}) {{']
                            new += indent_block(code)
                            new += [f'{ind}}}']
                            uses = label_uses(lines, lbl)
                            if uses <= 1:
                                lines[i:li + 1] = new
                            else:
                                lines[i:li] = new
                            did_something = True
                            break
            i += 1

        if did_something:
            continue

        # Pass C: backward single-use gotos
        i = 0
        while i < len(lines):
            g = extract_goto(lines[i])
            if g:
                cond, lbl = g
                li = find_label(lines, lbl, 0, i)
                if li >= 0 and label_uses(lines, lbl) == 1:
                    ind = get_indent(lines[li])
                    body = lines[li + 1:i]
                    if cond:
                        new = [f'{ind}do {{'] + indent_block(body) + [f'{ind}}} while ({cond});']
                    else:
                        new = [f'{ind}while (1) {{'] + indent_block(body) + [f'{ind}}}']
                    lines[li:i + 1] = new
                    did_something = True
                    break
            i += 1

        if not did_something:
            break

    # Cleanup unused labels
    changed_cleanup = True
    while changed_cleanup:
        changed_cleanup = False
        new = []
        for line in lines:
            lbl = is_label(line)
            if lbl and label_uses(lines, lbl) == 0:
                changed_cleanup = True
                continue
            new.append(line)
        lines = new

    # Cleanup blank lines
    final = []
    for line in lines:
        if not line.strip() and final and not final[-1].strip():
            continue
        final.append(line)

    result = count_gotos('\n'.join(final))
    return final, orig - result


def process_file(filename):
    with open(filename) as f:
        all_lines = f.read().split('\n')

    functions = find_functions(all_lines)
    total_elim = 0
    total_remain = 0

    for start, end in reversed(functions):
        func_lines = all_lines[start:end + 1]
        gc = count_gotos('\n'.join(func_lines))
        if gc == 0:
            continue

        new_lines, elim = process_func(func_lines)
        if elim > 0:
            all_lines[start:end + 1] = new_lines
            total_elim += elim
            remain = gc - elim
            total_remain += remain
            name = func_lines[0].strip()[:60]
            print(f'  {name}: -{elim}/{gc} ({remain} left)')
        else:
            total_remain += gc

    with open(filename, 'w') as f:
        f.write('\n'.join(all_lines))

    print(f'  TOTAL: {total_elim} eliminated, {total_remain} remaining')
    return total_elim, total_remain


if __name__ == '__main__':
    ge, gr = 0, 0
    for fn in sys.argv[1:]:
        print(f'\n=== {fn} ===')
        e, r = process_file(fn)
        ge += e
        gr += r
    print(f'\n=== GRAND: {ge} eliminated, {gr} remaining ===')
