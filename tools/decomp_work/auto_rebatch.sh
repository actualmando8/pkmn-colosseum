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
declare -A SEND=( [C1]=send-codex-safe [C2]=send-codex2-safe [C3]=send-codex3-safe [C4]=send-codex4-safe [C5]=send-codex5-safe [C6]=send-codex6-safe [C7]=send-codex7-safe [C8]=send-codex8-safe [SON]=send-sonnet-safe [OPUS]=send-worker-safe )
declare -A PANE=( [C1]=$CODEX_PANE [C2]=$CODEX2_PANE [C3]=$CODEX3_PANE [C4]=$CODEX4_PANE [C5]=$CODEX5_PANE [C6]=$CODEX6_PANE [C7]=$CODEX7_PANE [C8]=$CODEX8_PANE [SON]=$SONNET_PANE [OPUS]=$WORKER_PANE )
is_idle() { local a b; a=$(tmux capture-pane -p -t "$1" 2>/dev/null | md5sum); sleep 2; b=$(tmux capture-pane -p -t "$1" 2>/dev/null | md5sum); [ "$a" = "$b" ]; }
locked_files() { python tools/decomp_work/coordination/locks.py list 2>/dev/null | awk '{print $2}'; }

# Files already handed out earlier in THIS run (across BOTH queues) — guarantees two
# lanes never target the same TU in a single dispatch pass (the band harness locks
# per-FILE, so same-file = collision).
RUN_PICKED=""
# pick_line <queue> <assigned-file> -> echoes the first line whose file is free+unlocked
pick_line() {
  local queue="$1" assigned="$2" line f
  touch "$assigned"
  while IFS= read -r line; do
    [ -n "$line" ] || continue
    f=$(echo "$line" | awk '{print $1}')
    grep -qxF "$f" "$assigned" && continue
    echo "$LOCKS" | grep -qxF "$f" && continue
    printf '%s\n' "$RUN_PICKED" | grep -qxF "$f" && continue   # picked by another lane this run
    echo "$line"; return 0
  done < "$queue"
  return 1
}

for name in ${ASM_LANES:-OPUS SON C1 C2 C3 C4 C5 C6 C7 C8}; do
  is_idle "${PANE[$name]}" || continue
  LOCKS=$(locked_files)
  mode=crack; line=$(pick_line build/wall_queue.txt build/wall_assigned.txt)
  if [ -z "$line" ]; then mode=scratch; line=$(pick_line build/asm_queue.txt build/asm_assigned.txt); fi
  if [ -z "$line" ]; then echo "QUEUE-EXHAUSTED — $name idle, no free unlocked target (both queues)"; continue; fi
  file=$(echo "$line" | awk '{print $1}'); fns=$(echo "$line" | cut -d' ' -f2-)
  stem=$(basename "$file" .c); tag="pl_${stem}"
  RUN_PICKED="${RUN_PICKED}"$'\n'"${file}"   # claim it for this run so no other lane picks it
  if [ "$mode" = crack ]; then echo "$file" >> build/wall_assigned.txt; else echo "$file" >> build/asm_assigned.txt; fi
  for fn in $fns; do python tools/decomp_work/wall_ledger.py mark "$fn" "$name/$mode" >/dev/null 2>&1; done
  ff=""; case "$name" in C1|C2|C3|C4) ff="If a fn resists after ~3 lever attempts, report WALL <fn> <%> with the residual diff and move on.";; esac
  if [ "$mode" = crack ]; then
    prompt="CRACK (levers: read docs/CRACK_LEVERS.md). File: $file  TAG: $tag  fns: $fns. These are real C in canon <100% — band.py diff to size the miss (small=reg-alloc lever; large=m2c_draft reshape), fix scratch, band.py check, save at 100. Real C only (no asm/.inc). $ff SAVED <fn> 100.00 / WALL <fn> <%>."
  else
    prompt="FROM-SCRATCH asm->C (levers: docs/CRACK_LEVERS.md). File: $file  TAG: $tag  fns: $fns. m2c_draft each, write faithful REAL C (in-body externs), band.py check, save at 100. No asm/.inc fraud. $ff SAVED/WALL/SKIP per fn."
  fi
  ./tools/decomp_work/tmux_control/control.sh "${SEND[$name]}" "$prompt" >/dev/null 2>&1
  echo "REBATCH $name [$mode] -> $stem [$fns]"
done
