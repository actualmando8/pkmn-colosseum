#!/usr/bin/env python3
"""
Decomp Agent Runner

Sends decomp prompts to different backends (Ollama, OpenCode free models)
and verifies results. Coordinates with decomp_scheduler.py for task management.

Usage:
    python tools/decomp_agent.py --backend ollama --tier simple --count 5
    python tools/decomp_agent.py --backend ollama --function fn_XXXXXXXX
    python tools/decomp_agent.py --apply-and-verify fn_XXXXXXXX result.txt
"""

import argparse
import json
import os
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).parent.parent
WORK_DIR = ROOT / "tools" / "decomp_work"
QUEUE_FILE = WORK_DIR / "work_queue.json"
PROGRESS_FILE = WORK_DIR / "progress.json"
LOCK_DIR = WORK_DIR / "locks"
PROMPTS_DIR = WORK_DIR / "prompts"
RESULTS_DIR = WORK_DIR / "results"

OLLAMA_HOST = "10.0.0.3"
OLLAMA_PORT = 11434
OLLAMA_MODEL = "qwen2.5-coder:7b"

CODEX_CMD = "C:/Users/douglaswhittingham/AppData/Roaming/npm/codex.cmd"


def ensure_dirs():
    for d in [WORK_DIR, LOCK_DIR, PROMPTS_DIR, RESULTS_DIR]:
        d.mkdir(parents=True, exist_ok=True)


def load_queue():
    if QUEUE_FILE.exists():
        return json.loads(QUEUE_FILE.read_text())
    return []


def save_queue(queue):
    QUEUE_FILE.write_text(json.dumps(queue, indent=2))


def load_progress():
    if PROGRESS_FILE.exists():
        return json.loads(PROGRESS_FILE.read_text())
    return {"completed": [], "failed": [], "in_progress": []}


def save_progress(progress):
    PROGRESS_FILE.write_text(json.dumps(progress, indent=2))


def acquire_lock(fn_name):
    """Atomic-ish lock to prevent two agents from working the same function."""
    lock_file = LOCK_DIR / f"{fn_name}.lock"
    if lock_file.exists():
        # Check if stale (> 30 minutes)
        mtime = lock_file.stat().st_mtime
        if time.time() - mtime > 1800:
            lock_file.unlink()
        else:
            return False
    lock_file.write_text(json.dumps({
        "pid": os.getpid(),
        "time": time.time(),
        "host": os.environ.get("COMPUTERNAME", "unknown"),
    }))
    return True


def release_lock(fn_name):
    lock_file = LOCK_DIR / f"{fn_name}.lock"
    if lock_file.exists():
        lock_file.unlink()


def call_ollama(prompt, model=None):
    """Send prompt to Ollama on the GPU machine."""
    import urllib.request
    model = model or OLLAMA_MODEL

    url = f"http://{OLLAMA_HOST}:{OLLAMA_PORT}/api/generate"
    payload = json.dumps({
        "model": model,
        "prompt": prompt,
        "stream": False,
        "options": {
            "temperature": 0.1,
            "num_predict": 2048,
        }
    }).encode()

    req = urllib.request.Request(url, data=payload,
                                 headers={"Content-Type": "application/json"})
    try:
        with urllib.request.urlopen(req, timeout=120) as resp:
            result = json.loads(resp.read())
            return result.get("response", "")
    except Exception as e:
        print(f"Ollama error: {e}")
        return None


def call_codex(prompt):
    """Send prompt to Codex CLI for processing."""
    import tempfile
    # Write prompt to temp file
    with tempfile.NamedTemporaryFile(mode='w', suffix='.txt', delete=False) as f:
        f.write(prompt)
        prompt_file = f.name

    try:
        result = subprocess.run(
            [CODEX_CMD, "--quiet", "--approval-mode", "full-auto",
             "-m", "o4-mini",
             prompt],
            capture_output=True, text=True, cwd=str(ROOT),
            timeout=180
        )
        return result.stdout if result.stdout else result.stderr
    except subprocess.TimeoutExpired:
        print("Codex timed out")
        return None
    except Exception as e:
        print(f"Codex error: {e}")
        return None
    finally:
        os.unlink(prompt_file)


def extract_c_code(response):
    """Extract C code from model response, stripping markdown fences."""
    if not response:
        return None

    # Try to find code in ```c ... ``` blocks
    m = re.search(r'```c\s*\n(.*?)```', response, re.DOTALL)
    if m:
        return m.group(1).strip()

    # Try ``` ... ```
    m = re.search(r'```\s*\n(.*?)```', response, re.DOTALL)
    if m:
        return m.group(1).strip()

    # Return raw if it looks like C code
    if "void " in response or "return " in response or "extern " in response:
        # Strip any leading explanation text
        lines = response.split("\n")
        code_start = 0
        for i, line in enumerate(lines):
            if line.strip().startswith(("extern ", "void ", "u32 ", "s32 ",
                                        "f32 ", "u8 ", "u16 ", "#pragma ",
                                        "static ")):
                code_start = i
                break
        return "\n".join(lines[code_start:]).strip()

    return None


def apply_result(fn_name, c_code, wrapper_info):
    """Apply the decompiled C code to the source file, replacing the asm wrapper."""
    c_file = ROOT / wrapper_info["file"]
    content = c_file.read_text(encoding="utf-8", errors="replace")
    lines = content.split("\n")
    ln = wrapper_info["line_number"] - 1  # 0-indexed

    # Find the full #if 1 ... #endif block
    if_start = ln
    endif_line = None
    depth = 0
    for i in range(if_start, min(len(lines), if_start + 30)):
        stripped = lines[i].strip()
        if stripped.startswith("#if"):
            depth += 1
        elif stripped == "#endif":
            depth -= 1
            if depth == 0:
                endif_line = i
                break

    if endif_line is None:
        print(f"Could not find #endif for {fn_name}")
        return False

    # Find pragma push before the #if 1
    pragma_start = if_start
    for i in range(if_start - 1, max(0, if_start - 6), -1):
        stripped = lines[i].strip()
        if stripped in ("#pragma push", "#pragma pop"):
            break
        if stripped.startswith("#pragma"):
            pragma_start = i

    # Find pragma pop after #endif
    pragma_end = endif_line
    if endif_line + 1 < len(lines) and lines[endif_line + 1].strip() == "#pragma pop":
        pragma_end = endif_line + 1

    # Replace the block
    # Keep any externs that were before the #if 1
    externs = []
    for i in range(max(0, if_start - 10), if_start):
        stripped = lines[i].strip()
        if stripped.startswith("extern ") and stripped not in [l.strip() for l in lines[:i]]:
            externs.append(lines[i])

    replacement = c_code

    new_lines = lines[:pragma_start] + [replacement] + lines[pragma_end + 1:]
    c_file.write_text("\n".join(new_lines), encoding="utf-8")
    return True


def verify_function(fn_name, c_file_path):
    """Compile and match test a function."""
    # Compile
    result = subprocess.run(
        ["python", "tools/compile_check.py", c_file_path],
        capture_output=True, text=True, cwd=str(ROOT)
    )
    if result.returncode != 0 or "FAIL" in result.stdout:
        return "compile_error", result.stdout

    # Match test
    result = subprocess.run(
        ["python", "tools/match_test.py", fn_name],
        capture_output=True, text=True, cwd=str(ROOT)
    )
    output = result.stdout + result.stderr

    if "MATCHING" in output:
        return "match", output
    else:
        # Extract match percentage
        m = re.search(r'(\d+\.\d+)%', output)
        pct = float(m.group(1)) if m else 0.0
        return f"partial_{pct:.0f}", output


def process_function(fn_name, backend="ollama"):
    """Process a single function: generate prompt, call model, apply, verify."""
    ensure_dirs()
    queue = load_queue()

    wrapper = None
    for w in queue:
        if w["function"] == fn_name:
            wrapper = w
            break

    if not wrapper:
        print(f"{fn_name} not found in queue.")
        return False

    if not acquire_lock(fn_name):
        print(f"{fn_name} is locked by another agent.")
        return False

    print(f"\n{'='*50}")
    print(f"Processing: {fn_name} ({wrapper['asm_lines']} lines, {wrapper['tier']})")
    print(f"Backend: {backend}")
    print(f"{'='*50}")

    # Generate prompt
    from decomp_scheduler import generate_prompt
    prompt = generate_prompt(wrapper)
    if not prompt:
        print(f"Could not generate prompt for {fn_name}")
        release_lock(fn_name)
        return False

    # Save prompt
    (PROMPTS_DIR / f"{fn_name}.txt").write_text(prompt)

    # Call model
    print(f"Calling {backend}...")
    if backend == "ollama":
        response = call_ollama(prompt)
    elif backend == "codex":
        response = call_codex(prompt)
    else:
        print(f"Unknown backend: {backend}")
        release_lock(fn_name)
        return False

    if not response:
        print(f"No response from {backend}")
        release_lock(fn_name)
        return False

    # Save raw response
    (RESULTS_DIR / f"{fn_name}_response.txt").write_text(response)

    # Extract C code
    c_code = extract_c_code(response)
    if not c_code:
        print(f"Could not extract C code from response")
        release_lock(fn_name)
        return False

    (RESULTS_DIR / f"{fn_name}_code.c").write_text(c_code)
    print(f"Extracted C code ({len(c_code)} chars)")

    # Back up original file
    c_file = ROOT / wrapper["file"]
    backup = RESULTS_DIR / f"{fn_name}_backup.c"
    backup.write_text(c_file.read_text(encoding="utf-8", errors="replace"))

    # Apply
    print(f"Applying to {wrapper['file']}...")
    if not apply_result(fn_name, c_code, wrapper):
        print("Failed to apply result")
        # Restore backup
        c_file.write_text(backup.read_text())
        release_lock(fn_name)
        return False

    # Verify
    print("Verifying...")
    status, output = verify_function(fn_name, wrapper["file"])

    if status == "match":
        print(f"\n*** {fn_name} MATCHED 100%! ***")
        progress = load_progress()
        if fn_name not in progress["completed"]:
            progress["completed"].append(fn_name)
        save_progress(progress)
        release_lock(fn_name)
        return True
    else:
        print(f"\n{fn_name}: {status}")
        print("Reverting to backup...")
        c_file.write_text(backup.read_text())
        release_lock(fn_name)

        # Save failure info
        progress = load_progress()
        progress["failed"].append({
            "function": fn_name,
            "status": status,
            "backend": backend,
            "time": time.time(),
        })
        save_progress(progress)
        return False


def cmd_run(args):
    """Run decomp agent on functions."""
    ensure_dirs()
    queue = load_queue()
    if not queue:
        print("No queue. Run decomp_scheduler.py --scan first.")
        return

    sys.path.insert(0, str(ROOT / "tools"))

    if args.function:
        process_function(args.function, args.backend)
    else:
        tier = args.tier or "simple"
        count = args.count or 5
        candidates = [w for w in queue
                     if w["tier"] == tier and w["status"] == "pending"]
        candidates.sort(key=lambda w: w["asm_lines"])

        completed_fns = set(load_progress()["completed"])
        candidates = [c for c in candidates if c["function"] not in completed_fns]

        successes = 0
        failures = 0
        for w in candidates[:count]:
            if process_function(w["function"], args.backend):
                successes += 1
            else:
                failures += 1

        print(f"\n{'='*50}")
        print(f"Batch complete: {successes} matched, {failures} failed")
        print(f"{'='*50}")


def main():
    parser = argparse.ArgumentParser(description="Decomp Agent Runner")
    parser.add_argument("--backend", choices=["ollama", "codex", "opencode"],
                       default="ollama", help="Model backend")
    parser.add_argument("--function", metavar="FN", help="Process a specific function")
    parser.add_argument("--tier", choices=["simple", "medium", "complex", "hard"],
                       help="Process functions of this tier")
    parser.add_argument("--count", type=int, default=5, help="Number of functions to process")

    args = parser.parse_args()
    cmd_run(args)


if __name__ == "__main__":
    main()
