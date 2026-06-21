#!/usr/bin/env bash
# pane_io.sh — the SOLE owner of psmux. The ONLY process that ever runs a tmux
# client during autonomous operation. Single-threaded; every tmux call goes through
# txk (SIGKILL-bounded + serialized), so a wedged client dies in <=TXK_T seconds and
# the loop survives — nothing can pile up or block the machine.
#
# Each pass:
#   1. capture every agent pane -> build/hb/<NAME>.live, classify -> build/hb/<NAME>.state
#        idle  = screen byte-static across the pass AND no "esc to interrupt" AND not rate-limited
#        busy  = changing, or showing the interrupt indicator
#        rate  = rate-limit / usage-cap text on screen (idle but cannot work)
#        capfail = capture timed out / empty (treat as not-dispatchable)
#   2. drain build/dispatch/<NAME>.req -> send-keys the prompt + Enter, archive the req,
#      and force that pane's state to busy so the driver won't double-send.
#   writes build/hb/.alive (epoch) each pass so the driver can detect a dead owner.
#
# Modes:  --once       one pass then exit (testing)
#         --no-send    capture/classify only, never send (dry run against a live cockpit)
#         INTERVAL=N   seconds between passes (default 8)
set -uo pipefail
cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../.." || exit 1
export MSYS_NO_PATHCONV=1
source tools/decomp_work/tmux_control/txk.sh
source tools/decomp_work/tmux_control/panes.env 2>/dev/null

ONCE=0; NOSEND=0
for a in "$@"; do case "$a" in --once) ONCE=1;; --no-send) NOSEND=1;; esac; done
INTERVAL="${INTERVAL:-8}"
HB=build/hb; REQ=build/dispatch; mkdir -p "$HB" "$REQ" "$REQ/sent"

PIDF="build/.pane_io.pid"
if [ -f "$PIDF" ] && kill -0 "$(cat "$PIDF" 2>/dev/null)" 2>/dev/null; then
  echo "[pane_io] another instance ($(cat "$PIDF")) already running — exiting"; exit 0
fi
echo $$ > "$PIDF"; trap 'rm -f "$PIDF"' EXIT

# NAME -> pane id. Matches the names auto_rebatch dispatches to, so a dispatch req
# named build/dispatch/<NAME>.req lands on the right pane. Empty entries are skipped.
declare -A PANE=(
  [OPUS]="${WORKER_PANE:-}" [SON]="${SONNET_PANE:-}" [OPUS3]="${CODEX3_PANE:-}"
  [GLM]="${GLM_PANE:-}"
  [C1]="${CODEX_PANE:-}" [C2]="${CODEX2_PANE:-}" [C3]="${CODEX3_PANE:-}" [C4]="${CODEX4_PANE:-}"
  [C5]="${CODEX5_PANE:-}" [C6]="${CODEX6_PANE:-}" [C7]="${CODEX7_PANE:-}" [C8]="${CODEX8_PANE:-}"
)

classify() {  # classify <name> <pane> ; echoes idle|busy|rate|capfail and updates .prev
  local name="$1" pane="$2" cap rc
  cap=$(txk capture-pane -p -t "$pane" 2>/dev/null); rc=$?
  if [ $rc -ne 0 ] || [ -z "$cap" ]; then echo capfail; return; fi
  printf '%s' "$cap" > "$HB/$name.live"
  if printf '%s' "$cap" | tr -d ' ' | grep -qiE "esctoint"; then
    printf '%s' "$cap" > "$HB/$name.prev"; echo busy; return
  fi
  if printf '%s' "$cap" | grep -qiE "rate.?limit|usage limit|limit reached|too many request|try again (in|at|later)|resets? (at|in)|reached your|429 "; then
    printf '%s' "$cap" > "$HB/$name.prev"; echo rate; return
  fi
  local prev=""; [ -f "$HB/$name.prev" ] && prev=$(cat "$HB/$name.prev")
  printf '%s' "$cap" > "$HB/$name.prev"
  if [ "$cap" = "$prev" ]; then echo idle; else echo busy; fi
}

drain_one() {  # drain_one <name> <pane> : send a queued prompt if present
  local name="$1" pane="$2" req="$REQ/$name.req" prompt
  [ -f "$req" ] || return 0
  prompt=$(cat "$req")
  [ -z "$prompt" ] && { rm -f "$req"; return 0; }
  if [ "$NOSEND" = 1 ]; then echo "[pane_io] (--no-send) would dispatch -> $name"; return 0; fi
  # ATOMIC paste: a long `send-keys -l` floods the native-PE psmux console and drops chars
  # under the burst — only a corrupted fragment of the prompt reaches the agent (observed
  # 2026-06-20). load-buffer+paste-buffer injects the whole prompt in ONE operation, no
  # flood. Native psmux needs a WINDOWS path for the file (cygpath; MSYS /-paths fail with
  # "system cannot find the file"). The req has no trailing newline so paste won't submit;
  # send-keys Enter does. -d frees the buffer after paste.
  local win; win=$(cygpath -w "$req" 2>/dev/null)
  txk load-buffer -b "ds_$name" "$win"
  txk paste-buffer -d -b "ds_$name" -t "$pane"
  sleep 0.3
  txk send-keys -t "$pane" Enter
  mv -f "$req" "$REQ/sent/$name.$(date +%s)" 2>/dev/null || rm -f "$req"
  # force busy so the next driver tick won't re-dispatch before the TUI repaints
  echo "busy $(date +%s)" > "$HB/$name.state"; rm -f "$HB/$name.prev"
  echo "[pane_io] dispatched -> $name"
}

pass() {
  local name pane st
  echo "$(date +%s)" > "$HB/.alive"
  for name in "${!PANE[@]}"; do
    pane="${PANE[$name]}"; [ -n "$pane" ] || continue
    st=$(classify "$name" "$pane")
    echo "$st $(date +%s)" > "$HB/$name.state"
    echo "$(date +%s)" > "$HB/.alive"      # keep heartbeat fresh during the capture loop
  done
  for name in "${!PANE[@]}"; do
    pane="${PANE[$name]}"; [ -n "$pane" ] || continue
    drain_one "$name" "$pane"
    echo "$(date +%s)" > "$HB/.alive"      # ...and during (slower) dispatch drains
  done
}

echo "[pane_io] up — sole tmux owner, interval ${INTERVAL}s, txk_kill=${TXK_T}s$([ "$NOSEND" = 1 ] && echo ' [NO-SEND]')"
if [ "$ONCE" = 1 ]; then pass; exit 0; fi
while true; do pass; sleep "$INTERVAL"; done
