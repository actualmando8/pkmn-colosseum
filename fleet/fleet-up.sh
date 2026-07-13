#!/usr/bin/env bash
# One-command fleet bootstrap after a reboot/crash. Idempotent: skips anything
# already running. Assumes: harness at ~/gamecube-decomp-harness, lane worktrees
# at ~/pkmn-colosseum-{fromscratch,fs-medium,fs-large}, colima+docker installed.
set -uo pipefail
HARNESS=/Users/douglaswhittingham/gamecube-decomp-harness
GAME=/Users/douglaswhittingham/pkmn-colosseum
FLEET=${FLEET_DIR:-$GAME/fleet}
PAUSE_FILE=/tmp/grind/harness-paused.txt
RUNTIME_PATH=${FLEET_RUNTIME_PATH:-/Users/douglaswhittingham/.bun/bin:/opt/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin}
export PATH="$RUNTIME_PATH"
source "$FLEET/runtime.sh"

echo "[fleet-up] 1/5 container runtime"
colima status >/dev/null 2>&1 || colima start
docker ps --format '{{.Names}}' | grep -q agent-kernel-db || docker start agent-kernel-db
until docker exec agent-kernel-db pg_isready -U agent_kernel >/dev/null 2>&1; do sleep 2; done

echo "[fleet-up] 2/5 run-id files"
small_run=$(fleet_ensure_run_id small 2>/dev/null || true)
medium_run=$(fleet_ensure_run_id medium 2>/dev/null || true)
large_run=$(fleet_ensure_run_id large 2>/dev/null || true)

echo "[fleet-up] 3/5 lanes"
cd "$HARNESS"
if [ -f "$PAUSE_FILE" ]; then
  echo "[fleet-up] fleet paused; lane startup suppressed"
else
  if [ -n "$small_run" ]; then
    tmux has-session -t colo-fs-small 2>/dev/null || tmux new-session -d -s colo-fs-small "exec env PATH=$RUNTIME_PATH RUN=$small_run MAXW=3 FUZZY_MAX=87.999 bash projects/pkmn-colosseum/ops/start-fs-small.sh"
  else
    echo "[fleet-up] ERROR small lane has no valid persisted run ID"
  fi
  if [ -n "$medium_run" ]; then
    tmux has-session -t colo-fs-medium 2>/dev/null || tmux new-session -d -s colo-fs-medium "exec env PATH=$RUNTIME_PATH RUN=$medium_run MAXW=2 FUZZY_MAX=87.999 bash projects/pkmn-colosseum/ops/start-fs-medium.sh"
  else
    echo "[fleet-up] ERROR medium lane has no valid persisted run ID"
  fi
  if [ -n "$large_run" ]; then
    tmux has-session -t colo-fs-large 2>/dev/null || tmux new-session -d -s colo-fs-large "exec env PATH=$RUNTIME_PATH RUN=$large_run MAXW=1 FUZZY_MAX=87.999 bash projects/pkmn-colosseum/ops/start-fs-large.sh"
  else
    echo "[fleet-up] ERROR large lane has no valid persisted run ID"
  fi
fi

echo "[fleet-up] 4/5 dashboard + watchers"
tmux has-session -t harness-dashboard 2>/dev/null || tmux new-session -d -s harness-dashboard "export PATH=$RUNTIME_PATH; cd $HARNESS && bun run ui:server 2>&1 | tee /tmp/grind/dashboard.log"
tmux has-session -t watchdog    2>/dev/null || tmux new-session -d -s watchdog    "exec env PATH=$RUNTIME_PATH FLEET_DIR=$FLEET bash $FLEET/overnight_watchdog.sh"
tmux has-session -t crack-watch 2>/dev/null || tmux new-session -d -s crack-watch "exec env PATH=$RUNTIME_PATH bash $FLEET/crack_watch.sh"

echo "[fleet-up] 5/5 loops"
pgrep -f fs_push.sh >/dev/null || { nohup bash "$FLEET/fs_push.sh" > /tmp/grind/fs_push.log 2>&1 & disown; }

echo "[fleet-up] done:"
tmux ls
