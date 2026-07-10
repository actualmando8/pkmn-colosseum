#!/usr/bin/env bash
# One-command fleet bootstrap after a reboot/crash. Idempotent: skips anything
# already running. Assumes: harness at ~/gamecube-decomp-harness, lane worktrees
# at ~/pkmn-colosseum-{fromscratch,fs-medium,fs-large}, colima+docker installed.
set -uo pipefail
HARNESS=/Users/douglaswhittingham/gamecube-decomp-harness
GAME=/Users/douglaswhittingham/pkmn-colosseum
FLEET="$GAME/fleet"

echo "[fleet-up] 1/5 container runtime"
colima status >/dev/null 2>&1 || colima start
docker ps --format '{{.Names}}' | grep -q agent-kernel-db || docker start agent-kernel-db
until docker exec agent-kernel-db pg_isready -U agent_kernel >/dev/null 2>&1; do sleep 2; done

echo "[fleet-up] 2/5 run-id files"
mkdir -p /tmp/grind
[ -f /tmp/grind/fs-small_run.txt ]  || echo db660bd8-9e0a-4052-b04c-3572b0a62116 > /tmp/grind/fs-small_run.txt
[ -f /tmp/grind/fs-medium_run.txt ] || echo 88c578a7-6e26-40cd-aaa7-f7449d9f7e2a > /tmp/grind/fs-medium_run.txt
[ -f /tmp/grind/fs-large_run.txt ]  || echo 5f317c77-c320-430f-96cb-c14b537ccf71 > /tmp/grind/fs-large_run.txt

echo "[fleet-up] 3/5 lanes"
cd "$HARNESS"
tmux has-session -t colo-fs-small  2>/dev/null || tmux new-session -d -s colo-fs-small  "MAXW=5 bash projects/pkmn-colosseum/ops/start-fs-small.sh"
tmux has-session -t colo-fs-medium 2>/dev/null || tmux new-session -d -s colo-fs-medium "MAXW=2 bash projects/pkmn-colosseum/ops/start-fs-medium.sh"
tmux has-session -t colo-fs-large  2>/dev/null || tmux new-session -d -s colo-fs-large  "MAXW=1 bash projects/pkmn-colosseum/ops/start-fs-large.sh"

echo "[fleet-up] 4/5 dashboard + watchers"
tmux has-session -t harness-dashboard 2>/dev/null || tmux new-session -d -s harness-dashboard "cd $HARNESS && bun run ui:server 2>&1 | tee /tmp/grind/dashboard.log"
tmux has-session -t watchdog    2>/dev/null || tmux new-session -d -s watchdog    "bash $FLEET/overnight_watchdog.sh"
tmux has-session -t crack-watch 2>/dev/null || tmux new-session -d -s crack-watch "bash $FLEET/crack_watch.sh"

echo "[fleet-up] 5/5 loops"
pgrep -f fs_push.sh >/dev/null || { nohup bash "$FLEET/fs_push.sh" > /tmp/grind/fs_push.log 2>&1 & disown; }

echo "[fleet-up] done:"
tmux ls
