#!/usr/bin/env bash
# permuter_poll.sh — poll the Windows WSL permuter swarm over Tailscale and write
# build/permuter_status.json for the dashboard. The permuter (anneal_supervisor +
# grind2.py) runs on the Windows box's CPU; this just reports its liveness/load.
set -uo pipefail
cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/../.." || exit 1
WIN="${PERMUTER_HOST:-win}"
KEY="${PERMUTER_KEY:-$HOME/.ssh/id_ed25519}"
OUT="build/permuter_status.json"
INTERVAL="${PERMUTER_POLL_INTERVAL:-60}"

# remote status script (base64'd to dodge ssh/wsl/bash quoting)
read -r -d '' REMOTE <<'EOF'
cd /mnt/c/Users/douglaswhittingham/pkmn-colosseum 2>/dev/null || exit 0
LOG=tools/decomp_work/permuter/logs/anneal_supervisor.out
last=$(tail -1 "$LOG" 2>/dev/null | tr -d '"\\' | tr -s ' ')
# liveness via log freshness (robust vs pgrep name quirks + the 5s inter-cycle gap)
now=$(date +%s); m=$(stat -c %Y "$LOG" 2>/dev/null || echo 0)
python3 - "$last" "$now" "$m" <<'PY'
import json
import os
import subprocess
import sys

last, now_s, m_s = sys.argv[1], sys.argv[2], sys.argv[3]

def pgrep_count(pattern):
    try:
        out = subprocess.check_output(["pgrep", "-fc", pattern], text=True).strip()
        return int(out or "0")
    except Exception:
        return 0

def load(path, default):
    try:
        with open(path, encoding="utf-8") as fh:
            return json.load(fh)
    except Exception:
        return default

state = load(".omc/permuter_state.json", {})
queue_file = load(".omc/permuter_queue.json", [])
active = state.get("active") or {}
done = state.get("done") or []
wins = state.get("wins") or []
grind_processes = pgrep_count("grind2.py")
permuter_processes = pgrep_count("permuter.py")
alive = bool(grind_processes or permuter_processes)
try:
    alive = alive or (int(now_s) - int(m_s) < 300)
except Exception:
    pass
active_targets = sorted({v.get("fn") for v in active.values() if v.get("fn")})
summary = {
    "alive": alive,
    "cores": state.get("cores"),
    "profile": state.get("profile"),
    "workers": state.get("workers", grind_processes),
    "jobs": state.get("jobs"),
    "replicas": state.get("replicas", 1),
    "budget": state.get("budget"),
    "effective_slots": state.get("effective_slots"),
    "active": len(active_targets),
    "active_targets": active_targets,
    "queued": len(state.get("queue") or []),
    "targets": len(queue_file),
    "done": len(done),
    "wins": len(wins),
    "grind_processes": grind_processes,
    "permuter_processes": permuter_processes,
    "last": last,
}
print(json.dumps(summary, separators=(",", ":")))
PY
EOF
B64=$(printf '%s' "$REMOTE" | base64 | tr -d '\n')

echo "[permuter_poll] polling $WIN every ${INTERVAL}s -> $OUT"
while :; do
  js=$(ssh -o ConnectTimeout=20 -o ServerAliveInterval=5 -i "$KEY" "$WIN" \
        "C:\\Windows\\System32\\wsl.exe -e bash -c \"echo $B64 | base64 -d | bash\"" 2>/dev/null | tail -1)
  if printf '%s' "$js" | python3 -c "import sys,json;json.load(sys.stdin)" 2>/dev/null; then
    printf '%s' "$js" > "$OUT"
  else
    printf '{"alive":false,"workers":0,"targets":0,"last":"(unreachable)"}\n' > "$OUT"
  fi
  sleep "$INTERVAL"
done
