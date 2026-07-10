# Fleet: decomp campaign process supervision

Everything needed to run the autonomous decompilation fleet on this machine.
After a reboot or crash: `bash fleet/fleet-up.sh` — idempotent, brings up
colima/kernel-db, the three from-scratch lanes, dashboard, watchdog, and loops.

- `fleet-up.sh` — one-command bootstrap (also loaded at boot via launchd, see below)
- `overnight_watchdog.sh` — reaps hung sessions, restarts dead lanes/loops,
  recovers phantom claims (dead-PID detection), drain/rescope hints, worktree GC,
  strike-note KG ingest
- `crack_watch.sh` — appends newly scored/cracked targets to /tmp/grind/crack_watch.log
- `fs_push.sh` — pushes lane branches, keeps PRs open (open-PR check, not merged)
- `*_strike_loop.sh` — persistent codex strike loops (sol heavies / gpt-5.5 mid / spark smalls)
- `strike_notes_ingest.py` — SOL_NOTES.jsonl → KG path_facts (idempotent)
- `verify_fn.sh` — THE per-function match check (objdiff fuzzy, never DOL SHA)

launchd (auto-start watchdog at boot):
  cp fleet/com.dougchansan.decomp-fleet.plist ~/Library/LaunchAgents/
  launchctl load ~/Library/LaunchAgents/com.dougchansan.decomp-fleet.plist
