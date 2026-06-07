#!/usr/bin/env python3
"""pipeline_dash.py — the cohesive INTEGRATED DECOMP PIPELINE view that ties the
whole research stack together as one flow:

  QUEUE -> [T2 LLM Best-of-N seed] -> CANDIDATE POOL -> [T1 POPULATION-ANNEALING
  swarm (parallel tempering, 32 cores)] -> [T3 surrogate predict] -> VERIFY (100%)
  -> COMMIT (decomp.dev).

Reads .omc/permuter_state.json (grind2 swarm), .omc/agent_tokens.json (LLM agents),
report.json (decomp.dev metrics), and git (merged matches). Run with WSL python3."""
import json, os, sys, time, subprocess

REPO = "/mnt/c/Users/douglaswhittingham/pkmn-colosseum"
STATE = os.path.join(REPO, ".omc", "permuter_state.json")
TOKENS = os.path.join(REPO, ".omc", "agent_tokens.json")
POOL = os.path.join(REPO, ".omc", "permuter_pool")
REPORT = os.path.join(REPO, "report.json")
TRIAGE = os.path.join(REPO, ".omc", "triage_routes.json")        # triage_gate.py
ORACLE = os.path.join(REPO, ".omc", "coloring_oracle.json")      # coloring_oracle.py
FIRSTDEF = os.path.join(REPO, ".omc", "firstdef_plan.json")      # firstdef_invert.py

def fg(n): return f"\x1b[38;5;{n}m"
R = "\x1b[0m"; BD = "\x1b[1m"
CY = fg(51); GR = fg(46); YL = fg(226); OR = fg(208); RD = fg(196)
DM = fg(240); WH = fg(255); MG = fg(201); BL = fg(39); TEAL = fg(45); VI = fg(135)
GRAD = [fg(39), fg(45), fg(51), fg(87), fg(123)]


def jload(p, d):
    try:
        return json.load(open(p))
    except Exception:
        return d


def merged_matches():
    try:
        out = subprocess.run(["git", "-C", REPO, "log", "--oneline", "-40"],
                             capture_output=True, text=True, timeout=6).stdout
        return sum(1 for l in out.splitlines() if "via " in l and "100%" in l)
    except Exception:
        return 0


def box(label, lines, color, w=22):
    top = color + "+" + "-" * (w - 2) + "+" + R
    out = [top, color + "|" + R + BD + color + label.center(w - 2) + R + color + "|" + R]
    for ln in lines:
        out.append(color + "|" + R + ln[:w - 2].center(w - 2) + color + "|" + R)
    out.append(top)
    return out


def render(frame, tw, th):
    st = jload(STATE, {}); tok = jload(TOKENS, {}); rep = jload(REPORT, {}).get("measures", {})
    triage = jload(TRIAGE, {}); oracle = jload(ORACLE, {}); fdef = jload(FIRSTDEF, {})
    tcounts = triage.get("counts", {})
    act = st.get("active") or {}
    pool = sorted(os.listdir(POOL)) if os.path.isdir(POOL) else []
    done = st.get("done") or []
    wins = st.get("wins") or []
    queue = st.get("queue") or []
    agents = (tok.get("agents") or {})
    busy_llm = [n for n, a in agents.items() if a.get("status") == "busy"]
    cores = st.get("cores", "?"); workers = st.get("workers", "?")
    best_e = None
    for a in act.values():
        b = a.get("best")
        if b is not None and (best_e is None or b < best_e):
            best_e = b

    out = []
    title = "  I N T E G R A T E D   D E C O M P   P I P E L I N E  "
    out.append("")
    out.append("  " + "".join(GRAD[(i + frame) % len(GRAD)] + c for i, c in enumerate(title)) + R)
    out.append("  " + DM + "every wall function flows through all tiers; matches land on decomp.dev" + R)
    out.append("")

    # --- map each tracked function to its current pipeline stage + a stable number
    actfns = [v.get("fn") for v in act.values()]
    donefns = {d["fn"]: d.get("result") for d in done}
    allfns = list(dict.fromkeys(list(queue) + actfns + list(donefns)))
    num = {f: i + 1 for i, f in enumerate(allfns)}
    at = {"QUEUE": [], "ANNEAL": [], "POOL": [], "COMMIT": []}
    for f in queue:
        at["QUEUE"].append(f)
    for f in actfns:
        at["ANNEAL"].append(f)
    for f, res in donefns.items():
        at["COMMIT" if res == "WIN" else "POOL"].append(f)

    # pokeball per stage: empty(queue) -> spinning red(annealing/capturing) ->
    # wobble(pool) -> solid green(committed = caught!)
    POKE = {"QUEUE": (fg(245), "o"), "ANNEAL": (RD, "@"), "POOL": (YL, "0"), "COMMIT": (GR, "*")}
    SPIN = "◓◑◒◐"   # ◓◑◒◐ rotating half-circle = capture shake

    def toks(key, color):
        items = at.get(key, [])
        if not items:
            return DM + "  -" + R
        pcol, pball = POKE.get(key, (color, "o"))
        s = ""
        for f in items[:5]:
            ball = pball
            if key == "ANNEAL":
                ball = SPIN[(frame + num[f]) % 4]; pcol = RD
            elif key == "COMMIT":
                ball = "●"  # ● caught
            elif key == "QUEUE":
                ball = "○"  # ○ empty ball
            elif key == "POOL":
                ball = SPIN[(frame // 2 + num[f]) % 4]  # gentle wobble
            glow = (GRAD[(num[f] + frame) % len(GRAD)] + BD) if key == "ANNEAL" else color
            s += " " + pcol + BD + ball + R + glow + f"#{num[f]}" + R + DM + ":" + R + WH + f.replace("fn_", "") + R
        return s

    # research-derived routing counts (triage gate over the wall inventory)
    n_anneal_route = tcounts.get("ANNEAL", 0)
    n_invert = tcounts.get("ALLOC_INVERT", 0)
    n_park = tcounts.get("PARK_SCHEDULING", 0) + tcounts.get("PARK_STRUCTURAL", 0)
    n_pure = (oracle.get("counts", {}) or {}).get("PURE_RENAME", 0)
    n_plans = fdef.get("count", 0)

    # the integrated pipeline — VERTICAL flow. QUEUE fans through the TRIAGE GATE
    # into three tracks (anneal / allocator-inversion / park), then verify+commit.
    stages = [
        (BL,   "T0", "QUEUE",    f"{len(queue)} queued", toks("QUEUE", BL)),
        (WH,   "RA0","TRIAGE",   f"classify: {n_anneal_route} anneal | {n_invert} invert | {n_park} park", DM + "  penalty-decomp gate" + R),
        (VI,   "T2", "LLM SEED", f"{len(busy_llm)} agents seeding", DM + "  Best-of-N C" + R),
        (TEAL, "  ", "POOL",     f"{len(pool)} partials", toks("POOL", TEAL)),
        (MG,   "T1", "ANNEAL",   f"{len(act)} annealing (small-residual)", toks("ANNEAL", MG)),
        (fg(199),"RA1","ORACLE", f"{n_pure} pure-rename (saved-reg bijection)", DM + "  coloring-aware diff" + R),
        (fg(214),"RA2","INVERT", f"{n_plans} directed candidates (1 each, not N!)", DM + "  first-def order solve" + R),
        (YL,   "T3", "PREDICT",  "allocator surrogate (model)", DM + "  ALLOCATOR_MODEL.md" + R),
        (GR,   "  ", "VERIFY",   "compile_check==100%", DM + "  -" + R),
        (GR,   "  ", "COMMIT",   f"{merged_matches()} merged", toks("COMMIT", GR)),
    ]
    travel = frame % len(stages)  # a marker that descends through the stages
    for i, (col, tier, name, detail, tokens) in enumerate(stages):
        head = (">>" if i == travel else "  ")
        out.append("  " + (GRAD[frame % len(GRAD)] + BD + head + R) + col + BD + f"{tier} " + R
                   + col + f"{name:<9}" + R + DM + f" {detail}" + R + tokens)
        if i < len(stages) - 1:
            arr = (GRAD[(frame) % len(GRAD)] + BD + "  V" + R) if i == travel else (DM + "  v" + R)
            out.append("  " + arr)
    out.append("")

    # NEW WINNABLE TRACK: allocator-inversion (what cracks the pure-reg walls the
    # annealer provably can't). Shows the oracle's pure-rename targets + the
    # first-def directed recipe — straight from the research.
    out.append("  " + fg(214) + BD + "ALLOCATOR-INVERSION TRACK" + R + DM
               + "  (pure-reg walls SA can't reach -> directed first-definition solve)" + R)
    plans = (fdef.get("plans") or [])[:5]
    if plans:
        for p in plans:
            order = " -> ".join(p.get("first_definition_order", []))
            rs = p.get("random_search_space", 0)
            rs_s = f"{rs:,}" if isinstance(rs, int) else str(rs)
            out.append("    " + WH + f"{p['fn']:<14}" + R + DM + " dist=" + R + fg(214) + f"{p.get('swap_distance','?')}" + R
                       + DM + "  directed=" + R + GR + f"{p.get('directed_candidates','?')}" + R
                       + DM + " vs " + R + RD + rs_s + R + DM + " random" + R
                       + DM + "   first-def: " + R + CY + order + R)
    else:
        out.append("    " + DM + "run triage_gate.py + firstdef_invert.py to populate" + R)
    out.append("")

    # live swarm detail
    out.append("  " + MG + BD + "ANNEALING SWARM" + R + DM
               + f"  (population annealing / parallel tempering across {cores} cores)" + R)
    if act:
        for k, a in list(act.items())[:5]:
            e = "--" if a.get("score") is None else str(a.get("score"))
            b = "--" if a.get("best") is None else str(a.get("best"))
            bar_n = 0
            sc, bs = a.get("start") if False else None, None
            out.append("    " + WH + f"{a.get('fn',''):<14}" + R + DM + f"r{a.get('replica','?')}" + R
                       + DM + " iter " + R + WH + f"{a.get('iter',0):>6,}" + R
                       + DM + "  E=" + R + YL + f"{e:<6}" + R + DM + " best=" + R + GR + b + R)
    else:
        out.append("    " + DM + "swarm idle" + R)
    if best_e is not None:
        out.append("    " + CY + f"swarm best energy: {best_e}" + R + DM + "   (0 = byte-exact match)" + R)
    out.append("")

    # LLM tier
    out.append("  " + VI + BD + "LLM TIER " + R + DM + "(seed + stub C; Best-of-N)" + R)
    for n, a in agents.items():
        used = a.get("tokens_used", 0); lim = a.get("limit", 1)
        pct = clampi(int(100 * used / max(1, lim)))
        col = GR if a.get("status") == "busy" else DM
        tb = bar(pct / 100.0, 16)
        out.append("    " + col + f"{n:<11}" + R + DM + a.get("model", "")[:16].ljust(17) + R
                   + tb + f" {pct:>3d}% ctx" + DM + f"  {a.get('landed',0)} landed" + R)
    out.append("")

    # outcomes + decomp.dev
    if done:
        ds = "  ".join((GR + "[WIN]" + d["fn"] + R) if d.get("result") == "WIN"
                       else (DM + d["fn"] + R) for d in done[-6:])
        out.append("  " + GR + BD + "RESULTS " + R + ds)
    if wins:
        out.append("  " + GR + BD + "*** MATCHES: " + ", ".join(wins) + " ***" + R)
    if rep:
        out.append("  " + CY + BD + "decomp.dev  " + R
                   + GR + f"Code {rep.get('matched_code_percent',0):.2f}%" + R + DM + "   " + R
                   + YL + f"Fuzzy {rep.get('fuzzy_match_percent',0):.2f}%" + R + DM + "   " + R
                   + CY + f"Funcs {rep.get('matched_functions_percent',0):.2f}%" + R)
    return out


def clampi(v): return max(0, min(100, v))


def bar(frac, w):
    f = int(frac * w)
    s = ""
    for i in range(w):
        s += (GR if i < f else DM) + ("|" if i < f else ".") + R
    return "[" + s + "]"


def main():
    once = "--once" in sys.argv
    sys.stdout.write("\x1b[?25l")
    frame = 0
    try:
        while True:
            try:
                tw, th = os.get_terminal_size()
            except OSError:
                tw, th = 120, 30
            sys.stdout.write("\x1b[?2026h\x1b[H" + "".join(ln + R + "\x1b[K\n" for ln in render(frame, tw, th)[:th]) + "\x1b[J\x1b[?2026l")
            sys.stdout.flush()
            if once:
                break
            frame += 1
            time.sleep(0.4)
    except KeyboardInterrupt:
        pass
    finally:
        sys.stdout.write("\x1b[?25h" + R + "\n"); sys.stdout.flush()


if __name__ == "__main__":
    main()
