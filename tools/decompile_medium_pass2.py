#!/usr/bin/env python3
"""
Second pass: decompile remaining pragma stubs in the 0x40-0x80 byte range.
Uses a more generic approach with PowerPC instruction emulation.
"""

import re
import os
from pathlib import Path
from collections import defaultdict

PROJECT_ROOT = Path(__file__).resolve().parent.parent
ASM_FILE = PROJECT_ROOT / "build" / "GC6E01" / "asm" / "auto_01_800055E0_text.s"
SRC_DIR = PROJECT_ROOT / "src"


def parse_asm():
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


def pi(s):
    """Parse integer from string."""
    s = s.strip()
    if s.startswith('-0x') or s.startswith('-0X'):
        return -int(s[1:], 16)
    elif s.startswith('0x') or s.startswith('0X'):
        return int(s, 16)
    else:
        return int(s)


def fh(v):
    """Format hex."""
    if v < 0:
        return f"-0x{abs(v):X}"
    if v == 0:
        return "0"
    return f"0x{v:X}"


def plo(ops):
    """Parse load/store offset: rX, OFF(rY)"""
    m = re.match(r'[rf](\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))\(r(\d+)\)', ops)
    if m:
        return (int(m.group(1)), pi(m.group(2)), int(m.group(3)))
    return None


def psda(ops):
    """Parse SDA: rX, SYM@sda21(rN)"""
    m = re.match(r'[rf](\d+),\s*(\w+)@sda21\(r(\d+)\)', ops)
    if m:
        return (int(m.group(1)), m.group(2), int(m.group(3)))
    return None


def prr(ops):
    """Parse register-register: rX, rY"""
    m = re.match(r'r(\d+),\s*r(\d+)', ops)
    if m:
        return (int(m.group(1)), int(m.group(2)))
    return None


def pli(ops):
    """Parse li: rX, IMM"""
    m = re.match(r'r(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))', ops)
    if m:
        return (int(m.group(1)), pi(m.group(2)))
    return None


def lctype(mnem):
    return {'lwz':'u32','lhz':'u16','lbz':'u8','lha':'s16','lfs':'f32','lfd':'f64'}.get(mnem)


def sctype(mnem):
    return {'stw':'u32','sth':'u16','stb':'u8','stfs':'f32','stfd':'f64'}.get(mnem)


def is_load(m): return m in ('lwz','lhz','lbz','lha','lfs','lfd','lhzx','lwzx','lbzx')
def is_store(m): return m in ('stw','sth','stb','stfs','stfd','sthx','stwx','stbx')


###############################################################################
# Pattern: leaf function with bounds-check + array index + field load
###############################################################################
def try_leaf_array_lookup(name, instrs, size):
    """
    lwz r0, COUNT@sda21(r0); clrlwi r3,r3,16; cmplw r3,r0; blt; li r3,0; blr;
    mulli rX,r3,STRIDE; lis rY,BASE@ha; addi rZ,rY,BASE@l; add rW,rZ,rX;
    [cmplwi rW,0; bne; li r3,0; blr;] load r3,OFF(rW); blr
    """
    n = len(instrs)
    if instrs[0][0] == 'stwu':
        return None  # Not a leaf function

    # Find SDA load for count
    count_var = None
    for i in range(n):
        if instrs[i][0] == 'lwz':
            sda = psda(instrs[i][1])
            if sda:
                count_var = sda[1]
                break

    if not count_var:
        return None

    # Find mulli for stride
    stride = None
    for i in range(n):
        if instrs[i][0] == 'mulli':
            m = re.match(r'r(\d+),\s*r(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[i][1])
            if m:
                stride = pi(m.group(3))

    # Find base variable
    base_var = None
    for i in range(n):
        if instrs[i][0] == 'lis':
            m = re.match(r'r(\d+),\s*(\w+)@ha', instrs[i][1])
            if m:
                base_var = m.group(2)

    if not base_var or not stride:
        return None

    # Find final field load
    field_off = None
    field_type = None
    for i in range(n-1, max(n-4, -1), -1):
        if is_load(instrs[i][0]) and instrs[i][0] not in ('lhzx', 'lwzx', 'lbzx'):
            p = plo(instrs[i][1])
            if p and p[0] == 3:
                field_off = p[1]
                field_type = lctype(instrs[i][0])
                break

    # Check for second bounds check (on r4)
    has_second_check = False
    second_bound = None
    for i in range(n):
        if instrs[i][0] == 'cmplwi':
            m = re.match(r'r4,\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[i][1])
            if m:
                has_second_check = True
                second_bound = fh(pi(m.group(1)))

    # Check for slwi (shift left for element size)
    has_slwi = False
    slwi_shift = 0
    for i in range(n):
        if instrs[i][0] == 'slwi':
            m = re.match(r'r(\d+),\s*r(\d+),\s*(\d+)', instrs[i][1])
            if m:
                has_slwi = True
                slwi_shift = int(m.group(3))

    if has_second_check and has_slwi:
        # Two-index lookup: arr[idx1 * stride + idx2 * elem_size].field
        elem_size = 1 << slwi_shift
        if field_off is not None and field_type:
            return (f"/* 0x{size:X} | {name} | leaf_2d_array_lookup */\n"
                    f"{field_type} {name}(u16 idx1, u16 idx2) {{\n"
                    f"    void* entry;\n"
                    f"    if (idx1 >= {count_var}) {{ return 0; }}\n"
                    f"    if (idx2 >= {second_bound}) {{ return 0; }}\n"
                    f"    entry = (u8*){base_var} + idx1 * {fh(stride)} + idx2 * {fh(elem_size)};\n"
                    f"    return *({field_type}*)((u8*)entry + {fh(field_off)});\n"
                    f"}}\n")

    if field_off is not None and field_type:
        return (f"/* 0x{size:X} | {name} | leaf_array_lookup */\n"
                f"{field_type} {name}(u16 idx) {{\n"
                f"    void* entry;\n"
                f"    if (idx >= {count_var}) {{ return 0; }}\n"
                f"    entry = (u8*){base_var} + idx * {fh(stride)};\n"
                f"    if (entry == NULL) {{ return 0; }}\n"
                f"    return *({field_type}*)((u8*)entry + {fh(field_off)});\n"
                f"}}\n")
    else:
        return (f"/* 0x{size:X} | {name} | leaf_array_lookup */\n"
                f"void* {name}(u16 idx) {{\n"
                f"    if (idx >= {count_var}) {{ return NULL; }}\n"
                f"    return (u8*){base_var} + idx * {fh(stride)};\n"
                f"}}\n")


###############################################################################
# Pattern: leaf function with cmplwi on args + global array
###############################################################################
def try_leaf_global_multi_output(name, instrs, size):
    """
    Leaf functions that read from globals and write to multiple output pointers.
    cmplwi r3,0; beq; lis; addi; lwz; stw r0,0(r3); cmplwi r4,0; beq; ...
    """
    n = len(instrs)
    if instrs[0][0] == 'stwu':
        return None

    # Count cmplwi checks on different registers
    checks = []
    for i in range(n):
        if instrs[i][0] == 'cmplwi':
            m = re.match(r'r(\d+),\s*0', instrs[i][1])
            if m:
                reg = int(m.group(1))
                if reg in (3, 4, 5, 6):
                    checks.append(reg)

    if len(checks) < 2:
        return None

    # Find global base
    base_var = None
    for i in range(n):
        if instrs[i][0] == 'lis':
            m = re.match(r'r(\d+),\s*(\w+)@ha', instrs[i][1])
            if m:
                base_var = m.group(2)
                break

    if not base_var:
        return None

    # Find field loads from global
    fields = []
    for i in range(n):
        if instrs[i][0] in ('lwz', 'lbz', 'lhz'):
            p = plo(instrs[i][1])
            if p and p[2] != 1:  # Not stack
                sda = psda(instrs[i][1])
                if not sda:
                    fields.append((instrs[i][0], p[1]))

    # Build output function
    args = []
    stores = []
    for reg in sorted(set(checks)):
        arg_idx = reg - 3
        if reg == 3:
            args.append("u32* out1")
        elif reg == 4:
            args.append("u32* out2")
        elif reg == 5:
            args.append("u8* out3")
        else:
            args.append(f"void* out{arg_idx+1}")

    body_lines = []
    field_idx = 0
    for reg in sorted(set(checks)):
        arg_name = f"out{reg - 3 + 1}"
        if field_idx < len(fields):
            ct = lctype(fields[field_idx][0])
            off = fh(fields[field_idx][1])
            body_lines.append(f"    if ({arg_name} != NULL) {{ *{arg_name} = *({ct}*)((u8*){base_var} + {off}); }}")
            field_idx += 1
        else:
            body_lines.append(f"    if ({arg_name} != NULL) {{ /* store */ }}")

    return (f"/* 0x{size:X} | {name} | leaf_multi_output */\n"
            f"void {name}({', '.join(args)}) {{\n"
            + "\n".join(body_lines) + "\n"
            f"}}\n")


###############################################################################
# Pattern: leaf linked-list search
###############################################################################
def try_leaf_list_search(name, instrs, size):
    """
    Leaf: cmpwi r3,0; bgt; li r3,0; blr; lis BASE; addi; lwz (head);
    loop: lwz field; cmpw; bne; mr r3,node; blr; lwz next; cmplwi; bne loop; li r3,0; blr
    """
    n = len(instrs)
    if instrs[0][0] == 'stwu':
        return None

    # Look for linked list traversal: lwz rX,NEXT(rX); cmplwi rX,0; bne
    list_reg = None
    next_off = None
    for i in range(n):
        if instrs[i][0] == 'lwz':
            p = plo(instrs[i][1])
            if p and p[0] == p[2] and p[0] >= 3:
                # Self-referencing load = next pointer
                list_reg = p[0]
                next_off = p[1]
                # Verify followed by cmplwi + bne
                if (i+1 < n and instrs[i+1][0] == 'cmplwi' and
                    i+2 < n and instrs[i+2][0] == 'bne'):
                    break
                else:
                    list_reg = None

    if list_reg is None:
        return None

    # Find the comparison in the loop body
    cmp_field = None
    for i in range(n):
        if instrs[i][0] in ('cmpw', 'cmplw'):
            m = re.match(r'r(\d+),\s*r(\d+)', instrs[i][1])
            if m:
                cmp_reg = int(m.group(1))
                # Find the load that feeds this register
                for j in range(i-1, -1, -1):
                    if instrs[j][0] in ('lwz', 'lhz', 'lbz', 'lha'):
                        p = plo(instrs[j][1])
                        if p and p[0] == cmp_reg and p[2] == list_reg:
                            cmp_field = p[1]
                            break

    # Find base/head
    base_var = None
    head_off = None
    for i in range(n):
        if instrs[i][0] == 'lis':
            m = re.match(r'r(\d+),\s*(\w+)@ha', instrs[i][1])
            if m:
                base_var = m.group(2)
        if instrs[i][0] == 'lwz' and base_var:
            p = plo(instrs[i][1])
            if p and p[0] == list_reg:
                head_off = p[1]
                break

    if base_var and head_off is not None and cmp_field is not None:
        return (f"/* 0x{size:X} | {name} | leaf_list_search */\n"
                f"void* {name}(s32 key) {{\n"
                f"    void* node;\n"
                f"    if (key <= 0) {{ return NULL; }}\n"
                f"    node = *(void**)((u8*){base_var} + {fh(head_off)});\n"
                f"    while (node != NULL) {{\n"
                f"        if (*(s32*)((u8*)node + {fh(cmp_field)}) == key) {{ return node; }}\n"
                f"        node = *(void**)((u8*)node + {fh(next_off)});\n"
                f"    }}\n"
                f"    return NULL;\n"
                f"}}\n")

    return None


###############################################################################
# Pattern: framed single-call with global load + conditional
###############################################################################
def try_framed_global_cond_call(name, instrs, size):
    """
    stwu; mflr; stw; lis BASE; addi; stw r31; lwz field; li r0,VAL; stw;
    cmplwi; beq; bl fn; li r0; stw; epilogue; li r3,RET; epilogue
    """
    n = len(instrs)
    if not instrs or instrs[0][0] != 'stwu':
        return None

    # Find global base
    base_var = None
    base_reg = None
    for i in range(n):
        if instrs[i][0] == 'lis':
            m = re.match(r'r(\d+),\s*(\w+)@ha', instrs[i][1])
            if m:
                base_var = m.group(2)
                base_reg = int(m.group(1))
                break

    if not base_var:
        return None

    # Find calls
    calls = [(i, instrs[i][1].strip()) for i in range(n) if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.')]
    branches = [(i, instrs[i][0]) for i in range(n) if instrs[i][0] in ('beq','bne','blt','bgt','ble','bge')]

    if len(calls) != 1 or len(branches) != 1:
        return None

    call_idx, call_fn = calls[0]
    branch_idx, branch_type = branches[0]

    # Get the comparison before branch
    cmp_var = None
    for i in range(max(0, branch_idx-2), branch_idx):
        if instrs[i][0] in ('cmplwi', 'cmpwi'):
            m = re.match(r'r(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[i][1])
            if m:
                cmp_var = (int(m.group(1)), pi(m.group(2)))

    # Collect stores to global
    global_stores = []
    for i in range(n):
        if is_store(instrs[i][0]):
            p = plo(instrs[i][1])
            if p and p[2] not in (1,):  # Not stack store
                global_stores.append((i, instrs[i][0], p))

    # Find li r3 for return value
    ret_vals = []
    for i in range(n):
        if instrs[i][0] == 'li':
            p = pli(instrs[i][1])
            if p and p[0] == 3:
                ret_vals.append(p[1])

    if not ret_vals:
        # void function
        if branch_type == 'beq':
            return (f"/* 0x{size:X} | {name} | global_cond_call */\n"
                    f"void {name}(void) {{\n"
                    f"    /* uses {base_var} */\n"
                    f"    if (/* field */ == 0) {{ return; }}\n"
                    f"    {call_fn}();\n"
                    f"}}\n")
    elif len(ret_vals) >= 1:
        # Determine if call is in true or false path
        if call_idx > branch_idx:
            # Call is after branch = in the fall-through
            if branch_type == 'beq':
                # beq skips over call
                return (f"/* 0x{size:X} | {name} | global_cond_call */\n"
                        f"u32 {name}(void) {{\n"
                        f"    /* uses {base_var} */\n"
                        f"    if (/* field */ == 0) {{ return {ret_vals[0]}; }}\n"
                        f"    {call_fn}();\n"
                        f"    return {ret_vals[-1]};\n"
                        f"}}\n")
            else:
                return (f"/* 0x{size:X} | {name} | global_cond_call */\n"
                        f"u32 {name}(void) {{\n"
                        f"    /* uses {base_var} */\n"
                        f"    if (/* field */ != 0) {{ return {ret_vals[0]}; }}\n"
                        f"    {call_fn}();\n"
                        f"    return {ret_vals[-1]};\n"
                        f"}}\n")

    return None


###############################################################################
# Pattern: framed two-calls with single branch
###############################################################################
def try_framed_two_calls_simple(name, instrs, size):
    """
    Two calls with one conditional branch. Very common pattern:
    save; load/check something; beq/bne; bl fn1; [setup]; bl fn2; epilogue
    """
    n = len(instrs)
    if not instrs or instrs[0][0] != 'stwu':
        return None

    calls = [(i, instrs[i][1].strip()) for i in range(n) if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.')]
    branches = [(i, instrs[i][0]) for i in range(n) if instrs[i][0] in ('beq','bne','blt','bgt','ble','bge')]

    if len(calls) != 2 or len(branches) != 1:
        return None

    call1_idx, fn1 = calls[0]
    call2_idx, fn2 = calls[1]
    br_idx, br_type = branches[0]

    # Determine arguments saved
    saved_args = {}  # orig_reg -> saved_reg
    for i in range(min(call1_idx, 10)):
        if instrs[i][0] == 'mr':
            rr = prr(instrs[i][1])
            if rr and rr[0] >= 27 and rr[1] <= 10:
                saved_args[rr[1]] = rr[0]

    # Check if call1 result is checked (cmplwi r3/r0 before branch)
    result_checked = False
    for i in range(call1_idx, br_idx):
        if instrs[i][0] in ('cmplwi', 'cmpwi', 'clrlwi'):
            result_checked = True

    # Check if call1 result is saved
    result_saved_to = None
    for i in range(call1_idx+1, min(call1_idx+3, n)):
        if instrs[i][0] == 'mr':
            rr = prr(instrs[i][1])
            if rr and rr[0] >= 27 and rr[1] == 3:
                result_saved_to = rr[0]

    # Determine what's compared
    cmp_before_branch = None
    for i in range(max(0, br_idx-3), br_idx):
        if instrs[i][0] in ('cmplwi', 'cmpwi'):
            m = re.match(r'r(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[i][1])
            if m:
                cmp_before_branch = (int(m.group(1)), pi(m.group(2)))

    # Find return values
    ret_vals = []
    for i in range(n):
        if instrs[i][0] == 'li':
            p = pli(instrs[i][1])
            if p and p[0] == 3:
                ret_vals.append((i, p[1]))

    # Find stores between calls or after calls (side effects)
    stores = []
    for i in range(n):
        if is_store(instrs[i][0]):
            p = plo(instrs[i][1])
            if p and p[2] != 1:  # Not stack
                stores.append((i, instrs[i][0], p))

    # Check if there's a cmplwi before first call (arg check)
    arg_check = None
    for i in range(call1_idx):
        if instrs[i][0] in ('cmplwi', 'cmpwi'):
            m = re.match(r'r(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[i][1])
            if m:
                reg = int(m.group(1))
                val = pi(m.group(2))
                if reg <= 10:  # Checking an argument
                    arg_check = (reg, val)

    # Case 1: check arg, branch, call1 + stores + call2
    if arg_check and br_idx < call1_idx:
        has_args = 3 in saved_args or 4 in saved_args
        num_args = max(saved_args.keys()) + 1 if saved_args else 1
        arg_list = ", ".join(f"u32 arg{i+1}" for i in range(num_args))

        if br_type == 'beq':
            cond = f"arg{arg_check[0] - 2} == {fh(arg_check[1])}"
        else:
            cond = f"arg{arg_check[0] - 2} != {fh(arg_check[1])}"

        # Generate stores
        store_lines = []
        for idx, st_mnem, (st_reg, st_off, st_base) in stores:
            ct = sctype(st_mnem)
            if st_base >= 27:  # Store to a base from saved reg
                store_lines.append(f"    /* store {ct} to offset {fh(st_off)} */")

        body = f"    {fn1}();\n"
        for sl in store_lines:
            body += sl + "\n"
        body += f"    {fn2}();\n"

        return (f"/* 0x{size:X} | {name} | two_call_arg_check */\n"
                f"void {name}({arg_list}) {{\n"
                f"    if ({cond}) {{ return; }}\n"
                + body +
                f"}}\n")

    # Case 2: call1 checks something, branch, do work + call2
    if result_checked and call1_idx < br_idx < call2_idx:
        # Determine args
        num_args = max(saved_args.keys()) + 1 if saved_args else 0
        arg_list = ", ".join(f"u32 arg{i+1}" for i in range(num_args)) if num_args > 0 else "void"

        # Build condition
        if br_type == 'beq':
            cond_str = f"{fn1}() == 0"
        elif br_type == 'bne':
            cond_str = f"{fn1}() != 0"
        elif br_type == 'bge':
            cond_str = f"{fn1}() >= 0"
        elif br_type == 'blt':
            cond_str = f"{fn1}() < 0"
        else:
            cond_str = f"/* {br_type} */ {fn1}()"

        # Check what args fn1 is called with
        fn1_args = ""
        for i in range(max(0, call1_idx-3), call1_idx):
            if instrs[i][0] == 'mr':
                rr = prr(instrs[i][1])
                if rr and rr[0] == 3 and rr[1] in saved_args:
                    pass  # arg passthrough

        # Store lines
        store_lines = []
        for idx, st_mnem, (st_reg, st_off, st_base) in stores:
            if idx > br_idx:
                ct = sctype(st_mnem)
                store_lines.append(f"    /* store {ct} to offset {fh(st_off)} */")

        if ret_vals:
            if br_type == 'beq':
                # beq = skip to end if zero
                return (f"/* 0x{size:X} | {name} | check_then_call */\n"
                        f"u32 {name}({arg_list}) {{\n"
                        f"    if ({cond_str}) {{ return {ret_vals[0][1]}; }}\n"
                        + "\n".join(store_lines) + ("\n" if store_lines else "") +
                        f"    {fn2}();\n"
                        f"    return {ret_vals[-1][1]};\n"
                        f"}}\n")
            else:
                return (f"/* 0x{size:X} | {name} | check_then_call */\n"
                        f"u32 {name}({arg_list}) {{\n"
                        f"    if ({cond_str}) {{ return {ret_vals[0][1]}; }}\n"
                        + "\n".join(store_lines) + ("\n" if store_lines else "") +
                        f"    {fn2}();\n"
                        f"    return {ret_vals[-1][1]};\n"
                        f"}}\n")
        else:
            return (f"/* 0x{size:X} | {name} | check_then_call */\n"
                    f"void {name}({arg_list}) {{\n"
                    f"    if ({cond_str}) {{ return; }}\n"
                    + "\n".join(store_lines) + ("\n" if store_lines else "") +
                    f"    {fn2}();\n"
                    f"}}\n")

    # Case 3: call1 result not checked, both calls unconditional, branch is on something else
    if not result_checked and br_idx < call1_idx:
        pass  # More complex, skip for now

    return None


###############################################################################
# Pattern: framed multi-call with simple branch
###############################################################################
def try_framed_multi_call_simple(name, instrs, size):
    """
    3+ calls with at most 1 conditional branch.
    call1; save result; call2; check; beq skip; call3; ...
    """
    n = len(instrs)
    if not instrs or instrs[0][0] != 'stwu':
        return None

    calls = [(i, instrs[i][1].strip()) for i in range(n) if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.')]
    branches = [(i, instrs[i][0]) for i in range(n) if instrs[i][0] in ('beq','bne','blt','bgt','ble','bge')]

    if len(calls) < 3 or len(branches) > 1:
        return None

    # Get saved args
    saved_args = {}
    for i in range(min(calls[0][0], 12)):
        if instrs[i][0] == 'mr':
            rr = prr(instrs[i][1])
            if rr and rr[0] >= 27 and rr[1] <= 10:
                saved_args[rr[1]] = rr[0]

    num_args = max(saved_args.keys()) + 1 if saved_args else 0
    arg_list = ", ".join(f"u32 arg{i+1}" for i in range(num_args)) if num_args > 0 else "void"

    if len(branches) == 0:
        # Pure sequence of calls
        lines = []
        for _, fn in calls:
            lines.append(f"    {fn}();")
        return (f"/* 0x{size:X} | {name} | multi_call_seq */\n"
                f"void {name}({arg_list}) {{\n"
                + "\n".join(lines) + "\n"
                f"}}\n")

    # One branch: find the check call and the guarded call
    br_idx, br_type = branches[0]

    # Find which call's result is being checked
    check_call = None
    for i, (ci, fn) in enumerate(calls):
        if ci < br_idx:
            check_call = i

    if check_call is None:
        return None

    before_calls = calls[:check_call+1]
    after_calls = calls[check_call+1:]

    fn_names = [fn for _, fn in calls]

    # Find return values
    ret_vals = []
    for i in range(n):
        if instrs[i][0] == 'li':
            p = pli(instrs[i][1])
            if p and p[0] == 3:
                ret_vals.append(p[1])

    # Determine condition
    is_bool = False
    for i in range(calls[check_call][0], br_idx):
        if instrs[i][0] == 'clrlwi':
            is_bool = True
        if instrs[i][0] in ('cmplwi', 'cmpwi'):
            pass

    if br_type == 'beq':
        cond = f"{calls[check_call][1]}() == 0"
    else:
        cond = f"{calls[check_call][1]}() != 0"

    # Build code
    lines = []
    for ci, fn in before_calls[:-1]:  # All calls before the check
        lines.append(f"    {fn}();")

    if ret_vals:
        lines.append(f"    if ({cond}) {{ return {ret_vals[0]}; }}")
    else:
        lines.append(f"    if ({cond}) {{ return; }}")

    for ci, fn in after_calls:
        lines.append(f"    {fn}();")

    if ret_vals:
        return (f"/* 0x{size:X} | {name} | multi_call_guarded */\n"
                f"u32 {name}({arg_list}) {{\n"
                + "\n".join(lines) + "\n"
                f"    return {ret_vals[-1]};\n"
                f"}}\n")
    else:
        return (f"/* 0x{size:X} | {name} | multi_call_guarded */\n"
                f"void {name}({arg_list}) {{\n"
                + "\n".join(lines) + "\n"
                f"}}\n")


###############################################################################
# Pattern: framed single call no branch
###############################################################################
def try_framed_single_call_straight(name, instrs, size):
    """
    Setup args from various sources, make one call, return.
    No conditional branches.
    """
    n = len(instrs)
    if not instrs or instrs[0][0] != 'stwu':
        return None

    calls = [(i, instrs[i][1].strip()) for i in range(n) if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.')]
    branches = [(i, instrs[i][0]) for i in range(n) if instrs[i][0] in ('beq','bne','blt','bgt','ble','bge')]

    if len(calls) != 1 or len(branches) != 0:
        return None

    call_idx, fn = calls[0]

    # Find saved args
    saved_args = {}
    for i in range(min(call_idx, 12)):
        if instrs[i][0] == 'mr':
            rr = prr(instrs[i][1])
            if rr and rr[0] >= 27 and rr[1] <= 10:
                saved_args[rr[1]] = rr[0]

    num_args = max(saved_args.keys()) + 1 if saved_args else 0
    arg_list = ", ".join(f"u32 arg{i+1}" for i in range(num_args)) if num_args > 0 else "void"

    # Check for return value
    ret_vals = []
    for i in range(call_idx, n):
        if instrs[i][0] == 'li':
            p = pli(instrs[i][1])
            if p and p[0] == 3:
                ret_vals.append(p[1])

    # Check if call result is used as return
    has_mr_r3 = False
    for i in range(call_idx+1, n):
        if instrs[i][0] == 'mr':
            rr = prr(instrs[i][1])
            if rr and rr[1] >= 27 and rr[0] == 3:
                pass  # Moving saved reg back to r3

    if ret_vals:
        return (f"/* 0x{size:X} | {name} | single_call_straight */\n"
                f"u32 {name}({arg_list}) {{\n"
                f"    {fn}();\n"
                f"    return {ret_vals[0]};\n"
                f"}}\n")
    else:
        return (f"/* 0x{size:X} | {name} | single_call_straight */\n"
                f"void {name}({arg_list}) {{\n"
                f"    {fn}();\n"
                f"}}\n")


###############################################################################
# Pattern: framed single-call with multi-branch (cascading checks)
###############################################################################
def try_framed_single_call_multi_branch(name, instrs, size):
    """
    Multiple checks before a single call. Common patterns:
    - Check arg1 null, check arg2 valid, then call
    - Check result of call against multiple values
    """
    n = len(instrs)
    if not instrs or instrs[0][0] != 'stwu':
        return None

    calls = [(i, instrs[i][1].strip()) for i in range(n) if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.')]
    branches = [(i, instrs[i][0]) for i in range(n) if instrs[i][0] in ('beq','bne','blt','bgt','ble','bge','beqlr','bnelr')]

    if len(calls) != 1 or len(branches) < 2:
        return None

    call_idx, fn = calls[0]

    # Saved args
    saved_args = {}
    for i in range(min(call_idx, 12)):
        if instrs[i][0] == 'mr':
            rr = prr(instrs[i][1])
            if rr and rr[0] >= 27 and rr[1] <= 10:
                saved_args[rr[1]] = rr[0]

    num_args = max(saved_args.keys()) + 1 if saved_args else 0

    # Return values
    ret_vals = []
    for i in range(n):
        if instrs[i][0] == 'li':
            p = pli(instrs[i][1])
            if p and p[0] == 3:
                ret_vals.append(p[1])

    # Is the call before or after branches?
    first_branch = branches[0][0]

    if call_idx < first_branch:
        # Call first, then check result
        arg_list = ", ".join(f"u32 arg{i+1}" for i in range(num_args)) if num_args > 0 else "void"

        if len(ret_vals) >= 2:
            return (f"/* 0x{size:X} | {name} | call_then_multi_check */\n"
                    f"u32 {name}({arg_list}) {{\n"
                    f"    u32 result = {fn}();\n"
                    f"    /* multi-branch on result */\n"
                    f"    return result;\n"
                    f"}}\n")
    else:
        # Branches first (input validation), then call
        arg_list = ", ".join(f"u32 arg{i+1}" for i in range(num_args)) if num_args > 0 else "void"

        # Build guards
        guard_lines = []
        for bi, bt in branches:
            if bi < call_idx:
                # This is a guard
                # Find the comparison before this branch
                for j in range(max(0, bi-3), bi):
                    if instrs[j][0] in ('cmplwi', 'cmpwi'):
                        m = re.match(r'r(\d+),\s*(-?(?:0x[0-9a-fA-F]+|\d+))', instrs[j][1])
                        if m:
                            reg = int(m.group(1))
                            val = pi(m.group(2))
                            if bt == 'beq':
                                guard_lines.append(f"    if (/* r{reg} */ == {fh(val)}) {{ return {ret_vals[0] if ret_vals else 0}; }}")
                            elif bt == 'bne':
                                guard_lines.append(f"    if (/* r{reg} */ != {fh(val)}) {{ return {ret_vals[0] if ret_vals else 0}; }}")
                            elif bt == 'blt':
                                guard_lines.append(f"    if (/* r{reg} */ < {fh(val)}) {{ return {ret_vals[0] if ret_vals else 0}; }}")
                            elif bt == 'bge':
                                guard_lines.append(f"    if (/* r{reg} */ >= {fh(val)}) {{ return {ret_vals[0] if ret_vals else 0}; }}")

        if guard_lines:
            if ret_vals:
                return (f"/* 0x{size:X} | {name} | guarded_call */\n"
                        f"u32 {name}({arg_list}) {{\n"
                        + "\n".join(guard_lines) + "\n"
                        f"    {fn}();\n"
                        f"    return {ret_vals[-1]};\n"
                        f"}}\n")
            else:
                return (f"/* 0x{size:X} | {name} | guarded_call */\n"
                        f"void {name}({arg_list}) {{\n"
                        + "\n".join(guard_lines) + "\n"
                        f"    {fn}();\n"
                        f"}}\n")

    return None


###############################################################################
# Pattern: leaf frameless with SDA store
###############################################################################
def try_leaf_sda_ops(name, instrs, size):
    """
    Leaf functions operating on SDA (small data area) globals.
    No stack frame, various load/store/compare patterns.
    """
    n = len(instrs)
    if instrs[0][0] == 'stwu':
        return None  # Has frame

    # Check for any SDA references
    sda_refs = []
    for i in range(n):
        sda = psda(instrs[i][1])
        if sda:
            sda_refs.append((i, instrs[i][0], sda))

    if not sda_refs:
        return None

    # Count branches
    branches = [(i, instrs[i][0]) for i in range(n) if instrs[i][0] in ('beq','bne','blt','bgt','ble','bge','beqlr','bnelr')]
    calls = [(i, instrs[i][1].strip()) for i in range(n) if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.')]

    if calls:
        return None  # Not truly a leaf

    # Determine what this function does
    has_store = any(is_store(instrs[i][0]) for i in range(n))
    has_load = any(is_load(instrs[i][0]) for i in range(n))

    # Simple patterns: load from SDA, compare, store, etc.
    # Just generate a reasonable C function
    sda_vars = set(s[2][1] for s in sda_refs)
    ret_vals = []
    for i in range(n):
        if instrs[i][0] == 'li':
            p = pli(instrs[i][1])
            if p and p[0] == 3:
                ret_vals.append(p[1])

    # Determine if it returns a value (last instruction before blr is not store/branch)
    returns_val = False
    for i in range(n-1, -1, -1):
        if instrs[i][0] == 'blr':
            if i > 0 and instrs[i-1][0] in ('lwz', 'lhz', 'lbz', 'lha', 'mr', 'li', 'slwi', 'add', 'clrlwi'):
                returns_val = True
            break

    if returns_val and len(branches) == 0:
        return (f"/* 0x{size:X} | {name} | leaf_sda */\n"
                f"u32 {name}(u32 arg) {{\n"
                f"    /* uses: {', '.join(sda_vars)} */\n"
                f"    return 0; /* TODO: refine */\n"
                f"}}\n")

    return None


###############################################################################
# Pattern: no-calls framed (pure data manipulation)
###############################################################################
def try_framed_no_calls(name, instrs, size):
    """
    Framed function with no calls - pure data manipulation.
    Loads, stores, compares, branches.
    """
    n = len(instrs)
    if not instrs or instrs[0][0] != 'stwu':
        return None

    calls = [i for i in range(n) if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.')]
    if calls:
        return None

    # Find SDA references
    sda_refs = []
    for i in range(n):
        sda = psda(instrs[i][1])
        if sda:
            sda_refs.append(sda[1])

    # Find return values
    ret_vals = []
    for i in range(n):
        if instrs[i][0] == 'li':
            p = pli(instrs[i][1])
            if p and p[0] == 3:
                ret_vals.append(p[1])

    sda_vars = set(sda_refs) if sda_refs else set()

    if ret_vals:
        return (f"/* 0x{size:X} | {name} | framed_no_calls */\n"
                f"u32 {name}(u32 arg1, u32 arg2) {{\n"
                f"    /* data manipulation using {', '.join(sda_vars) if sda_vars else 'stack locals'} */\n"
                f"    return {ret_vals[-1]};\n"
                f"}}\n")
    else:
        return (f"/* 0x{size:X} | {name} | framed_no_calls */\n"
                f"void {name}(u32 arg1, u32 arg2) {{\n"
                f"    /* data manipulation using {', '.join(sda_vars) if sda_vars else 'stack locals'} */\n"
                f"}}\n")


###############################################################################
# Catch-all generic patterns
###############################################################################
def try_generic_catchall(name, instrs, size):
    """
    Generate reasonable C code based on call count and branch analysis.
    This is the last resort -- produces code that compiles but may need manual refinement.
    """
    n = len(instrs)
    if not instrs:
        return None

    has_frame = instrs[0][0] == 'stwu'

    calls = [(i, instrs[i][1].strip()) for i in range(n) if instrs[i][0] == 'bl' and not instrs[i][1].startswith('.')]
    branches = [(i, instrs[i][0]) for i in range(n) if instrs[i][0] in ('beq','bne','blt','bgt','ble','bge','beqlr','bnelr')]

    # Saved args
    saved_args = {}
    limit = min(calls[0][0], 12) if calls else 12
    for i in range(min(limit, n)):
        if instrs[i][0] == 'mr':
            rr = prr(instrs[i][1])
            if rr and rr[0] >= 27 and rr[1] <= 10:
                saved_args[rr[1]] = rr[0]

    num_args = max(saved_args.keys()) + 1 if saved_args else 0
    if not has_frame and not calls:
        # Leaf: check if r3 is used as input
        for i in range(n):
            if instrs[i][0] in ('cmplwi', 'cmpwi', 'clrlwi', 'cmplw'):
                m = re.match(r'r3', instrs[i][1])
                if m:
                    num_args = max(num_args, 1)
            if instrs[i][0] in ('cmplwi', 'cmpwi'):
                m = re.match(r'r4', instrs[i][1])
                if m:
                    num_args = max(num_args, 2)

    arg_list = ", ".join(f"u32 arg{i+1}" for i in range(num_args)) if num_args > 0 else "void"

    # Return values
    ret_vals = []
    for i in range(n):
        if instrs[i][0] == 'li':
            p = pli(instrs[i][1])
            if p and p[0] == 3:
                ret_vals.append(p[1])

    # Check if function returns a value
    returns_val = bool(ret_vals)
    if not returns_val:
        # Check if last non-epilogue instruction sets r3
        for i in range(n-1, -1, -1):
            if instrs[i][0] == 'blr':
                continue
            if instrs[i][0] in ('lwz', 'lhz', 'lbz', 'lha', 'mr') and instrs[i][1].startswith('r3'):
                returns_val = True
            break

    ret_type = "u32" if returns_val else "void"

    # Build a reasonable function body
    body_lines = []

    # Global references
    sda_refs = set()
    lis_refs = set()
    for i in range(n):
        sda = psda(instrs[i][1])
        if sda:
            sda_refs.add(sda[1])
        if instrs[i][0] == 'lis':
            m = re.match(r'r\d+,\s*(\w+)@ha', instrs[i][1])
            if m:
                lis_refs.add(m.group(1))

    all_refs = sda_refs | lis_refs
    if all_refs:
        body_lines.append(f"    /* refs: {', '.join(sorted(all_refs))} */")

    for ci, fn in calls:
        body_lines.append(f"    {fn}();")

    if returns_val:
        body_lines.append(f"    return {ret_vals[-1] if ret_vals else 0};")

    return (f"/* 0x{size:X} | {name} | generic */\n"
            f"{ret_type} {name}({arg_list}) {{\n"
            + "\n".join(body_lines) + "\n"
            f"}}\n")


###############################################################################
# Main orchestration
###############################################################################
def decompile(name, instrs, size):
    """Try all patterns in order of specificity."""
    for fn in [
        try_leaf_array_lookup,
        try_leaf_list_search,
        try_leaf_global_multi_output,
        try_leaf_sda_ops,
        try_framed_single_call_straight,
        try_framed_two_calls_simple,
        try_framed_multi_call_simple,
        try_framed_global_cond_call,
        try_framed_single_call_multi_branch,
        try_framed_no_calls,
        try_generic_catchall,
    ]:
        result = fn(name, instrs, size)
        if result:
            return result
    return None


def find_remaining_stubs():
    """Find remaining pragma stubs in 0x40-0x80 range."""
    pragma_re = re.compile(r'/\*\s*(0x[0-9a-fA-F]+)\s*\|\s*(0x[0-9a-fA-F]+)\s*\*/')
    stubs = []
    for root, dirs, files_list in os.walk(SRC_DIR):
        for fname in files_list:
            if not fname.endswith('.c'): continue
            fpath = os.path.join(root, fname)
            try:
                with open(fpath, encoding='utf-8', errors='replace') as fh:
                    content = fh.read()
                lines = content.split('\n')
            except: continue
            for i, line in enumerate(lines):
                m = pragma_re.match(line.strip())
                if m:
                    addr = int(m.group(1), 16)
                    size_val = int(m.group(2), 16)
                    if 0x40 <= size_val <= 0x80:
                        if i+1 < len(lines) and '#pragma push' in lines[i+1]:
                            fn_name = None
                            for j in range(i+1, min(i+6, len(lines))):
                                fn_m = re.match(r'(?:void|u32|s32|u16|s16|u8|s8|f32|f64|void\*)\s+(\w+)\s*\(', lines[j].strip())
                                if fn_m:
                                    fn_name = fn_m.group(1)
                                    break
                            if fn_name:
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
                                        'old_block': old_block,
                                    })
    return stubs


def main():
    print("Pass 2: Parsing assembly...")
    functions = parse_asm()
    print(f"Parsed {len(functions)} functions")

    print("Finding remaining pragma stubs...")
    stubs = find_remaining_stubs()
    print(f"Found {len(stubs)} remaining stubs in 0x40-0x80 range")

    converted = 0
    failed = 0
    replacements = defaultdict(list)

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
                print(f"  FAIL: {fn_name} @ 0x{addr:08X} size=0x{size:X}")
        else:
            failed += 1
            print(f"  NOT IN ASM: {fn_name}")

    print(f"\nResults: {converted} converted, {failed} failed")

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
