#!/usr/bin/env python3
"""
Decompile gs_render.c stub functions from PPC assembly.

This script reads the assembly file, extracts each function's instructions,
and converts them to C89 code by recognizing common patterns in the
GS rendering pipeline.

The output is the complete gs_render.c file with all stubs replaced.
"""

import re
import sys
import os

ASM_FILE = os.path.join(os.path.dirname(__file__), '..',
    'build', 'GC6E01', 'asm', 'auto_01_800055E0_text.s')

def load_asm_funcs(path):
    """Load all function bodies from the assembly file."""
    with open(path, 'r') as f:
        text = f.read()
    funcs = {}
    pat = r'\.fn (fn_[0-9A-Fa-f]+), global\n(.*?)\.endfn \1'
    for m in re.finditer(pat, text, re.DOTALL):
        funcs[m.group(1)] = m.group(2).strip()
    return funcs

def parse_instructions(body):
    """Parse asm body into structured instruction list."""
    result = []
    for line in body.split('\n'):
        line = line.strip()
        if line.startswith('.L_'):
            label = line.rstrip(':').strip()
            result.append({'type': 'label', 'name': label})
            continue
        m = re.match(r'/\*\s+([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f ]+\*/\s+(.*)', line)
        if m:
            addr = m.group(1)
            inst_text = m.group(2).strip()
            parts = inst_text.split(None, 1)
            mnemonic = parts[0] if parts else ''
            operands = parts[1] if len(parts) > 1 else ''
            result.append({
                'type': 'inst',
                'addr': addr,
                'mnem': mnemonic,
                'ops': operands,
                'raw': inst_text,
            })
    return result

def parse_operands(ops_str):
    """Parse comma-separated operands."""
    return [o.strip() for o in ops_str.split(',')]

def parse_mem_operand(op):
    """Parse memory operand like '0x4a4(r4)' or 'lbl_XXX@sda21(r0)'."""
    m = re.match(r'(.*)\((\w+)\)', op)
    if m:
        offset_str = m.group(1).strip()
        base = m.group(2)
        # Handle sda21 labels
        if '@sda21' in offset_str:
            label = offset_str.replace('@sda21', '')
            return {'label': label, 'base': base}
        # Handle ha/l labels
        if '@ha' in offset_str or '@l' in offset_str:
            label = re.sub(r'@\w+', '', offset_str)
            return {'label': label, 'base': base}
        # Numeric offset
        try:
            if offset_str.startswith('-'):
                offset = -int(offset_str[1:], 0)
            else:
                offset = int(offset_str, 0)
        except ValueError:
            offset = offset_str
        return {'offset': offset, 'base': base}
    return None


class PPCDecompiler:
    """Simple pattern-based PPC to C decompiler for GS render functions."""

    # GX FIFO address: 0xCC008000
    GX_FIFO = 0xCC008000

    def __init__(self, all_funcs):
        self.all_funcs = all_funcs
        # Track which SDA labels we've declared
        self.sda_decls = set()
        self.ha_decls = set()
        self.extern_fn_decls = set()

    def decompile(self, fn_name):
        """Decompile a function to C code. Returns C body string."""
        if fn_name not in self.all_funcs:
            return None

        insns = parse_instructions(self.all_funcs[fn_name])
        if not insns:
            return None

        # Filter to just instructions (no labels for now)
        real_insns = [i for i in insns if i['type'] == 'inst']

        # Try various pattern matchers
        c = self._try_trivial_patterns(fn_name, insns, real_insns)
        if c is not None:
            return c

        # Fall back to asm_inline approach
        return self._decompile_general(fn_name, insns, real_insns)

    def _try_trivial_patterns(self, fn_name, insns, real_insns):
        """Try to match simple function patterns."""
        n = len(real_insns)

        # Pattern: blr only (empty function)
        if n == 1 and real_insns[0]['mnem'] == 'blr':
            return '    /* empty */\n'

        # Pattern: simple 3-instruction getter/setter with sda21
        if n == 3 and real_insns[-1]['mnem'] == 'blr':
            return self._try_simple_accessor(fn_name, real_insns)

        # Pattern: 4-5 instruction leaf with no branches
        if n <= 5 and real_insns[-1]['mnem'] == 'blr':
            has_branch = any(i['mnem'].startswith('b') and i['mnem'] != 'blr'
                           for i in real_insns)
            if not has_branch:
                return self._try_leaf_function(fn_name, insns, real_insns)

        # Pattern: GX FIFO writers (write to 0xCC008000)
        if self._is_gx_fifo_writer(real_insns):
            return self._decompile_gx_fifo_writer(fn_name, insns, real_insns)

        # Pattern: simple wrapper (prologue + bl + epilogue)
        if self._is_simple_wrapper(real_insns):
            return self._decompile_simple_wrapper(fn_name, real_insns)

        return None

    def _try_simple_accessor(self, fn_name, insns):
        """Handle 3-instruction accessor patterns."""
        i0, i1, i2 = insns[0], insns[1], insns[2]

        # Getter: lwz rX, sda(r0); lwz/lbz/lhz r3, off(rX); blr
        if (i0['mnem'] == 'lwz' and '@sda21' in i0['ops'] and
            i1['mnem'] in ('lwz', 'lbz', 'lhz', 'lfs') and
            i2['mnem'] == 'blr'):
            ops0 = parse_operands(i0['ops'])
            sda_label = re.search(r'(lbl_\w+)', i0['ops']).group(1)
            mem1 = parse_mem_operand(parse_operands(i1['ops'])[1])

            load_type = {'lwz': 'u32', 'lbz': 'u8', 'lhz': 'u16', 'lfs': 'f32'}[i1['mnem']]
            cast = {'lwz': '*(u32*)', 'lbz': '*(u8*)', 'lhz': '*(u16*)', 'lfs': '*(f32*)'}[i1['mnem']]
            offset = mem1['offset'] if isinstance(mem1.get('offset'), int) else 0

            self.sda_decls.add(sda_label)
            if i1['mnem'] == 'lfs':
                return f'    return *(f32*)((u8*){sda_label} + 0x{offset:X});\n'
            return f'    return *({load_type}*)((u8*){sda_label} + 0x{offset:X});\n'

        # Setter: lwz rX, sda(r0); stw/stb/sth r3, off(rX); blr
        if (i0['mnem'] == 'lwz' and '@sda21' in i0['ops'] and
            i1['mnem'] in ('stw', 'stb', 'sth', 'stfs') and
            i2['mnem'] == 'blr'):
            sda_label = re.search(r'(lbl_\w+)', i0['ops']).group(1)
            ops1 = parse_operands(i1['ops'])
            src_reg = ops1[0]
            mem1 = parse_mem_operand(ops1[1])
            offset = mem1['offset'] if isinstance(mem1.get('offset'), int) else 0

            store_type = {'stw': 'u32', 'stb': 'u8', 'sth': 'u16', 'stfs': 'f32'}[i1['mnem']]
            self.sda_decls.add(sda_label)

            # r3 is first parameter
            return f'    *({store_type}*)((u8*){sda_label} + 0x{offset:X}) = p1;\n'

        # li r0, val; stb r0, off(r3); blr  -- set constant on param
        if (i0['mnem'] == 'li' and
            i1['mnem'] in ('stb', 'sth', 'stw') and
            i2['mnem'] == 'blr'):
            ops0 = parse_operands(i0['ops'])
            val = int(ops0[1], 0)
            ops1 = parse_operands(i1['ops'])
            mem = parse_mem_operand(ops1[1])
            offset = mem['offset'] if isinstance(mem.get('offset'), int) else 0
            store_type = {'stb': 'u8', 'sth': 'u16', 'stw': 'u32'}[i1['mnem']]
            return f'    *({store_type}*)((u8*)obj + 0x{offset:X}) = {val};\n'

        return None

    def _try_leaf_function(self, fn_name, all_insns, real_insns):
        """Handle small leaf functions with no branches."""
        lines = []
        # Track register state for simple forwarding
        regs = {}  # reg -> expression string

        for inst in real_insns:
            if inst['mnem'] == 'blr':
                break

            ops = parse_operands(inst['ops'])
            mnem = inst['mnem']

            if mnem in ('stfs',):
                mem = parse_mem_operand(ops[1])
                if mem and isinstance(mem.get('offset'), int):
                    # Determine which float param this is
                    freg = ops[0]
                    fnum = int(freg[1:])
                    offset = mem['offset']
                    lines.append(f'    *(f32*)((u8*){mem["base"]} + 0x{offset:X}) = f{fnum};')
                else:
                    return None
            elif mnem in ('stw', 'stb', 'sth'):
                mem = parse_mem_operand(ops[1])
                if mem and isinstance(mem.get('offset'), int):
                    store_type = {'stw': 'u32', 'stb': 'u8', 'sth': 'u16'}[mnem]
                    offset = mem['offset']
                    lines.append(f'    *({store_type}*)((u8*){mem["base"]} + 0x{offset:X}) = {ops[0]};')
                else:
                    return None
            elif mnem == 'lfs':
                pass  # load tracked in regs
            elif mnem in ('lwz', 'lbz', 'lhz'):
                pass  # load tracked in regs
            else:
                return None

        if lines:
            return '\n'.join(lines) + '\n'
        return None

    def _is_gx_fifo_writer(self, insns):
        """Check if function writes to GX FIFO (0xCC008000)."""
        for inst in insns:
            if inst['mnem'] == 'lis':
                ops = parse_operands(inst['ops'])
                if len(ops) >= 2 and ops[1].strip() == '0xcc01':
                    return True
        return False

    def _is_simple_wrapper(self, insns):
        """Check if function is a simple call wrapper."""
        if len(insns) < 6:
            return False
        # Check for standard prologue/epilogue with single bl
        bl_count = sum(1 for i in insns if i['mnem'] == 'bl')
        has_prologue = (insns[0]['mnem'] == 'stwu' and insns[1]['mnem'] == 'mflr')
        return bl_count == 1 and has_prologue and len(insns) <= 8

    def _decompile_simple_wrapper(self, fn_name, insns):
        """Decompile a simple function that just calls another."""
        for inst in insns:
            if inst['mnem'] == 'bl' and not inst['ops'].startswith('_'):
                target = inst['ops'].strip()
                self.extern_fn_decls.add(target)
                return f'    {target}();\n'
        return None

    def _decompile_gx_fifo_writer(self, fn_name, all_insns, real_insns):
        """Decompile GX FIFO writer functions."""
        # These write state fields to the GX FIFO at 0xCC008000
        # Pattern: load state ptr, load field, lis CC01, store to -0x8000(r3)

        lines = []
        # We'll build up the C code

        # Analyze what's being written
        writes = []
        state_ptr_reg = None
        index_reg = None
        index_multiply = None
        fifo_reg = None

        for inst in real_insns:
            mnem = inst['mnem']
            ops_str = inst['ops']
            ops = parse_operands(ops_str) if ops_str else []

            if mnem == 'blr':
                break

            if mnem == 'lwz' and '@sda21' in ops_str:
                label = re.search(r'(lbl_\w+)', ops_str).group(1)
                dst = ops[0]
                state_ptr_reg = dst
                self.sda_decls.add(label)

            elif mnem == 'lis' and '0xcc01' in ops_str:
                fifo_reg = ops[0]

            elif mnem == 'mulli':
                index_multiply = int(ops[2], 0)
                index_reg = ops[1]

            elif mnem == 'slwi':
                shift = int(ops[2])
                # slwi r0, rX, N means r0 = rX << N
                pass

            elif mnem == 'add':
                pass  # combining index with base

            elif mnem in ('lbz', 'lhz', 'lwz', 'lfs'):
                # Loading a field value
                mem = parse_mem_operand(ops[1])
                if mem and isinstance(mem.get('offset'), int):
                    offset = mem['offset']
                    load_type = {'lbz': 'u8', 'lhz': 'u16', 'lwz': 'u32', 'lfs': 'f32'}[mnem]
                    writes.append(('load', ops[0], load_type, offset, mem.get('base', '')))

            elif mnem in ('stb', 'sth', 'stw', 'stfs'):
                mem = parse_mem_operand(ops[1])
                if mem and isinstance(mem.get('offset'), int):
                    offset = mem['offset']
                    if offset == -0x8000 or offset == 0xFFFF8000:
                        store_type = {'stb': 'u8', 'sth': 'u16', 'stw': 'u32', 'stfs': 'f32'}[mnem]
                        writes.append(('fifo_write', ops[0], store_type))

        return None  # Fall through to general decompiler

    def _decompile_general(self, fn_name, all_insns, real_insns):
        """General decompilation using inline asm pattern recognition."""
        # For complex functions, we'll generate structured C with
        # register variables and goto labels

        lines = []
        # Track if we have a stack frame
        has_frame = real_insns[0]['mnem'] == 'stwu' if real_insns else False

        # Track register state
        regs = {}
        fregs = {}

        # Collect labels
        label_set = set()
        for inst in all_insns:
            if inst['type'] == 'label':
                label_set.add(inst['name'])

        # If function has labels or branches, use goto-based approach
        has_branches = any(i['mnem'].startswith('b') and i['mnem'] not in ('blr', 'bctrl', 'bctr')
                         for i in real_insns if i['type'] == 'inst')

        if not has_branches and not has_frame:
            # Simple leaf function
            return self._decompile_leaf_detailed(fn_name, all_insns, real_insns)

        # Complex function - generate register-level C
        return self._decompile_register_level(fn_name, all_insns, real_insns)

    def _decompile_leaf_detailed(self, fn_name, all_insns, real_insns):
        """Decompile leaf function without branches."""
        # Track register values
        regs = {}
        lines = []
        local_decls = []

        for inst in real_insns:
            if inst['type'] != 'inst':
                continue
            mnem = inst['mnem']
            if mnem == 'blr':
                break

            ops = parse_operands(inst['ops']) if inst['ops'] else []

            if mnem == 'lwz' and '@sda21' in inst['ops']:
                label = re.search(r'(lbl_\w+)', inst['ops']).group(1)
                self.sda_decls.add(label)
                regs[ops[0]] = f'(u8*){label}'

            elif mnem == 'lis':
                val = int(ops[1], 0)
                regs[ops[0]] = f'0x{val:08X}'

            elif mnem == 'mulli':
                regs[ops[0]] = f'({ops[1]} * {int(ops[2], 0)})'

            elif mnem == 'slwi':
                shift = int(ops[2])
                regs[ops[0]] = f'({ops[1]} << {shift})'

            elif mnem == 'add':
                regs[ops[0]] = f'({ops[1]} + {ops[2]})'

            elif mnem == 'addi':
                mem = parse_mem_operand(f'{ops[2]}({ops[1]})')
                # Actually addi is: rD, rA, SIMM
                val = int(ops[2], 0)
                if ops[1] == 'r0':
                    regs[ops[0]] = f'{val}'
                else:
                    regs[ops[0]] = f'({ops[1]} + {val})'

            elif mnem in ('lbz', 'lhz', 'lwz', 'lfs'):
                mem = parse_mem_operand(ops[1])
                if mem:
                    offset = mem.get('offset', 0)
                    base = mem['base']
                    load_type = {'lbz': 'u8', 'lhz': 'u16', 'lwz': 'u32', 'lfs': 'f32'}[mnem]
                    base_expr = regs.get(base, base)
                    if isinstance(offset, int):
                        regs[ops[0]] = f'*({load_type}*)({base_expr} + 0x{offset:X})'
                    else:
                        regs[ops[0]] = f'*({load_type}*)({base_expr} + {offset})'

            elif mnem in ('stb', 'sth', 'stw', 'stfs'):
                mem = parse_mem_operand(ops[1])
                if mem:
                    offset = mem.get('offset', 0)
                    base = mem['base']
                    store_type = {'stb': 'u8', 'sth': 'u16', 'stw': 'u32', 'stfs': 'f32'}[mnem]
                    base_expr = regs.get(base, base)
                    src = regs.get(ops[0], ops[0])
                    if isinstance(offset, int):
                        if offset < 0:
                            lines.append(f'    *({store_type}*)(({base_expr}) + ({offset})) = {src};')
                        else:
                            lines.append(f'    *({store_type}*)(({base_expr}) + 0x{offset:X}) = {src};')
                    else:
                        lines.append(f'    *({store_type}*)(({base_expr}) + {offset}) = {src};')

            elif mnem == 'li':
                regs[ops[0]] = ops[1]

            elif mnem == 'mr':
                regs[ops[0]] = regs.get(ops[1], ops[1])

        if lines:
            return '\n'.join(lines) + '\n'
        return None

    def _decompile_register_level(self, fn_name, all_insns, real_insns):
        """Generate register-level C code for complex functions."""
        # This generates C code that uses local variables for registers
        # and goto for branches. Not pretty but functionally equivalent.

        lines = []
        used_regs = set()
        used_fregs = set()
        used_labels = set()
        has_frame = False
        frame_size = 0
        saved_regs = []
        stack_vars = {}

        # First pass: analyze register/label usage
        for inst in all_insns:
            if inst['type'] == 'label':
                used_labels.add(inst['name'])
                continue
            if inst['type'] != 'inst':
                continue
            ops_str = inst.get('ops', '')
            # Find register references
            for m in re.finditer(r'\br(\d+)\b', ops_str):
                rn = int(m.group(1))
                if 3 <= rn <= 31:
                    used_regs.add(rn)
            for m in re.finditer(r'\bf(\d+)\b', ops_str):
                fn = int(m.group(1))
                used_fregs.add(fn)

            if inst['mnem'] == 'stwu' and 'r1' in ops_str:
                has_frame = True
                mem = parse_mem_operand(parse_operands(ops_str)[1])
                if mem and isinstance(mem.get('offset'), int):
                    frame_size = -mem['offset']

        # Generate local variable declarations
        reg_decls = []
        for r in sorted(used_regs):
            if r >= 3 and r <= 12:  # volatile regs
                reg_decls.append(f'    u32 r{r};')

        freg_decls = []
        for fr in sorted(used_fregs):
            if fr >= 0 and fr <= 13:
                freg_decls.append(f'    f32 f{fr};')

        # Build the function body with gotos
        body_lines = []

        for inst in all_insns:
            if inst['type'] == 'label':
                body_lines.append(f'{inst["name"]}:')
                continue

            if inst['type'] != 'inst':
                continue

            mnem = inst['mnem']
            ops_str = inst.get('ops', '')
            ops = parse_operands(ops_str) if ops_str else []

            # Skip prologue/epilogue
            if mnem in ('stwu', 'mflr', 'mtlr'):
                continue
            if mnem == 'blr':
                body_lines.append('    return;')
                continue
            if mnem == 'stw' and len(ops) >= 2:
                mem = parse_mem_operand(ops[1])
                if mem and mem.get('base') == 'r1':
                    # Stack save
                    continue
            if mnem == 'lwz' and len(ops) >= 2:
                mem = parse_mem_operand(ops[1])
                if mem and mem.get('base') == 'r1':
                    # Stack restore - skip if in epilogue area
                    continue
            if mnem == 'addi' and ops[0] == 'r1':
                continue

            # Translate instruction
            c_line = self._translate_instruction(mnem, ops, ops_str, inst)
            if c_line:
                body_lines.append(f'    {c_line}')

        result = '\n'.join(body_lines) + '\n'
        return result

    def _translate_instruction(self, mnem, ops, ops_str, inst):
        """Translate a single PPC instruction to C."""

        if mnem == 'li':
            return f'/* {mnem} */ {ops[0]} = {ops[1]};'

        if mnem == 'lis':
            val = ops[1].strip()
            if '@ha' in val:
                label = val.replace('@ha', '')
                return f'/* lis ha */ {ops[0]} = (u32){label};'
            return f'/* lis */ {ops[0]} = ({val} << 16);'

        if mnem == 'addi':
            src = ops[1]
            imm = ops[2].strip()
            if '@l' in imm:
                label = imm.replace('@l', '')
                return f'/* addi lo */ {ops[0]} = (u32){label};'
            return f'/* addi */ {ops[0]} = {src} + {imm};'

        if mnem == 'addis':
            return f'/* addis */ {ops[0]} = {ops[1]} + ({ops[2]} << 16);'

        if mnem in ('lwz', 'lbz', 'lhz', 'lfs'):
            mem = parse_mem_operand(ops[1])
            if mem:
                t = {'lwz': 'u32', 'lbz': 'u8', 'lhz': 'u16', 'lfs': 'f32'}[mnem]
                if 'label' in mem:
                    label = mem['label']
                    self.sda_decls.add(label)
                    return f'/* {mnem} sda */ {ops[0]} = *({t}*){label};'
                offset = mem['offset']
                base = mem['base']
                if isinstance(offset, int):
                    return f'/* {mnem} */ {ops[0]} = *({t}*)((u8*){base} + 0x{offset:X});'
                return f'/* {mnem} */ {ops[0]} = *({t}*)((u8*){base} + {offset});'

        if mnem in ('stw', 'stb', 'sth', 'stfs'):
            mem = parse_mem_operand(ops[1])
            if mem:
                t = {'stw': 'u32', 'stb': 'u8', 'sth': 'u16', 'stfs': 'f32'}[mnem]
                if 'label' in mem:
                    label = mem['label']
                    self.sda_decls.add(label)
                    return f'/* {mnem} sda */ *({t}*){label} = {ops[0]};'
                offset = mem['offset']
                base = mem['base']
                if isinstance(offset, int):
                    if offset < 0:
                        return f'/* {mnem} */ *({t}*)((u8*){base} + ({offset})) = {ops[0]};'
                    return f'/* {mnem} */ *({t}*)((u8*){base} + 0x{offset:X}) = {ops[0]};'
                return f'/* {mnem} */ *({t}*)((u8*){base} + {offset}) = {ops[0]};'

        if mnem == 'stbu':
            mem = parse_mem_operand(ops[1])
            if mem:
                if 'label' in mem:
                    label = mem['label']
                    self.sda_decls.add(label)
                    return f'/* stbu */ {mem["base"]} = (u8*){label}; *(u8*){mem["base"]} = {ops[0]};'
                offset = mem['offset']
                return f'/* stbu */ {mem["base"]} = (u8*){mem["base"]} + {offset}; *(u8*){mem["base"]} = {ops[0]};'

        if mnem == 'mr':
            return f'/* mr */ {ops[0]} = {ops[1]};'

        if mnem == 'mulli':
            return f'/* mulli */ {ops[0]} = {ops[1]} * {ops[2]};'

        if mnem == 'mullw':
            return f'/* mullw */ {ops[0]} = {ops[1]} * {ops[2]};'

        if mnem == 'add':
            return f'/* add */ {ops[0]} = {ops[1]} + {ops[2]};'

        if mnem == 'sub' or mnem == 'subf':
            return f'/* subf */ {ops[0]} = {ops[2]} - {ops[1]};'

        if mnem == 'neg':
            return f'/* neg */ {ops[0]} = -{ops[1]};'

        if mnem == 'slwi':
            return f'/* slwi */ {ops[0]} = {ops[1]} << {ops[2]};'

        if mnem == 'srwi':
            return f'/* srwi */ {ops[0]} = (u32){ops[1]} >> {ops[2]};'

        if mnem == 'srawi':
            return f'/* srawi */ {ops[0]} = (s32){ops[1]} >> {ops[2]};'

        if mnem == 'or':
            if ops[1] == ops[2]:
                return f'/* mr */ {ops[0]} = {ops[1]};'
            return f'/* or */ {ops[0]} = {ops[1]} | {ops[2]};'

        if mnem == 'ori':
            return f'/* ori */ {ops[0]} = {ops[1]} | {ops[2]};'

        if mnem == 'and':
            return f'/* and */ {ops[0]} = {ops[1]} & {ops[2]};'

        if mnem == 'andi.':
            return f'/* andi */ {ops[0]} = {ops[1]} & {ops[2]};'

        if mnem == 'xor':
            return f'/* xor */ {ops[0]} = {ops[1]} ^ {ops[2]};'

        if mnem == 'xori':
            return f'/* xori */ {ops[0]} = {ops[1]} ^ {ops[2]};'

        if mnem == 'nor':
            return f'/* nor */ {ops[0]} = ~({ops[1]} | {ops[2]});'

        if mnem == 'not':
            return f'/* not */ {ops[0]} = ~{ops[1]};'

        if mnem in ('clrlwi', 'clrlwi.'):
            n = int(ops[2])
            mask = (1 << (32 - n)) - 1
            return f'/* clrlwi */ {ops[0]} = {ops[1]} & 0x{mask:X};'

        if mnem == 'rlwinm' or mnem == 'rlwinm.':
            return f'/* {mnem} {ops_str} */;'

        if mnem == 'extsb':
            return f'/* extsb */ {ops[0]} = (s8){ops[1]};'

        if mnem == 'extsh':
            return f'/* extsh */ {ops[0]} = (s16){ops[1]};'

        if mnem == 'cmpwi':
            return f'/* cmpwi {ops_str} */;'

        if mnem == 'cmplwi':
            return f'/* cmplwi {ops_str} */;'

        if mnem == 'cmpw':
            return f'/* cmpw {ops_str} */;'

        if mnem == 'cmplw':
            return f'/* cmplw {ops_str} */;'

        if mnem == 'fcmpo':
            return f'/* fcmpo {ops_str} */;'

        # Branch instructions
        if mnem == 'b':
            target = ops[0].strip()
            if target.startswith('.L_'):
                return f'goto {target};'
            return f'/* b {target} */;'

        if mnem == 'beq':
            target = ops[-1].strip()
            if target.startswith('.L_'):
                return f'if (/* eq */) goto {target};'
            return f'/* beq {ops_str} */;'

        if mnem == 'bne':
            target = ops[-1].strip()
            if target.startswith('.L_'):
                return f'if (/* ne */) goto {target};'
            return f'/* bne {ops_str} */;'

        if mnem == 'bge':
            target = ops[-1].strip()
            if target.startswith('.L_'):
                return f'if (/* ge */) goto {target};'
            return f'/* bge {ops_str} */;'

        if mnem == 'bgt':
            target = ops[-1].strip()
            if target.startswith('.L_'):
                return f'if (/* gt */) goto {target};'
            return f'/* bgt {ops_str} */;'

        if mnem == 'ble':
            target = ops[-1].strip()
            if target.startswith('.L_'):
                return f'if (/* le */) goto {target};'
            return f'/* ble {ops_str} */;'

        if mnem == 'blt':
            target = ops[-1].strip()
            if target.startswith('.L_'):
                return f'if (/* lt */) goto {target};'
            return f'/* blt {ops_str} */;'

        if mnem == 'beqlr':
            return f'if (/* eq */) return;'

        if mnem == 'bnelr':
            return f'if (/* ne */) return;'

        if mnem == 'bgelr':
            return f'if (/* ge */) return;'

        if mnem == 'bgtlr':
            return f'if (/* gt */) return;'

        if mnem == 'blelr':
            return f'if (/* le */) return;'

        if mnem == 'bltlr':
            return f'if (/* lt */) return;'

        if mnem == 'bl':
            target = ops[0].strip()
            if target.startswith('_save') or target.startswith('_rest'):
                return f'/* {target} */;'
            self.extern_fn_decls.add(target)
            return f'{target}();'

        if mnem == 'bctrl':
            return f'/* bctrl -- indirect call */;'

        if mnem == 'mtctr':
            return f'/* mtctr {ops[0]} */;'

        if mnem == 'mfctr':
            return f'/* mfctr {ops[0]} */;'

        # Float ops
        if mnem == 'fmr':
            return f'/* fmr */ {ops[0]} = {ops[1]};'

        if mnem == 'fadds':
            return f'/* fadds */ {ops[0]} = {ops[1]} + {ops[2]};'

        if mnem == 'fsubs':
            return f'/* fsubs */ {ops[0]} = {ops[1]} - {ops[2]};'

        if mnem == 'fmuls':
            return f'/* fmuls */ {ops[0]} = {ops[1]} * {ops[2]};'

        if mnem == 'fdivs':
            return f'/* fdivs */ {ops[0]} = {ops[1]} / {ops[2]};'

        if mnem == 'fneg':
            return f'/* fneg */ {ops[0]} = -{ops[1]};'

        if mnem == 'fabs':
            return f'/* fabs */ {ops[0]} = fabs({ops[1]});'

        if mnem == 'fmadds':
            return f'/* fmadds */ {ops[0]} = {ops[1]} * {ops[2]} + {ops[3]};'

        if mnem == 'fmsubs':
            return f'/* fmsubs */ {ops[0]} = {ops[1]} * {ops[2]} - {ops[3]};'

        if mnem == 'fnmsubs':
            return f'/* fnmsubs */ {ops[0]} = -({ops[1]} * {ops[2]} - {ops[3]});'

        if mnem == 'fctiwz':
            return f'/* fctiwz */ {ops[0]} = (f64)(s32){ops[1]};'

        if mnem == 'stfd':
            mem = parse_mem_operand(ops[1])
            if mem and isinstance(mem.get('offset'), int):
                return f'/* stfd */ *(f64*)((u8*){mem["base"]} + 0x{mem["offset"]:X}) = {ops[0]};'

        if mnem == 'lfd':
            mem = parse_mem_operand(ops[1])
            if mem:
                if 'label' in mem:
                    return f'/* lfd sda */ {ops[0]} = *(f64*){mem["label"]};'
                if isinstance(mem.get('offset'), int):
                    return f'/* lfd */ {ops[0]} = *(f64*)((u8*){mem["base"]} + 0x{mem["offset"]:X});'

        # CR operations
        if mnem == 'crclr':
            return f'/* crclr {ops_str} */;'

        if mnem == 'crset':
            return f'/* crset {ops_str} */;'

        if mnem == 'cror':
            return f'/* cror {ops_str} */;'

        # ps_ paired single instructions
        if mnem.startswith('ps_'):
            return f'/* {mnem} {ops_str} */;'

        # Fallback
        return f'/* {mnem} {ops_str} */;'


def main():
    all_funcs = load_asm_funcs(os.path.abspath(ASM_FILE))
    decomp = PPCDecompiler(all_funcs)

    # Test on a few functions
    test_funcs = [
        'fn_800D45F8', 'fn_800D4604', 'fn_800D4610',
        'fn_800D7230', 'fn_800D724C', 'fn_800D7268',
        'fn_800D3F5C',
        'fn_800DCAA4',
        'fn_800E01F4',
        'fn_800E0000',
    ]

    for fn in test_funcs:
        c = decomp.decompile(fn)
        if c:
            print(f'=== {fn} ===')
            print(c)
            print()
        else:
            print(f'=== {fn} === FAILED')
            print()


if __name__ == '__main__':
    main()
