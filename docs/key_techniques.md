# Key Techniques — Pokémon Colosseum Decompilation

Everything we've learned about matching Metrowerks CodeWarrior GC output
byte-for-byte against the original Pokémon Colosseum DOL. This document
is a living reference — update it as new patterns are discovered.

**Last updated:** 2026-04-09

---

## Table of Contents

1. [Toolchain Setup](#1-toolchain-setup)
2. [Compiler Versions](#2-compiler-versions)
3. [Pragma Cookbook](#3-pragma-cookbook)
4. [C89 Rules](#4-c89-rules)
5. [Register Allocation](#5-register-allocation)
6. [SDA / Small Data Area](#6-sda--small-data-area)
7. [Float Matching](#7-float-matching)
8. [Branch Patterns](#8-branch-patterns)
9. [Signed vs Unsigned](#9-signed-vs-unsigned)
10. [Struct and Pointer Access](#10-struct-and-pointer-access)
11. [Common Mismatches](#11-common-mismatches)
12. [Workflow](#12-workflow)
13. [Tools Reference](#13-tools-reference)

---

## 1. Toolchain Setup

### Default compiler flags

```
-O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int
-warn off -use_lmw_stmw on -sdata 8 -sdata2 8
```

| Flag | Purpose |
|------|---------|
| `-O4,p` | Full optimization with peephole pass |
| `-use_lmw_stmw on` | Inline stmw/lmw for 4+ callee-saved registers |
| `-sdata 8 -sdata2 8` | Restore small-data thresholds stripped by `-nodefaults` |
| `-proc gekko` | GameCube CPU (PowerPC 750CXe) |
| `-fp hard` | Hardware floating point |
| `-enum int` | Enums are always 4 bytes |

### Compilers

Two compiler versions are used in this project:

| Compiler | Location | Used For |
|----------|----------|----------|
| CW GC/1.3 | `tools/mwcc_compiler/GC/1.3/` | Game code (`src/game/`), default |
| CW GC/1.2.5n | `tools/mwcc_compiler/GC/1.2.5n/` | Dolphin SDK (`src/dolphin/`), HSD (`src/hsd/`) |

---

## 2. Compiler Versions

**Always check the compiler version before decompiling a function.** Wrong
version creates an insurmountable match ceiling (typically 50-60%).

### How they differ

| Behavior | GC/1.3 | GC/1.2.5n |
|----------|--------|-----------|
| Prologue order | `stwu` first, then `mflr` | `mflr` first, then `stwu` |
| Register move | `mr rD, rA` (`or` encoding) | `addi rD, rA, 0` |
| Stack frame | Smaller, tighter | Larger (extra padding) |

### How to check

```bash
python tools/match_test.py fn_XXXXXXXX --verbose 2>&1 | grep "Compiler:"
```

### How to override

Edit `config/GC6E01/compile_config.json`:
```json
{
  "overrides": {
    "src/dolphin/dvd/DVDFsExtras.c": {
      "compiler": "GC/1.2.5n",
      "comment": "Dolphin SDK compiled with CW GC/1.2.5n"
    }
  }
}
```

### Known overrides (discovered so far)

- `src/dolphin/dvd/DVD.c` — GC/1.2.5n
- `src/dolphin/dvd/DVDLow.c` — GC/1.2.5n
- `src/dolphin/dvd/DVDQueue.c` — GC/1.2.5n
- `src/dolphin/dvd/DVDFsExtras.c` — GC/1.2.5n
- `src/hsd/hsd_class.c` — GC/1.2.5n

**Rule of thumb:** If a Dolphin SDK or HSD file is stuck at 50-60% match
with correct logic, try adding a GC/1.2.5n override.

---

## 3. Pragma Cookbook

### Optimization level

```c
#pragma push
#pragma optimization_level 0   // 0, 1, 2, 3, or 4
#pragma optimizewithasm off
void fn_XXXXXXXX(void) { ... }
#pragma pop
```

**When to use each level:**

| Level | Stack frame | Zero reuse | Scheduling | Use when |
|-------|-------------|------------|------------|----------|
| O0 | Large | No CSE | None | Original has stack-spilled locals |
| O1 | Moderate | Partial | Basic | Some scheduling but no full CSE |
| O2 | Moderate | Partial | Moderate | Middle ground |
| O4 | Minimal | Full CSE | Aggressive | Most game code (default) |

**Important:** The pragma on the asm wrapper (`#if 1` block) is NOT necessarily
the right pragma for the C replacement. The asm wrapper pragma was set to make
the assembler happy, not to match C codegen. Common pattern: wrapper has O0,
but the matching C needs no pragma (default O4).

### Peephole optimizer

```c
#pragma peephole off
void fn_XXXXXXXX(void) { ... }
#pragma peephole on
```

Use when prologue instruction order differs (li before stw) or when branch
pair optimization causes mismatches. Keep peephole ON when `srwi.` (record
form) is needed.

---

## 4. C89 Rules

CW for GameCube uses **strict C89**. All declarations must come before
statements in each block.

### The problem

```c
// ERROR in CW:
void fn(void) {
    u32 x = 1;
    do_something(x);
    u32 y = 2;      // Declaration after statement — compile error
}
```

### The fix: block scoping

```c
// OK in CW:
void fn(void) {
    u32 x = 1;
    do_something(x);
    {
        u32 y = 2;  // New block allows declaration
        use(y);
    }
}
```

### Block scoping also prevents CSE

When the original asm loads a global (e.g., `lwz r0, sym(r13)`) twice, it
means the compiler couldn't CSE the loads. Use block scopes to force reload:

```c
// Original asm: loads lbl_XXXX twice
{
    u8* p = (u8*)lbl_XXXX + offset;
    *(u32*)(p + 0x94) = val1;
}
{
    u8* p = (u8*)lbl_XXXX + offset;
    *(u32*)(p + 0x98) = val2;
}
```

---

## 5. Register Allocation

CW GC/1.3 at -O4 assigns callee-saved registers (r31 down to r14) by
**variable weight** = (number of uses) * (loop nesting depth factor).

### Rules

1. Highest-weight variable -> r31, second -> r30, etc.
2. Function parameters saved early get high weight (usually r3 -> r31)
3. Variables inside loops get much higher weight
4. Declaration order matters when weights are tied
5. Variables in inner `{ }` blocks may use volatile regs (r0, r3-r12)

### Controlling allocation

| Problem | Fix |
|---------|-----|
| Wrong register (r30 vs r29) | Reorder variable declarations |
| Extra callee-save | Remove unused declarations (they shift ALL numbering) |
| Short-lived var in callee-save | Use inner `{ }` block to keep it in volatile reg |
| Param used only once | Don't save to local — CW keeps it in r3/r4 |

---

## 6. SDA / Small Data Area

The GameCube uses two small data areas for fast access to globals:

| Register | Section | Base Address | Purpose |
|----------|---------|-------------|---------|
| r13 | .sdata / .sbss | 0x80480820 (`_SDA_BASE_`) | Writable globals |
| r2 | .sdata2 / .sbss2 | 0x804836A0 (`_SDA2_BASE_`) | Read-only constants |

### Computing addresses from asm offsets

```python
# r13-relative (sdata/sbss)
SDA_BASE = 0x80480820
raw_offset = 0xA878  # from instruction encoding
signed = raw_offset - 0x10000 if raw_offset >= 0x8000 else raw_offset
address = SDA_BASE + signed

# r2-relative (sdata2/sbss2)
SDA2_BASE = 0x804836A0
# Same formula
```

Then look up the symbol: `grep "ADDRESS" config/GC6E01/symbols.txt`

### Scalar vs array declarations

```c
extern u32 lbl_XXXX;      // -> SDA reloc (1 instruction)
extern u8  lbl_XXXX[];    // -> lis/addi pair (2 instructions, NEVER SDA)
```

**Critical:** Objects <= 8 bytes declared as scalars get SDA addressing.
Declare large structs or arrays with `[]` to force absolute addressing.

### String pointers in SDA2

```c
extern char lbl_XXXX;   // Declare as char scalar
&lbl_XXXX               // Address-of gets SDA2 reloc
```

---

## 7. Float Matching

### sdata2 float returns — MUST use extern labels

```c
// WRONG (50% match — relocation mismatch):
f32 fn_XXXXXXXX(void) {
    return 0.0f;
}

// RIGHT (100% match):
extern f32 lbl_8047DD60;  // Look up in symbols.txt
f32 fn_XXXXXXXX(void) {
    return lbl_8047DD60;
}
```

**Why:** Float literals create TU-local sdata2 entries with different
relocations than the original binary's global sdata2 symbols. The match
tool compares relocations, so the literal version always mismatches.

### Finding the float value from the DOL

```python
import struct
with open('orig/GC6E01/sys/main.dol', 'rb') as f:
    offsets = struct.unpack('>18I', f.read(72))
    addresses = struct.unpack('>18I', f.read(72))
    sizes = struct.unpack('>18I', f.read(72))
    TARGET = 0x8047DD60  # address from symbols.txt
    for i in range(18):
        if sizes[i] > 0 and addresses[i] <= TARGET < addresses[i] + sizes[i]:
            f.seek(offsets[i] + (TARGET - addresses[i]))
            print(struct.unpack('>f', f.read(4))[0])
```

### Type declarations

| Asm instruction | C declaration |
|----------------|---------------|
| `lfs` / `stfs` | `extern f32 lbl_XXX;` |
| `lfd` / `stfd` | `extern f64 lbl_XXX;` |
| `lwz` (integer load) | `extern u32 lbl_XXX;` (NOT f32!) |

### Integer-to-float conversion

```c
(f64)(u32)value   // -> xoris/stw/lfd/fsubs (unsigned conversion trick)
(f64)(s32)value   // -> different conversion path
(f32)(s32)value   // -> different from (f32)value
```

### Float register allocation

FP callee-saved regs (f14-f31) are assigned by **first-use order**, not
declaration order. Reorder float operations to match.

---

## 8. Branch Patterns

### `beq+b` vs `bne` — UNSOLVABLE from C

CW sometimes generates `beq @skip; b @target` where the target binary has
`bne @target`. This is a code generator artifact. Functions at 95-98% with
only this mismatch are effectively matched. **Accept and move on.**

### `mr.` vs `mr` + `cmplwi`

```c
// Generates mr. (record form) — 1 instruction:
if ((ptr = fn()) != NULL)

// Generates mr + cmplwi — 2 instructions:
ptr = fn();
if (ptr != NULL)
```

### `bnelr` — conditional return

A `cmplw; bnelr` pattern in the asm means the original had an early-return
comparison. CW generates this for leaf functions at O4:

```c
void fn(Type* info) {
    info->field1 = 0;
    info->field2 = 0;
    if (info == &global_instance) {
        // extra cleanup
    }
}
```

---

## 9. Signed vs Unsigned

The C variable type controls the compare instruction:

| C type | PPC compare | Extension |
|--------|------------|-----------|
| `u32` | `cmplwi` (unsigned) | none |
| `s32` | `cmpwi` (signed) | none |
| `u16` | `cmplwi` | `clrlwi` (zero-extend) |
| `s16` | `cmpwi` | `extsh` (sign-extend) |

### Special: `(u16)-1` equality

```c
if (val == (u16)-1)  // -> subf/cntlzw/srwi (branchless)
if (val == 0xFFFF)   // -> cmplwi r3, 0xFFFF (different!)
```

---

## 10. Struct and Pointer Access

### Global pointer dereference

When the asm does `lwz rN, sym(r13); lwz rM, offset(rN)`, the C is:

```c
extern u32 sym;  // holds a pointer value
u32 result = *(u32*)((u8*)sym + offset);
```

### Setting through a global pointer

```c
extern u32 sym;
*(u32*)((u8*)sym + offset) = value;
```

### Output parameters

When the asm stores to `0(r3)` and `0(r4)`:

```c
void fn(u32* out1, u32* out2) {
    *out1 = some_global;
    *out2 = another_global;
}
```

---

## 11. Common Mismatches

| Mismatch | Likely Fix |
|----------|-----------|
| Wrong register (r30 vs r29) | Reorder variable declarations |
| `cmplwi` vs `cmpwi` | Change variable to `u32` vs `s32` |
| `clrlwi` before `stb` | Use `u8` variable or cast `(u8)` |
| Extra `extsb` / `extsh` | Remove unnecessary `(s8)` / `(s16)` cast |
| `lis/addi` vs `la lbl(r2)` | Change `extern u8[]` to `extern u32` scalar |
| `mr` + `cmplwi` vs `mr.` | Use `if ((p = fn()) != NULL)` combined pattern |
| Missing `frsp` | Add explicit `(f32)` cast |
| `li r0, X` reordered | Try `#pragma peephole off` |
| `beq+b` vs `bne` | UNSOLVABLE — accept 2-instruction mismatch |
| Stack frame too small | Try different `#pragma optimization_level` |
| `mflr` order wrong | Wrong compiler version (try GC/1.2.5n) |
| `addi rD,rA,0` vs `mr` | Wrong compiler version |
| Float literal mismatch | Use `extern f32 lbl_XXX` instead of `0.0f` |
| `li` not reused across stores | Remove `#pragma optimization_level 0` |
| Two `li r0,0` vs one | Use block scoping to prevent CSE |

---

## 12. Workflow

### Per-function decompilation

1. **Read the .inc file** — understand the PPC assembly
2. **Check compiler version** — `--verbose` flag on match_test
3. **Check surrounding pragmas** — read context in the .c file
4. **Write C89 code** — declarations first, no mixed declarations
5. **Compile** — `python tools/compile_check.py <file.c>`
6. **Match test** — `python tools/match_test.py fn_XXXXXXXX`
7. **If < 100%: get diff** — use objdiff to compare instructions
8. **Iterate** — fix one mismatch at a time, retest

### Prioritization strategy

1. Start with **smallest functions** (2-5 line .inc files)
2. Batch **accessor/setter patterns** — they're mechanical
3. Do **medium functions** (10-30 lines) for steady progress
4. Leave **large functions** (100+ lines) for focused sessions
5. Park functions at 80-95% and revisit later with fresh eyes

### "Undefined label" cascade

Missing `extern` for a `bl fn_xxx` target in an asm wrapper kills ALL
subsequent asm blocks in the file. If you get a cascade of syntax errors,
check for a missing forward declaration.

---

## 13. Tools Reference

| Command | Purpose |
|---------|---------|
| `python tools/compile_check.py <file.c>` | Compile a single file |
| `python tools/compile_check.py --all` | Compile all 218 files |
| `python tools/match_test.py fn_XXXXXXXX` | Test match percentage |
| `python tools/match_test.py fn_XXXXXXXX --verbose` | Show compiler info |
| `python tools/assign_work.py --status` | Overall coverage stats |

### objdiff instruction comparison

```bash
tools/objdiff-cli.exe diff \
  -1 build/GC6E01/obj/auto_01_800055E0_text.o \
  -2 build/GC6E01/base/<path>.o \
  -o diff.json --format json fn_XXXXXXXX
```

Then parse with Python to see left vs right instruction differences.

### SDA address lookup

```python
SDA_BASE  = 0x80480820   # r13
SDA2_BASE = 0x804836A0   # r2

# Convert instruction offset to address:
raw = 0xA878  # from instruction hex
signed = raw - 0x10000 if raw >= 0x8000 else raw
addr = SDA_BASE + signed  # or SDA2_BASE for r2
# Then: grep addr in config/GC6E01/symbols.txt
```
