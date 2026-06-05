#!/usr/bin/env python3
"""research_log_view.py — live colored viewer for the research activity log
(.omc/research_log.jsonl, written by research_daemon.py). Newest at the bottom,
stage-colored, so you can watch the triage / oracle / inversion / swarm work as
it happens. Run with WSL python3. Ctrl-C to exit.

  python3 research_log_view.py            # live tail
  python3 research_log_view.py --once     # one frame (for capture)"""
import json, os, sys, time

REPO = "/mnt/c/Users/douglaswhittingham/pkmn-colosseum"
LOG = os.path.join(REPO, ".omc", "research_log.jsonl")


def fg(n): return f"\x1b[38;5;{n}m"
R = "\x1b[0m"; BD = "\x1b[1m"
DM = fg(240); WH = fg(255)
STAGE_COL = {
    "DAEMON": fg(245), "TRIAGE": fg(255), "ORACLE": fg(199), "INVERT": fg(214),
    "SWARM": fg(51), "VERIFY": fg(46), "PREDICT": fg(226),
}
LEVEL_COL = {"win": fg(46), "best": fg(118), "invert": fg(214), "route": fg(213),
             "warn": fg(196), "info": fg(245)}


def load_events():
    try:
        lines = open(LOG, errors="replace").read().splitlines()
    except OSError:
        return []
    out = []
    for ln in lines:
        try:
            out.append(json.loads(ln))
        except Exception:
            pass
    return out


def fmt_time(ts):
    try:
        return time.strftime("%H:%M:%S", time.localtime(ts))
    except Exception:
        return "--:--:--"


def render(evs, tw, th):
    out = []
    title = "  R E S E A R C H   A C T I V I T Y   L O G  "
    out.append("")
    out.append("  " + fg(213) + BD + title + R + DM
               + "  triage -> oracle -> allocator-inversion + annealing swarm" + R)
    out.append("  " + DM + "-" * min(tw - 4, 96) + R)
    # legend
    out.append("  " + DM + "stages: " + R + STAGE_COL["TRIAGE"] + "TRIAGE " + R
               + STAGE_COL["ORACLE"] + "ORACLE " + R + STAGE_COL["INVERT"] + "INVERT " + R
               + STAGE_COL["SWARM"] + "SWARM " + R + STAGE_COL["VERIFY"] + "VERIFY" + R)
    out.append("")
    body_h = max(4, th - len(out) - 1)
    for ev in evs[-body_h:]:
        scol = STAGE_COL.get(ev.get("stage", ""), DM)
        lcol = LEVEL_COL.get(ev.get("level", "info"), WH)
        stage = ev.get("stage", "?")[:6].ljust(6)
        fn = ev.get("fn") or ""
        fnpart = (" " + fg(45) + f"{fn:<13}" + R) if fn else " " + " " * 13
        star = "  "
        if ev.get("level") == "win":
            star = fg(46) + BD + "**" + R
        elif ev.get("level") in ("best", "invert"):
            star = lcol + "> " + R
        line = ("  " + DM + fmt_time(ev.get("ts", 0)) + R + " "
                + scol + BD + stage + R + fnpart + " " + star + lcol + ev.get("msg", "") + R)
        out.append(line[:tw + 80])   # allow for ansi width
    return out


def main():
    once = "--once" in sys.argv
    sys.stdout.write("\x1b[?25l")
    try:
        while True:
            try:
                tw, th = os.get_terminal_size()
            except OSError:
                tw, th = 110, 40
            evs = load_events()
            lines = render(evs, tw, th)
            sys.stdout.write("\x1b[H\x1b[2J" + "\n".join(lines[:th]) + R + "\x1b[J")
            sys.stdout.flush()
            if once:
                break
            time.sleep(0.5)
    except KeyboardInterrupt:
        pass
    finally:
        sys.stdout.write("\x1b[?25h" + R + "\n"); sys.stdout.flush()


if __name__ == "__main__":
    main()
