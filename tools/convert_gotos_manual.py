#!/usr/bin/env python3
"""
Manual goto converter for Pokemon Colosseum decomp.
Handles patterns the automated tools miss:
1. Single-ref forward conditional goto -> if/else wrapping
2. Single-ref forward unconditional goto (skip block) -> if/else
3. Single-ref backward goto -> while/do-while loops
4. Multi-ref forward gotos (all conditional) to same label -> do{...}while(0)+break
5. if(cond) goto L; ... L: return; -> if(!cond) { ... } return;
"""
import re
import sys
import os
import copy


def invert_cond(cond):
    """Invert a C condition. Returns None if unable."""
    cond = cond.strip()

    # Handle parenthesized expressions
    if cond.startswith('(') and cond.endswith(')'):
        inner = cond[1:-1]
        # Check balanced parens
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

    # Handle negation
    if cond.startswith('!'):
        return cond[1:]

    # Handle compound &&
    if ' && ' in cond and ' || ' not in cond:
        parts = cond.split(' && ')
        inverted = []
        for p in parts:
            inv = invert_cond(p.strip())
            if inv is None:
                return None
            inverted.append(inv)
        return ' || '.join(inverted)

    # Handle compound ||
    if ' || ' in cond and ' && ' not in cond:
        parts = cond.split(' || ')
        inverted = []
        for p in parts:
            inv = invert_cond(p.strip())
            if inv is None:
                return None
            inverted.append(inv)
        return ' && '.join(inverted)

    replacements = [
        (' == ', ' != '),
        (' != ', ' == '),
        (' < ', ' >= '),
        (' >= ', ' < '),
        (' > ', ' <= '),
        (' <= ', ' > '),
    ]
    for old, new in replacements:
        if old in cond and cond.count(old.strip()) == 1:
            return cond.replace(old, new, 1)
    return None


def build_indices(lines):
    """Build label position, reference count, and source maps."""
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
    """Get the indentation of a line."""
    return len(line) - len(line.lstrip())


def indent_lines(lines, start, end, spaces=4):
    """Add indentation to a range of lines."""
    for i in range(start, end):
        if lines[i].strip():  # Don't indent empty lines
            lines[i] = ' ' * spaces + lines[i]


def check_brace_balance(lines, start, end):
    """Check brace balance in a range of lines. Returns depth."""
    depth = 0
    for i in range(start, end):
        depth += lines[i].count('{') - lines[i].count('}')
    return depth


def has_other_gotos_in_range(lines, start, end, allowed_labels=None):
    """Check if there are gotos in range that target labels outside the range or outside allowed set."""
    if allowed_labels is None:
        allowed_labels = set()
    label_pos, _, _ = build_indices(lines)
    for i in range(start, end):
        for m in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[i]):
            lbl = m.group(1)
            if lbl in allowed_labels:
                continue
            if lbl in label_pos:
                pos = label_pos[lbl]
                if pos < start or pos >= end:
                    return True
    return False


def has_labels_referenced_from_outside(lines, start, end):
    """Check if any labels in range are referenced from outside."""
    _, _, label_sources = build_indices(lines)
    for i in range(start, end):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[i])
        if m:
            lbl = m.group(1)
            for src in label_sources.get(lbl, []):
                if src < start or src >= end:
                    return True
    return False


def convert_single_ref_forward_conditional(lines):
    """Convert: if (cond) goto L; ... L: ; -> if (!cond) { ... }"""
    changes = 0
    label_pos, label_refs, label_sources = build_indices(lines)

    converted = set()

    for lbl, refs in sorted(label_refs.items(), key=lambda x: label_sources.get(x[0], [0])[0] if x[0] in label_sources else 0):
        if refs != 1 or lbl not in label_pos:
            continue
        if lbl in converted:
            continue

        src_line = label_sources[lbl][0]
        dst_line = label_pos[lbl]

        if src_line >= dst_line:
            continue  # backward goto, skip

        stripped = lines[src_line].strip()

        # Pattern: if (cond) goto L;
        m = re.match(r'^if\s*\((.+)\)\s*goto\s+' + re.escape(lbl) + r'\s*;\s*$', stripped)
        if not m:
            continue

        cond = m.group(1)
        inv_cond = invert_cond(cond)
        if inv_cond is None:
            continue

        # Check brace balance between src and dst
        balance = check_brace_balance(lines, src_line + 1, dst_line)
        if balance != 0:
            continue

        # Check no labels in range are referenced from outside
        has_external = False
        _, _, all_sources = build_indices(lines)
        for i in range(src_line + 1, dst_line):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[i])
            if lm:
                inner_lbl = lm.group(1)
                for s in all_sources.get(inner_lbl, []):
                    if s < src_line or s >= dst_line:
                        has_external = True
                        break
                if has_external:
                    break
        if has_external:
            continue

        # Check no gotos in range point outside the range (except to our label)
        has_escape = False
        for i in range(src_line + 1, dst_line):
            for gm in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[i]):
                target = gm.group(1)
                if target == lbl:
                    continue
                if target in label_pos:
                    t = label_pos[target]
                    if t <= src_line or t > dst_line:
                        has_escape = True
                        break
            if has_escape:
                break
        if has_escape:
            continue

        # Check there's actual code between src and dst
        has_code = False
        for i in range(src_line + 1, dst_line):
            if lines[i].strip() and not re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[i]):
                has_code = True
                break
        if not has_code:
            # Just remove the goto and label
            lines[src_line] = ''
            lines[dst_line] = ''
            changes += 1
            converted.add(lbl)
            label_pos, label_refs, label_sources = build_indices(lines)
            continue

        # Apply: wrap in if (!cond) { ... }
        src_indent = ' ' * get_indent(lines[src_line])
        lines[src_line] = src_indent + 'if (' + inv_cond + ') {\n'

        # Indent body
        for i in range(src_line + 1, dst_line):
            if lines[i].strip():
                lines[i] = '    ' + lines[i]

        # Replace label with closing brace
        lines[dst_line] = src_indent + '}\n'

        changes += 1
        converted.add(lbl)
        # Rebuild indices after each conversion
        label_pos, label_refs, label_sources = build_indices(lines)

    return changes


def convert_single_ref_forward_unconditional(lines):
    """Convert: goto L; code... L: ; -> skip pattern (only for small blocks)"""
    changes = 0
    label_pos, label_refs, label_sources = build_indices(lines)

    for lbl, refs in list(label_refs.items()):
        if refs != 1 or lbl not in label_pos:
            continue

        src_line = label_sources[lbl][0]
        dst_line = label_pos[lbl]

        if src_line >= dst_line:
            continue

        stripped = lines[src_line].strip()

        # Pattern: goto L; (unconditional, must be sole statement on line)
        if not re.match(r'^goto\s+' + re.escape(lbl) + r'\s*;\s*$', stripped):
            continue

        # Check brace balance
        balance = check_brace_balance(lines, src_line + 1, dst_line)
        if balance != 0:
            continue

        # Check no external label refs
        has_external = False
        for i in range(src_line + 1, dst_line):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[i])
            if lm:
                inner_lbl = lm.group(1)
                for s in label_sources.get(inner_lbl, []):
                    if s < src_line or s >= dst_line:
                        has_external = True
                        break
                if has_external:
                    break
        if has_external:
            continue

        # Check no gotos in body escape
        has_escape = False
        for i in range(src_line + 1, dst_line):
            for gm in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[i]):
                target = gm.group(1)
                if target in label_pos:
                    t = label_pos[target]
                    if t <= src_line or t > dst_line:
                        has_escape = True
                        break
            if has_escape:
                break
        if has_escape:
            continue

        # The line before the goto should tell us the context
        # Check if there's an if statement right before that we can add an else to
        prev_line = src_line - 1
        while prev_line >= 0 and not lines[prev_line].strip():
            prev_line -= 1

        if prev_line >= 0 and lines[prev_line].strip() == '}':
            # Could be else block - find the matching if
            # For now, wrap in if(1) to create an else
            pass

        # Simple approach: just wrap the skipped code in an if(0) { } with comment
        # Actually better: look if we're right after a closing brace of an if block
        # and the goto is the "else" path
        # For now, skip complex ones. We'll handle this with the else pattern.

        # Check: is the goto right after a closing brace of an if?
        # goto L; skippedcode... L: is really like: else { skippedcode }
        # Let's check if prev is '}'
        if prev_line >= 0 and lines[prev_line].strip() == '}':
            src_indent = ' ' * get_indent(lines[src_line])
            lines[src_line] = src_indent + 'else {\n'
            for i in range(src_line + 1, dst_line):
                if lines[i].strip():
                    lines[i] = '    ' + lines[i]
            lines[dst_line] = src_indent + '}\n'
            changes += 1
            label_pos, label_refs, label_sources = build_indices(lines)

    return changes


def convert_single_ref_backward(lines):
    """Convert backward single-ref gotos to while/do-while loops."""
    changes = 0
    label_pos, label_refs, label_sources = build_indices(lines)

    for lbl, refs in list(label_refs.items()):
        if refs != 1 or lbl not in label_pos:
            continue

        src_line = label_sources[lbl][0]
        dst_line = label_pos[lbl]

        if src_line <= dst_line:
            continue  # forward goto, skip

        # Backward goto: src > dst
        # Check brace balance from label to goto
        balance = check_brace_balance(lines, dst_line, src_line + 1)
        if balance != 0:
            continue

        stripped = lines[src_line].strip()

        # Pattern 1: if (cond) goto L; -> do { ... } while (cond);
        m = re.match(r'^if\s*\((.+)\)\s*goto\s+' + re.escape(lbl) + r'\s*;\s*$', stripped)
        if m:
            cond = m.group(1)

            # Check no external refs to inner labels
            has_external = False
            for i in range(dst_line + 1, src_line):
                lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[i])
                if lm:
                    inner_lbl = lm.group(1)
                    for s in label_sources.get(inner_lbl, []):
                        if s < dst_line or s > src_line:
                            has_external = True
                            break
                    if has_external:
                        break
            if has_external:
                continue

            # Check no gotos in body escape
            has_escape = False
            for i in range(dst_line + 1, src_line):
                for gm in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[i]):
                    target = gm.group(1)
                    if target == lbl:
                        continue
                    if target in label_pos:
                        t = label_pos[target]
                        if t < dst_line or t > src_line:
                            has_escape = True
                            break
                if has_escape:
                    break
            if has_escape:
                continue

            src_indent = ' ' * get_indent(lines[dst_line])
            lines[dst_line] = src_indent + 'do {\n'
            for i in range(dst_line + 1, src_line):
                if lines[i].strip():
                    lines[i] = '    ' + lines[i]
            lines[src_line] = src_indent + '} while (' + cond + ');\n'
            changes += 1
            label_pos, label_refs, label_sources = build_indices(lines)
            continue

        # Pattern 2: goto L; (unconditional) -> while(1) { ... break; }
        # This is infinite loop if no break condition - skip for safety

    return changes


def convert_multi_ref_forward_do_while(lines):
    """Convert multi-ref forward gotos to do{...}while(0) with breaks.
    Handles both conditional and unconditional gotos before the label."""
    changes = 0
    label_pos, label_refs, label_sources = build_indices(lines)

    # Sort by label position (process from bottom up to avoid index shifts)
    sorted_labels = sorted(
        [(lbl, label_pos[lbl]) for lbl in label_refs if label_refs[lbl] >= 2 and lbl in label_pos],
        key=lambda x: -x[1]
    )

    for lbl, dst_line in sorted_labels:
        sources = label_sources.get(lbl, [])
        refs = label_refs.get(lbl, 0)

        if refs < 2:
            continue

        # All sources must be before the label
        if not all(s < dst_line for s in sources):
            continue

        # All sources must be either:
        # - conditional goto: if (cond) goto L;
        # - unconditional goto: goto L;
        goto_info = []  # (line_idx, type, cond_or_None)
        all_valid = True
        for s in sources:
            stripped = lines[s].strip()
            m_cond = re.match(r'^if\s*\((.+)\)\s*goto\s+' + re.escape(lbl) + r'\s*;\s*$', stripped)
            m_uncond = re.match(r'^goto\s+' + re.escape(lbl) + r'\s*;\s*$', stripped)
            if m_cond:
                goto_info.append((s, 'cond', m_cond.group(1)))
            elif m_uncond:
                goto_info.append((s, 'uncond', None))
            else:
                all_valid = False
                break
        if not all_valid:
            continue

        first_src = min(sources)

        # Check brace balance from first_src to dst
        balance = check_brace_balance(lines, first_src, dst_line)
        if balance != 0:
            continue

        # Check no labels in [first_src, dst_line) are referenced from outside
        has_external = False
        for i in range(first_src, dst_line):
            lm = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[i])
            if lm:
                inner_lbl = lm.group(1)
                if inner_lbl == lbl:
                    continue
                for s in label_sources.get(inner_lbl, []):
                    if s < first_src or s >= dst_line:
                        has_external = True
                        break
                if has_external:
                    break
        if has_external:
            continue

        # Check the label itself is only referenced from within the range
        for s in sources:
            if s < first_src:
                has_external = True
                break
        if has_external:
            continue

        # Check no gotos in range escape to outside
        has_escape = False
        for i in range(first_src, dst_line):
            for gm in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[i]):
                target = gm.group(1)
                if target == lbl:
                    continue
                if target in label_pos:
                    t = label_pos[target]
                    if t < first_src or t > dst_line:
                        has_escape = True
                        break
            if has_escape:
                break
        if has_escape:
            continue

        # Apply: wrap in do { ... } while(0);
        src_indent = ' ' * get_indent(lines[first_src])

        # Convert all gotos to breaks
        for s, gtype, cond in goto_info:
            s_indent = ' ' * get_indent(lines[s])
            if gtype == 'cond':
                lines[s] = s_indent + 'if (' + cond + ') break;\n'
            else:
                lines[s] = s_indent + 'break;\n'

        # Insert do { before first_src
        lines[first_src] = src_indent + 'do {\n' + lines[first_src]

        # Replace label with } while(0);
        lines[dst_line] = src_indent + '} while (0);\n'

        changes += refs
        # Rebuild indices
        label_pos, label_refs, label_sources = build_indices(lines)

    return changes


def remove_unreferenced_labels(lines):
    """Remove labels that have no goto references."""
    label_pos, label_refs, label_sources = build_indices(lines)
    changes = 0
    for lbl, pos in label_pos.items():
        if label_refs.get(lbl, 0) == 0:
            lines[pos] = ''
            changes += 1
    return changes


def remove_goto_to_next_label(lines):
    """Remove 'goto L;' when L: is the very next non-empty line."""
    changes = 0
    label_pos, label_refs, label_sources = build_indices(lines)

    for i, line in enumerate(lines):
        m = re.match(r'^(\s*)goto\s+(L_[0-9A-Fa-f]+)\s*;\s*$', line)
        if not m:
            continue
        lbl = m.group(2)
        if lbl not in label_pos:
            continue

        # Find next non-empty line
        next_line = i + 1
        while next_line < len(lines) and not lines[next_line].strip():
            next_line += 1

        if next_line == label_pos[lbl]:
            lines[i] = ''
            changes += 1
            label_pos, label_refs, label_sources = build_indices(lines)

    return changes


def process_file(filepath):
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()

    initial_gotos = len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\b', content))
    if initial_gotos == 0:
        return 0

    print(f'Processing {filepath}: {initial_gotos} gotos')
    lines = content.split('\n')

    total_changes = 0
    for iteration in range(10):
        c = 0
        c += remove_goto_to_next_label(lines)
        c += convert_single_ref_forward_conditional(lines)
        c += convert_single_ref_forward_unconditional(lines)
        c += convert_single_ref_backward(lines)
        c += convert_multi_ref_forward_do_while(lines)
        c += remove_unreferenced_labels(lines)
        total_changes += c
        if c == 0:
            break
        print(f'  Iteration {iteration+1}: {c} changes')

    content = '\n'.join(lines)
    # Clean up excessive blank lines
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
        print("Usage: convert_gotos_manual.py <file.c> [file2.c ...]")
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
