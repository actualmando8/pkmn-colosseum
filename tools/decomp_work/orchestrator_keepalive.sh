#!/bin/bash
# orchestrator_keepalive.sh — keeps the Codex orchestrator (pane %9) cycling.
# A Codex agent idles after each turn; this detached loop re-pokes it with an
# "orchestration round" whenever it goes idle, so it keeps driving the fleet
# autonomously without the Claude main-loop. Run detached (setsid nohup).
ORCH="${1:-%9}"
LOG=/mnt/c/Users/douglaswhittingham/pkmn-colosseum/tools/decomp_work/overnight/logs/orchestrator_keepalive.log
ROUND="Orchestration round (per docs/ORCHESTRATOR_HANDOFF.md): (1) capture %3 %4 %6 %8; re-dispatch ANY idle agent via tmux send-keys with its next function/task and 'keep grinding, do not idle'. (2) If %8 (PC-port) rebuilt a probe exe, run the batch via PowerShell (edit build_pc/_runbatch.ps1 \$exe to newest pcport_motion_probe_headless_*.exe; powershell.exe -NoProfile -ExecutionPolicy Bypass -File C:\\Users\\douglaswhittingham\\pkmn-colosseum\\build_pc\\_runbatch.ps1; read build_pc/_batch_out.txt) and paste pkx+char counts to %8. (3) Keep DeepSeek %6 fed (requeue failed<=400 to queued in tasks.json if queue-empty). (4) Periodically commit+push checkpoints to origin. (5) Work the priority TODO: finish pkx batch -> battle Colosseum -> fix non-winning annealer + DeepSeek. Then finish this turn; you'll be poked for the next round."
while true; do
  cap=$(tmux capture-pane -t "$ORCH" -p 2>/dev/null)
  if ! echo "$cap" | tail -6 | grep -q "esc to interrupt"; then
    echo "$(date +%H:%M:%S) orchestrator idle -> poking a round" >> "$LOG"
    tmux send-keys -t "$ORCH" C-u; sleep 1
    tmux send-keys -t "$ORCH" -l "$ROUND"; sleep 1
    tmux send-keys -t "$ORCH" Enter; sleep 2; tmux send-keys -t "$ORCH" Enter
    sleep 120   # give it a turn before checking again
  fi
  sleep 150
done
