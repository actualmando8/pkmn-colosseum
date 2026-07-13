#!/usr/bin/env bash
set -euo pipefail

ROOT=$(cd "$(dirname "$0")/.." && pwd)
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
export FLEET_RUN_TMP_DIR="$TMP/tmp"
export FLEET_RUN_STATE_DIR="$TMP/state"
source "$ROOT/fleet/runtime.sh"

VALID=fb947e95-fb57-481c-8cca-335330cfe483
NEXT=9a4d1295-9815-44d4-a73d-d1f3ac35edef
mkdir -p "$FLEET_RUN_TMP_DIR"
printf '%s\n' "$VALID" > "$FLEET_RUN_TMP_DIR/fs-small_run.txt"
[ "$(fleet_ensure_run_id small)" = "$VALID" ]
[ "$(cat "$FLEET_RUN_STATE_DIR/fs-small_run.txt")" = "$VALID" ]

rm "$FLEET_RUN_TMP_DIR/fs-small_run.txt"
[ "$(fleet_ensure_run_id small)" = "$VALID" ]
[ "$(cat "$FLEET_RUN_TMP_DIR/fs-small_run.txt")" = "$VALID" ]

printf '%s\n' PAUSED-legacy > "$FLEET_RUN_TMP_DIR/fs-small_run.txt"
[ "$(fleet_ensure_run_id small)" = "$VALID" ]
[ "$(cat "$FLEET_RUN_TMP_DIR/fs-small_run.txt")" = "$VALID" ]

printf '%s\n' "$NEXT" > "$FLEET_RUN_TMP_DIR/fs-small_run.txt"
[ "$(fleet_ensure_run_id small)" = "$VALID" ]
[ "$(cat "$FLEET_RUN_TMP_DIR/fs-small_run.txt")" = "$VALID" ]
[ "$(cat "$FLEET_RUN_STATE_DIR/fs-small_run.txt")" = "$VALID" ]

fleet_set_run_id small "$NEXT"
[ "$(fleet_ensure_run_id small)" = "$NEXT" ]
[ "$(cat "$FLEET_RUN_TMP_DIR/fs-small_run.txt")" = "$NEXT" ]
[ "$(cat "$FLEET_RUN_STATE_DIR/fs-small_run.txt")" = "$NEXT" ]

mkdir -p "$FLEET_RUN_STATE_DIR"
printf '%s\n' corrupt > "$FLEET_RUN_STATE_DIR/fs-medium_run.txt"
printf '%s\n' "$VALID" > "$FLEET_RUN_TMP_DIR/fs-medium_run.txt"
if fleet_ensure_run_id medium >/dev/null 2>&1; then
  echo "corrupt durable run ID did not fail closed" >&2
  exit 1
fi
rm "$FLEET_RUN_STATE_DIR/fs-medium_run.txt" "$FLEET_RUN_TMP_DIR/fs-medium_run.txt"

if fleet_ensure_run_id medium >/dev/null 2>&1; then
  echo "missing run ID did not fail closed" >&2
  exit 1
fi
if fleet_valid_run_id PAUSED-legacy; then
  echo "pause marker accepted as a run ID" >&2
  exit 1
fi

export FLEET_WORKTREE_ROOT="$TMP/worktrees"
export FLEET_QUARANTINE_ROOT="$TMP/quarantine"
BROKEN_CLAIM=11111111-2222-3333-4444-555555555555
mkdir -p "$FLEET_WORKTREE_ROOT/$BROKEN_CLAIM/source/build"
quarantined=$(fleet_quarantine_broken_worktree "$FLEET_WORKTREE_ROOT/$BROKEN_CLAIM/source" "$BROKEN_CLAIM")
[ ! -e "$FLEET_WORKTREE_ROOT/$BROKEN_CLAIM" ]
[ -d "$quarantined/source/build" ]

VALID_CLAIM=aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee
mkdir -p "$FLEET_WORKTREE_ROOT/$VALID_CLAIM/source"
touch "$FLEET_WORKTREE_ROOT/$VALID_CLAIM/source/.git"
if fleet_quarantine_broken_worktree "$FLEET_WORKTREE_ROOT/$VALID_CLAIM/source" "$VALID_CLAIM" >/dev/null; then
  echo "valid Git worktree was quarantined" >&2
  exit 1
fi
[ -e "$FLEET_WORKTREE_ROOT/$VALID_CLAIM/source/.git" ]

OUTSIDE_CLAIM=12345678-1234-1234-1234-123456789abc
mkdir -p "$TMP/outside/$OUTSIDE_CLAIM/source/build"
if fleet_quarantine_broken_worktree "$FLEET_WORKTREE_ROOT/../outside/$OUTSIDE_CLAIM/source" "$OUTSIDE_CLAIM" >/dev/null; then
  echo "traversal worktree path was quarantined" >&2
  exit 1
fi
[ -d "$TMP/outside/$OUTSIDE_CLAIM/source/build" ]

NESTED_CLAIM=87654321-4321-4321-4321-cba987654321
mkdir -p "$FLEET_WORKTREE_ROOT/nested/$NESTED_CLAIM/source/build"
if fleet_quarantine_broken_worktree "$FLEET_WORKTREE_ROOT/nested/$NESTED_CLAIM/source" "$NESTED_CLAIM" >/dev/null; then
  echo "nested worktree path was quarantined" >&2
  exit 1
fi
[ -d "$FLEET_WORKTREE_ROOT/nested/$NESTED_CLAIM/source/build" ]

# A run-loop is healthy only when its PPID chain reaches the tmux pane.
tmux() {
  case ${1:-} in
    has-session) return 0 ;;
    display-message) printf '100\n' ;;
    *) return 1 ;;
  esac
}
pgrep() { printf '300\n'; }
ps() {
  local pid=${!#}
  case "$pid" in
    300) printf '200\n' ;;
    200) printf '100\n' ;;
    *) return 1 ;;
  esac
}
fleet_lane_alive colo-fs-small "$VALID"
tmux() {
  case ${1:-} in
    has-session) return 0 ;;
    display-message) printf '999\n' ;;
    *) return 1 ;;
  esac
}
if fleet_lane_alive colo-fs-small "$VALID"; then
  echo "orphan run-loop accepted as healthy" >&2
  exit 1
fi

tmux() {
  case ${1:-} in
    display-message) printf '1000\n' ;;
    *) return 1 ;;
  esac
}
date() { printf '1200\n'; }
fleet_tmux_session_younger_than colo-fs-small 300
if fleet_tmux_session_younger_than colo-fs-small 100; then
  echo "old tmux session accepted as startup grace" >&2
  exit 1
fi

echo "fleet runtime tests passed"
