# Matching Guide for Pokemon Colosseum (GPXE01)

## Compiler: MetroWerks CodeWarrior GC 1.2.5n

This document covers the correct compiler flags, common pitfalls, and
techniques for writing C code that matches the original mwcceppc output.

---

## Compiler Flags

### Default flags (most translation units)

```
-O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn off
```

| Flag | Purpose |
|------|---------|
| `-O4,p` | Full optimization with peephole. This is the standard for Dolphin SDK and game code. |
| `-nodefaults` | Do not pull in default libraries. |
| `-proc gekko` | Target the Gekko (GameCube) processor. |
| `-fp hard` | Hardware floating point. |
| `-Cpp_exceptions off` | Disable C++ exception handling (smaller code). |
| `-enum int` | Enums are always `int`-sized (4 bytes). |
| `-warn off` | Suppress all warnings so compilation does not abort on implicit conversions. |

### Per-module overrides

No per-module flag overrides have been confirmed yet. Candidates to investigate:

- **CRT modules** (`src/crt/`): May use `-O4,s` (optimize for size) instead of `-O4,p`.
- **TRK modules** (`src/trk/`): Pure-asm files may need `-O0` or no optimization.
- **HSD modules** (`src/hsd/`): Adapted from Melee's sysdolphin; may match at `-O4,p`.

### Flag meanings

| Optimization | Description |
|-------------|-------------|
| `-O0` | No optimization. Useful for debugging; rarely matches release code. |
| `-O1` | Minimal optimization. |
| `-O2` | Standard optimization. |
| `-O4,p` | Full optimization + peephole. Most common for GCN titles. |
| `-O4,s` | Full optimization + size preference. Used for some CRT/library code. |

---

## Common Compilation Pitfalls

### 1. C89 mode (no declarations after statements)

MWCC for GCN enforces C89. Variable declarations **must** appear at the
start of a block, before any statements.

**Wrong (C99):**
```c
int x = get_value();
int y = x + 1;       /* ERROR: declaration after statement */
```

**Correct (C89):**
```c
int x;
int y;
x = get_value();
y = x + 1;
```

Or wrap in a new block scope:
```c
int x = get_value();
{
    int y = x + 1;
    /* ... */
}
```

### 2. Inline assembly syntax

MWCC uses `asm { ... }` for inline assembly, not GCC-style `__asm__`.

- `asm void FunctionName(void) { nofralloc ... }` for full-asm functions.
- `asm { sync }` for single inline instructions in C code.
- Use `opword 0xNNNNNNNN` to emit raw instruction words (not `.long`).
- Labels use `@name` prefix for local labels: `@skip:`, `@done:`.

**Bare `sync;` outside asm is illegal.** Use `asm { sync }` instead.

### 3. Section pragmas

Use `#pragma section "name"` followed by `__declspec(section "name")`.
Section names with a leading dot (`.dtors`) may not work directly; use
`".dtors$10"` or similar suffixed names.

### 4. Forward declarations for asm functions

If an asm function B is referenced by an earlier asm function A via
`b FunctionB`, you must forward-declare B:

```c
asm void FunctionB(void);  /* forward declaration */

asm void FunctionA(void) {
    nofralloc
    b FunctionB
}

asm void FunctionB(void) {
    nofralloc
    blr
}
```

### 5. Implicit function declarations

MWCC infers `int (...)` for undeclared functions. If you later declare the
function with a different return type or parameter list, the compiler emits
a "redeclared" error. Always add `extern` declarations before first use.

### 6. SPR names in inline assembly

Not all PowerPC SPR names are recognized by MWCC. Known issues:

| Symbolic Name | Use numeric SPR instead |
|--------------|------------------------|
| `TBL_R` | `268` |
| `TBU_R` | `269` |
| `TBL_W` | `284` |
| `TBU_W` | `285` |
| `USDA` | `947` |
| `mftbl` | `mfspr rN, 268` |
| `mftbu` | `mfspr rN, 269` |
| `mttbl` | `mtspr 284, rN` |
| `mttbu` | `mtspr 285, rN` |
| `mfpvr` | `mfspr rN, PVR` |
| `mtpvr` | PVR is read-only; use `nop` |

### 7. va_list / stdarg.h

MWCC for GCN does not ship `<stdarg.h>`. Define va_list manually:

```c
typedef struct __va_list_struct {
    u8  gpr;
    u8  fpr;
    u16 padding;
    u32* overflow_arg_area;
    u32* reg_save_area;
} __va_list_struct;
typedef __va_list_struct va_list[1];
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_end(ap)         ((void)0)
```

### 8. Header include ordering

Always include `dolphin/os/OSContext.h` before headers that use `OSContext*`
in typedefs (e.g., `dolphin/si/SI.h`, `dolphin/exi/EXI.h`). If a typedef
references `OSContext*` but the struct is not yet defined, the compiler
treats it as `int` and subsequent casts fail.

### 9. extern declarations inside function bodies

MWCC does not allow `extern` declarations inside a function body if the
function has already been implicitly declared. Move all extern declarations
to file scope.

---

## Tips for Matching mwcceppc Output

### Register allocation

MWCC assigns registers in a predictable pattern:
- Arguments arrive in r3-r10 (GPR) and f1-f8 (FPR).
- Local variables are allocated starting from r31 downward (r31, r30, r29...).
- The stack frame uses `stwu r1, -N(r1)` / `addi r1, r1, N`.

### Loop patterns

MWCC tends to:
- Use `bdnz` for counted loops when the trip count is known.
- Place the loop condition test at the bottom (`do { ... } while (cond)`
  compiles more directly than `while (cond) { ... }`).

### Volatile pointers

Hardware register accesses must use `volatile`. Without it, the compiler
may reorder or eliminate reads/writes:

```c
#define REG_BASE ((volatile u32*)0xCC006800)
```

### Struct member ordering

The order of struct members affects code generation. MWCC may generate
different load/store sequences depending on field offsets. When matching,
ensure struct layouts exactly match the original binary's expectations.

### Comparison patterns

- `if (x == 0)` and `if (!x)` may generate different code.
- `if (x != NULL)` vs `if (x)` can differ in branch direction.
- Cast comparisons carefully: `(u32)x < (u32)y` vs `(s32)x < (s32)y`
  generate `cmplw` vs `cmpw`.

### Function pointer casts

When passing function pointers, ensure the typedef matches exactly.
MWCC is strict about function pointer type compatibility and will not
implicitly convert between different function pointer types.

---

## Build Pipeline

1. **Compile**: `python tools/compile_check.py src/path/to/file.c`
2. **Compile all**: `python tools/compile_check.py --all`
3. **Match test**: `python tools/match_test.py FunctionName`
4. **Scan file**: `python tools/match_test.py --scan src/path/to/file.c`
5. **Interactive diff**: `python tools/compile_check.py src/file.c --diff --symbol FuncName`

---

## Current Status

- **Total source files**: 79
- **Compiling**: 79/79 (100%)
- **Compiler version**: MetroWerks CodeWarrior GC 1.2.5n
- **Default flags**: `-O4,p -nodefaults -proc gekko -fp hard -Cpp_exceptions off -enum int -warn off`
