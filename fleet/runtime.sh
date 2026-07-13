#!/usr/bin/env bash
# Shared, Bash 3.2-compatible fleet runtime helpers.

FLEET_RUN_TMP_DIR=${FLEET_RUN_TMP_DIR:-/tmp/grind}
FLEET_RUN_STATE_DIR=${FLEET_RUN_STATE_DIR:-/Users/douglaswhittingham/gamecube-decomp-harness/projects/pkmn-colosseum/state/fleet-runtime}
FLEET_WORKTREE_ROOT=${FLEET_WORKTREE_ROOT:-/Users/douglaswhittingham/gamecube-decomp-harness/projects/pkmn-colosseum/worktrees}
if [ "${FLEET_ADDITIONAL_WORKTREE_ROOTS+x}" != x ]; then
  FLEET_ADDITIONAL_WORKTREE_ROOTS=/Users/douglaswhittingham/gamecube-decomp-harness-live/projects/pkmn-colosseum/worktrees
fi
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

# Optional lane-specific launchers live at fixed names in durable state.  The
# file itself is the configuration: do not read a command line from state and
# do not invoke it through eval/bash.  A present but unsafe override fails
# closed (status 2); an absent override returns status 1 so callers retain the
# historical lane command.
fleet_restart_script_file() {
  local root=${FLEET_RUN_STATE_DIR%/}
  [ -n "$root" ] || root=/
  case ${1:-} in
    small|medium|large) printf '%s/fs-%s_restart.sh\n' "$root" "$1" ;;
    *) return 2 ;;
  esac
}

fleet_lane_restart_script() {
  local lane=$1 script state_dir script_dir
  script=$(fleet_restart_script_file "$lane") || return 2

  # A dangling symlink is not -e, so test -L independently before deciding the
  # override is absent.  Symlinks are rejected even when they point back into
  # the state directory: the executable must be the regular file named above.
  if [ ! -e "$script" ] && [ ! -L "$script" ]; then
    return 1
  fi
  [ ! -L "$script" ] || return 2
  [ -f "$script" ] || return 2
  [ -x "$script" ] || return 2

  state_dir=$(cd "$FLEET_RUN_STATE_DIR" 2>/dev/null && pwd -P) || return 2
  script_dir=$(cd "$(dirname "$script")" 2>/dev/null && pwd -P) || return 2
  [ "$script_dir" = "$state_dir" ] || return 2
  printf '%s\n' "$script"
}

# Build the single shell command tmux requires.  Every interpolated value is
# shell-quoted by Bash; the only executable override comes from the validated,
# fixed path above.  Nothing read from a state file is evaluated as shell text.
fleet_lane_restart_command() {
  local lane=$1 run_id=$2 runtime_path=$3 override status max_workers start_script
  fleet_valid_run_id "$run_id" || return 2

  if override=$(fleet_lane_restart_script "$lane"); then
    printf 'exec env PATH=%q RUN=%q %q\n' "$runtime_path" "$run_id" "$override"
    return 0
  else
    status=$?
  fi
  [ "$status" -eq 1 ] || return 2

  case "$lane" in
    small)
      max_workers=3
      start_script=projects/pkmn-colosseum/ops/start-fs-small.sh
      ;;
    medium)
      max_workers=2
      start_script=projects/pkmn-colosseum/ops/start-fs-medium.sh
      ;;
    large)
      max_workers=1
      start_script=projects/pkmn-colosseum/ops/start-fs-large.sh
      ;;
    *) return 2 ;;
  esac
  printf 'exec env PATH=%q RUN=%q MAXW=%q FUZZY_MAX=87.999 bash %q\n' \
    "$runtime_path" "$run_id" "$max_workers" "$start_script"
}

fleet_start_lane_session() {
  local lane=$1 session=$2 run_id=$3 runtime_path=$4 command
  command=$(fleet_lane_restart_command "$lane" "$run_id" "$runtime_path") || return 2
  tmux new-session -d -s "$session" "$command"
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

# Print the primary and optional colon-separated additional worker roots.  GC
# and broken-worktree recovery accept only absolute, non-root paths without
# traversal components.  Paths in this deployment contain no colons.
fleet_worker_worktree_roots() {
  local roots remaining root output
  roots=$FLEET_WORKTREE_ROOT
  [ -z "$FLEET_ADDITIONAL_WORKTREE_ROOTS" ] || \
    roots="$roots:$FLEET_ADDITIONAL_WORKTREE_ROOTS"
  remaining="$roots:"
  while [ -n "$remaining" ]; do
    root=${remaining%%:*}
    remaining=${remaining#*:}
    [ -n "$root" ] || return 2
    case "$root" in
      /*) ;;
      *) return 2 ;;
    esac
    case "/$root/" in
      *'/../'*|*'/./'*) return 2 ;;
    esac
    root=${root%/}
    [ -n "$root" ] || return 2
    if [ -z "$output" ]; then
      output=$root
    else
      output="$output
$root"
    fi
  done
  printf '%s\n' "$output"
}

# Require the canonical fleet layout <configured-root>/<claim UUID>/source.
# This is shared by quarantine and GC so a broadened root list cannot broaden
# deletion to unrelated directories.
fleet_managed_worker_path() {
  local worker_path=$1 expected_claim_id=$2 root
  fleet_valid_run_id "$expected_claim_id" || return 1
  while IFS= read -r root; do
    [ "$worker_path" = "$root/$expected_claim_id/source" ] || continue
    printf '%s\n' "$root"
    return 0
  done < <(fleet_worker_worktree_roots)
  return 1
}

# After a worker crash, an old claim can point at a partially deleted directory
# that is no longer a Git worktree. Preserve that debris for inspection while
# clearing the exact claim path so the scheduler can create a fresh worktree.
fleet_quarantine_broken_worktree() {
  local worker_path=$1 expected_claim_id=$2 root parent claim_id destination
  parent=${worker_path%/source}
  claim_id=${parent##*/}
  fleet_valid_run_id "$claim_id" || return 1
  [ "$claim_id" = "$expected_claim_id" ] || return 1
  root=$(fleet_managed_worker_path "$worker_path" "$claim_id") || return 1
  [ -d "$parent" ] || return 1
  [ ! -e "$worker_path/.git" ] || return 1
  mkdir -p "$FLEET_QUARANTINE_ROOT" || return 1
  destination="$FLEET_QUARANTINE_ROOT/${claim_id}-$(date +%s)-$$"
  mv "$parent" "$destination" || return 1
  printf '%s\n' "$destination"
}
