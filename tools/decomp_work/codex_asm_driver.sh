#!/bin/bash
# codex_asm_driver.sh — dedicated Codex-lane driver for FROM-SCRATCH asm->C decomp of
# the largest active asm-wrappers (build/asm_codex_queue.txt, built from
# active_asm_targets.py). Runs ALONGSIDE the main fleet_driver, which keeps Opus+Sonnet
# on the near-miss/LOW buckets. Codex xhigh reasoning is spent here (higher value than
# LOW near-misses) because each win is a real asm->C conversion that advances the
# decompiled-to-C metric. Per-file lock aware (shares coordination/locks.py with the
# main driver); marks attempted in the wall ledger. Codex lanes only.
cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../.." || exit 1
source tools/decomp_work/tmux_control/panes.env 2>/dev/null
export MSYS_NO_PATHCONV=1
INTERVAL="${INTERVAL:-45}"
LANES="${CODEX_ASM_LANES:-C1 C2 C3 C4}"
QUEUE="build/asm_codex_queue.txt"
declare -A SEND=( [C1]=send-codex-safe [C2]=send-codex2-safe [C3]=send-codex3-safe [C4]=send-codex4-safe )
declare -A PANE=( [C1]=$CODEX_PANE [C2]=$CODEX2_PANE [C3]=$CODEX3_PANE [C4]=$CODEX4_PANE )
echo "[codex_asm_driver] up — lanes:[$LANES] from $QUEUE, interval ${INTERVAL}s (from-scratch asm->C)"
while true; do
  if [ ! -s "$QUEUE" ]; then echo "[$(date +%H:%M)] ASM queue empty — regenerate build/asm_codex_queue.txt"; sleep "$INTERVAL"; continue; fi
  LOCKS=$(python tools/decomp_work/coordination/locks.py list 2>/dev/null | awk '{print $2}')
  declare -A SNAP IDLE; RUN_PICKED=""
  for n in $LANES; do SNAP[$n]=$(tmux capture-pane -p -t "${PANE[$n]}" 2>/dev/null | md5sum); done
  sleep 2
  for n in $LANES; do
    b=$(tmux capture-pane -p -t "${PANE[$n]}" 2>/dev/null | md5sum)
    [ "${SNAP[$n]}" = "$b" ] && IDLE[$n]=1 || IDLE[$n]=0
  done
  for n in $LANES; do
    [ "${IDLE[$n]:-0}" = 1 ] || continue
    line=""
    while IFS= read -r l; do
      [ -n "$l" ] || continue
      f=$(echo "$l" | awk '{print $1}')
      echo "$LOCKS" | grep -qxF "$f" && continue
      printf '%s\n' "$RUN_PICKED" | grep -qxF "$f" && continue
      line="$l"; break
    done < "$QUEUE"
    [ -n "$line" ] || { echo "[$(date +%H:%M)] ASM-QUEUE: $n idle, all files locked/taken"; continue; }
    file=$(echo "$line" | awk '{print $1}'); fns=$(echo "$line" | cut -d' ' -f2-)
    stem=$(basename "$file" .c); tag="pl_${stem}"
    RUN_PICKED="${RUN_PICKED}"$'\n'"${file}"
    for fn in $fns; do python tools/decomp_work/wall_ledger.py mark "$fn" "$n/asm" >/dev/null 2>&1; done
    prompt="FROM-SCRATCH asm->C (read docs/CRACK_LEVERS.md). File: $file  TAG: $tag  fns: $fns. These are active asm-wrappers (#if1 asm + #include .inc) needing a real C body. For EACH fn: run python tools/decomp_work/m2c_draft.py <fn> $file, then REWRITE the draft into faithful REAL C with NAMED locals + in-body externs — NEVER leave raw rNN register-locals (they pin the coloring). band.py init/check to measure; before filing any WALL run python tools/decomp_work/classify_residual.py $tag <fn>; at 100% band.py save (integrates by flipping #if1->#if0). NO asm{}/asm void/#include .inc fraud — a win must be real C. Report SAVED <fn> 100.00 / WALL <fn> <%> / SKIP <fn> <reason> per fn."
    ./tools/decomp_work/tmux_control/control.sh "${SEND[$n]}" "$prompt" >/dev/null 2>&1
    echo "[$(date +%H:%M)] ASM-DISPATCH $n -> $stem (${fns%% *} +$(($(echo $fns | wc -w)-1)) more)"
  done
  unset SNAP IDLE
  sleep "$INTERVAL"
done
