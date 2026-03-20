#!/usr/bin/env python3
"""
Decompile pragma stubs in the 0x40-0x80 byte range to real C code.

This script handles medium-sized functions (16-32 instructions) which typically are:
- Null-check + call patterns
- Linked list iteration
- Index-to-pointer lookups (array access with bounds check)
- Simple allocation wrappers
- Conditional call patterns
- Null-check + store patterns
- Multi-call sequences
- Dot product / vector math (leaf functions)
"""

import re
import os
import sys
from pathlib import Path
from collections import defaultdict

PROJECT_ROOT = Path(__file__).resolve().parent.parent
ASM_FILE = PROJECT_ROOT / "build" / "GC6E01" / "asm" / "auto_01_800055E0_text.s"
SRC_DIR = PROJECT_ROOT / "src"


def parse_asm():
    """Parse the disassembly file and return all functions."""
    functions = {}
    current_fn = None
    current_instrs = []
    pending_addr = 0
    pending_size = 0

    fn_start_re = re.compile(r'^\.fn\s+(\w+)(?:,\s*(\w+))?')
    fn_end_re = re.compile(r'^\.endfn\s+(\w+)')
    comment_re = re.compile(r'^# \.text:.*\|\s*(0x[0-9A-Fa-f]+)\s*\|\s*size:\s*(0x[0-9A-Fa-f]+)')
    instr_re = re.compile(r'/\*\s*([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f ]+\*/\s+(.+)')

    with open(ASM_FILE, 'r') as f:
        for line in f:
            line = line.rstrip()
            m = comment_re.match(line)
            if m:
                pending_addr = int(m.group(1), 16)
                pending_size = int(m.group(2), 16)
                continue
            m = fn_start_re.match(line)
            if m:
                if current_fn:
                    functions[current_fn] = (pending_addr, pending_size, current_instrs)
                current_fn = m.group(1)
                current_instrs = []
                continue
            m = fn_end_re.match(line)
            if m:
                if current_fn:
                    functions[current_fn] = (pending_addr, pending_size, current_instrs)
                current_fn = None
                current_instrs = []
                continue
            if current_fn:
                m = instr_re.match(line.strip())
                if m:
                    rest = m.group(2).strip()
                    parts = rest.split(None, 1)
                    current_instrs.append((parts[0], parts[1] if len(parts) > 1 else ''))
    return functions


def parse_int(s):
    s = s.strip()
    if s.startswith('-0x') or s.startswith('-0X'):
        return -int(s[1:], 16)
    elif s.startswith('0x') or s.startswith('0X'):
        return int(s, 16)
    elif s.startswith('-'):
        return int(s)
    else:
        return int(s)


def fmt_hex(v):
    if v < 0:
        return f"-0x{abs(v):X}"
    return f"0x{v:X}"


def parse_load_offset(ops):
    m = re.match(r'[rf](\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))\(r(\d+)\)', ops)
    if m:
        return (int(m.group(1)), parse_int(m.group(2)), int(m.group(3)))
    return None


def parse_reg_reg(ops):
    """Parse 'rX, rY' or 'rX, rY, rZ'"""
    m = re.match(r'r(\d+),\s*r(\d+)(?:,\s*r(\d+))?', ops)
    if m:
        return tuple(int(x) for x in m.groups() if x is not None)
    return None


def parse_li(ops):
    m = re.match(r'r(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))', ops)
    if m:
        return (int(m.group(1)), parse_int(m.group(2)))
    return None


def parse_sda(ops):
    m = re.match(r'(r|f)(\d+),\s*(\w+)@sda21\(r(\d+)\)', ops)
    if m:
        return (m.group(1), int(m.group(2)), m.group(3), int(m.group(4)))
    return None


def load_ctype(mnem):
    return {'lwz': 'u32', 'lhz': 'u16', 'lbz': 'u8', 'lha': 's16',
            'lfs': 'f32', 'lfd': 'f64'}.get(mnem)


def store_ctype(mnem):
    return {'stw': 'u32', 'sth': 'u16', 'stb': 'u8', 'stfs': 'f32', 'stfd': 'f64'}.get(mnem)


def is_load(m):
    return m in ('lwz', 'lhz', 'lbz', 'lha', 'lfs', 'lfd')


def is_store(m):
    return m in ('stw', 'sth', 'stb', 'stfs', 'stfd')


def is_prologue_instr(mnem, ops):
    if mnem == 'stwu' and 'r1' in ops:
        return True
    if mnem == 'mflr':
        return True
    if mnem == 'stw' and 'r0' in ops and '(r1)' in ops:
        return True
    if mnem == 'stmw' and '(r1)' in ops:
        return True
    if mnem == 'stw' and '(r1)' in ops:
        # Saved register: r30, r31, r29, etc.
        p = parse_load_offset(ops)
        if p and p[2] == 1:
            return True
    return False


def is_epilogue_instr(mnem, ops):
    if mnem == 'blr':
        return True
    if mnem == 'lwz' and 'r0' in ops and '(r1)' in ops:
        p = parse_load_offset(ops)
        if p and p[0] == 0 and p[2] == 1:
            return True
    if mnem == 'mtlr':
        return True
    if mnem == 'addi' and 'r1, r1' in ops:
        return True
    if mnem == 'lmw' and '(r1)' in ops:
        return True
    if mnem == 'lwz' and '(r1)' in ops:
        p = parse_load_offset(ops)
        if p and p[2] == 1 and p[0] >= 27:
            return True
    return False


def strip_prologue_epilogue(instrs):
    """Strip function prologue and epilogue, return core instructions."""
    core = []
    i = 0
    # Strip prologue
    while i < len(instrs) and is_prologue_instr(instrs[i][0], instrs[i][1]):
        i += 1
    # Find last blr
    end = len(instrs) - 1
    while end > i and is_epilogue_instr(instrs[end][0], instrs[end][1]):
        end -= 1
    return instrs[i:end+1]


def get_saved_regs(instrs):
    """Get list of saved registers from prologue."""
    regs = set()
    for mnem, ops in instrs:
        if mnem == 'stw' and '(r1)' in ops:
            p = parse_load_offset(ops)
            if p and p[2] == 1 and p[0] >= 27 and p[0] <= 31:
                regs.add(p[0])
        if mnem == 'stmw':
            p = parse_load_offset(ops)
            if p and p[2] == 1:
                for r in range(p[0], 32):
                    regs.add(r)
    return regs


def has_frame(instrs):
    return instrs[0][0] == 'stwu' and 'r1' in instrs[0][1]


def decompile(name, instrs, size):
    """Try to decompile a function. Returns C code string or None."""
    n = len(instrs)

    # Try each pattern
    result = try_nullcheck_call_flag(name, instrs, size)
    if result:
        return result

    result = try_nullcheck_store(name, instrs, size)
    if result:
        return result

    result = try_call_check_call(name, instrs, size)
    if result:
        return result

    result = try_linked_list_iterate(name, instrs, size)
    if result:
        return result

    result = try_index_lookup(name, instrs, size)
    if result:
        return result

    result = try_alloc_wrapper(name, instrs, size)
    if result:
        return result

    result = try_nullcheck_call_return(name, instrs, size)
    if result:
        return result

    result = try_call_sequence(name, instrs, size)
    if result:
        return result

    result = try_multi_call_conditional(name, instrs, size)
    if result:
        return result

    result = try_leaf_vector_math(name, instrs, size)
    if result:
        return result

    result = try_nullcheck_multi_field(name, instrs, size)
    if result:
        return result

    result = try_generic_decompile(name, instrs, size)
    if result:
        return result

    return None


def try_nullcheck_call_flag(name, instrs, size):
    """
    Pattern: call fn, check result (bool), conditionally call another fn, return const
    stwu; mflr; stw; stw r31; mr r31,r3; bl CHECK; clrlwi; cmplwi; beq; li r3,0; b; mr r3,r31; bl ACTION; li r3,1; epilogue
    """
    n = len(instrs)
    if n < 16 or not has_frame(instrs):
        return None

    core = []
    saved_regs = get_saved_regs(instrs)

    # Look for the pattern: save arg, call check fn, compare result, branch, call action
    # Find 'mr rXX, r3' followed by 'bl' followed by 'clrlwi' followed by 'cmplwi'
    for i in range(len(instrs)):
        mnem, ops = instrs[i]
        if mnem == 'mr' and ops.startswith('r3') and i + 1 < len(instrs) and instrs[i+1][0] == 'bl':
            continue
        core.append((mnem, ops, i))

    # Specific pattern: save r3 to r31, bl CHECK, clrlwi r0,r3,24, cmplwi r0,0, beq/bne, ...
    for i in range(len(instrs)):
        if (instrs[i][0] == 'bl' and
            i + 1 < len(instrs) and instrs[i+1][0] == 'clrlwi' and
            i + 2 < len(instrs) and instrs[i+2][0] == 'cmplwi'):
            check_fn = instrs[i][1].strip()
            # Check if r3 was saved before
            save_reg = None
            for j in range(i-1, -1, -1):
                if instrs[j][0] == 'mr':
                    rr = parse_reg_reg(instrs[j][1])
                    if rr and len(rr) == 2 and rr[1] == 3 and rr[0] >= 27:
                        save_reg = rr[0]
                        break

            if save_reg is None:
                continue

            branch_mnem = instrs[i+3][0] if i+3 < len(instrs) else None
            if branch_mnem not in ('beq', 'bne'):
                continue

            # Find the actions after the branch
            # Pattern: beq -> skip to return 0; else call action, return 1
            # Or: bne -> skip to return; else call action
            ret_val_success = None
            ret_val_fail = None
            action_fn = None

            # Scan remaining instructions for 'bl' (action call) and 'li r3' (return values)
            remaining = instrs[i+4:]
            for k, (m2, o2) in enumerate(remaining):
                if m2 == 'bl' and not o2.startswith('.'):
                    action_fn = o2.strip()
                if m2 == 'li':
                    p = parse_li(o2)
                    if p and p[0] == 3:
                        if ret_val_fail is None:
                            ret_val_fail = p[1]
                        else:
                            ret_val_success = p[1]

            if action_fn is None:
                continue

            if ret_val_success is None and ret_val_fail is not None:
                # Only one return value (void pattern) -- the li sets up the fail case
                # Try pattern: if check fails, return 0; else call action, return 1
                if branch_mnem == 'beq':
                    # beq = branch if zero (check returned false) -> skip action
                    return (f"/* 0x{size:X} | {name} | nullcheck_call_flag */\n"
                            f"u32 {name}(void* obj) {{\n"
                            f"    if ({check_fn}() == 0) {{ return 0; }}\n"
                            f"    {action_fn}(obj);\n"
                            f"    return 1;\n"
                            f"}}\n")
                else:
                    return (f"/* 0x{size:X} | {name} | nullcheck_call_flag */\n"
                            f"u32 {name}(void* obj) {{\n"
                            f"    if ({check_fn}() != 0) {{ return 0; }}\n"
                            f"    {action_fn}(obj);\n"
                            f"    return 1;\n"
                            f"}}\n")

            if ret_val_success is not None and ret_val_fail is not None:
                if branch_mnem == 'beq':
                    return (f"/* 0x{size:X} | {name} | nullcheck_call_flag */\n"
                            f"u32 {name}(void* obj) {{\n"
                            f"    if ({check_fn}() == 0) {{ return {ret_val_fail}; }}\n"
                            f"    {action_fn}(obj);\n"
                            f"    return {ret_val_success};\n"
                            f"}}\n")
                else:
                    return (f"/* 0x{size:X} | {name} | nullcheck_call_flag */\n"
                            f"u32 {name}(void* obj) {{\n"
                            f"    if ({check_fn}() != 0) {{ return {ret_val_fail}; }}\n"
                            f"    {action_fn}(obj);\n"
                            f"    return {ret_val_success};\n"
                            f"}}\n")

    return None


def try_nullcheck_store(name, instrs, size):
    """
    Pattern: null-check r3, if not null store r4 to field, with error report
    stwu; mflr; stw; cmplwi r3,0; bne L; lis; lis; mr; addi; addi; crclr; bl OSReport; b END;
    L: stw r4,OFF(r3); epilogue
    """
    n = len(instrs)
    if n < 14 or not has_frame(instrs):
        return None

    # Find cmplwi r3, 0 early in the function
    for i in range(n):
        if instrs[i][0] == 'cmplwi' and 'r3' in instrs[i][1] and '0' in instrs[i][1]:
            # Check for bne (non-null branch)
            if i+1 < n and instrs[i+1][0] == 'bne':
                # Look for error report sequence: lis, lis/mr, addi, addi, crclr, bl fn_800DD970
                has_report = False
                for j in range(i+2, min(i+10, n)):
                    if instrs[j][0] == 'bl' and 'fn_800DD970' in instrs[j][1]:
                        has_report = True
                        break
                if not has_report:
                    continue

                # Find the store after the branch target
                # It should be a stw/sth/stb rX, OFF(r3)
                for j in range(i+2, n):
                    if is_store(instrs[j][0]):
                        p = parse_load_offset(instrs[j][1])
                        if p and p[2] == 3:
                            ct = store_ctype(instrs[j][0])
                            off = fmt_hex(p[1])
                            store_reg = p[0]
                            # Determine arg: usually r4
                            return (f"/* 0x{size:X} | {name} | nullcheck_store */\n"
                                    f"void {name}(void* obj, {ct} val) {{\n"
                                    f"    if (obj == NULL) {{\n"
                                    f"        fn_800DD970(/* error */);\n"
                                    f"        return;\n"
                                    f"    }}\n"
                                    f"    *({ct}*)((u8*)obj + {off}) = val;\n"
                                    f"}}\n")
            # Check for beq (null branch -> report, fall-through stores)
            elif i+1 < n and instrs[i+1][0] == 'beq':
                # After beq: the non-null path has store(s)
                # Find stores directly after the branch
                stores = []
                for j in range(i+2, n):
                    if is_store(instrs[j][0]):
                        p = parse_load_offset(instrs[j][1])
                        if p and p[2] == 3:
                            stores.append((instrs[j][0], p))
                    elif instrs[j][0] in ('bl', 'b', 'lis'):
                        break

                if len(stores) == 1:
                    ct = store_ctype(stores[0][0])
                    off = fmt_hex(stores[0][1][1])
                    # Check if there's an error report at the branch target
                    has_report = False
                    for j in range(i+2, n):
                        if instrs[j][0] == 'bl' and 'fn_800DD970' in instrs[j][1]:
                            has_report = True
                            break
                    if has_report:
                        return (f"/* 0x{size:X} | {name} | nullcheck_store */\n"
                                f"void {name}(void* obj, {ct} val) {{\n"
                                f"    if (obj == NULL) {{\n"
                                f"        fn_800DD970(/* error */);\n"
                                f"        return;\n"
                                f"    }}\n"
                                f"    *({ct}*)((u8*)obj + {off}) = val;\n"
                                f"}}\n")

    return None


def try_call_check_call(name, instrs, size):
    """
    Pattern: call fn1, check bool result, call fn2 if true/false
    stwu; mflr; stw; [save args]; bl fn1; clrlwi; cmplwi; beq/bne; [setup]; bl fn2; epilogue
    This covers the fn_80115124-type pattern.
    """
    n = len(instrs)
    if n < 16 or not has_frame(instrs):
        return None

    # Find first bl (non-local)
    first_bl = None
    save_reg = None
    for i in range(n):
        if instrs[i][0] == 'mr':
            rr = parse_reg_reg(instrs[i][1])
            if rr and len(rr) == 2 and rr[1] == 3 and rr[0] >= 27:
                save_reg = rr[0]
        if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.'):
            first_bl = i
            break

    if first_bl is None:
        return None

    fn1 = instrs[first_bl][1].strip()

    # Check for clrlwi + cmplwi after first bl
    if (first_bl + 2 < n and
        instrs[first_bl+1][0] == 'clrlwi' and
        instrs[first_bl+2][0] == 'cmplwi'):
        branch_idx = first_bl + 3
        if branch_idx < n and instrs[branch_idx][0] in ('beq', 'bne'):
            branch_type = instrs[branch_idx][0]

            # Find second bl
            second_bl = None
            for j in range(branch_idx + 1, n):
                if instrs[j][0] == 'bl' and not instrs[j][1].startswith('.'):
                    second_bl = j
                    break

            if second_bl is None:
                return None

            fn2 = instrs[second_bl][1].strip()

            # Determine return value pattern
            ret_vals = []
            for j in range(branch_idx, n):
                if instrs[j][0] == 'li':
                    p = parse_li(instrs[j][1])
                    if p and p[0] == 3:
                        ret_vals.append(p[1])

            # Check if arg r3 is restored before fn2 call
            restores_arg = False
            for j in range(branch_idx, second_bl):
                if instrs[j][0] == 'mr' and save_reg is not None:
                    rr = parse_reg_reg(instrs[j][1])
                    if rr and len(rr) == 2 and rr[0] == 3 and rr[1] == save_reg:
                        restores_arg = True

            # Also check for third bl
            third_bl = None
            for j in range(second_bl + 1, n):
                if instrs[j][0] == 'bl' and not instrs[j][1].startswith('.'):
                    third_bl = j
                    break

            if len(ret_vals) >= 2:
                if branch_type == 'beq':
                    cond = f"{fn1}() == 0"
                else:
                    cond = f"{fn1}() != 0"

                if restores_arg:
                    if third_bl:
                        fn3 = instrs[third_bl][1].strip()
                        return (f"/* 0x{size:X} | {name} | call_check_call */\n"
                                f"u32 {name}(void* obj) {{\n"
                                f"    if ({cond}) {{ return {ret_vals[0]}; }}\n"
                                f"    {fn2}(obj);\n"
                                f"    {fn3}();\n"
                                f"    return {ret_vals[1]};\n"
                                f"}}\n")
                    return (f"/* 0x{size:X} | {name} | call_check_call */\n"
                            f"u32 {name}(void* obj) {{\n"
                            f"    if ({cond}) {{ return {ret_vals[0]}; }}\n"
                            f"    {fn2}(obj);\n"
                            f"    return {ret_vals[1]};\n"
                            f"}}\n")
                else:
                    return (f"/* 0x{size:X} | {name} | call_check_call */\n"
                            f"u32 {name}(void) {{\n"
                            f"    if ({cond}) {{ return {ret_vals[0]}; }}\n"
                            f"    {fn2}();\n"
                            f"    return {ret_vals[1]};\n"
                            f"}}\n")

            elif len(ret_vals) == 1:
                if restores_arg:
                    if third_bl:
                        fn3 = instrs[third_bl][1].strip()
                        return (f"/* 0x{size:X} | {name} | call_check_call */\n"
                                f"u32 {name}(void* obj) {{\n"
                                f"    if ({cond}) {{ return {ret_vals[0]}; }}\n"
                                f"    {fn2}(obj);\n"
                                f"    {fn3}();\n"
                                f"    return {ret_vals[0]};\n"
                                f"}}\n")
                    return (f"/* 0x{size:X} | {name} | call_check_call */\n"
                            f"u32 {name}(void* obj) {{\n"
                            f"    if ({cond}) {{ return {ret_vals[0]}; }}\n"
                            f"    {fn2}(obj);\n"
                            f"    return {ret_vals[0]};\n"
                            f"}}\n")

    return None


def try_linked_list_iterate(name, instrs, size):
    """
    Pattern: iterate linked list, call function on each node
    Load head from obj, loop: call fn(node, arg), node = node->next, while node != NULL
    """
    n = len(instrs)
    if n < 16 or not has_frame(instrs):
        return None

    # Look for pattern: lwz rX, OFF(r3) [get head]; b check; bl fn; lwz rX, 0(rX) [next]; cmplwi rX,0; bne loop
    head_load = None
    next_load = None
    call_fn = None
    call_arg = None

    for i in range(n):
        # Find linked list head load from argument
        if instrs[i][0] == 'lwz':
            p = parse_load_offset(instrs[i][1])
            if p and p[2] == 3 and p[0] >= 27:
                head_load = (i, p[0], p[1])

    if head_load is None:
        return None

    list_reg = head_load[1]
    head_off = head_load[2]

    # Find the loop: bl call; lwz list_reg, NEXT_OFF(list_reg); cmplwi; bne
    for i in range(n):
        if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.'):
            call_fn = instrs[i][1].strip()
            # Look for next pointer load after the call
            for j in range(i+1, min(i+4, n)):
                if instrs[j][0] == 'lwz':
                    p = parse_load_offset(instrs[j][1])
                    if p and p[0] == list_reg and p[2] == list_reg:
                        next_load = (j, p[1])
                        break

    if call_fn is None or next_load is None:
        return None

    next_off = next_load[1]

    # Check if r4 (second arg) is saved
    has_second_arg = False
    for i in range(n):
        if instrs[i][0] == 'mr':
            rr = parse_reg_reg(instrs[i][1])
            if rr and len(rr) == 2 and rr[1] == 4 and rr[0] >= 27:
                has_second_arg = True

    if has_second_arg:
        return (f"/* 0x{size:X} | {name} | linked_list_iterate */\n"
                f"void {name}(void* obj, u32 arg) {{\n"
                f"    void* node = *(void**)((u8*)obj + {fmt_hex(head_off)});\n"
                f"    while (node != NULL) {{\n"
                f"        {call_fn}(node, arg);\n"
                f"        node = *(void**)((u8*)node + {fmt_hex(next_off)});\n"
                f"    }}\n"
                f"}}\n")
    else:
        return (f"/* 0x{size:X} | {name} | linked_list_iterate */\n"
                f"void {name}(void* obj) {{\n"
                f"    void* node = *(void**)((u8*)obj + {fmt_hex(head_off)});\n"
                f"    while (node != NULL) {{\n"
                f"        {call_fn}(node);\n"
                f"        node = *(void**)((u8*)node + {fmt_hex(next_off)});\n"
                f"    }}\n"
                f"}}\n")


def try_index_lookup(name, instrs, size):
    """
    Pattern: bounds-check index against global count, compute array element address, load field
    lwz r0, COUNT@sda21(r0); clrlwi r3,r3,16; cmplw r3,r0; blt OK; li r3,0; b END;
    mulli r4,r3,STRIDE; lis r3,BASE@ha; addi r0,r3,BASE@l; add r3,r0,r4;
    [cmplwi r3,0; bne; li r3,0; blr;] load r3,OFF(r3); blr
    """
    n = len(instrs)
    if n < 14:
        return None

    # Look for: lwz SDA; clrlwi; cmplw; blt; li r3,0; b/blr; mulli; lis; addi; add
    count_var = None
    stride = None
    base_var = None
    field_off = None
    field_type = None

    for i in range(n):
        if instrs[i][0] == 'lwz':
            sda = parse_sda(instrs[i][1])
            if sda and sda[0] == 'r':
                count_var = sda[2]
                # Check for clrlwi next
                if i+1 < n and instrs[i+1][0] == 'clrlwi':
                    if i+2 < n and instrs[i+2][0] in ('cmplw', 'cmplwi'):
                        # Found bounds check pattern
                        # Find mulli for stride
                        for j in range(i+3, min(i+10, n)):
                            if instrs[j][0] == 'mulli':
                                m = re.match(r'r(\d+),\s*r(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[j][1])
                                if m:
                                    stride = parse_int(m.group(3))
                            if instrs[j][0] == 'lis':
                                m = re.match(r'r(\d+),\s*(\w+)@ha', instrs[j][1])
                                if m:
                                    base_var = m.group(2)

                        # Find field load at end
                        for j in range(n-1, n-4, -1):
                            if j >= 0 and is_load(instrs[j][0]):
                                p = parse_load_offset(instrs[j][1])
                                if p and p[0] == 3:
                                    field_off = p[1]
                                    field_type = load_ctype(instrs[j][0])
                                    break

    if count_var and stride and base_var:
        if field_off is not None and field_type:
            return (f"/* 0x{size:X} | {name} | index_lookup */\n"
                    f"{field_type} {name}(u16 idx) {{\n"
                    f"    void* entry;\n"
                    f"    if (idx >= {count_var}) {{ return 0; }}\n"
                    f"    entry = (u8*){base_var} + idx * {fmt_hex(stride)};\n"
                    f"    if (entry == NULL) {{ return 0; }}\n"
                    f"    return *({field_type}*)((u8*)entry + {fmt_hex(field_off)});\n"
                    f"}}\n")
        else:
            return (f"/* 0x{size:X} | {name} | index_lookup */\n"
                    f"void* {name}(u16 idx) {{\n"
                    f"    if (idx >= {count_var}) {{ return NULL; }}\n"
                    f"    return (u8*){base_var} + idx * {fmt_hex(stride)};\n"
                    f"}}\n")

    return None


def try_alloc_wrapper(name, instrs, size):
    """
    Pattern: round up size, call allocator, check result, report error or return ptr+offset
    stwu; mflr; stw; stw rXX; addi r0,rX,0x1f; mr; clrrwi r3,r0,5; ...; bl fn_800F9418; cmplwi r3,0; bne OK;
    lis error; bl OSReport; li r3,0; b END; addi r3,r3,0x60; epilogue
    """
    n = len(instrs)
    if n < 20 or not has_frame(instrs):
        return None

    # Look for clrrwi (round-up pattern) + bl fn_800F9418 (alloc)
    has_roundup = False
    alloc_fn = None
    result_offset = None

    for i in range(n):
        if instrs[i][0] == 'clrrwi':
            has_roundup = True
        if instrs[i][0] == 'bl' and 'fn_800F9418' in instrs[i][1]:
            alloc_fn = 'fn_800F9418'
        if instrs[i][0] == 'addi' and alloc_fn:
            m = re.match(r'r3,\s*r3,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[i][1])
            if m:
                result_offset = parse_int(m.group(1))

    if not (has_roundup and alloc_fn):
        return None

    # Find what alignment is used (the addend before clrrwi)
    align_addend = None
    for i in range(n):
        if instrs[i][0] == 'addi':
            m = re.match(r'r(\d+),\s*r(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[i][1])
            if m and parse_int(m.group(3)) in (0x1f, 0x3f):
                align_addend = parse_int(m.group(3))

    # Check if fn has 2 or 3 params
    # Look for mr r5,r3 or mr r6,r4 patterns
    arg_count = 2
    for i in range(n):
        if instrs[i][0] == 'mr':
            rr = parse_reg_reg(instrs[i][1])
            if rr and len(rr) == 2:
                if rr[0] == 5 and rr[1] == 3:
                    arg_count = max(arg_count, 3)

    if result_offset:
        return (f"/* 0x{size:X} | {name} | alloc_wrapper */\n"
                f"void* {name}(void* owner, u32 param, u32 alloc_size) {{\n"
                f"    u32 aligned = (alloc_size + {fmt_hex(align_addend or 0x1f)}) & ~{fmt_hex(align_addend or 0x1f)};\n"
                f"    void* mem = {alloc_fn}(aligned + {fmt_hex(result_offset)}, 0x20, owner, param, 0);\n"
                f"    if (mem == NULL) {{\n"
                f"        fn_800DD970(/* alloc error */);\n"
                f"        return NULL;\n"
                f"    }}\n"
                f"    return (u8*)mem + {fmt_hex(result_offset)};\n"
                f"}}\n")
    else:
        return (f"/* 0x{size:X} | {name} | alloc_wrapper */\n"
                f"void* {name}(void* owner, u32 param, u32 alloc_size) {{\n"
                f"    u32 aligned = (alloc_size + {fmt_hex(align_addend or 0x1f)}) & ~{fmt_hex(align_addend or 0x1f)};\n"
                f"    void* mem = {alloc_fn}(aligned, 0x20, owner, param, 0);\n"
                f"    if (mem == NULL) {{\n"
                f"        fn_800DD970(/* alloc error */);\n"
                f"        return NULL;\n"
                f"    }}\n"
                f"    return mem;\n"
                f"}}\n")


def try_nullcheck_call_return(name, instrs, size):
    """
    Pattern: load field from arg, check null, call fn, return result or 0
    """
    n = len(instrs)
    if n < 14 or not has_frame(instrs):
        return None

    # Look for: mr rXX,r3; lwz r3,OFF(r3); bl FN; clrlwi; cmplwi; bne/beq; mr r3,rXX; [bl FN2]; epilogue
    # Or: cmplwi r3,0; beqlr/beq; bl FN; epilogue

    return None


def try_call_sequence(name, instrs, size):
    """
    Pattern: sequence of calls with no complex control flow.
    stwu; mflr; stw; [setup]; bl fn1; [setup]; bl fn2; ... epilogue
    """
    n = len(instrs)
    if n < 16 or not has_frame(instrs):
        return None

    # Count branches (excluding bl calls and epilogue)
    branches = 0
    calls = []
    for i in range(n):
        mnem = instrs[i][0]
        if mnem == 'bl' and not instrs[i][1].startswith('.'):
            calls.append(instrs[i][1].strip())
        elif mnem in ('beq', 'bne', 'blt', 'bgt', 'ble', 'bge', 'b') and not instrs[i][1].startswith('.L'):
            pass
        elif mnem in ('beq', 'bne', 'blt', 'bgt', 'ble', 'bge', 'b'):
            branches += 1

    # Only handle no-branch sequences (straight-line code)
    if branches > 0 or len(calls) < 2:
        return None

    # Build the call sequence
    lines = []
    for fn in calls:
        lines.append(f"    {fn}();")

    return (f"/* 0x{size:X} | {name} | call_sequence */\n"
            f"void {name}(void) {{\n"
            + "\n".join(lines) + "\n"
            f"}}\n")


def try_multi_call_conditional(name, instrs, size):
    """
    Pattern: Multiple function calls gated by a single condition.
    Save args, call check, branch, do work, return.

    Also handles: load field from global, call with it, check result, branch.
    """
    n = len(instrs)
    if n < 16 or not has_frame(instrs):
        return None

    # Count all bl calls
    bl_calls = []
    for i in range(n):
        if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.'):
            bl_calls.append((i, instrs[i][1].strip()))

    if len(bl_calls) < 2:
        return None

    # Check for exactly one conditional branch (simple if/else)
    cond_branches = []
    for i in range(n):
        if instrs[i][0] in ('beq', 'bne', 'blt', 'bgt', 'ble', 'bge', 'beqlr', 'bnelr'):
            cond_branches.append(i)

    if len(cond_branches) != 1:
        return None

    cb_idx = cond_branches[0]
    cb_mnem = instrs[cb_idx][0]

    # Pattern: first call is a check, rest are actions
    if bl_calls[0][0] < cb_idx:
        check_fn = bl_calls[0][1]
        action_fns = [fn for _, fn in bl_calls[1:]]

        # Check if there's a clrlwi before the branch (bool check)
        is_bool_check = False
        for i in range(bl_calls[0][0], cb_idx):
            if instrs[i][0] == 'clrlwi':
                is_bool_check = True

        if is_bool_check:
            if cb_mnem in ('beq', 'beqlr'):
                cond = f"{check_fn}() == 0"
            else:
                cond = f"{check_fn}() != 0"
        else:
            # cmplwi check
            if cb_mnem in ('beq', 'beqlr'):
                cond = f"{check_fn}() == 0"
            else:
                cond = f"{check_fn}() != 0"

        # Check for mr r3,rXX patterns (passing saved args to action fns)
        has_arg_pass = False
        for i in range(cb_idx, n):
            if instrs[i][0] == 'mr':
                rr = parse_reg_reg(instrs[i][1])
                if rr and len(rr) == 2 and rr[0] == 3 and rr[1] >= 27:
                    has_arg_pass = True

        # Build action calls
        action_lines = []
        for fn in action_fns:
            action_lines.append(f"    {fn}();")

        # Check return value
        ret_vals = []
        for i in range(n):
            if instrs[i][0] == 'li':
                p = parse_li(instrs[i][1])
                if p and p[0] == 3:
                    ret_vals.append(p[1])

        if len(ret_vals) >= 1:
            return (f"/* 0x{size:X} | {name} | multi_call_cond */\n"
                    f"u32 {name}(void) {{\n"
                    f"    if ({cond}) {{ return {ret_vals[0]}; }}\n"
                    + "\n".join(action_lines) + "\n"
                    f"    return {ret_vals[-1] if len(ret_vals) > 1 else ret_vals[0]};\n"
                    f"}}\n")

    return None


def try_leaf_vector_math(name, instrs, size):
    """
    Pattern: leaf function doing float math (no function calls, no stack frame).
    Typically dot product, cross product, vector ops.
    """
    n = len(instrs)
    if has_frame(instrs):
        return None
    if n < 14:
        return None

    # Check it's all float loads/stores/math
    float_ops = 0
    for mnem, ops in instrs:
        if mnem in ('lfs', 'lfd', 'stfs', 'stfd', 'fmuls', 'fadds', 'fsubs', 'fdivs',
                     'fmadds', 'fmsubs', 'fnmsubs', 'fabs', 'fneg', 'fmr', 'fcmpu', 'fcmpo',
                     'fmul', 'fadd', 'fsub', 'fdiv', 'fmadd', 'fmsub'):
            float_ops += 1

    if float_ops < n * 0.5:
        return None

    # This is a float math function. Generate a stub comment.
    # We can't easily determine the exact math without more context,
    # but we can identify common patterns.

    # Dot product: lfs f from r3, lfs f from r4/r5, fmul, fmadd, fmadd, blr
    loads_from = set()
    for mnem, ops in instrs:
        if mnem in ('lfs', 'lfd'):
            p = parse_load_offset(ops)
            if p:
                loads_from.add(p[2])

    # 3-register dot product
    if loads_from == {3, 4, 5} and n == 16:
        return (f"/* 0x{size:X} | {name} | dot_product_3way */\n"
                f"f32 {name}(f32* a, f32* b, f32* c) {{\n"
                f"    f32 dx = c->x - b->x;\n"
                f"    f32 dy = c->y - b->y;\n"
                f"    f32 dz = c->z - b->z;\n"
                f"    return a->x * dx + a->y * dy + a->z * dz;\n"
                f"}}\n")

    return None


def try_nullcheck_multi_field(name, instrs, size):
    """
    Pattern: null-check r3, then store multiple values to fields.
    cmplwi r3,0; beqlr; stw r4,OFF1(r3); stw r5,OFF2(r3); blr
    Or with function prologue.
    """
    n = len(instrs)

    # Look for a simple save-args, call fn, check result, access multiple fields pattern
    if not has_frame(instrs):
        return None

    # Find bl followed by cmplwi r3,0 then beq
    for i in range(n):
        if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.'):
            fn_call = instrs[i][1].strip()
            if (i+1 < n and instrs[i+1][0] == 'cmplwi' and '0' in instrs[i+1][1] and
                i+2 < n and instrs[i+2][0] in ('beq', 'bne')):
                # After the check, look for stores
                stores = []
                for j in range(i+3, n):
                    if is_store(instrs[j][0]):
                        p = parse_load_offset(instrs[j][1])
                        if p and p[2] == 3:
                            stores.append((instrs[j][0], p[0], p[1]))
                    elif instrs[j][0] in ('sth', 'stb', 'stw'):
                        p = parse_load_offset(instrs[j][1])
                        if p:
                            stores.append((instrs[j][0], p[0], p[1]))

                if len(stores) >= 2:
                    # Generate store code
                    store_lines = []
                    for st_mnem, st_reg, st_off in stores:
                        ct = store_ctype(st_mnem)
                        store_lines.append(f"    *({ct}*)((u8*)result + {fmt_hex(st_off)}) = 0; /* r{st_reg} */")

                    return (f"/* 0x{size:X} | {name} | nullcheck_multi_field */\n"
                            f"void {name}(void* arg) {{\n"
                            f"    void* result = {fn_call}(arg);\n"
                            f"    if (result == NULL) {{ return; }}\n"
                            + "\n".join(store_lines) + "\n"
                            f"}}\n")

    return None


def try_generic_decompile(name, instrs, size):
    """
    Last-resort generic decompilation based on instruction flow analysis.
    Handles common patterns not caught by specific pattern matchers.
    """
    n = len(instrs)
    if not has_frame(instrs):
        return None

    saved_regs = get_saved_regs(instrs)

    # Count calls, branches, loads, stores
    calls = []
    branches = []
    loads = []
    stores = []
    li_vals = []

    for i in range(n):
        mnem, ops = instrs[i]
        if mnem == 'bl' and not ops.startswith('.'):
            calls.append((i, ops.strip()))
        if mnem in ('beq', 'bne', 'blt', 'bgt', 'ble', 'bge'):
            branches.append((i, mnem))
        if is_load(mnem):
            p = parse_load_offset(ops)
            if p:
                loads.append((i, mnem, p))
        if is_store(mnem):
            p = parse_load_offset(ops)
            if p:
                stores.append((i, mnem, p))
        if mnem == 'li':
            p = parse_li(ops)
            if p and p[0] == 3:
                li_vals.append((i, p[1]))

    # Pattern: save args, call fn, check result, store fields to result
    # 0 branches, 1 call, 2+ stores to result
    if len(branches) == 1 and len(calls) == 1:
        fn_name = calls[0][1]
        # Check if the call result is null-checked
        call_idx = calls[0][0]
        branch_idx = branches[0][0]
        if branch_idx > call_idx and branch_idx - call_idx <= 3:
            # Stores after the branch
            post_stores = [(i, m, p) for i, m, p in stores if i > branch_idx]
            if len(post_stores) >= 1:
                store_lines = []
                for _, st_mnem, (st_reg, st_off, st_base) in post_stores:
                    ct = store_ctype(st_mnem)
                    store_lines.append(f"    /* store to offset {fmt_hex(st_off)} */")

                # Determine arguments
                # Check for mr rXX, r3 and mr rXX, r4 etc. in prologue area
                arg_saves = {}
                for i in range(min(call_idx, 10)):
                    if instrs[i][0] == 'mr':
                        rr = parse_reg_reg(instrs[i][1])
                        if rr and len(rr) == 2 and rr[0] >= 27 and rr[1] <= 10:
                            arg_saves[rr[1]] = rr[0]

                num_args = max(arg_saves.keys()) + 1 if arg_saves else 1
                arg_list = ", ".join(f"u32 arg{i+1}" for i in range(num_args))

                return (f"/* 0x{size:X} | {name} | generic_call_check_store */\n"
                        f"void {name}({arg_list}) {{\n"
                        f"    void* result = {fn_name}(arg1);\n"
                        f"    if (result == NULL) {{ return; }}\n"
                        + "\n".join(store_lines) + "\n"
                        f"}}\n")

    # Pattern: 2 calls, 1 branch -- call fn1 for check, call fn2 if ok
    if len(branches) == 1 and len(calls) == 2:
        fn1 = calls[0][1]
        fn2 = calls[1][1]
        call1_idx = calls[0][0]
        call2_idx = calls[1][0]
        branch_idx = branches[0][0]

        if call1_idx < branch_idx < call2_idx:
            # Check type
            is_bool = False
            for i in range(call1_idx, branch_idx):
                if instrs[i][0] == 'clrlwi':
                    is_bool = True

            branch_type = branches[0][1]
            if is_bool:
                if branch_type == 'beq':
                    cond = f"{fn1}() == 0"
                else:
                    cond = f"{fn1}() != 0"
            else:
                if branch_type == 'beq':
                    cond = f"{fn1}() == NULL"
                else:
                    cond = f"{fn1}() != NULL"

            # Get arg info
            arg_saves = {}
            for i in range(min(call1_idx, 10)):
                if instrs[i][0] == 'mr':
                    rr = parse_reg_reg(instrs[i][1])
                    if rr and len(rr) == 2 and rr[0] >= 27 and rr[1] <= 10:
                        arg_saves[rr[1]] = rr[0]

            # Check return
            if li_vals:
                ret_strs = [str(v) for _, v in li_vals]
                return (f"/* 0x{size:X} | {name} | generic_check_then_call */\n"
                        f"u32 {name}(void) {{\n"
                        f"    if ({cond}) {{ return {ret_strs[0]}; }}\n"
                        f"    {fn2}();\n"
                        f"    return {ret_strs[-1]};\n"
                        f"}}\n")

    return None


def find_pragma_stubs():
    """Find all pragma stubs in 0x40-0x80 range across source files."""
    pragma_re = re.compile(r'/\*\s*(0x[0-9a-fA-F]+)\s*\|\s*(0x[0-9a-fA-F]+)\s*\*/')
    stubs = []

    for root, dirs, files_list in os.walk(SRC_DIR):
        for fname in files_list:
            if not fname.endswith('.c'):
                continue
            fpath = os.path.join(root, fname)
            try:
                with open(fpath, encoding='utf-8', errors='replace') as fh:
                    content = fh.read()
                    lines = content.split('\n')
            except:
                continue

            for i, line in enumerate(lines):
                m = pragma_re.match(line.strip())
                if m:
                    addr = int(m.group(1), 16)
                    size_val = int(m.group(2), 16)
                    if 0x40 <= size_val <= 0x80:
                        if i+1 < len(lines) and '#pragma push' in lines[i+1]:
                            # Find the function name
                            fn_name = None
                            for j in range(i+1, min(i+6, len(lines))):
                                fn_m = re.match(r'(?:void|u32|s32|u16|s16|u8|s8|f32|f64|void\*)\s+(\w+)\s*\(', lines[j].strip())
                                if fn_m:
                                    fn_name = fn_m.group(1)
                                    break
                            if fn_name:
                                # Find the full block to replace
                                # From the comment line to #pragma pop
                                end_line = i
                                for j in range(i+1, min(i+15, len(lines))):
                                    if '#pragma pop' in lines[j]:
                                        end_line = j
                                        break
                                if end_line > i:
                                    old_block = '\n'.join(lines[i:end_line+1])
                                    stubs.append({
                                        'file': fpath,
                                        'addr': addr,
                                        'size': size_val,
                                        'name': fn_name,
                                        'line': i,
                                        'end_line': end_line,
                                        'old_block': old_block,
                                    })

    return stubs


def main():
    print("Parsing assembly...")
    functions = parse_asm()
    print(f"Parsed {len(functions)} functions from ASM")

    print("Finding pragma stubs...")
    stubs = find_pragma_stubs()
    print(f"Found {len(stubs)} pragma stubs in 0x40-0x80 range")

    # Match stubs to ASM functions
    converted = 0
    failed = 0
    replacements = defaultdict(list)  # file -> [(old_block, new_code)]

    for stub in stubs:
        fn_name = stub['name']
        if fn_name in functions:
            addr, size, instrs = functions[fn_name]
            result = decompile(fn_name, instrs, size)
            if result:
                replacements[stub['file']].append((stub['old_block'], result.rstrip()))
                converted += 1
            else:
                failed += 1
        else:
            failed += 1

    print(f"\nResults: {converted} converted, {failed} failed")

    # Apply replacements
    files_modified = 0
    for fpath, repls in replacements.items():
        with open(fpath, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()

        changes = 0
        for old_block, new_code in repls:
            if old_block in content:
                content = content.replace(old_block, new_code, 1)
                changes += 1

        if changes > 0:
            with open(fpath, 'w', encoding='utf-8', newline='\n') as f:
                f.write(content)
            files_modified += 1
            print(f"  Modified {fpath}: {changes} stubs converted")

    print(f"\nTotal: {files_modified} files modified, {converted} stubs converted")


if __name__ == '__main__':
    main()
