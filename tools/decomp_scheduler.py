#!/usr/bin/env python3
"""
Decomp Task Scheduler & Load Balancer

Scans all remaining #if 1 asm wrappers, classifies by complexity,
generates self-contained decomp prompts, and routes to the appropriate
model backend (free cloud, local GPU, or Claude).

Usage:
    python tools/decomp_scheduler.py --scan          # Scan and classify all wrappers
    python tools/decomp_scheduler.py --generate N    # Generate N task prompts
    python tools/decomp_scheduler.py --status        # Show progress
    python tools/decomp_scheduler.py --verify FILE   # Verify a decompiled function
"""

import argparse
import json
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).parent.parent
WORK_DIR = ROOT / "tools" / "decomp_work"
QUEUE_FILE = WORK_DIR / "work_queue.json"
PROGRESS_FILE = WORK_DIR / "progress.json"
LOCK_DIR = WORK_DIR / "locks"
PROMPTS_DIR = WORK_DIR / "prompts"

SDA_BASE = 0x80480820
SDA2_BASE = 0x804836A0

# Complexity tiers
TIER_SIMPLE = "simple"      # 2-10 asm lines -> free cloud / local GPU
TIER_MEDIUM = "medium"      # 11-30 asm lines -> local GPU / Claude
TIER_COMPLEX = "complex"    # 31-80 asm lines -> Claude
TIER_HARD = "hard"          # 80+ asm lines -> Claude only

TIER_LIMITS = {
    TIER_SIMPLE: (0, 10),
    TIER_MEDIUM: (11, 30),
    TIER_COMPLEX: (31, 80),
    TIER_HARD: (81, 99999),
}

TIER_BACKENDS = {
    TIER_SIMPLE: ["ollama", "opencode", "claude"],
    TIER_MEDIUM: ["ollama", "claude"],
    TIER_COMPLEX: ["claude"],
    TIER_HARD: ["claude"],
}


def ensure_dirs():
    for d in [WORK_DIR, LOCK_DIR, PROMPTS_DIR]:
        d.mkdir(parents=True, exist_ok=True)


def scan_wrappers():
    """Find all remaining #if 1 asm wrappers and their .inc files."""
    wrappers = []
    src_dir = ROOT / "src"

    for c_file in src_dir.rglob("*.c"):
        rel_path = c_file.relative_to(ROOT)
        content = c_file.read_text(encoding="utf-8", errors="replace")
        lines = content.split("\n")

        i = 0
        while i < len(lines):
            line = lines[i].strip()
            if line == "#if 1" and i + 1 < len(lines):
                next_line = lines[i + 1].strip()
                m = re.match(r'asm void (\w+)\(void\)\s*\{', next_line)
                if m:
                    fn_name = m.group(1)
                    # Find the .inc file
                    inc_match = None
                    if i + 2 < len(lines):
                        inc_line = lines[i + 2].strip()
                        inc_m = re.match(r'#include "([^"]+)"', inc_line)
                        if inc_m:
                            inc_match = inc_m.group(1)

                    if inc_match:
                        inc_path = ROOT / inc_match
                        if inc_path.exists():
                            inc_content = inc_path.read_text(encoding="utf-8", errors="replace")
                            asm_lines = [l for l in inc_content.strip().split("\n")
                                        if l.strip() and not l.strip().startswith("nofralloc")]
                            line_count = len(asm_lines)
                        else:
                            line_count = 0
                    else:
                        line_count = 0

                    # Determine surrounding pragmas
                    pragmas = []
                    for j in range(max(0, i - 5), i):
                        pl = lines[j].strip()
                        if pl.startswith("#pragma"):
                            pragmas.append(pl)

                    # Classify complexity
                    tier = TIER_SIMPLE
                    for t, (lo, hi) in TIER_LIMITS.items():
                        if lo <= line_count <= hi:
                            tier = t
                            break

                    wrappers.append({
                        "function": fn_name,
                        "file": str(rel_path).replace("\\", "/"),
                        "inc_file": inc_match,
                        "asm_lines": line_count,
                        "tier": tier,
                        "line_number": i + 1,
                        "pragmas": pragmas,
                        "status": "pending",
                    })
            i += 1

    return wrappers


def load_progress():
    if PROGRESS_FILE.exists():
        return json.loads(PROGRESS_FILE.read_text())
    return {"completed": [], "failed": [], "in_progress": []}


def save_progress(progress):
    PROGRESS_FILE.write_text(json.dumps(progress, indent=2))


def load_queue():
    if QUEUE_FILE.exists():
        return json.loads(QUEUE_FILE.read_text())
    return []


def save_queue(queue):
    QUEUE_FILE.write_text(json.dumps(queue, indent=2))


def acquire_lock(fn_name):
    lock_file = LOCK_DIR / f"{fn_name}.lock"
    if lock_file.exists():
        return False
    lock_file.write_text(f"locked by pid {os.getpid()}")
    return True


def release_lock(fn_name):
    lock_file = LOCK_DIR / f"{fn_name}.lock"
    if lock_file.exists():
        lock_file.unlink()


def generate_prompt(wrapper):
    """Generate a self-contained decomp prompt for a single function."""
    inc_path = ROOT / wrapper["inc_file"]
    if not inc_path.exists():
        return None

    inc_content = inc_path.read_text(encoding="utf-8", errors="replace")

    # Read surrounding context from the C file
    c_file = ROOT / wrapper["file"]
    c_content = c_file.read_text(encoding="utf-8", errors="replace")
    c_lines = c_content.split("\n")
    ln = wrapper["line_number"] - 1
    context_start = max(0, ln - 15)
    context_end = min(len(c_lines), ln + 20)
    context = "\n".join(c_lines[context_start:context_end])

    prompt = f"""You are decompiling a GameCube (PowerPC) function for Pokemon Colosseum.
Convert the following PPC assembly into matching C89 code.

RULES:
- C89 only: ALL declarations before statements in each block
- Use block scoping {{ }} when the asm loads a global twice
- Do NOT use float literals (0.0f) for sdata2 returns — use extern f32 lbl_XXXXXXXX
- extern labels for sdata globals: compute from SDA_BASE=0x80480820 (r13) or SDA2_BASE=0x804836A0 (r2)
- Signed 16-bit offset: if raw >= 0x8000, offset = raw - 0x10000
- Look up symbol names in the surrounding context or use lbl_XXXXXXXX format
- The function must compile with: mwcceppc -O4,p -proc gekko -fp hard -enum int
- Output ONLY the C function replacement (no asm wrapper, no #if blocks)

FUNCTION: {wrapper['function']}
FILE: {wrapper['file']}
SIZE: {wrapper['asm_lines']} asm instructions

ASSEMBLY ({wrapper['inc_file']}):
```
{inc_content}
```

SURROUNDING C CONTEXT:
```c
{context}
```

PRAGMAS IN EFFECT: {', '.join(wrapper['pragmas']) if wrapper['pragmas'] else 'default (none)'}

Write the C89 replacement function. Include any needed extern declarations.
Remember: leaf functions (nofralloc, no bl calls) should NOT have pragmas — use default O4.
Functions with stwu/mflr prologue that just call one function are simple thunks: void fn(void) {{ other_fn(); }}
"""
    return prompt


def cmd_scan(args):
    """Scan and classify all wrappers."""
    ensure_dirs()
    wrappers = scan_wrappers()

    # Filter out already-completed
    progress = load_progress()
    completed_fns = set(progress["completed"])
    wrappers = [w for w in wrappers if w["function"] not in completed_fns]

    save_queue(wrappers)

    # Print summary
    by_tier = {}
    by_file = {}
    for w in wrappers:
        by_tier.setdefault(w["tier"], []).append(w)
        by_file.setdefault(w["file"], []).append(w)

    print(f"\n{'='*60}")
    print(f"DECOMP WORK QUEUE SCAN")
    print(f"{'='*60}")
    print(f"Total remaining wrappers: {len(wrappers)}")
    print()
    print("By complexity tier:")
    for tier in [TIER_SIMPLE, TIER_MEDIUM, TIER_COMPLEX, TIER_HARD]:
        items = by_tier.get(tier, [])
        backends = ", ".join(TIER_BACKENDS.get(tier, []))
        print(f"  {tier:10s}: {len(items):4d} functions  -> backends: {backends}")

    print()
    print("Top 10 files by remaining wrappers:")
    sorted_files = sorted(by_file.items(), key=lambda x: -len(x[1]))
    for f, items in sorted_files[:10]:
        tiers = {}
        for w in items:
            tiers[w["tier"]] = tiers.get(w["tier"], 0) + 1
        tier_str = " ".join(f"{t}:{c}" for t, c in sorted(tiers.items()))
        print(f"  {len(items):4d}  {f}  ({tier_str})")

    print(f"\n{'='*60}")


def cmd_generate(args):
    """Generate N task prompts for the specified tier."""
    ensure_dirs()
    queue = load_queue()
    if not queue:
        print("No queue. Run --scan first.")
        return

    tier = args.tier or TIER_SIMPLE
    count = args.generate

    candidates = [w for w in queue if w["tier"] == tier and w["status"] == "pending"]
    candidates.sort(key=lambda w: w["asm_lines"])

    generated = 0
    for w in candidates[:count]:
        prompt = generate_prompt(w)
        if prompt:
            fn = w["function"]
            prompt_file = PROMPTS_DIR / f"{fn}.txt"
            prompt_file.write_text(prompt)
            print(f"Generated: {prompt_file.name} ({w['asm_lines']} lines, {w['tier']})")
            generated += 1

    print(f"\nGenerated {generated} prompts in {PROMPTS_DIR}")


def cmd_verify(args):
    """Verify a decompiled function by compiling and match testing."""
    fn_name = args.verify
    # Find the function in the queue
    queue = load_queue()
    wrapper = None
    for w in queue:
        if w["function"] == fn_name:
            wrapper = w
            break

    if not wrapper:
        print(f"Function {fn_name} not found in queue.")
        return

    c_file = wrapper["file"]

    # Compile
    print(f"Compiling {c_file}...")
    result = subprocess.run(
        ["python", "tools/compile_check.py", c_file],
        capture_output=True, text=True, cwd=str(ROOT)
    )
    if result.returncode != 0:
        print(f"COMPILE FAILED:\n{result.stdout}\n{result.stderr}")
        return False

    # Match test
    print(f"Match testing {fn_name}...")
    result = subprocess.run(
        ["python", "tools/match_test.py", fn_name],
        capture_output=True, text=True, cwd=str(ROOT)
    )
    output = result.stdout + result.stderr
    print(output)

    if "MATCHING" in output:
        # Update progress
        progress = load_progress()
        if fn_name not in progress["completed"]:
            progress["completed"].append(fn_name)
        save_progress(progress)
        release_lock(fn_name)
        print(f"\n*** {fn_name} VERIFIED 100% MATCH ***")
        return True
    else:
        print(f"\n{fn_name} not yet matching.")
        return False


def cmd_status(args):
    """Show overall progress."""
    ensure_dirs()
    progress = load_progress()
    queue = load_queue()

    total = len(queue) + len(progress["completed"])
    done = len(progress["completed"])
    remaining = len(queue)

    print(f"\n{'='*60}")
    print(f"DECOMP PROGRESS")
    print(f"{'='*60}")
    print(f"Completed:  {done}")
    print(f"Remaining:  {remaining}")
    print(f"Total:      {total}")
    if total > 0:
        print(f"Progress:   {done/total*100:.1f}%")

    if queue:
        by_tier = {}
        for w in queue:
            by_tier.setdefault(w["tier"], []).append(w)
        print("\nRemaining by tier:")
        for tier in [TIER_SIMPLE, TIER_MEDIUM, TIER_COMPLEX, TIER_HARD]:
            items = by_tier.get(tier, [])
            if items:
                print(f"  {tier:10s}: {len(items)}")

    # Show locks (in-progress)
    locks = list(LOCK_DIR.glob("*.lock"))
    if locks:
        print(f"\nIn progress ({len(locks)}):")
        for l in locks:
            print(f"  {l.stem}")

    print(f"{'='*60}")


def main():
    parser = argparse.ArgumentParser(description="Decomp Task Scheduler")
    parser.add_argument("--scan", action="store_true", help="Scan and classify all wrappers")
    parser.add_argument("--generate", type=int, metavar="N", help="Generate N task prompts")
    parser.add_argument("--tier", choices=["simple", "medium", "complex", "hard"],
                       help="Tier for --generate (default: simple)")
    parser.add_argument("--verify", metavar="FN", help="Verify a decompiled function")
    parser.add_argument("--status", action="store_true", help="Show progress")

    args = parser.parse_args()

    if args.scan:
        cmd_scan(args)
    elif args.generate:
        cmd_generate(args)
    elif args.verify:
        cmd_verify(args)
    elif args.status:
        cmd_status(args)
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
