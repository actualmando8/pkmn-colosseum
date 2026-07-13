# Fleet: decomp campaign process supervision

Everything needed to run the autonomous decompilation fleet on this machine.
After a reboot or crash: `bash fleet/fleet-up.sh` — idempotent, brings up
colima/kernel-db, the three from-scratch lanes, dashboard, watchdog, and loops.

- `fleet-up.sh` — one-command bootstrap (also loaded at boot via launchd, see below)
- `overnight_watchdog.sh` — reaps hung sessions, restarts dead lanes/loops,
  verifies real run-loop PIDs (not only tmux names), respects the fleet pause
  sentinel, recovers phantom claims, and performs worktree GC/KG ingest
- `crack_watch.sh` — appends newly scored/cracked targets to /tmp/grind/crack_watch.log
- `fs_push.sh` — publishes only new source/header epoch changes; suppresses
  paused, metadata-only, divergent, and previously posted branch heads
- `*_strike_loop.sh` — persistent codex strike loops (sol heavies / gpt-5.5 mid / spark smalls)
- `strike_notes_ingest.py` — SOL_NOTES.jsonl → KG path_facts (idempotent)
- `verify_fn.sh` — THE per-function match check (objdiff fuzzy, never DOL SHA)

launchd (auto-start watchdog at boot):
  cp fleet/com.dougchansan.decomp-fleet.plist ~/Library/LaunchAgents/
  launchctl load ~/Library/LaunchAgents/com.dougchansan.decomp-fleet.plist
