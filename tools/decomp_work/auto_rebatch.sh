#!/bin/bash
# auto_rebatch.sh — overnight autonomous lane refill, LOCK-AWARE, DUAL-QUEUE.
# Feeds idle lanes from two queues, highest-value first:
#   1. build/wall_queue.txt  — NEARWALL fns (real C @95-99.95%, reg-alloc/scheduler
#      walls). CRACK them with CW levers; these are 1-3 instruction diffs = winnable.
#   2. build/asm_queue.txt   — active asm-wrappers (undecompiled). From-scratch decomp.
# Idle = capture-diff (pane byte-identical over 2s). Each lane gets a DISTINCT file
# that is neither already-assigned nor band-locked (the harness locks per FILE).
# Marks assigned fns attempted in the ledger so coverage is tracked. GLM untouched.
cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../.." || exit 1
source tools/decomp_work/tmux_control/panes.env 2>/dev/null
export MSYS_NO_PATHCONV=1
# NOTE: SON pane (%8) and OPUS2/OPUS3 are now OPUS workers (Sonnet retired 2026-06-20).
# OPUS3 reuses the freed C3 pane (%9) via its send-codex3-safe sender (model-agnostic).
declare -A SEND=( [C1]=send-codex-safe [C2]=send-codex2-safe [C3]=send-codex3-safe [C4]=send-codex4-safe [C5]=send-codex5-safe [C6]=send-codex6-safe [C7]=send-codex7-safe [C8]=send-codex8-safe [SON]=send-sonnet-safe [OPUS]=send-worker-safe [OPUS3]=send-codex3-safe [GLM]=send-glm-safe )
declare -A PANE=( [C1]=$CODEX_PANE [C2]=$CODEX2_PANE [C3]=$CODEX3_PANE [C4]=$CODEX4_PANE [C5]=$CODEX5_PANE [C6]=$CODEX6_PANE [C7]=$CODEX7_PANE [C8]=$CODEX8_PANE [SON]=$SONNET_PANE [OPUS]=$WORKER_PANE [OPUS3]=$CODEX3_PANE [GLM]=$GLM_PANE )
is_idle() { local a b; a=$(tmux capture-pane -p -t "$1" 2>/dev/null | md5sum); sleep 2; b=$(tmux capture-pane -p -t "$1" 2>/dev/null | md5sum); [ "$a" = "$b" ]; }
locked_files() { python tools/decomp_work/coordination/locks.py list 2>/dev/null | awk '{print $2}'; }

# Files already handed out earlier in THIS run (across BOTH queues) — guarantees two
# lanes never target the same TU in a single dispatch pass (the band harness locks
# per-FILE, so same-file = collision).
RUN_PICKED=""
# pick_line <queue> <assigned-file> -> echoes the first line whose file is free+unlocked
pick_line() {
  local queue="$1" line f
  # NOTE: no permanent assigned-list — a file is "taken" only while a lane holds its
  # band LOCK (checked below). Once a lane finishes (lock released) the file is free
  # to re-offer, so lanes never starve. Per-fn dedup is handled by the ledger (the
  # queue is regenerated each cycle from fresh-unattempted fns).
  while IFS= read -r line; do
    [ -n "$line" ] || continue
    f=$(echo "$line" | awk '{print $1}')
    echo "$LOCKS" | grep -qxF "$f" && continue          # currently locked by a live lane
    printf '%s\n' "$RUN_PICKED" | grep -qxF "$f" && continue   # picked by another lane this run
    echo "$line"; return 0
  done < "$queue"
  return 1
}

# GLM omitted (weekly cap). Codex trimmed to just C1-C2 (2026-06-19 ~13:40) to conserve
# the last ~5% of Codex usage until reset (~15:34); C3-C8 finish their current batch
# then go idle (driver stops feeding them). Re-add C3-C8 at reset via ASM_LANES default.
LANES="${ASM_LANES:-OPUS SON C1 C2}"
# BATCHED idle detection: snapshot every lane, wait ONCE, re-snapshot. A pane that
# is byte-identical across the 2s window is idle. Doing all lanes in one 2s window
# (instead of 2s sequentially per lane = ~20s) makes rebatch near-instant so
# fast-finishing lanes don't sit idle between cycles.
declare -A SNAP IDLE
for name in $LANES; do SNAP[$name]=$(tmux capture-pane -p -t "${PANE[$name]}" 2>/dev/null | md5sum); done
sleep 2
for name in $LANES; do
  cap=$(tmux capture-pane -p -t "${PANE[$name]}" 2>/dev/null)
  # A lane RUNNING a turn shows "esc to interrupt" (both Claude & Codex TUIs). Never treat
  # such a pane as idle even if its screen was briefly static (slow compile / thinking) —
  # that false-positive made the driver fire a 2nd prompt onto an already-working lane.
  # Idle = NO interrupt indicator AND byte-static over the 2s window.
  if echo "$cap" | tr -d ' ' | grep -qiE "esctoint"; then IDLE[$name]=0; continue; fi
  # rate-limited / usage-capped pane: idle but can't work — don't dispatch (dead retries)
  if echo "$cap" | grep -qiE "rate.?limit|usage limit|limit reached|too many request|try again (in|at|later)|resets? (at|in)|reached your|429 "; then
    echo "RATE-LIMITED $name — skipping"; IDLE[$name]=0; continue
  fi
  [ "${SNAP[$name]}" = "$(echo "$cap" | md5sum)" ] && IDLE[$name]=1
done
LOCKS=$(locked_files)
for name in $LANES; do
  [ "${IDLE[$name]:-0}" = 1 ] || continue
  mode=crack; line=$(pick_line build/wall_queue.txt build/wall_assigned.txt)
  if [ -z "$line" ]; then mode=scratch; line=$(pick_line build/asm_queue.txt build/asm_assigned.txt); fi
  if [ -z "$line" ]; then echo "QUEUE-EXHAUSTED — $name idle, no free unlocked target (both queues)"; continue; fi
  file=$(echo "$line" | awk '{print $1}'); fns=$(echo "$line" | cut -d' ' -f2-)
  stem=$(basename "$file" .c); tag="pl_${stem}"
  RUN_PICKED="${RUN_PICKED}"$'\n'"${file}"   # claim it for this run so no other lane picks it
  if [ "$mode" = crack ]; then echo "$file" >> build/wall_assigned.txt; else echo "$file" >> build/asm_assigned.txt; fi
  for fn in $fns; do python tools/decomp_work/wall_ledger.py mark "$fn" "$name/$mode" >/dev/null 2>&1; done
  # MANDATORY WALL GATE: a fn may only be filed WALL after classify_residual.py says
  # so. If it exits 0 (REG-COLORING) the residual is winnable (named locals + decl-order)
  # and walling it is forbidden — this is what stalled fn_80200E00 at 97.92%.
  gate="BEFORE any WALL you MUST run: python tools/decomp_work/classify_residual.py $tag <fn>. If it prints REG-COLORING (exit 0) you may NOT wall it — rewrite with NAMED locals (never raw rNN locals, they pin the coloring) + declaration-order lever until 100. Only RELOC/SCHEDULING/SHAPE verdicts may WALL/REWORK."
  ff=""; case "$name" in C1|C2|C3|C4) ff="If a fn still resists after the classifier says REG-COLORING and ~4 decl-order attempts, leave it in scratch and report WALL <fn> <%> + the classifier verdict, then move on.";; esac
  if [ "$mode" = crack ]; then
    prompt="CRACK (levers: read docs/CRACK_LEVERS.md). File: $file  TAG: $tag  fns: $fns. These are real C in canon <100%. For EACH fn: band.py diff, then classify_residual.py to pick the fix (REG-COLORING=named-locals+decl-order; SHAPE=m2c_draft reshape). Fix scratch, band.py check, save at 100. Real C only (no asm/.inc/rNN-locals). KG (shared lever memory): kg.py q lever-targets <fn> BEFORE each fn for proven levers; kg.py record-crack <fn> <lever-slug> AFTER each save (record-lever <slug> --title if NEW) so every lane reuses it. $gate $ff SAVED <fn> 100.00 / WALL <fn> <%>."
  else
    prompt="FROM-SCRATCH asm->C (levers: docs/CRACK_LEVERS.md). File: $file  TAG: $tag  fns: $fns. m2c_draft each, then REWRITE into faithful REAL C with NAMED locals (in-body externs) — do NOT leave raw rNN register-locals, they pin the coloring. band.py check, save at 100. No asm/.inc fraud. $gate $ff SAVED/WALL/SKIP per fn."
  fi
  ./tools/decomp_work/tmux_control/control.sh "${SEND[$name]}" "$prompt" >/dev/null 2>&1
  echo "REBATCH $name [$mode] -> $stem [$fns]"
done
