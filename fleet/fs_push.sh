#!/usr/bin/env bash
# Publish substantive from-scratch epoch changes. A branch is pushed only when
# its local tip contains new source/header work relative to the remote tip, and
# a PR is opened only when that exact tip has never been posted before.
set -uo pipefail

LOCK=${FS_PUSH_LOCK:-/tmp/grind/fs_push.lock}
PAUSE_FILE=${FS_PUSH_PAUSE_FILE:-/tmp/grind/harness-paused.txt}
POLL_SECONDS=${FS_PUSH_POLL_SECONDS:-1200}
REPO=${FS_PUSH_REPO:-dougchansan/pkmn-colosseum}
PUBLISH_PATHS=(src include)

# "worktree|branch|title" triples
LANES=(
  "/Users/douglaswhittingham/pkmn-colosseum-fromscratch|harness/from-scratch|From-scratch decomp lane: SMALL (<=256B fresh fns)"
  "/Users/douglaswhittingham/pkmn-colosseum-fs-medium|harness/fs-medium|From-scratch decomp lane: MEDIUM (257-1024B fresh fns)"
  "/Users/douglaswhittingham/pkmn-colosseum-fs-large|harness/fs-large|From-scratch decomp lane: LARGE (>=1025B fresh fns)"
)

log() {
  printf '[%s] %s\n' "$(date '+%H:%M')" "$*"
}

meaningful_changes_between() {
  local base=$1 head=$2
  ! git diff --quiet "$base" "$head" -- "${PUBLISH_PATHS[@]}"
}

publish_lane() {
  local lane=$1 wt rest branch title head remote_ref remote_head base
  local pr_json same_head_pr open_pr

  wt=${lane%%|*}
  rest=${lane#*|}
  branch=${rest%%|*}
  title=${rest#*|}
  cd "$wt" 2>/dev/null || { log "skip $branch: missing worktree $wt"; return 0; }

  git fetch --quiet origin "+refs/heads/master:refs/remotes/origin/master" 2>/dev/null || {
    log "skip $branch: failed to fetch origin/master"
    return 0
  }
  git fetch --quiet origin "+refs/heads/$branch:refs/remotes/origin/$branch" 2>/dev/null || true

  head=$(git rev-parse --verify --quiet "$branch^{commit}" 2>/dev/null) || {
    log "skip $branch: cannot resolve local branch"
    return 0
  }

  # Do not publish source wins on a stale epoch branch. Those PRs replay old
  # history, conflict with already-merged work, and obscure the actual delta.
  # The lane must first be reconciled so current origin/master is its ancestor.
  if ! git merge-base --is-ancestor origin/master "$head" 2>/dev/null; then
    log "skip $branch: head is behind or diverged from origin/master; reconcile first"
    return 0
  fi

  remote_ref="refs/remotes/origin/$branch"
  remote_head=$(git rev-parse --verify --quiet "$remote_ref^{commit}" 2>/dev/null) || remote_head=

  # GitHub is the publication ledger. Fail closed if it cannot be read: a
  # transient API problem must never create another duplicate PR.
  if ! pr_json=$(gh pr list --repo "$REPO" --head "$branch" --state all \
      --limit 1000 --json number,state,headRefOid 2>/dev/null); then
    log "skip $branch: failed to read prior PR heads"
    return 0
  fi
  if ! same_head_pr=$(printf '%s' "$pr_json" | jq -r --arg head "$head" \
      '[.[] | select(.headRefOid == $head) | .number][0] // empty'); then
    log "skip $branch: failed to parse prior PR heads"
    return 0
  fi
  if [ -n "$same_head_pr" ]; then
    log "skip $branch: head ${head:0:10} already posted as PR #$same_head_pr"
    return 0
  fi
  if ! open_pr=$(printf '%s' "$pr_json" | jq -r \
      '[.[] | select(.state == "OPEN") | .number][0] // empty'); then
    log "skip $branch: failed to parse open PR state"
    return 0
  fi

  if [ -n "$remote_head" ] && [ "$head" != "$remote_head" ]; then
    # Epoch branches are expected to advance linearly. Never force-push over a
    # diverged remote; leave it for an explicit reconciliation.
    if ! git merge-base --is-ancestor "$remote_head" "$head"; then
      log "skip $branch: local and remote histories diverged"
      return 0
    fi
    base=$remote_head
  elif [ -n "$remote_head" ]; then
    # A previous push may have succeeded before PR creation failed. Permit one
    # retry only when this exact head has no PR ledger entry.
    base=$(git merge-base origin/master "$head" 2>/dev/null || true)
  else
    base=$(git merge-base origin/master "$head" 2>/dev/null || true)
  fi

  if [ -z "$base" ] || ! meaningful_changes_between "$base" "$head"; then
    log "skip $branch: no new source/header changes at ${head:0:10}"
    return 0
  fi

  if [ "$head" != "$remote_head" ]; then
    if ! git push origin "$head:refs/heads/$branch"; then
      log "ERROR failed to push $branch at ${head:0:10}"
      return 0
    fi
    log "pushed $branch at ${head:0:10}"
  fi

  if [ -n "$open_pr" ]; then
    log "updated PR #$open_pr for $branch"
    return 0
  fi

  if gh pr create --repo "$REPO" --base master --head "$branch" \
      --title "$title" \
      --body "Automated epoch changes from the size-tiered from-scratch harness lane. Published only when a new epoch contains source/header changes; identical and metadata-only heads are suppressed by fleet/fs_push.sh."; then
    log "opened PR for $branch at ${head:0:10}"
  else
    log "ERROR failed to create PR for $branch at ${head:0:10}"
  fi
}

main() {
  while true; do
    (
      flock -n 9 || exit 0
      if [ -f "$PAUSE_FILE" ]; then
        log "fleet paused; publication suppressed"
      else
        for lane in "${LANES[@]}"; do
          publish_lane "$lane"
        done
      fi
    ) 9>"$LOCK"
    [ "${FS_PUSH_ONCE:-0}" = 1 ] && break
    sleep "$POLL_SECONDS"
  done
}

if [ "${BASH_SOURCE[0]}" = "$0" ]; then
  main "$@"
fi
