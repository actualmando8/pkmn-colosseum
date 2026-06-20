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
CONT_CAP="${CONT_CAP:-8}"           # max CONTINUE turns on one file before rotating
STATE="build/codex_lane_state"; mkdir -p "$STATE"
declare -A SEND=( [C1]=send-codex-safe [C2]=send-codex2-safe [C3]=send-codex3-safe [C4]=send-codex4-safe )
declare -A PANE=( [C1]=$CODEX_PANE [C2]=$CODEX2_PANE [C3]=$CODEX3_PANE [C4]=$CODEX4_PANE )
# a file is COMPLETE for a lane when every target fn is saved in band_wins, OR the lane
# has spent CONT_CAP continue-turns on it (walled/stuck — move on, partial work persists).
file_complete() {  # <stem> <fns...>
  local stem="$1"; shift; local wins="build/band_wins/pl_${stem}.json"
  [ -f "$wins" ] || return 1
  local fn; for fn in "$@"; do grep -q "$fn" "$wins" 2>/dev/null || return 1; done
  return 0
}
echo "[codex_asm_driver] up — lanes:[$LANES] from $QUEUE, interval ${INTERVAL}s, sticky (cap ${CONT_CAP}) from-scratch asm->C"
while true; do
  if [ ! -s "$QUEUE" ]; then echo "[$(date +%H:%M)] ASM queue empty — regenerate build/asm_codex_queue.txt"; sleep "$INTERVAL"; continue; fi
  LOCKS=$(python tools/decomp_work/coordination/locks.py list 2>/dev/null | awk '{print $2}')
  declare -A SNAP IDLE; RUN_PICKED=""
  # Protect EVERY lane's currently-owned file (busy lanes included) so an idle lane
  # can't grab a file another lane is mid-conversion on (would clobber the shared band
  # scratch). Seed the dedup set from all persisted lane state before any assignment.
  for n in $LANES; do
    sf=$(cat "$STATE/$n.file" 2>/dev/null)
    [ -n "$sf" ] && RUN_PICKED="${RUN_PICKED}"$'\n'"${sf}"
  done
  for n in $LANES; do SNAP[$n]=$(tmux capture-pane -p -t "${PANE[$n]}" 2>/dev/null | md5sum); done
  sleep 2
  for n in $LANES; do
    b=$(tmux capture-pane -p -t "${PANE[$n]}" 2>/dev/null | md5sum)
    [ "${SNAP[$n]}" = "$b" ] && IDLE[$n]=1 || IDLE[$n]=0
  done
  for n in $LANES; do
    [ "${IDLE[$n]:-0}" = 1 ] || continue
    curfile=$(cat "$STATE/$n.file" 2>/dev/null); curfns=$(cat "$STATE/$n.fns" 2>/dev/null)
    cnt=$(cat "$STATE/$n.cnt" 2>/dev/null || echo 0)
    # STICKY: stay on the current file (CONTINUE) until all targets saved or cap hit.
    if [ -n "$curfile" ]; then
      stem=$(basename "$curfile" .c); tag="pl_${stem}"
      if file_complete "$stem" $curfns; then
        echo "[$(date +%H:%M)] ASM-ROTATE $n off $stem (all targets saved)"; rm -f "$STATE/$n".{file,fns,cnt}; curfile=""
      elif [ "$cnt" -ge "$CONT_CAP" ]; then
        echo "[$(date +%H:%M)] ASM-ROTATE $n off $stem (cap ${CONT_CAP} reached)"; rm -f "$STATE/$n".{file,fns,cnt}; curfile=""
      else
        cnt=$((cnt+1)); echo "$cnt" > "$STATE/$n.cnt"
        RUN_PICKED="${RUN_PICKED}"$'\n'"${curfile}"
        prompt="CONTINUE converting $curfile (TAG $tag). For EACH target fn below not yet at 100%: refine the band scratch into faithful REAL C with NAMED locals (no rNN register-locals), band.py check; before any WALL run python tools/decomp_work/classify_residual.py $tag <fn>; at 100% band.py save. fns: $curfns. Report SAVED/WALL/SKIP per fn; when ALL are SAVED or conclusively WALL/SKIP say FILE-DONE."
        ./tools/decomp_work/tmux_control/control.sh "${SEND[$n]}" "$prompt" >/dev/null 2>&1
        echo "[$(date +%H:%M)] ASM-CONTINUE $n -> $stem (turn $cnt/$CONT_CAP)"
        continue
      fi
    fi
    # assign a NEW file (lane has none, or just rotated off)
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
    echo "$file" > "$STATE/$n.file"; echo "$fns" > "$STATE/$n.fns"; echo 0 > "$STATE/$n.cnt"
    for fn in $fns; do python tools/decomp_work/wall_ledger.py mark "$fn" "$n/asm" >/dev/null 2>&1; done
    prompt="FROM-SCRATCH asm->C (read docs/CRACK_LEVERS.md). File: $file  TAG: $tag  fns: $fns. These are active asm-wrappers (#if1 asm + #include .inc) needing a real C body. For EACH fn: run python tools/decomp_work/m2c_draft.py <fn> $file, then REWRITE the draft into faithful REAL C with NAMED locals + in-body externs — NEVER leave raw rNN register-locals (they pin the coloring). band.py init/check to measure; before filing any WALL run python tools/decomp_work/classify_residual.py $tag <fn>; at 100% band.py save (integrates by flipping #if1->#if0). NO asm{}/asm void/#include .inc fraud — a win must be real C. Stay on THIS file across turns until all targets are done. Report SAVED/WALL/SKIP per fn; say FILE-DONE when all targets are resolved."
    ./tools/decomp_work/tmux_control/control.sh "${SEND[$n]}" "$prompt" >/dev/null 2>&1
    echo "[$(date +%H:%M)] ASM-DISPATCH $n -> $stem (${fns%% *} +$(($(echo $fns | wc -w)-1)) more)"
  done
  unset SNAP IDLE
  sleep "$INTERVAL"
done
