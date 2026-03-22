#!/usr/bin/env python3
"""
Safe goto converter for Pokemon Colosseum decompilation.
Each pass is designed to never break brace balance or scope.
Works iteratively until no more changes are made.
"""
import re
import sys
import os
import copy


def invert_cond(cond):
    """Invert a condition expression."""
    cond = cond.strip()
    if ' && ' in cond and ' || ' not in cond:
        parts = cond.split(' && ')
        inv = [invert_cond(p.strip()) for p in parts]
        if None in inv:
            return None
        return ' || '.join(inv)
    if ' || ' in cond and ' && ' not in cond:
        parts = cond.split(' || ')
        inv = [invert_cond(p.strip()) for p in parts]
        if None in inv:
            return None
        return ' && '.join(inv)
    for old, new in [(' == ', ' != '), (' != ', ' == '), (' < ', ' >= '),
                     (' >= ', ' < '), (' > ', ' <= '), (' <= ', ' > ')]:
        if old in cond:
            count = 0
            for op_check in [' == ', ' != ', ' < ', ' >= ', ' > ', ' <= ']:
                count += cond.count(op_check)
            if count == 1:
                return cond.replace(old, new, 1)
    return None


def build_indices(lines):
    label_pos = {}
    label_rc = {}
    label_sources = {}
    for i, line in enumerate(lines):
        m = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', line)
        if m:
            label_pos[m.group(1)] = i
    for i, line in enumerate(lines):
        for m in re.finditer(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', line):
            lbl = m.group(1)
            label_rc[lbl] = label_rc.get(lbl, 0) + 1
            if lbl not in label_sources:
                label_sources[lbl] = []
            label_sources[lbl].append(i)
    return label_pos, label_rc, label_sources


def count_gotos(lines):
    return sum(len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\b', line)) for line in lines)


def find_function_ranges(lines):
    ranges = []
    depth = 0
    func_start = None
    for i, line in enumerate(lines):
        opens = line.count('{')
        closes = line.count('}')
        if depth == 0 and opens > 0:
            func_start = i
        depth += opens - closes
        if depth == 0 and func_start is not None:
            ranges.append((func_start, i))
            func_start = None
    return ranges


def verify_brace_balance(lines, func_ranges):
    for start, end in func_ranges:
        depth = 0
        for i in range(start, end + 1):
            depth += lines[i].count('{') - lines[i].count('}')
        if depth != 0:
            return False, start
    return True, -1


def pass_goto_next(lines, lp, rc, ls):
    """Remove goto to immediately following label."""
    changes = 0
    for i in range(len(lines) - 1):
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        label = m.group(2)
        for k in range(i + 1, min(i + 5, len(lines))):
            if lines[k].strip():
                ml = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
                if ml and ml.group(1) == label:
                    lines[i] = ''
                    rc[label] = rc.get(label, 0) - 1
                    changes += 1
                break
    return changes


def pass_skip_return(lines, lp, rc, ls):
    """if (cond) goto L; return; -> if (!cond) return; (multi-ref safe)"""
    changes = 0
    for i in range(len(lines) - 1):
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        indent, cond, label = m.group(1), m.group(2), m.group(3)
        if label not in lp:
            continue
        label_idx = lp[label]
        if label_idx <= i:
            continue
        next_idx = None
        for k in range(i + 1, min(i + 3, len(lines))):
            if lines[k].strip():
                next_idx = k
                break
        if next_idx is None:
            continue
        stripped = lines[next_idx].strip()
        if stripped != 'return;':
            continue
        if label_idx < next_idx + 1:
            continue
        inv = invert_cond(cond)
        if inv is None:
            continue
        lines[i] = indent + 'if (' + inv + ') return;'
        lines[next_idx] = ''
        rc[label] = rc.get(label, 0) - 1
        changes += 1
    return changes


def pass_skip_terminating_block(lines, lp, rc, ls):
    """if (cond) goto L; <code>; return/break; ... L: -> if (!cond) { <code>; return; } (multi-ref safe)"""
    changes = 0
    i = 0
    while i < len(lines):
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            i += 1
            continue
        indent, cond, label = m.group(1), m.group(2), m.group(3)
        if label not in lp:
            i += 1
            continue
        label_idx = lp[label]
        if label_idx <= i:
            i += 1
            continue
        block_lines = []
        has_label = False
        has_goto = False
        for k in range(i + 1, label_idx):
            s = lines[k].strip()
            if not s:
                continue
            if re.match(r'^L_[0-9A-Fa-f]+\s*:\s*;?\s*$', s):
                has_label = True
                break
            if re.search(r'\bgoto\s+L_[0-9A-Fa-f]+\s*;', s):
                has_goto = True
                break
            block_lines.append(k)
        if has_label or has_goto or len(block_lines) == 0 or len(block_lines) > 10:
            i += 1
            continue
        last_stmt = lines[block_lines[-1]].strip()
        if not (last_stmt.startswith('return') or last_stmt == 'break;' or last_stmt == 'continue;'):
            i += 1
            continue
        # Verify brace balance in block
        bd = 0
        for k in block_lines:
            bd += lines[k].count('{') - lines[k].count('}')
        if bd != 0:
            i += 1
            continue
        inv = invert_cond(cond)
        if inv is None:
            i += 1
            continue
        lines[i] = indent + 'if (' + inv + ') {'
        for k in block_lines:
            lines[k] = '    ' + lines[k]
        last_block_line = block_lines[-1]
        lines[last_block_line] = lines[last_block_line] + '\n' + indent + '}'
        for k in range(last_block_line + 1, label_idx):
            lines[k] = ''
        rc[label] = rc.get(label, 0) - 1
        changes += 1
        i += 1
    return changes


def pass_or_chain(lines, lp, rc, ls):
    """Combine consecutive if-gotos to same label."""
    changes = 0
    i = 0
    while i < len(lines):
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            i += 1
            continue
        indent, first_cond, label = m.group(1), m.group(2), m.group(3)
        conds = [first_cond]
        j = i + 1
        while j < len(lines):
            if not lines[j].strip():
                j += 1
                continue
            mj = re.match(r'^' + re.escape(indent) + r'if \((.+)\) goto ' + re.escape(label) + r';$', lines[j])
            if mj:
                conds.append(mj.group(1))
                j += 1
            else:
                break
        if len(conds) > 1:
            lines[i] = indent + 'if (' + ' || '.join(conds) + ') goto ' + label + ';'
            for k in range(i + 1, i + len(conds)):
                if re.match(r'^\s+if \(.+\) goto ' + re.escape(label) + r';$', lines[k]):
                    lines[k] = ''
                    rc[label] = rc.get(label, 0) - 1
                    changes += 1
            i = j
        else:
            i += 1
    return changes


def pass_if_else(lines, lp, rc, ls):
    """if (cond) goto L; <else>; goto END; L: <then>; END: -> if/else"""
    changes = 0
    for i in range(len(lines)):
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        indent, cond, then_label = m.group(1), m.group(2), m.group(3)
        if then_label not in lp or rc.get(then_label, 0) != 1:
            continue
        then_label_idx = lp[then_label]
        if then_label_idx <= i:
            continue
        end_goto_idx = end_label = None
        for k in range(then_label_idx - 1, i, -1):
            s = lines[k].strip()
            if not s:
                continue
            mg = re.match(r'^goto (L_[0-9A-Fa-f]+);$', s)
            if mg:
                end_goto_idx = k
                end_label = mg.group(1)
            break
        if end_goto_idx is None or end_label is None or end_label not in lp:
            continue
        end_label_idx = lp[end_label]
        if end_label_idx <= then_label_idx:
            continue
        has_ext = False
        for k in range(i + 1, end_goto_idx):
            ml = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if ml:
                for src in ls.get(ml.group(1), []):
                    if src < i or src >= end_goto_idx:
                        has_ext = True
                        break
            gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
            if gm and gm.group(1) not in [then_label, end_label]:
                t_lbl = gm.group(1)
                if t_lbl in lp and (lp[t_lbl] < i or lp[t_lbl] > end_label_idx):
                    has_ext = True
        if has_ext:
            continue
        for k in range(then_label_idx + 1, end_label_idx):
            ml = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if ml:
                for src in ls.get(ml.group(1), []):
                    if src < then_label_idx or src >= end_label_idx:
                        has_ext = True
                        break
            gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
            if gm:
                t_lbl = gm.group(1)
                if t_lbl in lp and (lp[t_lbl] < i or lp[t_lbl] > end_label_idx):
                    has_ext = True
        if has_ext:
            continue
        inv = invert_cond(cond)
        if inv is None:
            continue
        lines[i] = indent + 'if (' + inv + ') {'
        for k in range(i + 1, end_goto_idx):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[end_goto_idx] = indent + '} else {'
        lines[then_label_idx] = ''
        for k in range(then_label_idx + 1, end_label_idx):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        rc[then_label] = 0
        rc[end_label] = rc.get(end_label, 0) - 1
        if rc.get(end_label, 0) <= 0:
            lines[end_label_idx] = indent + '}'
        else:
            lines[end_label_idx] = indent + '}\n' + lines[end_label_idx]
        changes += 2
    return changes


def pass_fwd_single_skip(lines, lp, rc, ls):
    """Forward single-ref: if (cond) goto L; <body>; L: -> if (!cond) { <body>; }"""
    changes = 0
    for i in range(len(lines) - 1, -1, -1):
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        indent, cond, label = m.group(1), m.group(2), m.group(3)
        if rc.get(label, 0) != 1 or label not in lp:
            continue
        label_idx = lp[label]
        if label_idx <= i or label_idx - i > 500:
            continue
        body_labels = set()
        for k in range(i + 1, label_idx):
            ml = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if ml:
                body_labels.add(ml.group(1))
        all_internal = True
        for bl in body_labels:
            for src in ls.get(bl, []):
                if src < i or src >= label_idx:
                    all_internal = False
                    break
            if not all_internal:
                break
        if not all_internal:
            continue
        has_bad = False
        for k in range(i + 1, label_idx):
            gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
            if gm:
                target_lbl = gm.group(1)
                if target_lbl in body_labels or target_lbl == label:
                    continue
                if target_lbl in lp:
                    t = lp[target_lbl]
                    if t >= label_idx:
                        continue
                    if t < i:
                        has_bad = True
                        break
        if has_bad:
            continue
        # Verify brace depth: body must have balanced braces
        brace_depth = 0
        for k in range(i + 1, label_idx):
            brace_depth += lines[k].count('{') - lines[k].count('}')
        if brace_depth != 0:
            continue
        inv = invert_cond(cond)
        if inv is None:
            continue
        lines[i] = indent + 'if (' + inv + ') {'
        for k in range(i + 1, label_idx):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[label_idx] = indent + '}'
        rc[label] = 0
        changes += 1
    return changes


def pass_multi_ref_skip(lines, lp, rc, ls):
    """Multi-ref forward: convert each if-goto to the same label independently.
    Wraps code between each goto and the next goto/label in if (!cond) { ... }.
    Works for both terminating and non-terminating blocks."""
    changes = 0
    # Process labels in order
    for label in sorted(lp.keys(), key=lambda x: lp[x]):
        pos = lp[label]
        ref_count = rc.get(label, 0)
        if ref_count < 2:
            continue
        sources = sorted(ls.get(label, []))
        if not sources:
            continue
        # Try converting each if-goto source (from last to first to avoid index shifts)
        for si in range(len(sources) - 1, -1, -1):
            src = sources[si]
            m = re.match(r'^(\s+)if \((.+)\) goto ' + re.escape(label) + r';$', lines[src])
            if not m:
                continue
            indent, cond = m.group(1), m.group(2)
            if src >= pos:
                continue
            # Find the end of this block: next source or label pos
            block_end = pos
            for ns in sources:
                if ns > src and ns < block_end:
                    block_end = ns
                    break
            # Check block between src+1 and block_end
            block_lines = []
            has_label_ext = False
            has_bad_goto = False
            brace_depth = 0
            for k in range(src + 1, block_end):
                s = lines[k].strip()
                if not s:
                    continue
                brace_depth += lines[k].count('{') - lines[k].count('}')
                ml = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', s)
                if ml:
                    lbl_name = ml.group(1)
                    for ss in ls.get(lbl_name, []):
                        if ss < src or ss >= block_end:
                            has_label_ext = True
                            break
                gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', s)
                if gm and gm.group(1) != label:
                    t_lbl = gm.group(1)
                    if t_lbl in lp and lp[t_lbl] < src:
                        has_bad_goto = True
                block_lines.append(k)
            if has_label_ext or has_bad_goto:
                continue
            if brace_depth != 0:
                continue
            if len(block_lines) == 0:
                continue
            inv = invert_cond(cond)
            if inv is None:
                continue
            # Convert: wrap block in if (!cond) { ... }
            lines[src] = indent + 'if (' + inv + ') {'
            for k in block_lines:
                lines[k] = '    ' + lines[k]
            last_k = block_lines[-1]
            lines[last_k] = lines[last_k] + '\n' + indent + '}'
            for k in range(last_k + 1, block_end):
                if not lines[k].strip():
                    lines[k] = ''
            rc[label] = rc.get(label, 0) - 1
            changes += 1
            # Rebuild sources for this label since lines changed
            sources = []
            for ii in range(len(lines)):
                if re.search(r'\bgoto\s+' + re.escape(label) + r'\s*;', lines[ii]):
                    sources.append(ii)
            break  # Only convert one per label per iteration (will retry)
    return changes


def pass_do_while(lines, lp, rc, ls):
    """Backward goto -> do-while: L: <body>; if (cond) goto L; -> do { <body>; } while (cond);"""
    changes = 0
    for i in range(len(lines) - 1, -1, -1):
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        indent, cond, label = m.group(1), m.group(2), m.group(3)
        if label not in lp:
            continue
        label_idx = lp[label]
        if label_idx >= i or rc.get(label, 0) != 1:
            continue
        body_labels = set()
        for k in range(label_idx + 1, i):
            ml = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if ml:
                body_labels.add(ml.group(1))
        all_internal = True
        for bl in body_labels:
            for src in ls.get(bl, []):
                if src <= label_idx or src >= i:
                    all_internal = False
                    break
            if not all_internal:
                break
        if not all_internal:
            continue
        has_bad = False
        for k in range(label_idx + 1, i):
            gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
            if gm:
                target = gm.group(1)
                if target in body_labels or target == label:
                    continue
                if target in lp and (lp[target] < label_idx or lp[target] > i):
                    has_bad = True
                    break
        if has_bad:
            continue
        # Verify brace depth: the goto must be at same depth as the label
        brace_depth = 0
        for k in range(label_idx, i):
            brace_depth += lines[k].count('{') - lines[k].count('}')
        if brace_depth != 0:
            continue
        lines[label_idx] = indent + 'do {'
        for k in range(label_idx + 1, i):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[i] = indent + '} while (' + cond + ');'
        rc[label] = 0
        changes += 1
    return changes


def pass_break_from_loop(lines, lp, rc, ls):
    """goto to label just after loop end -> break"""
    changes = 0
    loop_ranges = []
    for i, line in enumerate(lines):
        s = line.strip()
        if re.match(r'(do\s*\{|while\s*\(.+\)\s*\{|for\s*\(.*\)\s*\{)', s):
            depth = line.count('{') - line.count('}')
            for j in range(i + 1, len(lines)):
                depth += lines[j].count('{') - lines[j].count('}')
                if depth <= 0:
                    loop_ranges.append((i, j))
                    break
    for i in range(len(lines)):
        m = re.match(r'^(\s+)(?:if \((.+)\) )?goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        indent, cond, lbl = m.group(1), m.group(2), m.group(3)
        if lbl not in lp:
            continue
        target = lp[lbl]
        for (start, end) in loop_ranges:
            if start < i < end:
                found = False
                for check in range(end, min(end + 3, len(lines))):
                    s = lines[check].strip()
                    if s:
                        ml = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', s)
                        if ml and ml.group(1) == lbl:
                            found = True
                        break
                if found or target == end + 1:
                    if cond:
                        lines[i] = indent + 'if (' + cond + ') break;'
                    else:
                        lines[i] = indent + 'break;'
                    rc[lbl] = rc.get(lbl, 0) - 1
                    changes += 1
                    break
    return changes


def pass_continue_in_loop(lines, lp, rc, ls):
    """goto to loop head/condition -> continue"""
    changes = 0
    loop_ranges = []
    for i, line in enumerate(lines):
        s = line.strip()
        m_do = re.match(r'do\s*\{', s)
        m_while = re.match(r'while\s*\(.+\)\s*\{', s)
        if m_do or m_while:
            depth = line.count('{') - line.count('}')
            for j in range(i + 1, len(lines)):
                depth += lines[j].count('{') - lines[j].count('}')
                if depth <= 0:
                    loop_ranges.append((i, j, 'do-while' if m_do else 'while'))
                    break
    for i in range(len(lines)):
        m = re.match(r'^(\s+)(?:if \((.+)\) )?goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        indent, cond, lbl = m.group(1), m.group(2), m.group(3)
        if lbl not in lp:
            continue
        target = lp[lbl]
        best_loop = None
        for (start, end, ltype) in loop_ranges:
            if start < i < end:
                if best_loop is None or start > best_loop[0]:
                    best_loop = (start, end, ltype)
        if best_loop is None:
            continue
        loop_start, loop_end, loop_type = best_loop
        is_continue = (target == loop_start) or (loop_type == 'do-while' and target == loop_end)
        if is_continue:
            if cond:
                lines[i] = indent + 'if (' + cond + ') continue;'
            else:
                lines[i] = indent + 'continue;'
            rc[lbl] = rc.get(lbl, 0) - 1
            changes += 1
    return changes


def pass_remove_unused_labels(lines, lp, rc, ls):
    """Remove labels with no remaining references."""
    changes = 0
    for label, pos in list(lp.items()):
        if rc.get(label, 0) <= 0:
            m = re.match(r'^(\s*)(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[pos])
            if m:
                lines[pos] = ''
                changes += 1
    return changes


def pass_goto_return(lines, lp, rc, ls):
    """goto L where L leads to return -> return (multi-ref safe)"""
    changes = 0
    return_labels = set()
    return_val_labels = {}
    for lbl, pos in lp.items():
        for k in range(pos + 1, min(pos + 3, len(lines))):
            s = lines[k].strip()
            if not s:
                continue
            if s == 'return;':
                return_labels.add(lbl)
            else:
                m = re.match(r'^return\s+(.+?)\s*;$', s)
                if m:
                    return_val_labels[lbl] = m.group(1)
            break
    for i in range(len(lines)):
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if m and m.group(2) in return_labels:
            lines[i] = m.group(1) + 'return;'
            rc[m.group(2)] = rc.get(m.group(2), 0) - 1
            changes += 1
            continue
        if m and m.group(2) in return_val_labels:
            lines[i] = m.group(1) + 'return ' + return_val_labels[m.group(2)] + ';'
            rc[m.group(2)] = rc.get(m.group(2), 0) - 1
            changes += 1
            continue
        m = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if m and m.group(3) in return_labels:
            lines[i] = m.group(1) + 'if (' + m.group(2) + ') return;'
            rc[m.group(3)] = rc.get(m.group(3), 0) - 1
            changes += 1
            continue
        if m and m.group(3) in return_val_labels:
            lines[i] = m.group(1) + 'if (' + m.group(2) + ') return ' + return_val_labels[m.group(3)] + ';'
            rc[m.group(3)] = rc.get(m.group(3), 0) - 1
            changes += 1
    return changes


def pass_assign_goto_return(lines, lp, rc, ls):
    """var = expr; goto L; ... L: return var; -> return expr;"""
    changes = 0
    return_var_labels = {}
    for lbl, pos in lp.items():
        for k in range(pos + 1, min(pos + 3, len(lines))):
            s = lines[k].strip()
            if not s:
                continue
            m = re.match(r'^return\s+(\w+)\s*;$', s)
            if m:
                return_var_labels[lbl] = m.group(1)
            break
    for i in range(len(lines) - 1, -1, -1):
        m_assign = re.match(r'^(\s+)(\w+)\s*=\s*(.+?);\s*$', lines[i])
        if not m_assign:
            continue
        indent, var, value = m_assign.group(1), m_assign.group(2), m_assign.group(3)
        for j in range(i + 1, min(i + 3, len(lines))):
            s = lines[j].strip()
            if not s:
                continue
            mg = re.match(r'^goto (L_[0-9A-Fa-f]+);$', s)
            if mg:
                lbl = mg.group(1)
                if lbl in return_var_labels and return_var_labels[lbl] == var:
                    lines[i] = indent + 'return ' + value + ';'
                    lines[j] = ''
                    rc[lbl] = rc.get(lbl, 0) - 1
                    changes += 1
            break
    return changes


def pass_multi_ref_consecutive(lines, lp, rc, ls):
    """All refs to a label are consecutive if-gotos -> combined if with body."""
    changes = 0
    processed = set()
    for label, pos in sorted(lp.items(), key=lambda x: x[1]):
        if label in processed or rc.get(label, 0) < 2:
            continue
        sources = ls.get(label, [])
        if not sources:
            continue
        refs = []
        for src in sorted(sources):
            m = re.match(r'^(\s+)if \((.+)\) goto ' + re.escape(label) + r';$', lines[src])
            if m:
                refs.append((src, m.group(1), m.group(2)))
        if len(refs) != len(sources):
            continue
        refs.sort(key=lambda x: x[0])
        consecutive = True
        for ri in range(1, len(refs)):
            between_empty = True
            for k in range(refs[ri-1][0] + 1, refs[ri][0]):
                if lines[k].strip():
                    between_empty = False
                    break
            if not between_empty:
                consecutive = False
                break
        if not consecutive or any(r[0] >= pos for r in refs):
            continue
        first_idx, last_idx = refs[0][0], refs[-1][0]
        has_bad = False
        for k in range(last_idx + 1, pos):
            ml = re.match(r'^\s*(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', lines[k])
            if ml:
                for src in ls.get(ml.group(1), []):
                    if src < first_idx or src >= pos:
                        has_bad = True
                        break
            gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
            if gm and gm.group(1) != label:
                if gm.group(1) in lp and (lp[gm.group(1)] < first_idx or lp[gm.group(1)] > pos):
                    has_bad = True
        if has_bad:
            continue
        inv_conds = []
        valid = True
        for _, _, cond in refs:
            inv = invert_cond(cond)
            if inv is None:
                valid = False
                break
            inv_conds.append(inv)
        if not valid:
            continue
        indent = refs[0][1]
        lines[first_idx] = indent + 'if (' + ' && '.join(inv_conds) + ') {'
        for k in range(first_idx + 1, last_idx + 1):
            lines[k] = ''
        for k in range(last_idx + 1, pos):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[pos] = indent + '}'
        rc[label] = 0
        processed.add(label)
        changes += len(refs)
    return changes


def pass_while_loop(lines, lp, rc, ls):
    """goto COND; BODY: <body>; COND: if (c) goto BODY; -> while (c) { <body>; }"""
    changes = 0
    for i in range(len(lines)):
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        indent = m.group(1)
        cond_label = m.group(2)
        if cond_label not in lp:
            continue
        cond_idx = lp[cond_label]
        if cond_idx <= i or rc.get(cond_label, 0) != 1:
            continue
        body_label = body_idx = None
        for k in range(i + 1, min(i + 3, cond_idx)):
            s = lines[k].strip()
            if s:
                ml = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', s)
                if ml:
                    body_label = ml.group(1)
                    body_idx = k
                break
        if body_label is None:
            continue
        back_goto_idx = back_goto_cond = None
        for k in range(cond_idx + 1, min(cond_idx + 5, len(lines))):
            s = lines[k].strip()
            if not s:
                continue
            mb = re.match(r'^if \((.+)\) goto ' + re.escape(body_label) + r';$', s)
            if mb:
                back_goto_idx = k
                back_goto_cond = mb.group(1)
            break
        if back_goto_idx is None:
            continue
        has_bad = False
        for k in range(body_idx + 1, cond_idx):
            gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', lines[k])
            if gm:
                target_lbl = gm.group(1)
                if target_lbl == body_label or target_lbl == cond_label:
                    continue
                if target_lbl in lp and (lp[target_lbl] < body_idx or lp[target_lbl] > back_goto_idx + 2):
                    has_bad = True
                    break
        if has_bad:
            continue
        # Verify brace balance in body
        brace_depth = 0
        for k in range(body_idx, cond_idx):
            brace_depth += lines[k].count('{') - lines[k].count('}')
        if brace_depth != 0:
            continue
        lines[i] = indent + 'while (' + back_goto_cond + ') {'
        lines[body_idx] = ''
        lines[cond_idx] = ''
        for k in range(body_idx + 1, cond_idx):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[back_goto_idx] = indent + '}'
        rc[cond_label] = 0
        rc[body_label] = rc.get(body_label, 0) - 1
        changes += 2
    return changes


def pass_cascaded_if_else(lines, lp, rc, ls):
    """if (c1) goto L1; <else1>; goto END; L1: if (c2) goto L2; <else2>; goto END; L2: -> if/else if/else"""
    changes = 0
    processed = set()
    for i in range(len(lines)):
        if i in processed:
            continue
        m1 = re.match(r'^(\s+)if \((.+)\) goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m1:
            continue
        indent, first_cond, first_label = m1.group(1), m1.group(2), m1.group(3)
        if first_label not in lp or rc.get(first_label, 0) != 1:
            continue
        first_label_idx = lp[first_label]
        if first_label_idx <= i:
            continue
        end_goto_idx = end_label = None
        for k in range(first_label_idx - 1, i, -1):
            s = lines[k].strip()
            if not s:
                continue
            mg = re.match(r'^goto (L_[0-9A-Fa-f]+);$', s)
            if mg:
                end_goto_idx = k
                end_label = mg.group(1)
            break
        if end_goto_idx is None or end_label is None or end_label not in lp:
            continue
        end_label_idx = lp[end_label]
        if end_label_idx <= first_label_idx:
            continue
        if any(re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]) for k in range(i + 1, end_goto_idx)):
            continue
        chain = [{'cond': first_cond, 'body_start': i + 1, 'body_end': end_goto_idx,
                  'skip_goto_idx': end_goto_idx, 'label': first_label, 'label_idx': first_label_idx}]
        cur_label_idx = first_label_idx
        chain_end_label = end_label
        while True:
            next_if_idx = next_cond_val = next_label_val = None
            for k in range(cur_label_idx + 1, min(cur_label_idx + 3, len(lines))):
                s = lines[k].strip()
                if not s:
                    continue
                mn = re.match(r'^if \((.+)\) goto (L_[0-9A-Fa-f]+);$', s)
                if mn:
                    next_if_idx, next_cond_val, next_label_val = k, mn.group(1), mn.group(2)
                break
            if next_if_idx is None:
                break
            if next_label_val not in lp or rc.get(next_label_val, 0) != 1:
                break
            next_label_idx = lp[next_label_val]
            if next_label_idx <= next_if_idx:
                break
            next_end_goto_idx = None
            for k in range(next_label_idx - 1, next_if_idx, -1):
                s = lines[k].strip()
                if not s:
                    continue
                mg = re.match(r'^goto (L_[0-9A-Fa-f]+);$', s)
                if mg and mg.group(1) == chain_end_label:
                    next_end_goto_idx = k
                break
            if next_end_goto_idx is None:
                break
            if any(re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]) for k in range(next_if_idx + 1, next_end_goto_idx)):
                break
            chain.append({'cond': next_cond_val, 'body_start': next_if_idx + 1, 'body_end': next_end_goto_idx,
                          'skip_goto_idx': next_end_goto_idx, 'label': next_label_val, 'label_idx': next_label_idx})
            cur_label_idx = next_label_idx
        if len(chain) < 2:
            continue
        if not all(invert_cond(c['cond']) is not None for c in chain):
            continue
        inv = invert_cond(chain[0]['cond'])
        lines[i] = indent + 'if (' + inv + ') {'
        for k in range(chain[0]['body_start'], chain[0]['body_end']):
            if lines[k].strip():
                lines[k] = '    ' + lines[k]
        lines[chain[0]['skip_goto_idx']] = ''
        rc[chain[0]['label']] = 0
        rc[chain_end_label] = rc.get(chain_end_label, 0) - 1
        processed.add(i)
        for ci in range(1, len(chain)):
            c = chain[ci]
            inv = invert_cond(c['cond'])
            lines[c['label_idx']] = ''
            lines[chain[ci - 1]['label_idx']] = indent + '} else if (' + inv + ') {'
            for k in range(c['body_start'], c['body_end']):
                if lines[k].strip():
                    lines[k] = '    ' + lines[k]
            lines[c['skip_goto_idx']] = ''
            rc[c['label']] = 0
            rc[chain_end_label] = rc.get(chain_end_label, 0) - 1
        last_label_idx = chain[-1]['label_idx']
        else_start, else_end = last_label_idx + 1, end_label_idx
        has_else_body = any(lines[k].strip() for k in range(else_start, else_end))
        has_else_label = any(re.match(r'^\s*L_[0-9A-Fa-f]+\s*:\s*;?\s*$', lines[k]) for k in range(else_start, else_end))
        if has_else_body and not has_else_label:
            lines[last_label_idx] = indent + '} else {'
            for k in range(else_start, else_end):
                if lines[k].strip():
                    lines[k] = '    ' + lines[k]
            lines[else_end] = indent + '}'
        else:
            lines[last_label_idx] = indent + '}'
        changes += len(chain) * 2
    return changes


def pass_uncond_forward_deadcode(lines, lp, rc, ls):
    """Unconditional goto where all code between goto and label is unreachable."""
    changes = 0
    for i in range(len(lines)):
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        label = m.group(2)
        if label not in lp:
            continue
        label_idx = lp[label]
        if label_idx <= i or rc.get(label, 0) != 1:
            continue
        all_dead = True
        for k in range(i + 1, label_idx):
            s = lines[k].strip()
            if not s:
                continue
            if re.match(r'^L_[0-9A-Fa-f]+\s*:\s*;?\s*$', s):
                continue
            all_dead = False
            break
        if not all_dead:
            continue
        lines[i] = ''
        lines[label_idx] = ''
        rc[label] = 0
        changes += 1
    return changes


def pass_goto_in_if_else(lines, lp, rc, ls):
    """Pattern: goto END; } <else-code>; END: -> } else { <else-code>; }
    Converts goto-at-end-of-if-block into else block."""
    changes = 0
    for i in range(len(lines) - 1, -1, -1):
        m = re.match(r'^(\s+)goto (L_[0-9A-Fa-f]+);$', lines[i])
        if not m:
            continue
        goto_indent = m.group(1)
        end_label = m.group(2)
        if end_label not in lp:
            continue
        end_pos = lp[end_label]
        if end_pos <= i:
            continue
        if rc.get(end_label, 0) != 1:
            continue
        # Next non-empty line must be '}'
        close_brace_idx = None
        for k in range(i + 1, min(i + 3, len(lines))):
            s = lines[k].strip()
            if s:
                if s == '}':
                    close_brace_idx = k
                break
        if close_brace_idx is None:
            continue
        brace_indent = re.match(r'^(\s*)', lines[close_brace_idx]).group(1)
        # Verify the closing brace belongs to an if statement, not a loop/switch
        # Walk backward from close_brace_idx to find the matching open brace
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
        # Must be an if(...) { block
        if not re.match(r'^if\s*\(', open_line_stripped):
            continue
        # Else body is between close_brace_idx+1 and end_pos
        else_lines = []
        has_label_inside = False
        has_bad_goto = False
        brace_depth = 0
        for k in range(close_brace_idx + 1, end_pos):
            s = lines[k].strip()
            if not s:
                continue
            brace_depth += lines[k].count('{') - lines[k].count('}')
            ml = re.match(r'^(L_[0-9A-Fa-f]+)\s*:\s*;?\s*$', s)
            if ml:
                lbl_name = ml.group(1)
                for src in ls.get(lbl_name, []):
                    if src < i or src >= end_pos:
                        has_label_inside = True
                        break
            gm = re.search(r'\bgoto\s+(L_[0-9A-Fa-f]+)\s*;', s)
            if gm and gm.group(1) != end_label:
                t_lbl = gm.group(1)
                if t_lbl in lp and lp[t_lbl] < i:
                    has_bad_goto = True
            else_lines.append(k)
        if has_label_inside or has_bad_goto:
            continue
        if brace_depth != 0:
            continue
        if len(else_lines) == 0:
            continue
        if len(else_lines) > 200:
            continue
        # Convert: remove goto, change } to } else {, indent else body, add }
        lines[i] = ''  # remove goto
        lines[close_brace_idx] = brace_indent + '} else {'
        for k in else_lines:
            lines[k] = '    ' + lines[k]
        # Clear empty lines between else body and label
        for k in range(close_brace_idx + 1, end_pos):
            if k not in else_lines and not lines[k].strip():
                lines[k] = ''
        # Handle end label
        rc[end_label] = rc.get(end_label, 0) - 1
        if rc.get(end_label, 0) <= 0:
            lines[end_pos] = brace_indent + '}'
        else:
            lines[end_pos] = brace_indent + '}\n' + lines[end_pos]
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
    original_lines = lines[:]
    total_removed = 0
    prev_count = initial_gotos
    passes = [
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
        # ('goto_in_if_else', pass_goto_in_if_else),  # disabled: creates double else blocks
        ('uncond_forward_deadcode', pass_uncond_forward_deadcode),
        ('remove_unused_labels', pass_remove_unused_labels),
    ]
    for iteration in range(30):
        lp, rc, ls_map = build_indices(lines)
        iter_total = 0
        for name, func in passes:
            lp, rc, ls_map = build_indices(lines)
            c = func(lines, lp, rc, ls_map)
            if c > 0:
                iter_total += c
        current = count_gotos(lines)
        removed = prev_count - current
        if removed <= 0:
            break
        prev_count = current
        total_removed += removed
        print(f'  Iteration {iteration + 1}: {removed} gotos removed ({current} remaining)')
    # Verify brace balance
    func_ranges_new = find_function_ranges(lines)
    ok, bad_line = verify_brace_balance(lines, func_ranges_new)
    if not ok:
        print(f'  WARNING: Brace imbalance near line {bad_line + 1}, reverting!')
        lines = original_lines
        total_removed = 0
    content = '\n'.join(lines)
    content = re.sub(r'\n{4,}', '\n\n\n', content)
    final_gotos = len(re.findall(r'\bgoto\s+L_[0-9A-Fa-f]+\b', content))
    print(f'  Result: {initial_gotos} -> {final_gotos} gotos ({initial_gotos - final_gotos} removed)')
    with open(filepath, 'w', encoding='utf-8', errors='replace') as f:
        f.write(content)
    return initial_gotos - final_gotos


def main():
    files = sys.argv[1:]
    if not files:
        print("Usage: convert_gotos_safe.py <file.c> [file2.c ...]")
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
