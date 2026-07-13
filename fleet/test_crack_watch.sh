#!/bin/bash
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/crack_watch.sh"

TEST_TMP=$(mktemp -d "${TMPDIR:-/tmp}/crack-watch-test.XXXXXX")
trap 'rm -rf "$TEST_TMP"' EXIT HUP INT TERM

SMALL_RUN_FILE="$TEST_TMP/fs-small_run.txt"
MEDIUM_RUN_FILE="$TEST_TMP/fs-medium_run.txt"
LARGE_RUN_FILE="$TEST_TMP/fs-large_run.txt"

assert_equal() {
  if [ "$1" != "$2" ]; then
    printf 'expected:\n%s\nactual:\n%s\n' "$1" "$2" >&2
    exit 1
  fi
}

SMALL_CURRENT=11111111-1111-1111-1111-111111111111
SMALL_NEXT=22222222-2222-2222-2222-222222222222
MEDIUM_NEXT=33333333-3333-3333-3333-333333333333
LARGE_NEXT=44444444-4444-4444-4444-444444444444

printf '%s\n' "$SMALL_CURRENT" > "$SMALL_RUN_FILE"
printf 'PAUSED-20260712T205346Z\n' > "$MEDIUM_RUN_FILE"
: > "$LARGE_RUN_FILE"

assert_equal "$SMALL_CURRENT" "$(active_run_ids)"
assert_equal "'$SMALL_CURRENT'" "$(run_id_sql_list)"

# The next call must re-read all files rather than retain the first snapshot.
printf '%s\n' "$SMALL_NEXT" > "$SMALL_RUN_FILE"
printf '%s\n' "$MEDIUM_NEXT" > "$MEDIUM_RUN_FILE"
printf '%s\n' "$LARGE_NEXT" > "$LARGE_RUN_FILE"

assert_equal "$SMALL_NEXT
$MEDIUM_NEXT
$LARGE_NEXT" "$(active_run_ids)"
assert_equal "'$SMALL_NEXT','$MEDIUM_NEXT','$LARGE_NEXT'" "$(run_id_sql_list)"

# Unsafe values cannot be interpolated into the SQL query.
printf "bad'id\n" > "$MEDIUM_RUN_FILE"
assert_equal "'$SMALL_NEXT','$LARGE_NEXT'" "$(run_id_sql_list)"

printf 'crack_watch tests passed\n'
