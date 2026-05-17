#!/usr/bin/env python3
"""automatch.py - zero-token deterministic match sweeper.

For every near-miss function in a source file, mechanically try a curated
catalog of function-local `#pragma push`/`pop` variants, recompile, and
measure that one symbol with objdiff-cli. Keep the best variant; revert the
rest. No LLM in the loop.

This is the mandatory PRE-PASS before any LLM agent touches a file: it
auto-lands the mechanical wins (peephole/scheduling/opt-level/fp_contract
permutations) and PROVES which functions are genuinely blocked, so agent
tokens are only spent where deterministic sweeps plateau.

Usage:
    python tools/automatch.py src/game/scene_init.c
    python tools/automatch.py src/game/scene_init.c --band 85 99.99
    python tools/automatch.py src/game/scene_init.c --symbol fn_80037180
    python tools/automatch.py src/game/scene_init.c --apply
    python tools/automatch.py src/game/scene_init.c --apply --report out.md

Without --apply it is a dry run (reports what WOULD improve). With --apply
it writes the winning pragma stack for each improved function, then
re-measures the whole file to guarantee no net regression before keeping.

Safety:
  - Never edits `#if 0` asm blocks or `*_fn_*.inc` files.
  - Only inserts `#pragma push` ... `#pragma pop` around the active C body.
  - Verifies whole-file matched-count is monotonic before accepting --apply.
"""

import argparse
import json
import re
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TARGET_O = ROOT / "build" / "GC6E01" / "obj" / "auto_01_800055E0_text.o"
OBJDIFF = ROOT / "tools" / "objdiff-cli.exe"

sys.path.insert(0, str(ROOT / "tools"))
import compile_check  # noqa: E402  (reuse its compile + path logic)

# Curated variant catalog. Each entry is the list of pragma lines injected
# between `#pragma push` and the function signature. Order matters: the
# documented winners (see memory feedback_*) come first so we stop early.
PRAGMA_VARIANTS = [
    [],  # baseline (no push/pop) — establishes the reference match%
    ["#pragma peephole off"],
    ["#pragma scheduling off"],
    ["#pragma scheduling on"],
    ["#pragma optimization_level 1"],
    ["#pragma optimization_level 2"],
    ["#pragma optimization_level 3"],
    ["#pragma fp_contract on"],
    ["#pragma peephole off", "#pragma scheduling on"],
    ["#pragma peephole off", "#pragma scheduling off"],
    ["#pragma peephole off", "#pragma optimization_level 2"],
    ["#pragma scheduling on", "#pragma fp_contract on"],
    ["#pragma optimization_level 2", "#pragma fp_contract on"],
    ["#pragma optimization_level 2", "#pragma peephole off",
     "#pragma scheduling on"],
]

FN_HEADER_RE = re.compile(r"^/\*\s*(fn_[0-9A-Fa-f]+)\s*-\s*0x", re.M)


class Block:
    """One function region: header comment .. next header comment (or EOF)."""

    def __init__(self, name, start, end, lines):
        self.name = name
        self.start = start          # line index of /* fn_ ... */ header
        self.end = end              # exclusive
        self.lines = lines          # reference to the file's line list

    def find_c_def(self):
        """Return (sig_idx, body_close_idx) for the active C definition.

        Skips any `#if 0 ... #else` asm-wrapper region. body_close_idx is the
        line containing the function's final closing brace at column 0.
        Returns None if no plain C body is found (e.g. pure asm stub).
        """
        i = self.start
        in_if0 = False
        sig_idx = None
        while i < self.end:
            ln = self.lines[i]
            s = ln.strip()
            if s.startswith("#if 0"):
                in_if0 = True
            elif s.startswith("#else") and in_if0:
                in_if0 = False
            elif s.startswith("#endif"):
                in_if0 = False
            elif not in_if0 and re.match(
                r"^[A-Za-z_].*\b" + re.escape(self.name) + r"\s*\(", ln
            ) and "asm " not in ln and "#include" not in ln:
                sig_idx = i
                break
            i += 1
        if sig_idx is None:
            return None
        # Find the matching closing brace at column 0 (top-level).
        depth = 0
        seen_open = False
        j = sig_idx
        while j < self.end:
            for ch in self.lines[j]:
                if ch == "{":
                    depth += 1
                    seen_open = True
                elif ch == "}":
                    depth -= 1
            if seen_open and depth == 0:
                return (sig_idx, j)
            j += 1
        return None


def parse_blocks(text):
    lines = text.splitlines(keepends=True)
    heads = [(m.start(), m.group(1)) for m in FN_HEADER_RE.finditer(text)]
    # Map char offset -> line index.
    offs, acc = [], 0
    for ln in lines:
        offs.append(acc)
        acc += len(ln)

    def line_of(charpos):
        lo, hi = 0, len(offs) - 1
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if offs[mid] <= charpos:
                lo = mid
            else:
                hi = mid - 1
        return lo

    blocks = []
    for k, (cpos, name) in enumerate(heads):
        start = line_of(cpos)
        end = line_of(heads[k + 1][0]) if k + 1 < len(heads) else len(lines)
        blocks.append(Block(name, start, end, lines))
    return lines, blocks


def measure(src_path, symbols):
    """Compile src and return {symbol: match_percent} for the given symbols."""
    try:
        base_o = compile_check.compile_source(src_path)
    except SystemExit:
        return None  # compile failed
    cmd = [str(OBJDIFF), "diff", "-1", str(TARGET_O), "-2", str(base_o),
           "-o", "-", "--format", "json",
           "-c", "ppc.calculatePoolRelocations=false"]
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=str(ROOT))
    if r.returncode != 0:
        return None
    j = json.loads(r.stdout)
    out = {}
    for s in j.get("right", {}).get("symbols", []):
        if s.get("kind") == "SYMBOL_FUNCTION":
            out[s.get("name", "")] = s.get("match_percent", 0.0)
    return {s: out.get(s, 0.0) for s in symbols} if symbols else out


def matched_count(pcts):
    return sum(1 for v in pcts.values() if v >= 100.0)


def wrap_block(lines, sig_idx, close_idx, pragmas):
    """Return a new line list with push/pragmas around [sig_idx, close_idx]."""
    if not pragmas:
        return list(lines)
    new = list(lines)
    block = ["#pragma push\n"] + [p + "\n" for p in pragmas]
    new[sig_idx:sig_idx] = block
    # close_idx shifts by len(block)
    ins_at = close_idx + len(block) + 1
    new[ins_at:ins_at] = ["#pragma pop\n"]
    return new


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("source")
    ap.add_argument("--band", nargs=2, type=float, default=[80.0, 99.99],
                    metavar=("LO", "HI"),
                    help="only sweep functions with LO <= match%% < HI")
    ap.add_argument("--symbol", help="sweep just this one function")
    ap.add_argument("--apply", action="store_true",
                    help="write winning pragmas (default: dry run)")
    ap.add_argument("--report", help="write a markdown report here")
    args = ap.parse_args()

    src = Path(args.source)
    if not src.is_absolute():
        src = ROOT / src
    original = src.read_text(encoding="utf-8", errors="replace")

    # Baseline measurement of every function in the file.
    print(f"[automatch] baseline compile of {src.name} ...")
    base = measure(src, None)
    if base is None:
        sys.exit("baseline compile failed — fix the file first")
    base_matched = matched_count(base)
    print(f"[automatch] baseline: {base_matched}/{len(base)} @ 100%")

    if args.symbol:
        targets = [args.symbol]
    else:
        lo, hi = args.band
        targets = sorted(
            (n for n, p in base.items() if lo <= p < hi),
            key=lambda n: -base[n],
        )
    print(f"[automatch] {len(targets)} near-miss targets in band "
          f"{args.band[0]}-{args.band[1]}%")

    lines, blocks = parse_blocks(original)
    by_name = {b.name: b for b in blocks}

    results = []  # (name, base%, best%, best_variant)
    accepted = 0
    t0 = time.time()

    for idx, name in enumerate(targets, 1):
        blk = by_name.get(name)
        if blk is None:
            continue
        loc = blk.find_c_def()
        if loc is None:
            continue  # pure asm stub, nothing to sweep
        sig_idx, close_idx = loc
        b0 = base[name]
        best_pct, best_var = b0, None

        for variant in PRAGMA_VARIANTS:
            if not variant:
                continue  # baseline already known
            trial = wrap_block(lines, sig_idx, close_idx, variant)
            src.write_text("".join(trial), encoding="utf-8")
            m = measure(src, [name])
            if m is None:
                continue
            pct = m.get(name, 0.0)
            if pct > best_pct + 1e-6:
                best_pct, best_var = pct, variant
            if best_pct >= 100.0:
                break

        # restore pristine file before next function
        src.write_text(original, encoding="utf-8")
        tag = ("=100" if best_pct >= 100 else f"+{best_pct - b0:.2f}") \
            if best_var else "no change"
        print(f"  [{idx}/{len(targets)}] {name} {b0:.2f}% -> "
              f"{best_pct:.2f}%  ({tag})"
              + (f"  {best_var}" if best_var else ""))
        results.append((name, b0, best_pct, best_var))

    # Apply phase: re-insert every winning variant, then verify no regression.
    if args.apply:
        wins = [(n, v) for (n, b, p, v) in results if v and p > b + 1e-6]
        if wins:
            cur, _ = parse_blocks(original)
            cur_text = original
            for name, variant in wins:
                ls, bs = parse_blocks(cur_text)
                bn = {b.name: b for b in bs}[name]
                loc = bn.find_c_def()
                if loc is None:
                    continue
                si, ci = loc
                cur_text = "".join(wrap_block(ls, si, ci, variant))
            src.write_text(cur_text, encoding="utf-8")
            final = measure(src, None)
            if final is None or matched_count(final) < base_matched:
                src.write_text(original, encoding="utf-8")
                print("[automatch] APPLY REVERTED — net regression detected")
            else:
                print(f"[automatch] APPLIED {len(wins)} wins. "
                      f"matched {base_matched} -> {matched_count(final)}")
        else:
            print("[automatch] no improvements to apply")

    dt = time.time() - t0
    improved = [r for r in results if r[3] and r[2] > r[1] + 1e-6]
    solved = [r for r in results if r[2] >= 100.0]
    print(f"\n[automatch] done in {dt:.0f}s — "
          f"{len(solved)} reached 100%, {len(improved)} improved, "
          f"{len(targets) - len(improved)} unchanged (deterministically "
          f"blocked — escalate THOSE to an LLM agent)")

    if args.report:
        with open(args.report, "w", encoding="utf-8") as f:
            f.write(f"# automatch report: {src.name}\n\n")
            f.write(f"baseline {base_matched}/{len(base)} @ 100%, "
                    f"swept {len(targets)} near-misses in {dt:.0f}s\n\n")
            f.write("## Solved (deterministic, zero-token)\n\n")
            for n, b, p, v in solved:
                f.write(f"- **{n}** {b:.2f}% -> 100% via `{' '.join(v)}`\n")
            f.write("\n## Improved but not 100%\n\n")
            for n, b, p, v in improved:
                if p < 100:
                    f.write(f"- {n} {b:.2f}% -> {p:.2f}% "
                            f"via `{' '.join(v)}`\n")
            f.write("\n## Blocked — escalate to LLM agent\n\n")
            for n, b, p, v in results:
                if not v or p <= b + 1e-6:
                    f.write(f"- {n} stuck at {b:.2f}% "
                            f"(no pragma variant helped)\n")
        print(f"[automatch] report -> {args.report}")


if __name__ == "__main__":
    main()
