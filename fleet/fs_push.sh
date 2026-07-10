#!/usr/bin/env bash
# Unified from-scratch push->PR loop. Every 20 min, for each size-tiered lane
# worktree, push its branch to origin (if ahead of origin/master) and ensure a PR
# exists. Covers all 3 from-scratch lanes: small (harness/from-scratch, reuses the
# original fromscratch worktree), medium (harness/fs-medium), large (harness/fs-large).
set -uo pipefail
LOCK=/tmp/grind/fs_push.lock
# "worktree|branch|title" triples
LANES=(
  "/Users/douglaswhittingham/pkmn-colosseum-fromscratch|harness/from-scratch|From-scratch decomp lane: SMALL (<=256B fresh fns)"
  "/Users/douglaswhittingham/pkmn-colosseum-fs-medium|harness/fs-medium|From-scratch decomp lane: MEDIUM (257-1024B fresh fns)"
  "/Users/douglaswhittingham/pkmn-colosseum-fs-large|harness/fs-large|From-scratch decomp lane: LARGE (>=1025B fresh fns)"
)
while true; do
  ( flock -n 9 || exit 0
    for lane in "${LANES[@]}"; do
      WT=${lane%%|*}; rest=${lane#*|}; BR=${rest%%|*}; TITLE=${rest#*|}
      cd "$WT" 2>/dev/null || continue
      ahead=$(git rev-list --count origin/master.."$BR" 2>/dev/null || echo 0)
      if [ "${ahead:-0}" -gt 0 ]; then
        git push -u origin "$BR" 2>&1 | tail -1
        if [ -z "$(gh pr list --head "$BR" --state open --json number --jq '.[].number' 2>/dev/null)" ]; then
          gh pr create --base master --head "$BR" \
            --title "$TITLE" \
            --body "Automated epoch commits from the size-tiered from-scratch harness lane (fuzzy-0 functions decompiled from scratch). Auto-updated by .handoff/fs_push.sh." 2>&1 | tail -1
        fi
        echo "[$(date '+%H:%M')] pushed $BR ($ahead ahead)"
      else echo "[$(date '+%H:%M')] no new commits on $BR"; fi
    done
  ) 9>"$LOCK"
  sleep 1200
done
