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
  # Codex prints a benign welcome line "You have N usage limit reset available. Run /usage"
  # at startup — NOT a rate-limit. Strip it before the rate check so codex lanes aren't
  # falsely gated as rate-limited and skipped by the dispatcher (they'd sit idle forever,
  # never getting a task to scroll the banner off-screen).
  if printf '%s' "$cap" | grep -ivE "usage limit reset|run /usage" | grep -qiE "rate.?limit|usage limit|limit reached|too many request|try again (in|at|later)|resets? (at|in)|reached your|429 "; then
    printf '%s' "$cap" > "$HB/$name.prev"; echo rate; return
  fi
  local prev=""; [ -f "$HB/$name.prev" ] && prev=$(cat "$HB/$name.prev")
  printf '%s' "$cap" > "$HB/$name.prev"
  if [ "$cap" = "$prev" ]; then echo idle; else echo busy; fi
}

drain_one() {  # drain_one <name> <pane> : point the agent at a task FILE (no long-prompt send)
  local name="$1" pane="$2" req="$REQ/$name.req" task="$REQ/$name.task"
  [ -f "$req" ] || return 0
  [ -s "$req" ] || { rm -f "$req"; return 0; }
  if [ "$NOSEND" = 1 ]; then echo "[pane_io] (--no-send) would dispatch -> $name"; return 0; fi
  # Long-prompt delivery on native psmux is fundamentally unreliable under load:
  #   - send-keys -l truncates (drops the head, leaving "...AVED <fn> <%>" fragments).
  #   - paste-buffer -t is ignored (hits the active pane) AND truncates when the loaded
  #     server makes the paste slower than the SIGKILL timeout.
  # So DON'T send the prompt at all: stage the full task in a file and send a SHORT, reliable
  # command (~50 chars, well under any truncation threshold) telling the agent to read it.
  mv -f "$req" "$task" 2>/dev/null || { cp -f "$req" "$task" && rm -f "$req"; }
  local cmd="Read build/dispatch/$name.task and do exactly what it says."
  _txk_acquire
  timeout -s KILL 10 "$TMUX_BIN" send-keys -t "$pane" C-u 2>/dev/null            # clear stale/truncated input
  # /clear the agent's context BEFORE each new task so every assignment starts fresh (no
  # carryover bloat across tasks). The KG (tools/decomp_work/kg/kg.db) is the durable shared
  # memory, so cracked levers survive the clear; the band scratch on disk is resume-safe too.
  # Only reached when the lane is idle (cooldown + idle-gated by auto_rebatch), so this never
  # wipes an in-progress task. Set NO_CLEAR_ON_DISPATCH=1 to skip.
  if [ "${NO_CLEAR_ON_DISPATCH:-0}" != 1 ]; then
    timeout -s KILL 10 "$TMUX_BIN" send-keys -t "$pane" -l "/clear" 2>/dev/null
    sleep 0.3
    timeout -s KILL 10 "$TMUX_BIN" send-keys -t "$pane" Enter 2>/dev/null
    sleep 1.0                                                                     # let /clear take effect
    timeout -s KILL 10 "$TMUX_BIN" send-keys -t "$pane" C-u 2>/dev/null          # clear residual input post-/clear
  fi
  timeout -s KILL 10 "$TMUX_BIN" send-keys -t "$pane" -l "$cmd" 2>/dev/null
  sleep 0.3
  timeout -s KILL 10 "$TMUX_BIN" send-keys -t "$pane" Enter 2>/dev/null
  _txk_release
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
