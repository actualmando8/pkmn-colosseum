#!/bin/bash
# auto_rebatch.sh — autonomous lane refill, LOCK-AWARE, DUAL-QUEUE, TMUX-FREE.
#
# 2026-06-20 rearchitecture: this script no longer touches psmux at all. It reads
# idle state from build/hb/<NAME>.state (written by pane_io.sh, the sole tmux owner)
# and dispatches by writing build/dispatch/<NAME>.req (drained + sent by pane_io).
# That removes the 4-deep nested-bash pipeline that drove native-PE tmux clients and
# wedged the loop. The driver can no longer hang on a tmux call.
#
# Feeds idle lanes from two queues, highest-value first:
#   1. build/wall_queue.txt  — NEARWALL fns (real C @95-99.95%): CRACK with CW levers.
#   2. build/asm_queue.txt   — active asm-wrappers: from-scratch decomp.
# Each lane gets a DISTINCT file that is neither already-assigned nor band-locked.
cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../.." || exit 1
export MSYS_NO_PATHCONV=1
HB=build/hb; REQ=build/dispatch; mkdir -p "$HB" "$REQ"

LANES="${ASM_LANES:-OPUS SON C1 C2}"
FRESH="${HB_FRESH:-45}"   # a state/alive file older than this many seconds is not trusted
# Dispatch cooldown: after a lane is tasked, do not re-task it for this many seconds even if
# it reads idle. A freshly-prompted agent briefly looks idle while it starts (one static
# pane_io pass, no "esc to interrupt"); without this it gets re-prompted before finishing its
# first assignment. An actively-working agent reads busy well within this window, so the only
# cost is a lane that genuinely finished fast waiting a bit for its next packet.
COOLDOWN="${DISPATCH_COOLDOWN:-150}"

# The tmux owner must be alive and recently active, else states are stale and reqs we
# write would never be sent. Refuse to dispatch — safer than queuing into the void.
now=$(date +%s)
alive=$(cat "$HB/.alive" 2>/dev/null || echo 0)
if [ $(( now - ${alive:-0} )) -gt "$FRESH" ]; then
  echo "PANE-IO-DOWN (alive=${alive}, now=${now}) — not dispatching this cycle"; exit 0
fi

locked_files() { timeout 15 python tools/decomp_work/coordination/locks.py list 2>/dev/null | awk '{print $2}'; }

# idle = state file says idle, is fresh, AND no dispatch req already pending for it
declare -A IDLE
for name in $LANES; do
  IDLE[$name]=0
  [ -f "$REQ/$name.req" ] && continue                       # a prompt is already queued/unsent
  # cooldown: skip a lane that was tasked within the last COOLDOWN seconds (its .task mtime is
  # the last-dispatch time) so we never re-prompt an agent still starting/working its packet.
  tf="$REQ/$name.task"
  if [ -f "$tf" ]; then
    tmt=$(stat -c %Y "$tf" 2>/dev/null || echo 0)
    [ $(( now - ${tmt:-0} )) -lt "$COOLDOWN" ] && continue
  fi
  read -r st ts _ < "$HB/$name.state" 2>/dev/null || continue
  [ "$st" = idle ] || continue
  [ $(( now - ${ts:-0} )) -le "$FRESH" ] && IDLE[$name]=1
done

LOCKS=$(locked_files)
# Spawn-tax-immune contention guard: files already owned by another busy/pending lane.
# locks.py (python+sqlite) is slow / times out under the MSYS process-spawn tax, so its
# list silently empties and two lanes draw the same file. This pure-file check is the
# reliable backstop: derive each lane's file from its pending .req or active .task
# ("File: <path>"), count it taken only while that lane is busy or has an unsent req
# (an idle lane's last file is finished and free to reassign).
BUSY_FILES=""
for _o in $LANES; do
  _of=""
  for _f in "$REQ/$_o.req" "$REQ/$_o.task"; do
    [ -f "$_f" ] || continue
    _of=$(sed -n 's/.*File: \([^ ]*\).*/\1/p' "$_f" | head -1); [ -n "$_of" ] && break
  done
  [ -n "$_of" ] || continue
  read -r _ost _ < "$HB/$_o.state" 2>/dev/null || _ost=""
  if [ "$_ost" = busy ] || [ -f "$REQ/$_o.req" ]; then BUSY_FILES="${BUSY_FILES}"$'\n'"${_of}"; fi
done
RUN_PICKED=""
pick_line() {  # pick_line <queue> -> first line whose file is free + unlocked + unowned + unpicked-this-run
  local queue="$1" line f
  [ -f "$queue" ] || return 1
  while IFS= read -r line; do
    [ -n "$line" ] || continue
    f=$(echo "$line" | awk '{print $1}')
    echo "$LOCKS" | grep -qxF "$f" && continue
    printf '%s\n' "$BUSY_FILES" | grep -qxF "$f" && continue   # owned by another busy/pending lane
    printf '%s\n' "$RUN_PICKED" | grep -qxF "$f" && continue
    echo "$line"; return 0
  done < "$queue"
  return 1
}

for name in $LANES; do
  [ "${IDLE[$name]:-0}" = 1 ] || continue
  # Mode order: crack (wall_queue, real-C near-misses) first by default. When build/.scratch_first
  # exists, do from-scratch ASM (asm_queue) FIRST instead — set when the near-miss crack pool is
  # mined out and the fleet should grind asm-wrappers into C. Either mode falls back to the other
  # when its queue is empty, so a lane is never left idle while work of any kind remains.
  if [ -f build/.scratch_first ]; then
    mode=scratch; line=$(pick_line build/asm_queue.txt)
    [ -z "$line" ] && { mode=crack; line=$(pick_line build/wall_queue.txt); }
  else
    mode=crack; line=$(pick_line build/wall_queue.txt)
    [ -z "$line" ] && { mode=scratch; line=$(pick_line build/asm_queue.txt); }
  fi
  if [ -z "$line" ]; then echo "QUEUE-EXHAUSTED — $name idle, no free unlocked target"; continue; fi
  file=$(echo "$line" | awk '{print $1}'); fns=$(echo "$line" | cut -d' ' -f2-)
  stem=$(basename "$file" .c); tag="pl_${stem}"
  RUN_PICKED="${RUN_PICKED}"$'\n'"${file}"
  for fn in $fns; do timeout 10 python tools/decomp_work/wall_ledger.py mark "$fn" "$name/$mode" >/dev/null 2>&1; done
  gate="BEFORE any WALL you MUST run: python tools/decomp_work/classify_residual.py $tag <fn>. If it prints REG-COLORING (exit 0) you may NOT wall it — rewrite with NAMED locals (never raw rNN locals, they pin the coloring) + declaration-order lever until 100. Only RELOC/SCHEDULING/SHAPE verdicts may WALL/REWORK."
  ff=""; case "$name" in C1|C2|C3|C4) ff="If a fn still resists after the classifier says REG-COLORING and ~4 decl-order attempts, leave it in scratch and report WALL <fn> <%> + the classifier verdict, then move on.";; esac
  if [ "$mode" = crack ]; then
    prompt="CRACK (levers: read docs/CRACK_LEVERS.md). File: $file  TAG: $tag  fns: $fns. These are real C in canon <100%. For EACH fn: band.py diff, then classify_residual.py to pick the fix (REG-COLORING=named-locals+decl-order; SHAPE=m2c_draft reshape). Fix scratch, band.py check, save at 100. Real C only (no asm/.inc/rNN-locals). KG (shared lever memory): BEFORE each fn run python tools/decomp_work/kg/kg.py q lever-targets <fn> for proven levers; AFTER each save run python tools/decomp_work/kg/kg.py record-crack <fn> <lever-slug> (and python tools/decomp_work/kg/kg.py record-lever <slug> --title \"...\" --desc \"...\" if you found a NEW lever) so every lane reuses it. $gate $ff SAVED <fn> 100.00 / WALL <fn> <%>."
  else
    prompt="FROM-SCRATCH asm->C (levers: docs/CRACK_LEVERS.md). File: $file  TAG: $tag  fns: $fns. m2c_draft each, then REWRITE into faithful REAL C with NAMED locals (in-body externs) — no raw rNN register-locals. band.py check, save at 100. No asm/.inc fraud. KG (shared lever memory): BEFORE each fn run python tools/decomp_work/kg/kg.py q lever-targets <fn> for proven levers; AFTER each save run python tools/decomp_work/kg/kg.py record-crack <fn> <lever-slug> (and python tools/decomp_work/kg/kg.py record-lever <slug> --title \"...\" --desc \"...\" if you found a NEW lever) so every lane reuses it. $gate $ff SAVED/WALL/SKIP per fn."
  fi
  printf '%s' "$prompt" > "$REQ/$name.req"        # pane_io sends this; never touch tmux here
  echo "REBATCH $name [$mode] -> $stem [$fns]"
done
