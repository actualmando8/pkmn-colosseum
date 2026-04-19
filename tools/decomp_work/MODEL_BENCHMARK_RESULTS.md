# Local model benchmark — fn_80115C48 (2026-04-18)

**Target function**: `fn_80115C48` in `gs_field_world.c` — a known-tractable table-walk with mtctr/bdnz pattern. Human Opus hits 100%.

**Prompt**: assembled via `tools/decomp_work/build_prompt.py gs_field_world fn_80115C48` (12KB prompt containing CW_QUIRKS.md + pattern detector output + existing externs + raw asm).

**Hardware**: RTX 3090 (24GB), ollama.

## Results

| Model | Size | Result | Notes |
|---|---|---|---|
| **qwen2.5-coder:32b** | 19GB | **95.0% ✓** | Clean C, followed prompt format, all externs correct. Missed 5% because it hoisted `count` out of the for-init (suppressing mtctr/bdnz). |
| codestral:22b | 12GB | ✗ | Invented a format string for fn_800DD970; wrong struct stride math (`index*6 + 15` makes no sense); tagged code block "python" instead of "c". |
| deepseek-coder-v2:16b | 8.9GB | ✗ | Emitted inline PPC assembly mixed into C. Fundamentally misunderstood the task. |
| deepseek-r1:14b | 9GB | ✗ | Only placeholder stubs (`// process something`, `// do something`). Never produced complete code. |
| qwen2.5-coder:7b | 4.7GB | ✗ | Literal `lbl_COUNT` in code (didn't substitute from prompt); wrong offsets. |

## Recommendation

**For byte-match decomp on RTX 3090: use `qwen2.5-coder:32b` only.** No other local model on the 3090 produced usable output on a "known-tractable" function.

Next steps for improvement:
1. **Teach qwen2.5-coder:32b the for-init inlining rule better** — maybe add a stronger hint in CW_QUIRKS.md about "NEVER hoist count into local — keep in for()".
2. **Try Ollama with lower temperature** (ollama run -temperature 0.1) for more deterministic output.
3. **Try 5-shot prompting**: include 2-3 already-matched examples (fn_80115C48 100% + fn_8001501C 100%) as reference before the target function.

## Recommended pipeline

```bash
# Generate prompt
python3 tools/decomp_work/build_prompt.py <stem> <fn> > /tmp/prompt.md
scp /tmp/prompt.md douglaswhittingham@10.0.0.3:/tmp/prompt.md

# Run model (remove ANSI escape codes)
ssh douglaswhittingham@10.0.0.3 "cat /tmp/prompt.md | ollama run qwen2.5-coder:32b 2>&1 | sed 's/\x1b\[[0-9;?]*[a-zA-Z]//g' | tr -d '\r'" > /tmp/reply.txt

# Extract the C block
awk '/```c|```$/{p=!p;next}p' /tmp/reply.txt > /tmp/reply.c

# Paste into src/game/<stem>.c #else block, compile, measure
```

Time per function: ~60-90 seconds for qwen2.5-coder:32b on 3090 (one-time model load + ~30s generation).

## Broader implications

The **pipeline works** — with the right prompt (CW_QUIRKS + detector hints + externs + asm), a local model produces usable output. This unblocks parallel throughput without Opus tokens.

**Workflow going forward:**
1. Claude Opus (this session) → picks candidate + verifies/merges
2. qwen2.5-coder:32b on 3090 → drafts C from the built prompt (free, ~60s)
3. Claude Opus → reads draft, fixes for-init/pragma issues, runs match, commits or reverts

This is dramatically cheaper than "Opus writes everything from scratch" and more reliable than "Codex with minimal context".
