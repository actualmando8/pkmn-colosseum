#!/usr/bin/env python3
"""C89 pre-lint for model-generated function bodies.

kimi-k3 repeatedly emits C99-style mid-block declarations (a declaration after
a statement in the same block). CodeWarrior 1.x rejects these with an
uninformative caret-only "declaration syntax error", so the compile-loop wasted
whole rounds (0/8 on two lanes) without the model learning why.

This lint catches the pattern BEFORE compiling and returns precise, line-level
feedback the model can act on. Heuristic but tuned to the observed failure:
a line that begins with a type token and declares a name, occurring after a
non-declaration statement at the same brace depth.

Run directly to execute the self-test:  python c89_lint.py
"""
import re

# Type tokens that begin a declaration in this codebase.
_TYPES = (r"u8|s8|u16|s16|u32|s32|f32|f64|int|char|short|long|void|float|"
          r"double|unsigned|signed|BOOL|bool|register|const|volatile|struct|"
          r"union|enum")
# A line that looks like a declaration: starts with a type (optionally several
# type words / const / *), then an identifier, then '=' or ';' or '[' or ','.
_DECL_RE = re.compile(
    r'^\s*(?:(?:' + _TYPES + r')\b[\w\s]*?[\s*]+)'   # leading type words + ptr
    r'[A-Za-z_]\w*\s*(?:=|;|\[|,)')
# Keywords that begin a STATEMENT, never a declaration.
_STMT_KW = re.compile(r'^\s*(?:if|for|while|return|switch|do|else|goto|break|'
                      r'continue|case|default)\b')


def _strip_comments(s: str) -> str:
    s = re.sub(r'/\*.*?\*/', '', s, flags=re.S)
    s = re.sub(r'//[^\n]*', '', s)
    return s


def lint(body: str):
    """Return a list of (line_no, text, reason) for C89 violations."""
    lines = _strip_comments(body).splitlines()
    depth = 0
    # per-block: has a statement appeared yet at this depth?
    stmt_seen = [False]
    violations = []
    for i, raw in enumerate(lines, 1):
        line = raw.strip()
        if not line:
            continue
        opens = raw.count("{")
        closes = raw.count("}")

        # Classify the line BEFORE adjusting depth (its declaration-ness is
        # judged at the depth it starts in).
        is_stmt_kw = bool(_STMT_KW.match(line))
        is_label = bool(re.match(r'^[A-Za-z_]\w*\s*:\s*(?:;|$)', line))
        # Only flag declarations that begin with a KNOWN type token. This
        # misses mid-block typedef-name decls ("Work* w = ...") but has zero
        # false positives on assignments ("x = ...;") -- and a false reject
        # poisons the compile loop worse than a missed lint (the compiler
        # still catches typedef-name mid-block decls, just less legibly).
        looks_decl = bool(_DECL_RE.match(line))
        is_pure_brace = line in ("{", "}") or set(line) <= set("{}; ")

        if not is_pure_brace and not is_label:
            if looks_decl and not is_stmt_kw:
                if depth >= 1 and stmt_seen[depth]:
                    violations.append(
                        (i, line[:70],
                         "declaration after a statement (C89 needs all "
                         "declarations at the top of the block)"))
            else:
                # a real statement at this depth
                if depth >= 1:
                    stmt_seen[depth] = True

        # adjust depth, tracking per-block statement flags
        for _ in range(opens):
            depth += 1
            if len(stmt_seen) <= depth:
                stmt_seen.append(False)
            else:
                stmt_seen[depth] = False
        for _ in range(closes):
            if depth >= 1:
                stmt_seen[depth] = False
            depth = max(0, depth - 1)
    return violations


def _selftest():
    bad = """
void f(void) {
    u32 a;
    a = get();
    u32 b = a + 1;     /* VIOLATION: decl after statement */
    use(b);
}
"""
    good = """
void f(void) {
    u32 a;
    u32 b;
    a = get();
    b = a + 1;
    use(b);
}
"""
    good_nested = """
void f(void) {
    u32 a;
    a = get();
    if (a) {
        u32 c = a;     /* OK: top of a NEW block */
        use(c);
    }
}
"""
    # assignments to existing vars must NOT be flagged (false-positive guard)
    assigns_ok = """
void f(void) {
    u32 handle;
    Work* w;
    fadeCheck(1);
    handle = get();
    w = getw();
    w->field = handle;
    lbl_8047A5A0 = NULL;
    handle = other();
}
"""
    tests = [("bad", bad, 1), ("good", good, 0),
             ("good_nested", good_nested, 0), ("assigns_ok", assigns_ok, 0)]
    ok = True
    for name, src, expect in tests:
        v = lint(src)
        status = "PASS" if len(v) == expect else "FAIL"
        if len(v) != expect:
            ok = False
        print(f"[{status}] {name}: expected {expect} violation(s), got "
              f"{len(v)} {[x[0] for x in v]}")
    print("SELFTEST", "PASSED" if ok else "FAILED")
    return ok


if __name__ == "__main__":
    import sys
    sys.exit(0 if _selftest() else 1)
