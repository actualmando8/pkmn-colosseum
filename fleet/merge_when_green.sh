#!/bin/bash
# merge_when_green.sh <pr-number> — race-free, format-safe merge gate.
# History: the tab-separated `gh pr checks` output made awk read '(GC6E01)' as
# the status column, and an empty API response parsed as "all green" (that
# combination merged red PR #38 and blocked green PR #40). This version uses
# structured JSON, requires >=4 completed checks, and fails closed on empty.
PR=${1:?pr number}
REPO=dougchansan/pkmn-colosseum
while true; do
  VERDICT=$(gh pr view "$PR" --repo "$REPO" --json statusCheckRollup 2>/dev/null | python3 -c "
import json, sys
try:
    checks = json.load(sys.stdin)['statusCheckRollup']
except Exception:
    print('ERROR'); raise SystemExit
if not checks or len(checks) < 4:
    print('WAIT'); raise SystemExit
if any(c.get('status') != 'COMPLETED' for c in checks):
    print('WAIT'); raise SystemExit
print('GREEN' if all(c.get('conclusion') == 'SUCCESS' for c in checks) else 'RED')
")
  case "$VERDICT" in
    GREEN)
      gh pr merge "$PR" --repo "$REPO" --merge >/dev/null 2>&1 && echo "PR$PR-MERGED" && exit 0
      echo "PR$PR-MERGE-FAILED"; exit 1 ;;
    RED)
      gh pr view "$PR" --repo "$REPO" --json statusCheckRollup --jq '.statusCheckRollup[] | "\(.name): \(.conclusion)"'
      echo "PR$PR-RED"; exit 1 ;;
    *) sleep 30 ;;
  esac
done
