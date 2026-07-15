#!/usr/bin/env python3
"""Quality gate: block match-cheating patterns in newly added source.

Rules:
- Any added `#include "*.inc"` in src/ fails (raw-asm shim).
- Any new or changed asm block fails unless its enclosing function fits one of
  two authentic SDK classes:
  - hardware-register/privileged primitives (PPCMfmsr etc.); or
  - the explicitly named Dolphin SDK paired-single math routines.

MWCC cannot emit the required privileged or paired-single instructions from C.
The paired-single exception is restricted to one source unit, known SDK
symbols, and the target's verified mnemonic set. Calls and general GPR memory
access remain forbidden, so game logic cannot hide behind the exception.
"""

import re
import subprocess
import sys


HARDWARE_ALLOWED = {
    # SPR / MSR / FPSCR access
    "mfmsr", "mtmsr", "mfspr", "mtspr", "mfdec", "mtdec",
    "mffs", "mtfsf", "mtfsb0", "mtfsb1",
    # synchronization / control
    "sync", "isync", "eieio", "rfi", "sc", "nop",
    # register/immediate shuffling needed around SPR ops (no memory access)
    "li", "lis", "ori", "oris", "or", "mr", "rlwinm", "addi",
    # control flow: plain branch and return only (no bl -- no calls)
    "b", "blr", "bne", "beq", "bne-", "beq-", "bne+", "beq+", "cmpwi",
}

DOLPHIN_PAIRED_SINGLE_PATH = "src/dolphin/sdk_range_800A2D38.c"

# These 26 routines are the paired-single implementations in this SDK unit.
# PSMTXRotRad and PSMTXRotAxisRad are deliberately absent: the vendor source
# implements them as C wrappers, and both already match from C.
DOLPHIN_PAIRED_SINGLE_FUNCTIONS = {
    "PSMTXIdentity",
    "PSMTXCopy",
    "PSMTXConcat",
    "PSMTXTranspose",
    "PSMTXInverse",
    "PSMTXInvXpose",
    "PSMTXRotTrig",
    "__PSMTXRotAxisRadInternal",
    "PSMTXTrans",
    "PSMTXTransApply",
    "PSMTXScale",
    "PSMTXScaleApply",
    "PSMTXQuat",
    "PSMTXMultVec",
    "PSMTXMultVecSR",
    "PSVECAdd",
    "PSVECSubtract",
    "PSVECScale",
    "PSVECNormalize",
    "PSVECSquareMag",
    "PSVECMag",
    "PSVECDotProduct",
    "PSVECCrossProduct",
    "PSVECSquareDistance",
    "PSVECDistance",
    "PSQUATMultiply",
}

# Exact mnemonic union of the 26 target functions. This includes ten support
# instructions omitted from the contributor's initial sketch, but needed by
# the target: ps_muls1, frsqrte, fadds, fsubs, fmuls, fmadds, fnmsubs, fcmpu,
# cmplwi, and stwu. It intentionally excludes bl, lwz/stw, and update-form
# paired loads/stores because none belongs to this target unit.
DOLPHIN_PAIRED_SINGLE_ALLOWED = {
    "addi", "b", "beq", "blr", "bne", "cmplwi",
    "fadds", "fcmpu", "fmadds", "fmuls", "fnmsubs", "fres", "frsp",
    "frsqrte", "fsubs", "lfd", "lfs", "li", "lis", "ori",
    "ps_add", "ps_cmpo0", "ps_madd", "ps_madds0", "ps_madds1",
    "ps_merge00", "ps_merge01", "ps_merge10", "ps_merge11", "ps_msub",
    "ps_mul", "ps_muls0", "ps_muls1", "ps_neg", "ps_nmadd", "ps_nmsub",
    "ps_sub", "ps_sum0", "ps_sum1", "psq_l", "psq_st",
    "stfd", "stfs", "stwu",
}

HUNK = re.compile(r"^@@ -\d+(?:,\d+)? \+(\d+)(?:,\d+)? @@")
ASM_FUNCTION = re.compile(
    r"(?m)^[ \t]*asm[ \t\r\n]+(?:[A-Za-z_]\w*[ \t\r\n*]+)+"
    r"(?P<name>[A-Za-z_]\w*)[ \t\r\n]*\([^;{}]*\)[ \t\r\n]*\{"
)
FUNCTION = re.compile(
    r"(?m)^[ \t]*(?!asm\b)(?:[A-Za-z_]\w*[ \t\r\n*]+)+"
    r"(?P<name>[A-Za-z_]\w*)[ \t\r\n]*\([^;{}]*\)[ \t\r\n]*\{"
)
INLINE_ASM = re.compile(r"\basm\s*(?:volatile\s*)?\{")
ASM_TOKEN = re.compile(r"\basm\b")
INC_INCLUDE = re.compile(r'^[ \t]*#[ \t]*include[ \t]*"[^"\r\n]*\.inc"')
MACRO_DEFINE = re.compile(r"(?m)^[ \t]*#[ \t]*define[ \t]+(?P<name>[A-Za-z_]\w*)")
IDENTIFIER = re.compile(r"\b[A-Za-z_]\w*\b")
LABEL_PREFIX = re.compile(r"^(?P<label>[.A-Za-z_]\w*):(?P<rest>.*)$")
LOCAL_BRANCHES = {"b", "beq", "bne", "beq-", "bne-", "beq+", "bne+"}

# The C preprocessor runs before MWCC parses an asm block. Redefining one of
# these tokens can make the source text pass this scanner while emitting a
# different symbol, opcode, or register. No active source/header currently
# defines one, so additions are rejected fail-closed.
ASM_PROTECTED_MACROS = (
    HARDWARE_ALLOWED
    | DOLPHIN_PAIRED_SINGLE_ALLOWED
    | DOLPHIN_PAIRED_SINGLE_FUNCTIONS
    | {"asm", "nofralloc", "fralloc", "entry"}
    | {f"r{i}" for i in range(32)}
    | {f"f{i}" for i in range(32)}
    | {f"qr{i}" for i in range(8)}
    | {f"cr{i}" for i in range(8)}
)
ASM_PROTECTED_MACROS_LOWER = {name.lower() for name in ASM_PROTECTED_MACROS}


def _mask_non_code(source: str) -> str:
    """Blank comments and literals while preserving offsets and newlines."""
    out = []
    state = "code"
    quote = ""
    i = 0
    while i < len(source):
        ch = source[i]
        nxt = source[i + 1] if i + 1 < len(source) else ""
        if state == "code":
            if ch == "/" and nxt == "/":
                out.extend((" ", " "))
                state = "line_comment"
                i += 2
                continue
            if ch == "/" and nxt == "*":
                out.extend((" ", " "))
                state = "block_comment"
                i += 2
                continue
            if ch in ('"', "'"):
                quote = ch
                out.append(" ")
                state = "literal"
                i += 1
                continue
            out.append(ch)
            i += 1
            continue

        if state == "line_comment":
            if ch == "\n":
                out.append("\n")
                state = "code"
            else:
                out.append(" ")
            i += 1
            continue

        if state == "block_comment":
            if ch == "*" and nxt == "/":
                out.extend((" ", " "))
                state = "code"
                i += 2
            else:
                out.append("\n" if ch == "\n" else " ")
                i += 1
            continue

        # String or character literal.
        if ch == "\\" and nxt:
            out.append(" ")
            out.append("\n" if nxt == "\n" else " ")
            i += 2
        elif ch == quote:
            out.append(" ")
            state = "code"
            i += 1
        else:
            out.append("\n" if ch == "\n" else " ")
            i += 1

    return "".join(out)


def _matching_brace(masked: str, opening: int) -> int | None:
    depth = 0
    for i in range(opening, len(masked)):
        if masked[i] == "{":
            depth += 1
        elif masked[i] == "}":
            depth -= 1
            if depth == 0:
                return i
    return None


def _line_number(source: str, offset: int) -> int:
    return source.count("\n", 0, offset) + 1


def _function_regions(source: str, masked: str) -> list[dict]:
    regions = []
    for match in FUNCTION.finditer(masked):
        opening = masked.find("{", match.start(), match.end())
        closing = _matching_brace(masked, opening)
        if closing is None:
            continue
        regions.append({
            "func": match.group("name"),
            "start": match.start(),
            "end": closing,
        })
    return regions


def find_asm_regions(source: str) -> list[dict]:
    """Return whole-function and inline asm regions with enclosing symbols."""
    masked = _mask_non_code(source)
    functions = _function_regions(source, masked)
    regions = []

    for match in ASM_FUNCTION.finditer(masked):
        opening = masked.find("{", match.start(), match.end())
        closing = _matching_brace(masked, opening)
        end = closing if closing is not None else len(source) - 1
        regions.append({
            "func": match.group("name"),
            "body": source[opening + 1:end],
            "start_line": _line_number(source, match.start()),
            "end_line": _line_number(source, end),
            "closed": closing is not None,
        })

    for match in INLINE_ASM.finditer(masked):
        opening = masked.find("{", match.start(), match.end())
        closing = _matching_brace(masked, opening)
        end = closing if closing is not None else len(source) - 1
        enclosing = [
            region for region in functions
            if region["start"] <= match.start() <= region["end"]
        ]
        func = max(enclosing, key=lambda region: region["start"])["func"] if enclosing else ""
        regions.append({
            "func": func,
            "body": source[opening + 1:end],
            "start_line": _line_number(source, match.start()),
            "end_line": _line_number(source, end),
            "closed": closing is not None,
        })

    return regions


def allowlist_for(path: str, func: str) -> tuple[set[str], str, bool]:
    if path == DOLPHIN_PAIRED_SINGLE_PATH and func in DOLPHIN_PAIRED_SINGLE_FUNCTIONS:
        return DOLPHIN_PAIRED_SINGLE_ALLOWED, "Dolphin paired-single SDK", True
    return HARDWARE_ALLOWED, "hardware-primitive", False


def _asm_statements(body: str) -> tuple[list[str], set[str]]:
    """Split MWCC asm statements and collect labels without losing semicolon ops."""
    statements = []
    labels = set()
    for raw in _mask_non_code(body).splitlines():
        line = raw.strip()
        if not line:
            continue
        # A leading # is a C preprocessor directive, not an assembler comment.
        # Preserve it so the allowlist rejects it. Elsewhere # starts a comment.
        if not line.startswith("#"):
            line = line.split("#", 1)[0]
        for raw_statement in line.split(";"):
            statement = raw_statement.strip()
            while statement:
                match = LABEL_PREFIX.match(statement)
                if not match:
                    break
                labels.add(match.group("label"))
                statement = match.group("rest").strip()
            if statement:
                statements.append(statement)
    return statements, labels


def asm_body_ok(body: str, path: str = "", func: str = "") -> bool:
    allowed, allowlist_name, require_paired_single = allowlist_for(path, func)
    saw_paired_single = False
    statements, labels = _asm_statements(body)
    for line in statements:
        if line in ("{", "}", "nofralloc", "fralloc"):
            continue
        mnemonic = line.split()[0].lower()
        if mnemonic.startswith(("ps_", "psq_")):
            saw_paired_single = True
        if mnemonic not in allowed:
            print(
                f"::error::asm instruction '{mnemonic}' outside "
                f"{allowlist_name} allowlist for {path}:{func}: {line}"
            )
            return False
        if mnemonic in LOCAL_BRANCHES:
            parts = line.split(None, 1)
            target = parts[1].strip() if len(parts) == 2 else ""
            if target not in labels:
                print(
                    f"::error::asm branch target '{target}' is not a label "
                    f"defined inside {path}:{func}"
                )
                return False
    if require_paired_single and not saw_paired_single:
        print(f"::error::{path}:{func} uses paired-single exception without a ps_/psq_ instruction")
        return False
    return True


def added_lines_from_diff(diff: str) -> dict[str, list[tuple[int, str]]]:
    added: dict[str, list[tuple[int, str]]] = {}
    current_file = None
    new_line = None
    for line in diff.splitlines():
        if line.startswith("+++ b/"):
            current_file = line[6:]
            added.setdefault(current_file, [])
            continue
        match = HUNK.match(line)
        if match:
            new_line = int(match.group(1))
            continue
        if current_file is None or new_line is None:
            continue
        if line.startswith("+") and not line.startswith("+++"):
            added[current_file].append((new_line, line[1:]))
            new_line += 1
        elif line.startswith("-") and not line.startswith("---"):
            continue
        elif not line.startswith("\\"):
            new_line += 1
    return added


def scan_source(path: str, source: str, added_lines: list[tuple[int, str]]) -> bool:
    """Validate every asm function/block touched by an added diff line."""
    added_numbers = {line_number for line_number, _ in added_lines}
    masked = _mask_non_code(source)
    source_lines = source.splitlines()
    masked_lines = masked.splitlines()
    regions = find_asm_regions(source)
    touched = [
        region for region in regions
        if any(region["start_line"] <= line <= region["end_line"] for line in added_numbers)
    ]

    fail = False
    macro_definitions = [
        (match.group("name"), _line_number(masked, match.start()))
        for match in MACRO_DEFINE.finditer(masked)
    ]
    for name, line_number in macro_definitions:
        if line_number in added_numbers and name.lower() in ASM_PROTECTED_MACROS_LOWER:
            print(f"::error::asm-sensitive macro '{name}' added in {path}:{line_number}")
            fail = True

    for line_number, _ in added_lines:
        if not 1 <= line_number <= len(source_lines):
            print(f"::error::cannot map added line {path}:{line_number} to HEAD source")
            fail = True
            continue
        text = source_lines[line_number - 1]
        code = masked_lines[line_number - 1]
        include = INC_INCLUDE.match(text)
        hash_index = text.find("#", 0, include.end()) if include else -1
        if include and code[hash_index] == "#":
            print(
                f"::error::.inc include added in {path}:{line_number} "
                "— raw-asm shim, not a real match."
            )
            fail = True
        if ASM_TOKEN.search(code) and not any(
            region["start_line"] <= line_number <= region["end_line"] for region in regions
        ):
            print(f"::error::unparseable or unterminated asm block in {path}:{line_number}")
            fail = True

    touched_funcs = {region["func"] for region in touched}
    for func in sorted(touched_funcs):
        func_regions = [region for region in regions if region["func"] == func]
        if not func or any(not region["closed"] for region in func_regions):
            print(f"::error::cannot safely map asm block to a closed function in {path}")
            fail = True
            continue
        combined_body = "\n".join(region["body"] for region in func_regions)
        asm_identifiers = set(IDENTIFIER.findall(combined_body)) | {func}
        shadowed = sorted({name for name, _ in macro_definitions} & asm_identifiers)
        if shadowed:
            print(
                f"::error::C preprocessor macro(s) can rewrite asm in {path}:{func}: "
                + ", ".join(shadowed)
            )
            fail = True
            continue
        if asm_body_ok(combined_body, path, func):
            _, allowlist_name, _ = allowlist_for(path, func)
            print(f"asm in {func} ({path}): {allowlist_name} allowlist OK")
        else:
            print(
                f"::error::asm-wrapper/inline-asm in {path}:{func} is outside "
                "the authentic SDK exceptions"
            )
            fail = True
    return not fail


def main() -> int:
    if len(sys.argv) != 3:
        print(f"usage: {sys.argv[0]} BASE HEAD", file=sys.stderr)
        return 2
    base, head = sys.argv[1], sys.argv[2]
    diff = subprocess.run(
        [
            "git", "diff", "--unified=0", f"{base}...{head}", "--",
            "src/**/*.c", "src/**/*.h",
        ],
        capture_output=True,
        text=True,
        check=True,
    ).stdout
    added_by_path = added_lines_from_diff(diff)
    fail = False

    for path, added_lines in added_by_path.items():
        try:
            source = subprocess.run(
                ["git", "show", f"{head}:{path}"],
                capture_output=True,
                text=True,
                check=True,
            ).stdout
        except subprocess.CalledProcessError:
            print(f"::error::cannot read changed source {path} at {head}")
            fail = True
            continue
        if not scan_source(path, source, added_lines):
            fail = True

    if not fail:
        print("No asm-wrapper / inline-asm / .inc cheats in added source.")
    return int(fail)


if __name__ == "__main__":
    sys.exit(main())
