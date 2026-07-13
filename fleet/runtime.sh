#!/usr/bin/env bash
# Shared, Bash 3.2-compatible fleet runtime helpers.

FLEET_RUN_TMP_DIR=${FLEET_RUN_TMP_DIR:-/tmp/grind}
FLEET_RUN_STATE_DIR=${FLEET_RUN_STATE_DIR:-/Users/douglaswhittingham/gamecube-decomp-harness/projects/pkmn-colosseum/state/fleet-runtime}
FLEET_WORKTREE_ROOT=${FLEET_WORKTREE_ROOT:-/Users/douglaswhittingham/gamecube-decomp-harness/projects/pkmn-colosseum/worktrees}
FLEET_QUARANTINE_ROOT=${FLEET_QUARANTINE_ROOT:-/tmp/grind/quarantined-worktrees}

fleet_valid_run_id() {
  [[ ${1:-} =~ ^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$ ]]
}

fleet_run_file() {
  case ${1:-} in
    small|medium|large) printf '%s/fs-%s_run.txt\n' "$FLEET_RUN_TMP_DIR" "$1" ;;
    *) return 1 ;;
  esac
}

fleet_persisted_run_file() {
  case ${1:-} in
    small|medium|large) printf '%s/fs-%s_run.txt\n' "$FLEET_RUN_STATE_DIR" "$1" ;;
    *) return 1 ;;
  esac
}

fleet_write_run_id() {
  local destination=$1 run_id=$2 temporary
  temporary="${destination}.tmp.$$"
  printf '%s\n' "$run_id" > "$temporary" || return 1
  mv "$temporary" "$destination"
}

# Rotate a lane deliberately. The durable copy is authoritative; writing it
# first means a partial update is repaired on the next fleet_ensure_run_id.
fleet_set_run_id() {
  local lane=$1 run_id=$2 live_file persisted_file
  fleet_valid_run_id "$run_id" || return 1
  live_file=$(fleet_run_file "$lane") || return 1
  persisted_file=$(fleet_persisted_run_file "$lane") || return 1
  mkdir -p "$FLEET_RUN_TMP_DIR" "$FLEET_RUN_STATE_DIR" || return 1
  fleet_write_run_id "$persisted_file" "$run_id" || return 1
  fleet_write_run_id "$live_file" "$run_id"
}

# The persistent copy is authoritative once it exists. If /tmp was cleared or
# disagrees, restore it from durable state. Only the first migration may seed a
# missing durable copy from a valid live ID. Invalid values (including legacy
# PAUSED-* markers) are never launched.
fleet_ensure_run_id() {
  local lane=$1 live_file persisted_file run_id persisted
  live_file=$(fleet_run_file "$lane") || return 1
  persisted_file=$(fleet_persisted_run_file "$lane") || return 1
  mkdir -p "$FLEET_RUN_TMP_DIR" "$FLEET_RUN_STATE_DIR" || return 1

  persisted=$(cat "$persisted_file" 2>/dev/null || true)
  if fleet_valid_run_id "$persisted"; then
    run_id=$(cat "$live_file" 2>/dev/null || true)
    [ "$run_id" = "$persisted" ] || fleet_write_run_id "$live_file" "$persisted" || return 1
    printf '%s\n' "$persisted"
    return 0
  fi
  [ ! -e "$persisted_file" ] || return 1

  run_id=$(cat "$live_file" 2>/dev/null || true)
  fleet_valid_run_id "$run_id" || return 1
  fleet_write_run_id "$persisted_file" "$run_id" || return 1
  printf '%s\n' "$run_id"
}

# Require the exact run-loop process to descend from the named tmux pane. This
# rejects both idle placeholder panes and orphaned run-loops.
fleet_lane_alive() {
  local session=$1 run_id=$2 pane_pid pid parent depth
  fleet_valid_run_id "$run_id" || return 1
  tmux has-session -t "$session" 2>/dev/null || return 1
  pane_pid=$(tmux display-message -p -t "$session" '#{pane_pid}' 2>/dev/null) || return 1
  for pid in $(pgrep -f "run-loop --run-id $run_id" 2>/dev/null); do
    depth=0
    while [ -n "$pid" ] && [ "$pid" -gt 1 ] 2>/dev/null && [ "$depth" -lt 20 ]; do
      [ "$pid" = "$pane_pid" ] && return 0
      parent=$(ps -o ppid= -p "$pid" 2>/dev/null | tr -d ' ')
      [ -n "$parent" ] || break
      pid=$parent
      depth=$((depth + 1))
    done
  done
  return 1
}

# A newly created tmux session needs time to start its workers and emit fresh
# database activity before the stall detector is allowed to recycle it.
fleet_tmux_session_younger_than() {
  local session=$1 limit=$2 created now age
  created=$(tmux display-message -p -t "$session" '#{session_created}' 2>/dev/null) || return 1
  now=$(date +%s) || return 1
  case "$created:$now:$limit" in
    *[!0-9:]*) return 1 ;;
  esac
  [ "$now" -ge "$created" ] 2>/dev/null || return 1
  age=$((now - created))
  [ "$age" -lt "$limit" ]
}

# After a worker crash, an old claim can point at a partially deleted directory
# that is no longer a Git worktree. Preserve that debris for inspection while
# clearing the exact claim path so the scheduler can create a fresh worktree.
fleet_quarantine_broken_worktree() {
  local worker_path=$1 expected_claim_id=$2 root parent claim_id destination
  root=${FLEET_WORKTREE_ROOT%/}
  parent=${worker_path%/source}
  claim_id=${parent##*/}
  fleet_valid_run_id "$claim_id" || return 1
  [ "$claim_id" = "$expected_claim_id" ] || return 1
  [ "$worker_path" = "$root/$claim_id/source" ] || return 1
  [ -d "$parent" ] || return 1
  [ ! -e "$worker_path/.git" ] || return 1
  mkdir -p "$FLEET_QUARANTINE_ROOT" || return 1
  destination="$FLEET_QUARANTINE_ROOT/${claim_id}-$(date +%s)-$$"
  mv "$parent" "$destination" || return 1
  printf '%s\n' "$destination"
}
