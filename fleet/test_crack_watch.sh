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

printf 'small-current\n' > "$SMALL_RUN_FILE"
printf 'PAUSED-20260712T205346Z\n' > "$MEDIUM_RUN_FILE"
: > "$LARGE_RUN_FILE"

assert_equal "small-current" "$(active_run_ids)"
assert_equal "'small-current'" "$(run_id_sql_list)"

# The next call must re-read all files rather than retain the first snapshot.
printf 'small-next\n' > "$SMALL_RUN_FILE"
printf 'medium-next\n' > "$MEDIUM_RUN_FILE"
printf 'large-next\n' > "$LARGE_RUN_FILE"

assert_equal "small-next
medium-next
large-next" "$(active_run_ids)"
assert_equal "'small-next','medium-next','large-next'" "$(run_id_sql_list)"

# Unsafe values cannot be interpolated into the SQL query.
printf "bad'id\n" > "$MEDIUM_RUN_FILE"
assert_equal "'small-next','large-next'" "$(run_id_sql_list)"

printf 'crack_watch tests passed\n'
