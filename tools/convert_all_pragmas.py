#!/usr/bin/env python3
"""
convert_all_pragmas.py - Convert ALL pragma-guarded register-level C functions
to idiomatic C89 across the entire Pokemon Colosseum decomp codebase.

Handles two pragma patterns:
  Pattern A: #pragma optimization_level 0 ... #pragma optimization_level 4
  Pattern B: #pragma push / #pragma optimization_level 0 / #pragma optimizewithasm off
             ... #pragma pop

Transformations:
  1. Remove pragma wrappers entirely
  2. Rename register variables (r0-r31, f0-f31) to meaningful local names
  3. Map parameter registers (r3-r10, f1-f8) to function param names
  4. Clean up comparison patterns
  5. Remove r1 (stack pointer) artifacts
  6. Remove asm-artifact comments (stmw, lmw, psq_st, etc.)
  7. Remove epilogue register restores from stack
  8. Preserve extern declarations, sp[] buffers, comments, and semantic content
"""

import re
import sys
import os
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = PROJECT_ROOT / "src"

PARAM_INT_REGS = ['r3', 'r4', 'r5', 'r6', 'r7', 'r8', 'r9', 'r10']
PARAM_FLOAT_REGS = ['f1', 'f2', 'f3', 'f4', 'f5', 'f6', 'f7', 'f8']


def reg_sort_key(r):
    if r.startswith('r'):
        return int(r[1:])
    elif r.startswith('f'):
        return 100 + int(r[1:])
    return 200


# ============================================================================
# Block finder - handles both patterns
# ============================================================================

def find_all_pragma_blocks(lines):
    """
    Find all pragma-guarded function blocks in both patterns.
    Returns list of (start, end, pattern_type) tuples.
    pattern_type: 'A' for opt_level 0/4, 'B' for push/pop
    """
    blocks = []
    i = 0
    while i < len(lines):
        s = lines[i].strip()

        # Pattern A: #pragma optimization_level 0 ... #pragma optimization_level 4
        if s == '#pragma optimization_level 0':
            # Check this isn't inside a push/pop block (Pattern B)
            # Look back to see if there was a recent #pragma push
            is_pattern_b = False
            for k in range(max(0, i - 3), i):
                if lines[k].strip() == '#pragma push':
                    is_pattern_b = True
                    break

            if not is_pattern_b:
                start = i
                j = i + 1
                while j < len(lines):
                    if lines[j].strip() == '#pragma optimization_level 4':
                        blocks.append((start, j, 'A'))
                        i = j + 1
                        break
                    j += 1
                else:
                    i += 1
                continue

        # Pattern B: #pragma push ... #pragma pop (with optimization_level 0 inside)
        if s == '#pragma push':
            # Check if this block contains optimization_level 0
            start = i
            depth = 1
            has_opt_level = False
            has_force_active = False
            j = i + 1
            while j < len(lines) and depth > 0:
                sj = lines[j].strip()
                if sj == '#pragma push':
                    depth += 1
                elif sj == '#pragma pop':
                    depth -= 1
                elif sj == '#pragma optimization_level 0':
                    has_opt_level = True
                elif sj == '#pragma force_active on':
                    has_force_active = True
                j += 1
            end = j - 1  # Points to matching #pragma pop

            if has_opt_level and not has_force_active:
                blocks.append((start, end, 'B'))
                i = end + 1
            elif has_force_active and not has_opt_level:
                # This is just a force_active block - skip for now
                i = end + 1
            elif has_force_active and has_opt_level:
                # Complex: force_active wrapping individual push/pop blocks
                # Need to find inner blocks
                # Skip the outer push, find inner push/pop with opt_level
                i += 1
                continue
            else:
                i = end + 1
        else:
            i += 1

    return blocks


# ============================================================================
# Function parser
# ============================================================================

def parse_function_block(lines, start, end, pattern_type):
    """Parse a pragma block into structured components."""
    block_lines = [lines[k].rstrip('\n').rstrip('\r') for k in range(start, end + 1)]

    # Find function signature
    sig_line_idx = None
    for i, l in enumerate(block_lines):
        s = l.strip()
        if s.startswith('#pragma'):
            continue
        if s == '':
            continue
        if s.startswith('/*') and '*/' in s and '{' not in s:
            continue
        if s.startswith('//'):
            continue

        # Function signature pattern
        m = re.match(
            r'^((?:static\s+)?(?:void|u32|s32|u16|s16|u8|s8|int|f32|f64|BOOL)\s*\**)\s*'
            r'(\w+)\s*\(([^)]*)\)\s*\{?\s*$', s)
        if m:
            sig_line_idx = i
            break

    if sig_line_idx is None:
        return None

    sig_s = block_lines[sig_line_idx].strip()
    m = re.match(
        r'^((?:static\s+)?(?:void|u32|s32|u16|s16|u8|s8|int|f32|f64|BOOL)\s*\**)\s*'
        r'(\w+)\s*\(([^)]*)\)', sig_s)
    if not m:
        return None

    ret_type = m.group(1).strip()
    func_name = m.group(2)
    params_str = m.group(3).strip()

    # Parse parameters
    params = []
    if params_str and params_str != 'void':
        for p in params_str.split(','):
            p = p.strip()
            if not p:
                continue
            parts = p.rsplit(None, 1)
            if len(parts) == 2:
                ptype = parts[0].strip()
                pname = parts[1].strip().lstrip('*')
                if '*' in parts[1] and not ptype.endswith('*'):
                    ptype += '*'
                params.append((ptype, pname))
            else:
                params.append(('u32', p))

    # Find body between { and }
    body_start_idx = sig_line_idx
    if '{' in block_lines[sig_line_idx]:
        body_start_idx = sig_line_idx + 1
    else:
        for k in range(sig_line_idx + 1, len(block_lines)):
            if '{' in block_lines[k].strip():
                body_start_idx = k + 1
                break

    # Find closing brace
    brace_depth = 1
    body_end_idx = body_start_idx
    for k in range(body_start_idx, len(block_lines)):
        s = block_lines[k].strip()
        brace_depth += s.count('{') - s.count('}')
        if brace_depth <= 0:
            body_end_idx = k
            break

    # Parse body into components
    extern_decls = []
    reg_decls = {}
    sp_decl = None
    ctr_fn_decl = False
    ctr_decl = False
    body_lines = []

    for k in range(body_start_idx, body_end_idx):
        s = block_lines[k].strip()
        if not s:
            continue
        if s.startswith('#pragma'):
            continue

        # Extern declarations
        if s.startswith('extern '):
            extern_decls.append(s)
            continue

        # Stack buffer
        m_sp = re.match(r'^u8\s+sp\[(0x[0-9A-Fa-f]+|\d+)\]\s*;$', s)
        if m_sp:
            sp_decl = m_sp.group(1)
            continue

        # Register declarations
        m_reg = re.match(r'^(u32|s32)\s+(r\d+)\s*=\s*(.+?)\s*;$', s)
        if m_reg:
            reg_decls[m_reg.group(2)] = (m_reg.group(1), m_reg.group(3))
            continue

        # Float register declarations
        m_freg = re.match(r'^(f32|f64)\s+(f\d+)\s*=\s*(.+?)\s*;$', s)
        if m_freg:
            reg_decls[m_freg.group(2)] = (m_freg.group(1), m_freg.group(3))
            continue

        # ctr_fn / ctr declarations
        if re.match(r'^void\s+\(\*ctr_fn\)', s):
            ctr_fn_decl = True
            continue
        if re.match(r'^u32\s+ctr\s*=\s*0\s*;$', s):
            ctr_decl = True
            continue

        body_lines.append(s)

    # Find comment/doc block before the pragma (for preservation)
    pre_comment_lines = []
    check = start - 1
    while check >= 0:
        cs = lines[check].strip()
        if cs == '' or cs.startswith('/*') or cs.startswith('*') or cs.startswith('//') or cs.endswith('*/'):
            pre_comment_lines.insert(0, lines[check].rstrip('\n').rstrip('\r'))
            check -= 1
        else:
            break

    return {
        'ret_type': ret_type,
        'func_name': func_name,
        'params': params,
        'params_str': params_str,
        'extern_decls': extern_decls,
        'reg_decls': reg_decls,
        'sp_decl': sp_decl,
        'ctr_fn_decl': ctr_fn_decl,
        'ctr_decl': ctr_decl,
        'body': body_lines,
        'pre_comment': pre_comment_lines,
    }


# ============================================================================
# Rename map builder
# ============================================================================

def build_rename_map(parsed):
    params = parsed['params']
    body = parsed['body']
    reg_decls = parsed['reg_decls']
    rename = {}

    # Map parameter registers to function parameter names
    param_reg_map = {}
    int_idx = 0
    float_idx = 0
    for ptype, pname in params:
        if 'f32' in ptype or 'f64' in ptype or 'float' in ptype:
            if float_idx < len(PARAM_FLOAT_REGS):
                param_reg_map[PARAM_FLOAT_REGS[float_idx]] = pname
                float_idx += 1
        else:
            if int_idx < len(PARAM_INT_REGS):
                param_reg_map[PARAM_INT_REGS[int_idx]] = pname
                int_idx += 1

    # Find callee-saved register saves
    saved_reg_map = {}
    for line in body[:30]:
        m = re.match(r'^(r\d+)\s*=\s*(r[3-9]|r10)\s*;$', line)
        if m:
            dst, src = m.group(1), m.group(2)
            if int(dst[1:]) >= 14:
                saved_reg_map[dst] = src

    # Map callee-saved regs holding params
    for saved_reg, param_reg in saved_reg_map.items():
        if param_reg in param_reg_map:
            rename[saved_reg] = param_reg_map[param_reg]

    # r0 -> tmp
    if 'r0' in reg_decls:
        rename['r0'] = 'tmp'

    # Assign param regs
    for reg, name in param_reg_map.items():
        if reg not in rename:
            rename[reg] = name

    return rename


# ============================================================================
# Code transformations
# ============================================================================

def clean_asm_artifacts(line):
    """Remove asm-artifact comments and epilogue noise."""
    s = line.strip()

    # Asm comments
    if re.match(r'^/\*\s*(stmw|lmw|stfd|lfd|psq_st|psq_l|mflr|mtlr|stwu|'
                r'lwzx|stbx|subi|clrlslwi|lbzx|rlwinm|rlwimi|xoris|'
                r'subic|crclr|crset|mfcr|mtcrf|cntlzw|mulhw|divw|'
                r'subfic|addze|addme|mulli|subf|srawi|subfe|eqv|nand|'
                r'andc|orc|extsb|extsh|neg|slw|srw|sraw|mullw|divwu)\s.*\*/;?\s*$', s):
        return None

    if s in ('/* indirect jump via ctr */;',
             '/* crclr cr1eq */;',
             '/* crset cr1eq */;'):
        return None

    return line


def is_epilogue_line(line):
    """Check if a line is part of function epilogue (register restore from stack)."""
    s = line.strip()
    # rXX = *(u32*)(sp + 0xNN);  -- register restore
    if re.match(r'^r\d+\s*=\s*\*\(u32\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\)\s*;$', s):
        return True
    # fXX = *(f64*)(sp + 0xNN);  -- float register restore
    if re.match(r'^f\d+\s*=\s*\*\(f64\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\)\s*;$', s):
        return True
    # *(f64*)(sp + 0xNN) = fXX;  -- float register save (prologue)
    if re.match(r'^\*\(f64\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\)\s*=\s*f\d+\s*;$', s):
        return True
    return False


def transform_comparison(line):
    """Transform register-level comparisons to cleaner C."""
    s = line.strip()

    # if ((u32)REG OP (u32)VAL) goto LABEL;
    m = re.match(
        r'^if\s+\(\(u32\)(\w+)\s*(==|!=|<|>|<=|>=)\s*\(u32\)(0x[0-9A-Fa-f]+|-?\d+)\)\s*goto\s+(\w+)\s*;$', s)
    if m:
        reg, op, val, label = m.group(1), m.group(2), m.group(3), m.group(4)
        val = simplify_val(val)
        return f'if ({reg} {op} {val}) goto {label};'

    # if ((s32)REG OP (s32)VAL) goto LABEL;
    m = re.match(
        r'^if\s+\(\(s32\)(\w+)\s*(==|!=|<|>|<=|>=)\s*\(s32\)(0x[0-9A-Fa-f]+|-?\d+)\)\s*goto\s+(\w+)\s*;$', s)
    if m:
        reg, op, val, label = m.group(1), m.group(2), m.group(3), m.group(4)
        val = simplify_val(val)
        return f'if ((s32){reg} {op} {val}) goto {label};'

    # if ((u32)REG OP (u32)REG2) goto LABEL;
    m = re.match(
        r'^if\s+\(\(u32\)(\w+)\s*(==|!=|<|>|<=|>=)\s*\(u32\)(\w+)\)\s*goto\s+(\w+)\s*;$', s)
    if m:
        r1, op, r2, label = m.group(1), m.group(2), m.group(3), m.group(4)
        return f'if ({r1} {op} {r2}) goto {label};'

    # if ((s32)REG OP (s32)REG2) goto LABEL;
    m = re.match(
        r'^if\s+\(\(s32\)(\w+)\s*(==|!=|<|>|<=|>=)\s*\(s32\)(\w+)\)\s*goto\s+(\w+)\s*;$', s)
    if m:
        r1, op, r2, label = m.group(1), m.group(2), m.group(3), m.group(4)
        return f'if ((s32){r1} {op} (s32){r2}) goto {label};'

    return line


def simplify_val(val):
    if val.startswith('0x') or val.startswith('-0x'):
        try:
            n = int(val, 16) if not val.startswith('-') else -int(val[1:], 16)
            if 0 <= n <= 9:
                return str(n)
        except ValueError:
            pass
    return val


def transform_shift_construct(line):
    """Transform (VAL << SHIFT) to hex constant."""
    s = line.strip()
    m = re.match(r'^(\w+)\s*=\s*\((0x[0-9A-Fa-f]+|\d+)\s*<<\s*(\d+)\)\s*;$', s)
    if m:
        reg, val, shift = m.group(1), m.group(2), int(m.group(3))
        try:
            n = int(val, 0)
            result = n << shift
            return f'{reg} = 0x{result:X};'
        except ValueError:
            pass
    return line


def transform_line(line, rename_map):
    """Apply all transformations to a single line."""
    s = line.strip()
    if not s:
        return ''

    # Remove asm artifacts
    cleaned = clean_asm_artifacts(line)
    if cleaned is None:
        return None

    # Skip epilogue lines
    if is_epilogue_line(s):
        return None

    # Skip r1 = (u32)sp
    if re.match(r'^r1\s*=\s*\(u32\)sp\s*;$', s):
        return None

    # Skip prologue float saves
    if re.match(r'^\*\(f64\*\)\(sp\s*\+\s*0x[0-9A-Fa-f]+\)\s*=\s*f\d+\s*;$', s):
        return None

    line = s

    # Transform comparisons
    line = transform_comparison(line)

    # Transform shift constructs
    line = transform_shift_construct(line)

    # Apply register renames
    for old in sorted(rename_map.keys(), key=lambda x: -len(x)):
        new = rename_map[old]
        if old != new:
            line = re.sub(r'\b' + re.escape(old) + r'\b', new, line)

    return line


# ============================================================================
# Function converter
# ============================================================================

def convert_function(parsed, rename_map):
    """Convert a parsed function to idiomatic C89."""
    func_name = parsed['func_name']
    ret_type = parsed['ret_type']
    params_str = parsed['params_str']
    extern_decls = parsed['extern_decls']
    reg_decls = parsed['reg_decls']
    sp_decl = parsed['sp_decl']
    ctr_fn_decl = parsed['ctr_fn_decl']
    ctr_decl = parsed['ctr_decl']
    body = parsed['body']
    params = parsed['params']

    out = []

    # Function signature
    if params_str:
        out.append(f'{ret_type} {func_name}({params_str}) {{')
    else:
        out.append(f'{ret_type} {func_name}(void) {{')

    # Extern declarations
    for ext in extern_decls:
        out.append(f'    {ext}')

    # Stack buffer
    if sp_decl:
        out.append(f'    u8 sp[{sp_decl}];')

    # Determine which param registers are used
    param_reg_names = set()
    int_idx = 0
    float_idx = 0
    for ptype, pname in params:
        if 'f32' in ptype or 'f64' in ptype or 'float' in ptype:
            if float_idx < len(PARAM_FLOAT_REGS):
                param_reg_names.add(PARAM_FLOAT_REGS[float_idx])
                float_idx += 1
        else:
            if int_idx < len(PARAM_INT_REGS):
                param_reg_names.add(PARAM_INT_REGS[int_idx])
                int_idx += 1

    declared_vars = set()

    # Declare register variables
    for reg in sorted(reg_decls.keys(), key=reg_sort_key):
        if reg == 'r1':
            continue

        rtype, rinit = reg_decls[reg]
        var_name = rename_map.get(reg, reg)

        if var_name in declared_vars:
            continue

        # Skip if this reg is a function param that maps to a param name
        if reg in param_reg_names and reg in rename_map:
            pname = rename_map[reg]
            # Check if the param name is actually a function parameter
            is_func_param = False
            for pt, pn in params:
                if pn == pname:
                    is_func_param = True
                    break
            if is_func_param:
                # Also skip if it's a callee-saved reg that was renamed to a param
                continue

        # Also check if a callee-saved reg is renamed to a param name
        if var_name != reg:
            is_func_param = False
            for pt, pn in params:
                if pn == var_name:
                    is_func_param = True
                    break
            if is_func_param:
                continue

        # Apply renames to init value
        init_val = rinit
        if init_val == '(u32)sp':
            continue
        for old in sorted(rename_map.keys(), key=lambda x: -len(x)):
            new = rename_map[old]
            if old != new:
                init_val = re.sub(r'\b' + re.escape(old) + r'\b', new, init_val)

        out.append(f'    {rtype} {var_name} = {init_val};')
        declared_vars.add(var_name)

    if ctr_fn_decl:
        out.append('    void (*ctr_fn)(void) = 0;')
    if ctr_decl:
        out.append('    u32 ctr = 0;')

    # Separator
    if extern_decls or sp_decl or declared_vars or ctr_fn_decl or ctr_decl:
        out.append('')

    # Transform body
    for line in body:
        transformed = transform_line(line, rename_map)
        if transformed is None:
            continue
        if transformed == '':
            out.append('')
            continue

        s = transformed.strip()

        # Labels
        if re.match(r'^L_[0-9A-Fa-f]+\s*:\s*;?\s*$', s):
            label = re.match(r'^(L_[0-9A-Fa-f]+)', s).group(1)
            out.append(f'{label}:')
            continue

        out.append(f'    {s}')

    out.append('}')
    return out


# ============================================================================
# File processor
# ============================================================================

def process_file(filepath):
    """Process a single source file."""
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        lines = f.readlines()
    lines = [l.rstrip('\n').rstrip('\r') for l in lines]

    blocks = find_all_pragma_blocks(lines)
    if not blocks:
        return 0, 0

    total = len(blocks)
    converted = 0
    errors = []

    # Process in reverse order
    for start, end, pat_type in reversed(blocks):
        try:
            parsed = parse_function_block(lines, start, end, pat_type)
            if parsed is None:
                errors.append(f"  L{start+1}: parse failed")
                continue

            rename_map = build_rename_map(parsed)
            new_lines = convert_function(parsed, rename_map)

            # Replace the block (including pragma lines)
            lines[start:end + 1] = new_lines
            converted += 1

        except Exception as e:
            errors.append(f"  L{start+1}: {e}")

    # Write back
    with open(filepath, 'w', encoding='utf-8', newline='\n') as f:
        for line in lines:
            f.write(line + '\n')

    if errors:
        print(f"    Errors ({len(errors)}):")
        for e in errors[:3]:
            print(f"      {e}")
        if len(errors) > 3:
            print(f"      ... +{len(errors)-3} more")

    return converted, total


# ============================================================================
# Main
# ============================================================================

def main():
    src_files = sorted(SRC_DIR.rglob("*.c"))

    total_converted = 0
    total_blocks = 0
    files_processed = 0

    for src_file in src_files:
        with open(src_file, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
        if '#pragma optimization_level 0' not in content:
            continue

        rel = src_file.relative_to(PROJECT_ROOT)
        converted, total = process_file(str(src_file))
        if total > 0:
            files_processed += 1
            total_converted += converted
            total_blocks += total
            status = "OK" if converted == total else f"{converted}/{total}"
            print(f"  {rel}: {status} ({converted} blocks)")

    print(f"\n{'='*60}")
    print(f"TOTAL: {total_converted}/{total_blocks} blocks in {files_processed} files")

    # Count remaining
    remaining = 0
    for src_file in src_files:
        with open(src_file, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
        remaining += content.count('#pragma optimization_level 0')
    print(f"Remaining #pragma optimization_level 0: {remaining}")

    return 0


if __name__ == '__main__':
    sys.exit(main() or 0)
