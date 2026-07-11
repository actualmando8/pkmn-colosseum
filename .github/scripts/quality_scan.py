#!/usr/bin/env python3
"""Quality gate: block match-cheating patterns in newly added source.

Rules:
- Any added `#include "*.inc"` in src/ fails (raw-asm shim).
- Any added `asm` function fails UNLESS its body consists solely of
  hardware-register/privileged instructions (the Dolphin SDK primitive class:
  PPCMfmsr etc.). These functions were genuinely authored as asm in the SDK --
  MWCC cannot emit mfmsr/mtspr/sync from C, so an asm function is the only
  byte-exact and the only authentic decompilation. The allowlist deliberately
  excludes loads/stores/branches-with-links so no game logic can sneak through.
  Precedent: src/dolphin/os/OSTime.c, OSCache.c.
"""
import re
import subprocess
import sys

ALLOWED = {
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
LABEL = re.compile(r"^[.\w]+:$")


def asm_body_ok(body: str) -> bool:
    for raw in body.splitlines():
        line = raw.split("//")[0].split("#")[0].strip().rstrip(";")
        if not line or LABEL.match(line):
            continue
        if line in ("{", "}") or line.startswith(("nofralloc", "fralloc", "entry ")):
            continue
        mnemonic = line.split()[0].lower().rstrip(".")
        if mnemonic not in ALLOWED:
            print(f"::error::asm instruction '{mnemonic}' outside hardware-primitive allowlist: {line}")
            return False
    return True


def main() -> int:
    base, head = sys.argv[1], sys.argv[2]
    diff = subprocess.run(
        ["git", "diff", f"{base}...{head}", "--", "src/**/*.c", "src/**/*.h"],
        capture_output=True, text=True, check=True,
    ).stdout
    fail = 0
    current_file = None
    added_asm_funcs = []
    for line in diff.splitlines():
        if line.startswith("+++ b/"):
            current_file = line[6:]
            continue
        if not line.startswith("+") or line.startswith("+++"):
            continue
        text = line[1:]
        if re.search(r'#include\s*"[^"]*\.inc"', text):
            print(f"::error::.inc include added in {current_file} — raw-asm shim, not a real match.")
            fail = 1
        m = re.match(r"\s*asm\s+[\w\s\*]*?([A-Za-z_]\w*)\s*\(", text)
        if m:
            added_asm_funcs.append((current_file, m.group(1)))

    for path, func in added_asm_funcs:
        try:
            src = subprocess.run(["git", "show", f"{head}:{path}"], capture_output=True, text=True, check=True).stdout
        except subprocess.CalledProcessError:
            print(f"::error::cannot read {path} at head to validate asm function {func}")
            fail = 1
            continue
        m = re.search(rf"\basm\b[\w\s\*]*?\b{re.escape(func)}\s*\([^{{;]*\)\s*\{{", src)
        if not m:
            print(f"::error::asm function {func} declared but body not found in {path}")
            fail = 1
            continue
        i = src.index("{", m.start())
        depth, j = 0, i
        while j < len(src):
            if src[j] == "{":
                depth += 1
            elif src[j] == "}":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        body = src[i + 1 : j]
        if asm_body_ok(body):
            print(f"asm function {func} in {path}: hardware-primitive allowlist OK")
        else:
            print(f"::error::asm-wrapper function {func} added in {path} — embeds raw target asm, not real C. Decompile it instead.")
            fail = 1

    if fail == 0:
        print("No asm-wrapper / .inc cheats in added source.")
    return fail


if __name__ == "__main__":
    sys.exit(main())
