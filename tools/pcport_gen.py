#!/usr/bin/env python3
"""pcport_gen.py — generate PC-compilable copies of decomp .c files.

For the PC port (functional C, NOT byte-match), `#if 1 asm { #include .inc } #else
<C> #endif` wrappers must use the C branch — clang/MSVC can't assemble PowerPC .inc.
Every such wrapper already carries an inactive `#else` C reference body. This tool
emits a copy with each *asm-active* (`#if 1`) wrapper replaced by its `#else` C, so
the file compiles for x86. The ORIGINAL source is untouched (the CodeWarrior matching
build keeps the asm); only the generated copy is compiled by the PC build.

`#if 0` asm wrappers already select their C via the preprocessor and pass through.

Usage:
  python tools/pcport_gen.py src/hsd/hsd_dobj.c            -> build_pc/gen/hsd/hsd_dobj.c
  python tools/pcport_gen.py --out-dir build_pc/gen src/hsd/hsd_dobj.c src/hsd/hsd_jobj.c
"""
import argparse, re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

# Per-file PC-only preamble: targeted decls/includes some decompiled TUs need to
# compile on a strict compiler (the CodeWarrior matching build never sees these,
# since only the generated copy carries them). Keeps originals pristine.
PREAMBLE = {
    "hsd/hsd_render.c":       "#include <string.h>\n",
    # OSReport + the CW-builtin varargs placeholder `__va_arg()` left behind by the
    # decomp; K&R protos accept the no-arg placeholder calls. -Wimplicit-function-
    # declaration is a default ERROR in modern clang, so a decl is mandatory.
    "hsd/hsd_util.c":         "void OSReport();\nvoid __va_arg();\n",
    # fn_80196E10/fn_8019D9DC/fn_801AA498 are implicit calls needing a decl. memset needs a
    # real prototype for its valid 3-arg call (clang errors on the undeclared library fn);
    # the lone 0-arg `memset();` in dead post-return code is removed by TEXT_FIXUPS below,
    # so pulling in <string.h>'s real 3-arg proto is now conflict-free.
    "hsd/hsd_jobj_display.c": ("#include <string.h>\n"
                               "extern void fn_80196E10();\nextern void fn_8019D9DC();\n"
                               "extern void fn_801AA498();\n"),
    "hsd/hsd_memory_ext.c":   "extern void fn_80196E10();\n",
    # Track-D engine TUs (parallel-analysis workflow, 2026-06-02): forward-decls for
    # functions used before their in-TU macro/decl, or referenced-but-not-declared.
    # GSmemSplitBlock is called (lines 336/412) before its `#define ->fn_800E2DB0`
    # appears (line 460); K&R forward decl, return u16 matches its assignment.
    "game/gs_mem.c":          "unsigned short GSmemSplitBlock();\n",
    # DVDInit + memset called without a decl (pcport_compat.h omits <string.h>).
    "game/gs_dvd.c":          "void DVDInit();\nvoid* memset(void*, int, unsigned int);\n",
    # fn_80129280 referenced but not declared in this TU; void* matches the
    # cast-to-void* + pointer-arith call sites (same form as battle_main.c).
    "game/battle/battle_waza.c": "void* fn_80129280();\n",
}

# Per-file extern-unification: CodeWarrior tolerated the same data label being
# block-scoped `extern`-declared with DIFFERENT types in different functions of one
# TU (e.g. `u8 lbl_X[]` in one fn and `HSD_CObjInfo lbl_X` in another). clang rejects
# this with "redeclaration ... with a different type". Since the link is type-agnostic,
# we rewrite EVERY block-scoped / file-scope `extern <type> lbl_X[..];` declaration of a
# conflicting label to one canonical form, so clang sees a single consistent type per
# symbol within the TU. The canonical form is chosen so all *usages* still compile
# (decay vs &-of for arrays/scalars); residual pointer/array arg mismatches are warnings
# silenced by the build's -w. Applied only to the generated copy; originals stay pristine.
# Map: rel-path -> { label_name: canonical_extern_decl_without_trailing_semicolon }
EXTERN_UNIFY = {
    "hsd/hsd_cobj.c": {
        # Array form: bare-decay usages (arith/compare/cast) dominate; the lone
        # `&lbl_8036C678` (HSD_CLASS_INFO cast) accepts an array address fine.
        "lbl_8036C678": "extern u8 lbl_8036C678[]",
        # Array form: the bare-decay site (line 412, passed as const char*) needs the
        # array->ptr decay; the `&lbl` sites become char(*)[] -> const char*, a mere
        # -Wincompatible-pointer-types warning (silenced by -w), NOT the default-ERROR
        # -Wint-conversion that the scalar form would trigger on the bare-decay site
        # under modern clang.
        "lbl_8047D958": "extern char lbl_8047D958[]",
        "lbl_8047D960": "extern char lbl_8047D960[]",
    },
}

# Per-file function-prototype neutralization to K&R empty-parens `extern <ret> fn_X();`.
# Use ONLY when a function's argument types are the sole conflict and its return type is
# consistent everywhere AND no real (prototyped) DEFINITION with promotion-affected params
# (float/char/short) exists in the TU — otherwise `()` is itself incompatible with that
# definition. The `()` form has unspecified parameters, so any argument shape at a call
# site is accepted. `-Wstrict-prototypes` on `()` is a warning, silenced by -w.
# Map: rel-path -> set of function names to neutralize to K&R prototypes.
FUNC_PROTO_KR = {
    "game/battle/battle_main.c": {
        # Block-scoped decls re-type the args (u32,u32,u32) vs the file-scope
        # prototypes' (s32,s32,s32); same void return. K&R `()` is compatible with
        # the surviving (comment-bearing, regex-skipped) file-scope prototype, and
        # accepts the 3-arg call sites. (sndPlay-with-fade / sndFade.)
        "fn_801659FC", "fn_80165A20",
    },
    "hsd/hsd_cobj.c": {
        # fn_80196E10 is the assert helper: callers pass the file/expr labels
        # inconsistently as `&lbl` (char(*)[]) and bare `lbl` (char*). No single data
        # type makes both clean against a `(const char*,...)` proto, so neutralize the
        # proto to K&R `()` — accepts every argument shape. Its definition's params
        # (const char*, u32, const char*) are promotion-safe, so `()` stays compatible.
        "fn_80196E10",
    },
}

# Per-file removal of redundant arg-less STUB prototypes. The generator emits placeholder
# `extern void fn_X(void);` / `extern <ret> fn_X();` stubs at file scope, but these same
# functions ALSO have a real typed declaration (block-scoped, with args) or a real typed
# DEFINITION elsewhere in the TU. clang errors "conflicting types" when the stub's return
# type (`void`) or arg-less shape clashes with the real one — and K&R can't bridge a
# differing return type or a float/short/char-param definition. Dropping the redundant
# stub lines leaves exactly the real typed decl/definition, which clang accepts. Listed
# names are verified to have a surviving typed decl/definition in the same TU.
# Map: rel-path -> set of function names whose arg-less stub prototypes are dropped.
FUNC_STUB_DROP = {
    "game/battle/battle_main.c": {
        # fn_801EF634 has a real DEFINITION `u16 fn_801EF634(void)` (line 431) that
        # precedes this redundant block-scoped `extern void fn_801EF634();` stub
        # (line 828) and all its call sites; the void-return stub clashes with the
        # u16 definition. Dropping it leaves the in-scope definition.
        "fn_801EF634",
    },
    "hsd/hsd_cobj.c": {
        "fn_801C25E4", "fn_801C2670", "fn_80191DCC", "fn_80191E88",
        "fn_800BD640", "fn_800BD744",
    },
}

# Per-file prototype RETYPE: rewrite a function's stub/forward prototype to an exact
# canonical signature. Use when the stub is genuinely NEEDED as a forward declaration
# (the symbol is referenced before its definition) AND its real definition has a
# prototyped signature that neither K&R `()` nor a dropped stub can satisfy (e.g. float
# params, which default-promotion makes incompatible with `()`). The canonical text MUST
# match the real definition's signature so the forward decl and definition agree.
# Map: rel-path -> { fn_name: canonical_prototype_without_trailing_semicolon }.
FUNC_PROTO_RETYPE = {
    "game/battle/battle_main.c": {
        # Two block-scoped decls disagree on BOTH return and args: `void
        # fn_80129280()` (line 485, called 0-arg) vs `void* fn_80129280(u32,u32)`
        # (line 543, called as fn_80129280(0,0xF) with the result used). Retype both
        # to the K&R-args `void* fn_80129280()`: a single consistent return (void*)
        # with unspecified params, so BOTH the 0-arg and 2-arg call sites compile.
        "fn_80129280": "void* fn_80129280()",
    },
    "hsd/hsd_cobj.c": {
        # Referenced at file scope as `(u8*)fn_80196C54` before its definition; the def is
        # `void fn_80196C54(int, f32, f32, f32, f32, f32, f32)`. The `(void)` stub clashed
        # with the float-param definition, so retype the forward decl to match it exactly.
        "fn_80196C54": "void fn_80196C54(int, f32, f32, f32, f32, f32, f32)",
    },
}

# Per-file exact-substring fixups applied to the FINAL generated text (last pass). For
# the rare decomp artifact that no declaration-level rewrite can fix — e.g. a call with
# the wrong arity in dead (post-`return`) code that clang still type-checks. Each entry is
# (old_exact_substring -> new_substring); must match verbatim. Applied only to the
# generated copy; the original byte-match source is never touched.
# Map: rel-path -> list of (old, new) pairs.
TEXT_FIXUPS = {
    # gs_dvd.c: the decompiled GSDVD_CloseHandle / GSDVD_Open DEFINITIONS are typed
    # s32 but their bodies have bare `return;` (dropped r3) — clang -Wreturn-mismatch
    # is a hard error. They are definitions (not extern decls) so the proto rewriters
    # don't reach them; fix the return type textually (void is correct — they return
    # no value; cross-TU callers ignore return type at link).
    "game/gs_dvd.c": [
        ("s32 GSDVD_CloseHandle(u32 handleIndex, u32 mode) {",
         "void GSDVD_CloseHandle(u32 handleIndex, u32 mode) {"),
        ("s32 GSDVD_Open(u32 slotIndex, u32 resId, void* callback, u32 param1, u32 param2, u32 param3, u32 param4, u32 param5) {",
         "void GSDVD_Open(u32 slotIndex, u32 resId, void* callback, u32 param1, u32 param2, u32 param3, u32 param4, u32 param5) {"),
    ],
    "game/battle/battle_main.c": [
        # The file-scope `fn_800D3088` decl (line 79) has a void return but its
        # block-scoped re-decl (line 1223) + call site (`i += fn_800D3088()`) use a
        # u32 return. Line 79's trailing comment makes it regex-invisible to RETYPE,
        # so fix the return here. Unique prefix (line 1223 is `extern u32 ...`).
        ("extern void fn_800D3088(void);", "extern u32 fn_800D3088(void);"),
        # The scene render table is a real data OBJECT: the code uses both
        # `&lbl_80375CC8` (the table address) and `(u32)lbl_80375CC8`. The array
        # form makes both correct (lbl == &lbl == table addr); the decomp's lone
        # `void*` decl (line 324) was wrong. Its trailing comment makes it
        # regex-invisible to EXTERN_UNIFY, so fix it textually (the block-scoped
        # `extern u8 lbl_80375CC8[];` at line 817 already agrees).
        ("extern void* lbl_80375CC8;", "extern u8 lbl_80375CC8[];"),
    ],
    "hsd/hsd_jobj_display.c": [
        # Dead 0-arg call (after an unconditional `return`) to the clang builtin `memset`,
        # whose fixed 3-arg proto rejects it ("too few arguments"). Neutralize to a no-op.
        ("    memset();\n", "    /* pcport: dead 0-arg memset() removed */;\n"),
    ],
}

# A standalone function PROTOTYPE line (declaration, not definition): begins with
# `extern`, names fn_X, has a parameter list, and ends with `;` (no body brace).
_FUNC_PROTO_RE = re.compile(
    r'^(?P<indent>\s*)extern\s+(?P<ret>[^;{}]*?)\b(?P<fn>fn_[0-9A-Fa-f]+)\s*\([^;{}]*\)\s*;\s*$'
)

# A standalone arg-less STUB prototype: `extern <ret> fn_X(void);` or `extern <ret> fn_X();`
# (empty or `void`-only parameter list). Used to drop redundant placeholder stubs when a
# real typed decl/definition exists elsewhere in the TU.
_FUNC_STUB_RE = re.compile(
    r'^\s*extern\s+[^;{}]*?\b(?P<fn>fn_[0-9A-Fa-f]+)\s*\(\s*(?:void\s*)?\)\s*;\s*$'
)

# Matches a line whose entire (trimmed) content is a single extern data-object
# declaration of one label, capturing leading indentation and the label name:
#   <indent>extern <type...> lbl_NAME;     or   <indent>extern <type...> lbl_NAME[];
# Deliberately excludes function decls (the label is immediately followed by `;` or
# `[..];`, never `(`).
_EXTERN_DECL_RE = re.compile(
    r'^(?P<indent>\s*)extern\s+[^;{}()]*?\b(?P<label>lbl_[0-9A-Fa-f]+)\s*(?:\[\s*\])?\s*;\s*$'
)


def unify_externs(lines, mapping):
    """Rewrite every standalone `extern ... lbl_X ...;` line whose label is in `mapping`
    to that label's canonical declaration, leaving all other lines (and all *uses* of the
    label inside expressions) untouched. Returns (lines, rewritten_count)."""
    out, rewrites = [], 0
    for ln in lines:
        m = _EXTERN_DECL_RE.match(ln)
        if m and m.group('label') in mapping:
            canon = mapping[m.group('label')]
            new = f"{m.group('indent')}{canon};"
            if new != ln:
                rewrites += 1
            out.append(new)
        else:
            out.append(ln)
    return out, rewrites


def neutralize_func_protos(lines, names):
    """Rewrite every standalone prototype `extern <ret> fn_X(<args>);` whose name is in
    `names` to the K&R empty-parens form `extern <ret> fn_X();`. Definitions (lines with a
    body `{`) and call sites are untouched. Returns (lines, rewritten_count)."""
    out, rewrites = [], 0
    for ln in lines:
        m = _FUNC_PROTO_RE.match(ln)
        if m and m.group('fn') in names:
            new = f"{m.group('indent')}extern {m.group('ret').strip()} {m.group('fn')}();"
            if new != ln:
                rewrites += 1
            out.append(new)
        else:
            out.append(ln)
    return out, rewrites


def retype_func_protos(lines, mapping):
    """Rewrite every standalone prototype line for a function in `mapping` to the supplied
    canonical signature. Only declaration lines (ending `;`, no body `{`) are touched;
    definitions are matched separately and skipped. Returns (lines, rewritten_count)."""
    out, rewrites = [], 0
    for ln in lines:
        m = _FUNC_PROTO_RE.match(ln)
        if m and m.group('fn') in mapping:
            indent = m.group('indent')
            new = f"{indent}extern {mapping[m.group('fn')]};"
            if new != ln:
                rewrites += 1
            out.append(new)
        else:
            out.append(ln)
    return out, rewrites


def drop_func_stubs(lines, names):
    """Drop every standalone arg-less STUB prototype (`extern <ret> fn_X(void);` or
    `... fn_X();`) whose name is in `names`, replacing it with an explanatory comment so
    line-for-line readability is preserved. A real typed decl/definition for each name is
    expected to survive elsewhere in the TU. Returns (lines, dropped_count)."""
    out, dropped = [], 0
    for ln in lines:
        m = _FUNC_STUB_RE.match(ln)
        if m and m.group('fn') in names:
            indent = ln[:len(ln) - len(ln.lstrip())]
            out.append(f"{indent}/* pcport: redundant {m.group('fn')} stub proto dropped "
                       f"(real typed decl/def in TU) */")
            dropped += 1
        else:
            out.append(ln)
    return out, dropped


ASM_RE   = re.compile(r'^\s*asm\s')
INC_RE   = re.compile(r'#\s*include\s+"[^"]*\.inc"')
IF1_RE   = re.compile(r'^\s*#\s*if\s+1\b')
ELSE_RE  = re.compile(r'^\s*#\s*else\b')
ENDIF_RE = re.compile(r'^\s*#\s*endif\b')
IF_RE    = re.compile(r'^\s*#\s*if')          # #if / #ifdef / #ifndef


def transform(lines):
    out, i, n, flipped = [], 0, len(lines), 0
    while i < n:
        if IF1_RE.match(lines[i]):
            # scan the #if-1 branch (to its #else at depth 1); is it an asm wrapper?
            k, d, else_idx, has_asm, has_inc = i + 1, 1, None, False, False
            while k < n and d >= 1:
                if ELSE_RE.match(lines[k]) and d == 1:
                    else_idx = k; break
                if IF_RE.match(lines[k]): d += 1
                elif ENDIF_RE.match(lines[k]): d -= 1
                if d == 1:
                    if ASM_RE.match(lines[k]): has_asm = True
                    if INC_RE.search(lines[k]): has_inc = True
                k += 1
            if else_idx is not None and has_asm and has_inc:
                # emit only the #else branch (drop #if1/asm and the matching #endif)
                k, d = else_idx + 1, 1
                while k < n and d >= 1:
                    if IF_RE.match(lines[k]):
                        d += 1; out.append(lines[k])
                    elif ENDIF_RE.match(lines[k]):
                        d -= 1
                        if d == 0:
                            break          # drop the wrapper's closing #endif
                        out.append(lines[k])
                    else:
                        out.append(lines[k])
                    k += 1
                i = k + 1
                flipped += 1
                continue
        out.append(lines[i]); i += 1
    return out, flipped


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("files", nargs="+")
    ap.add_argument("--out-dir", default="build_pc/gen")
    a = ap.parse_args()
    out_root = ROOT / a.out_dir
    for f in a.files:
        src = (ROOT / f) if not Path(f).is_absolute() else Path(f)
        rel = src.resolve().relative_to(ROOT / "src")
        lines = src.read_text(errors="replace").splitlines()
        gen, flipped = transform(lines)
        unify_map = EXTERN_UNIFY.get(rel.as_posix())
        rewrites = 0
        if unify_map:
            gen, rewrites = unify_externs(gen, unify_map)
        kr_names = FUNC_PROTO_KR.get(rel.as_posix())
        kr = 0
        if kr_names:
            gen, kr = neutralize_func_protos(gen, kr_names)
        retype_map = FUNC_PROTO_RETYPE.get(rel.as_posix())
        retyped = 0
        if retype_map:
            gen, retyped = retype_func_protos(gen, retype_map)
        drop_names = FUNC_STUB_DROP.get(rel.as_posix())
        dropped = 0
        if drop_names:
            gen, dropped = drop_func_stubs(gen, drop_names)
        dst = out_root / rel
        dst.parent.mkdir(parents=True, exist_ok=True)
        header = (f"/* GENERATED by tools/pcport_gen.py from src/{rel.as_posix()} — "
                  f"{flipped} asm wrapper(s) replaced with their #else C; "
                  f"{rewrites} conflicting data extern(s) unified; "
                  f"{kr} func proto(s) neutralized; {retyped} retyped; "
                  f"{dropped} stub proto(s) dropped. DO NOT EDIT. */\n")
        pre = PREAMBLE.get(rel.as_posix(), "")
        text = header + pre + "\n".join(gen) + "\n"
        fixes = 0
        for old, new in TEXT_FIXUPS.get(rel.as_posix(), []):
            if old in text:
                text = text.replace(old, new)
                fixes += 1
        dst.write_text(text)
        print(f"  {rel.as_posix():<28} flipped {flipped}, "
              f"unified {rewrites} data extern(s), {kr} kr-proto(s), "
              f"{retyped} retype(s), {dropped} stub-drop(s), {fixes} text fixup(s) -> "
              f"{dst.relative_to(ROOT).as_posix()}")


if __name__ == "__main__":
    main()
