#!/usr/bin/env bash
set -euo pipefail

ROOT=$(cd "$(dirname "$0")/.." && pwd)
source "$ROOT/fleet/fs_push.sh"

TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT
REPO_DIR="$TMP/repo"

git init -q "$REPO_DIR"
git -C "$REPO_DIR" config user.email fleet-test@example.invalid
git -C "$REPO_DIR" config user.name fleet-test
mkdir -p "$REPO_DIR/src" "$REPO_DIR/include"
printf 'base\n' > "$REPO_DIR/src/unit.c"
printf 'base\n' > "$REPO_DIR/README.md"
git -C "$REPO_DIR" add src/unit.c README.md
git -C "$REPO_DIR" commit -q -m base
BASE=$(git -C "$REPO_DIR" rev-parse HEAD)

cd "$REPO_DIR"

# Clean and README-only epochs are not publishable.
if meaningful_changes_between "$BASE" "$BASE"; then
  echo "clean tree incorrectly considered meaningful" >&2
  exit 1
fi
printf 'metrics only\n' > README.md
git add README.md
git commit -q -m metadata
README_HEAD=$(git rev-parse HEAD)
if meaningful_changes_between "$BASE" "$README_HEAD"; then
  echo "README-only epoch incorrectly considered meaningful" >&2
  exit 1
fi

# Tracked edits, deletions, and untracked files under payload paths count.
printf 'source win\n' > src/unit.c
git add src/unit.c
git commit -q -m source
SOURCE_HEAD=$(git rev-parse HEAD)
meaningful_changes_between "$README_HEAD" "$SOURCE_HEAD"

git rm -q src/unit.c
git commit -q -m deletion
DELETE_HEAD=$(git rev-parse HEAD)
meaningful_changes_between "$SOURCE_HEAD" "$DELETE_HEAD"

printf 'header win\n' > include/new.h
git add include/new.h
git commit -q -m header
HEADER_HEAD=$(git rev-parse HEAD)
meaningful_changes_between "$DELETE_HEAD" "$HEADER_HEAD"

echo "fs_push gate tests passed"

# Exercise the full publisher against a local bare remote and a mocked GitHub
# CLI. This catches duplicate-PR regressions without touching the real repo.
MOCK_BIN="$TMP/bin"
MOCK_GH_LOG="$TMP/gh.log"
mkdir -p "$MOCK_BIN"
touch "$MOCK_GH_LOG"
printf '%s\n' \
  '#!/usr/bin/env bash' \
  'case "${1:-} ${2:-}" in' \
  '  "pr list")' \
  '    [ "${MOCK_GH_LIST_FAIL:-0}" = 1 ] && exit 1' \
  '    printf "%s\\n" "${MOCK_PR_JSON:-[]}"' \
  '    ;;' \
  '  "pr create")' \
  '    printf "%s\\n" "$*" >> "$MOCK_GH_LOG"' \
  '    [ "${MOCK_GH_CREATE_FAIL:-0}" = 1 ] && exit 1' \
  '    printf "%s\\n" "https://example.invalid/pull/test"' \
  '    ;;' \
  '  *) exit 2 ;;' \
  'esac' > "$MOCK_BIN/gh"
chmod +x "$MOCK_BIN/gh"
export PATH="$MOCK_BIN:$PATH"
export MOCK_GH_LOG

REMOTE_DIR="$TMP/remote.git"
PUBLISH_REPO="$TMP/publisher"
git init -q --bare "$REMOTE_DIR"
git init -q -b master "$PUBLISH_REPO"
git -C "$PUBLISH_REPO" config user.email fleet-test@example.invalid
git -C "$PUBLISH_REPO" config user.name fleet-test
mkdir -p "$PUBLISH_REPO/src"
printf 'base\n' > "$PUBLISH_REPO/src/unit.c"
printf 'base\n' > "$PUBLISH_REPO/README.md"
git -C "$PUBLISH_REPO" add src/unit.c README.md
git -C "$PUBLISH_REPO" commit -q -m base
git -C "$PUBLISH_REPO" remote add origin "$REMOTE_DIR"
git -C "$PUBLISH_REPO" push -q -u origin master
git -C "$PUBLISH_REPO" switch -q -c lane/test
printf 'first payload\n' > "$PUBLISH_REPO/src/unit.c"
git -C "$PUBLISH_REPO" add src/unit.c
git -C "$PUBLISH_REPO" commit -q -m payload
PAYLOAD_HEAD=$(git -C "$PUBLISH_REPO" rev-parse HEAD)
TEST_LANE="$PUBLISH_REPO|lane/test|Test lane"

# A closed PR still owns its exact head forever; do not repush or recreate it.
export MOCK_PR_JSON="[{\"number\":9,\"state\":\"CLOSED\",\"headRefOid\":\"$PAYLOAD_HEAD\"}]"
publish_lane "$TEST_LANE"
if git --git-dir="$REMOTE_DIR" show-ref --verify --quiet refs/heads/lane/test; then
  echo "previously posted head was repushed" >&2
  exit 1
fi
[ ! -s "$MOCK_GH_LOG" ]

# A metadata-only tip after an already-pushed payload is not publishable.
git -C "$PUBLISH_REPO" push -q origin "$PAYLOAD_HEAD:refs/heads/lane/test"
printf 'metrics only\n' > "$PUBLISH_REPO/README.md"
git -C "$PUBLISH_REPO" add README.md
git -C "$PUBLISH_REPO" commit -q -m metadata
METADATA_HEAD=$(git -C "$PUBLISH_REPO" rev-parse HEAD)
publish_lane "$TEST_LANE"
[ "$(git --git-dir="$REMOTE_DIR" rev-parse refs/heads/lane/test)" = "$PAYLOAD_HEAD" ]
[ "$METADATA_HEAD" != "$PAYLOAD_HEAD" ]
[ ! -s "$MOCK_GH_LOG" ]

# A new source tip updates an existing open PR without creating another one.
printf 'second payload\n' > "$PUBLISH_REPO/src/unit.c"
git -C "$PUBLISH_REPO" add src/unit.c
git -C "$PUBLISH_REPO" commit -q -m payload-2
SECOND_HEAD=$(git -C "$PUBLISH_REPO" rev-parse HEAD)
export MOCK_PR_JSON="[{\"number\":10,\"state\":\"OPEN\",\"headRefOid\":\"$PAYLOAD_HEAD\"}]"
publish_lane "$TEST_LANE"
[ "$(git --git-dir="$REMOTE_DIR" rev-parse refs/heads/lane/test)" = "$SECOND_HEAD" ]
[ ! -s "$MOCK_GH_LOG" ]

# If push succeeds but PR creation fails, retry creation without repushing.
git -C "$PUBLISH_REPO" switch -q -c lane/retry master
printf 'retry payload\n' > "$PUBLISH_REPO/src/unit.c"
git -C "$PUBLISH_REPO" add src/unit.c
git -C "$PUBLISH_REPO" commit -q -m retry-payload
RETRY_HEAD=$(git -C "$PUBLISH_REPO" rev-parse HEAD)
RETRY_LANE="$PUBLISH_REPO|lane/retry|Retry lane"
export MOCK_PR_JSON='[]'
export MOCK_GH_CREATE_FAIL=1
publish_lane "$RETRY_LANE"
[ "$(git --git-dir="$REMOTE_DIR" rev-parse refs/heads/lane/retry)" = "$RETRY_HEAD" ]
[ "$(wc -l < "$MOCK_GH_LOG" | tr -d ' ')" = 1 ]
export MOCK_GH_CREATE_FAIL=0
publish_lane "$RETRY_LANE"
[ "$(wc -l < "$MOCK_GH_LOG" | tr -d ' ')" = 2 ]
export MOCK_PR_JSON="[{\"number\":11,\"state\":\"CLOSED\",\"headRefOid\":\"$RETRY_HEAD\"}]"
publish_lane "$RETRY_LANE"
[ "$(wc -l < "$MOCK_GH_LOG" | tr -d ' ')" = 2 ]

# Bad ledger data and the fleet pause sentinel both fail closed.
git -C "$PUBLISH_REPO" switch -q -c lane/bad-ledger master
printf 'bad ledger payload\n' > "$PUBLISH_REPO/src/unit.c"
git -C "$PUBLISH_REPO" add src/unit.c
git -C "$PUBLISH_REPO" commit -q -m bad-ledger-payload
export MOCK_PR_JSON='not-json'
publish_lane "$PUBLISH_REPO|lane/bad-ledger|Bad ledger lane" 2>/dev/null
if git --git-dir="$REMOTE_DIR" show-ref --verify --quiet refs/heads/lane/bad-ledger; then
  echo "publisher did not fail closed on invalid PR data" >&2
  exit 1
fi

PAUSE_FILE="$TMP/paused"
LOCK="$TMP/publisher.lock"
LANES=("$TEST_LANE")
FS_PUSH_ONCE=1
touch "$PAUSE_FILE"
main
[ "$(wc -l < "$MOCK_GH_LOG" | tr -d ' ')" = 2 ]

echo "fs_push integration tests passed"
