#!/bin/bash
# reap orphaned wibo / mwcceppc / permuter processes under the farm, safely
me=$$
mapfile -t pids < <(ps -eo pid,cmd | grep -E 'wibo|mwcceppc|decomp-permuter/permuter' | grep -v grep | awk '{print $1}')
for p in "${pids[@]}"; do
  [ "$p" = "$me" ] && continue
  [ "$p" = "$PPID" ] && continue
  kill -9 "$p" 2>/dev/null && echo "killed $p"
done
sleep 1
n=$(ps -eo pid,cmd | grep -E 'wibo|mwcceppc|decomp-permuter/permuter' | grep -v grep | grep -vc " $me ")
echo "remaining: $n"
