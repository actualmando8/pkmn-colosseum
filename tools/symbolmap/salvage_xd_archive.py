#!/usr/bin/env python3
"""salvage_xd_archive.py - revalidate the archived campaign's XD-port
evidence against the CURRENT symbols.txt, without needing a live XD
(GXXE01) disc/asm tree.

Why this exists
----------------
The previous campaign had a real Pokemon XD asm split wired up at some
point and used it to produce two evidence files that are still checked
into `archive/previous_campaign/config/GC6E01/symbolmap/`:

  xd_port.json          cross-game shared-string-literal name ports
                        (Stage 3 PORT, see port_xd.py) + self-name
                        proposal validation against XD's name vocabulary
                        (Stage 3 VALIDATE).
  structural_ports.json  cross-game identical-mnemonic-fingerprint name
                        ports (see structural_port.py).

That XD asm/dol is not available in this tree (no disc image was
committed - see the README "Enabling The XD Port" section for how to
provide one). But the *evidence itself* does not depend on having XD data
locally: every proposal is keyed by a Colosseum function's OWN address
(`fn_ADDRESS` / `lbl_ADDRESS`), and the DOL being ported (GC6E01) hasn't
changed, so those addresses are still meaningful today. What HAS likely
changed since the archive was written is how many of those functions are
still unnamed - substantial decomp work has landed since then.

This script re-derives each archived proposal's target address from its
`fn_`/`lbl_` name and looks it up in the CURRENT config/GC6E01/symbols.txt,
classifying it as:

  still-open        current symbol at that address is still fn_/lbl_ -
                    the proposal remains a live, actionable naming lead.
  already-applied   current symbol has a real name that matches (exactly,
                    or as `<xd_name>_<selfaddr>` - a naming convention
                    already used in this project for disambiguated ports,
                    e.g. `generateParticle_8017424C`) - independent
                    confirmation the archived evidence was correct.
  already-conflict  current symbol has a real name that does NOT match -
                    surfaced for awareness only, never auto-applied.
  address-not-found  no function symbol at that address anymore (should be
                    rare - the DOL is fixed - but split/merge of a range
                    could shift a boundary).

Only `still-open` entries are written back into the pipeline's native
xd_port.json / structural_ports.json so they compose with build_symbol_map.py
exactly like freshly-mined evidence would. Everything (including the other
three buckets, for transparency and QA) is written to
structural_port_salvage.md / xd_port.md.

If a *live* port_xd.py run already produced entries in xd_port.json (i.e.
XD data has since been wired up), this script merges additively and never
overwrites a live entry.

Usage:
    python tools/symbolmap/salvage_xd_archive.py \
        --archive-dir archive/previous_campaign/config/GC6E01/symbolmap \
        --sm-dir config/GC6E01/symbolmap \
        --symbols config/GC6E01/symbols.txt
"""

import argparse
import json
import re
from pathlib import Path

FN_ADDR = re.compile(r"^(?:fn|lbl)_([0-9A-Fa-f]{8})$")
SYM_LINE = re.compile(
    r"^(?P<name>\S+)\s*=\s*\.\w+:0x(?P<addr>[0-9A-Fa-f]+);.*type:function")
TRAILING_ADDR = re.compile(r"_[0-9A-Fa-f]{8}$")


def load_addr_index(symbols: Path) -> dict:
    """8-hex-upper addr -> current symbol name, for every function symbol."""
    idx = {}
    for line in symbols.read_text(encoding="utf-8").splitlines():
        m = SYM_LINE.match(line)
        if m:
            idx[m.group("addr").upper()] = m.group("name")
    return idx


def classify(fn: str, proposed_name: str, addr_index: dict):
    """-> (status, addr_or_None, current_name_or_None)"""
    m = FN_ADDR.match(fn)
    if not m:
        return "skip-already-named-at-archive-time", None, None
    addr = m.group(1).upper()
    cur = addr_index.get(addr)
    if cur is None:
        return "address-not-found", addr, None
    if FN_ADDR.match(cur):
        return "still-open", addr, cur
    # Both sides may carry a disambiguating `_<selfaddr>` suffix - this
    # project's own convention (e.g. `generateParticle_8017424C`) AND XD's
    # archived proposal (e.g. `generateParticle_801947D4`, XD's OWN
    # address). Strip both before comparing so the comparison is on the
    # semantic base name, not on which game's address got appended.
    base_cur = TRAILING_ADDR.sub("", cur)
    base_proposed = TRAILING_ADDR.sub("", proposed_name)
    if cur == proposed_name or base_cur == base_proposed:
        return "already-applied", addr, cur
    return "already-conflict", addr, cur


def salvage_structural(archive_dir: Path, addr_index: dict):
    p = archive_dir / "structural_ports.json"
    if not p.is_file():
        return [], {}
    entries = json.loads(p.read_text(encoding="utf-8"))
    buckets = {"still-open": [], "already-applied": [], "already-conflict": [],
               "address-not-found": [], "skip-already-named-at-archive-time": []}
    for e in entries:
        status, addr, cur = classify(e["fn"], e["xd_name"], addr_index)
        row = dict(e, salvage_status=status, addr=f"0x{addr}" if addr else None,
                   current_name=cur)
        buckets[status].append(row)
    return entries, buckets


def salvage_xd_port(archive_dir: Path, addr_index: dict):
    p = archive_dir / "xd_port.json"
    if not p.is_file():
        return {"corroborated": [], "ported": []}, {}
    data = json.loads(p.read_text(encoding="utf-8"))
    buckets = {"still-open": [], "already-applied": [], "already-conflict": [],
               "address-not-found": [], "skip-already-named-at-archive-time": []}
    for e in data.get("ported", []):
        status, addr, cur = classify(e["fn"], e["name"], addr_index)
        row = dict(e, salvage_status=status, addr=f"0x{addr}" if addr else None,
                   current_name=cur)
        buckets[status].append(row)
    return data, buckets


def merge_native(sm_dir: Path, filename: str, key: str, still_open_rows: list):
    """Merge still-open salvage rows into an existing (possibly live-run)
    native artifact, keyed by `key` (usually 'fn'), never overwriting a
    live entry for the same key."""
    path = sm_dir / filename
    existing = []
    if path.is_file():
        try:
            loaded = json.loads(path.read_text(encoding="utf-8"))
            existing = loaded if isinstance(loaded, list) else loaded.get(key, [])
        except (json.JSONDecodeError, AttributeError):
            existing = []
    have = {r["fn"] for r in existing if "fn" in r}
    added = 0
    for r in still_open_rows:
        if r["fn"] not in have:
            existing.append({k: v for k, v in r.items() if k != "salvage_status"} |
                            {"salvaged": True,
                             "salvage_source": "archive/previous_campaign"})
            have.add(r["fn"])
            added += 1
    return existing, added


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--archive-dir", required=True, type=Path)
    ap.add_argument("--sm-dir", required=True, type=Path)
    ap.add_argument("--symbols", required=True, type=Path)
    args = ap.parse_args()

    if not args.archive_dir.is_dir():
        print(f"[salvage] archive dir not found: {args.archive_dir} - "
              "nothing to salvage")
        return

    addr_index = load_addr_index(args.symbols)
    args.sm_dir.mkdir(parents=True, exist_ok=True)

    # --- structural_ports.json --------------------------------------------
    struct_all, struct_buckets = salvage_structural(args.archive_dir, addr_index)
    struct_merged, struct_added = merge_native(
        args.sm_dir, "structural_ports.json", "fn", struct_buckets.get("still-open", []))
    struct_merged.sort(key=lambda r: (r.get("confidence") != "HIGH",
                                      -r.get("n", 0)))
    (args.sm_dir / "structural_ports.json").write_text(
        json.dumps(struct_merged, ensure_ascii=False, indent=1), encoding="utf-8")

    md = ["# Structural-port salvage report", "",
          f"Source: `{args.archive_dir}/structural_ports.json` "
          f"({len(struct_all)} archived mnemonic-fingerprint XD ports), "
          f"revalidated against the CURRENT `{args.symbols}`.", "",
          "No live XD asm is available in this tree, so this evidence was "
          "not re-mined - only re-checked address-by-address against "
          "current symbol names. See mine_xrefs.py/structural_port.py "
          "porting notes for how to wire up a live XD split.", "",
          "| status | count | meaning |",
          "|---|---|---|",
          f"| still-open | {len(struct_buckets['still-open'])} | live REVIEW-band lead - function is still `fn_`/`lbl_` today |",
          f"| already-applied | {len(struct_buckets['already-applied'])} | independently confirmed - already renamed to match (or `name_ADDR` variant) |",
          f"| already-conflict | {len(struct_buckets['already-conflict'])} | already renamed, but to a DIFFERENT name (see samples) |",
          f"| address-not-found | {len(struct_buckets['address-not-found'])} | no function symbol at that address anymore |",
          f"| skip (non-address fn) | {len(struct_buckets['skip-already-named-at-archive-time'])} | archived `fn` was already a real name at archive time |",
          "", f"`structural_ports.json` in this directory now holds "
          f"{len(struct_merged)} entries ({struct_added} newly merged from "
          "salvage) - the still-open band only.", ""]

    md.append("## Sample: still-open (actionable REVIEW leads)\n")
    md.append("| fn | addr | -> xd_name | n | confidence | shared strings |")
    md.append("|---|---|---|---|---|---|")
    for r in sorted(struct_buckets["still-open"],
                    key=lambda r: (r["confidence"] != "HIGH", -r["n"]))[:10]:
        sh = "; ".join(s[:28] for s in r.get("shared_strings", []))
        md.append(f"| `{r['fn']}` | {r['addr']} | **{r['xd_name']}** | {r['n']} "
                  f"| {r['confidence']} | `{sh}` |")

    md.append("\n## Sample: already-applied (QA confirmation, no action needed)\n")
    md.append("| fn | addr | archived xd_name | current name |")
    md.append("|---|---|---|---|")
    for r in struct_buckets["already-applied"][:10]:
        md.append(f"| `{r['fn']}` | {r['addr']} | {r['xd_name']} | "
                  f"**{r['current_name']}** |")

    if struct_buckets["already-conflict"]:
        md.append("\n## already-conflict (informational only)\n")
        md.append("| fn | addr | archived xd_name | current name |")
        md.append("|---|---|---|---|")
        for r in struct_buckets["already-conflict"][:10]:
            md.append(f"| `{r['fn']}` | {r['addr']} | {r['xd_name']} | "
                      f"{r['current_name']} |")

    (args.sm_dir / "structural_port_salvage.md").write_text(
        "\n".join(md) + "\n", encoding="utf-8")

    # --- xd_port.json / xd_port.md (merge with a live port_xd.py run) ------
    xd_all, xd_buckets = salvage_xd_port(args.archive_dir, addr_index)
    xd_path = args.sm_dir / "xd_port.json"
    live = {"corroborated": [], "ported": []}
    if xd_path.is_file():
        try:
            live = json.loads(xd_path.read_text(encoding="utf-8"))
        except json.JSONDecodeError:
            pass
    # xd_port.json is a {"corroborated": [...], "ported": [...]} dict, not a
    # bare list, so merge "ported" directly against the already-loaded
    # `live` dict rather than via the generic merge_native() helper.
    have = {r["fn"] for r in live.get("ported", [])}
    ported_added = 0
    for r in xd_buckets.get("still-open", []):
        if r["fn"] not in have:
            live.setdefault("ported", []).append(
                {k: v for k, v in r.items() if k != "salvage_status"} |
                {"salvaged": True, "salvage_source": "archive/previous_campaign"})
            have.add(r["fn"])
            ported_added += 1
    # corroborated (self-name proposal validated against XD vocabulary) has
    # no address to re-check - it's a static fact about the archived XD
    # vocabulary, so pass it through once, tagged.
    if not live.get("corroborated") and xd_all.get("corroborated"):
        live["corroborated"] = [dict(c, salvaged=True) for c in xd_all["corroborated"]]
    xd_path.write_text(json.dumps(live, ensure_ascii=False, indent=1),
                       encoding="utf-8")

    xdmd = ["# XD (GXXE01) port salvage report", "",
           f"Source: `{args.archive_dir}/xd_port.json` "
           f"({len(xd_all.get('ported', []))} archived shared-string ports, "
           f"{len(xd_all.get('corroborated', []))} archived self-name "
           "validations against XD's vocabulary), revalidated against the "
           f"CURRENT `{args.symbols}`.", "",
           "| status | count |",
           "|---|---|",
           f"| still-open | {len(xd_buckets.get('still-open', []))} |",
           f"| already-applied | {len(xd_buckets.get('already-applied', []))} |",
           f"| already-conflict | {len(xd_buckets.get('already-conflict', []))} |",
           f"| address-not-found | {len(xd_buckets.get('address-not-found', []))} |",
           "",
           f"`xd_port.json` now holds {len(live.get('ported', []))} `ported` "
           f"entries ({ported_added} newly merged from salvage) and "
           f"{len(live.get('corroborated', []))} `corroborated` entries.",
           "",
           "`xd_vocabulary.json` (XD names grouped by engine prefix) was "
           "copied through unchanged - it has no address dependency."]
    if xd_buckets.get("still-open"):
        xdmd.append("\n## Sample: still-open\n")
        xdmd.append("| fn | addr | -> name | score | confidence |")
        xdmd.append("|---|---|---|---|---|")
        for r in xd_buckets["still-open"][:10]:
            xdmd.append(f"| `{r['fn']}` | {r['addr']} | **{r['name']}** | "
                       f"{r.get('score')} | {r.get('confidence')} |")
    if xd_buckets.get("already-applied"):
        xdmd.append("\n## Sample: already-applied\n")
        xdmd.append("| fn | addr | archived name | current name |")
        xdmd.append("|---|---|---|---|")
        for r in xd_buckets["already-applied"][:10]:
            xdmd.append(f"| `{r['fn']}` | {r['addr']} | {r['name']} | "
                       f"**{r['current_name']}** |")
    (args.sm_dir / "xd_port.md").write_text("\n".join(xdmd) + "\n",
                                            encoding="utf-8")

    # xd_vocabulary.json: pure reference data, no address dependency - copy
    # through unless a live port_xd.py run already populated a non-trivial
    # one (port_xd.py always writes *some* file here, even an empty `{}`
    # stub when no live XD symbols were found, so "file exists" alone isn't
    # enough of a guard).
    voc_out = args.sm_dir / "xd_vocabulary.json"
    voc_src = args.archive_dir / "xd_vocabulary.json"
    live_voc_empty = True
    if voc_out.is_file():
        try:
            live_voc_empty = not json.loads(voc_out.read_text(encoding="utf-8"))
        except json.JSONDecodeError:
            live_voc_empty = True
    if live_voc_empty and voc_src.is_file():
        voc_out.write_text(voc_src.read_text(encoding="utf-8"), encoding="utf-8")

    print(f"[salvage] structural: still-open={len(struct_buckets['still-open'])} "
          f"already-applied={len(struct_buckets['already-applied'])} "
          f"already-conflict={len(struct_buckets['already-conflict'])} "
          f"not-found={len(struct_buckets['address-not-found'])}")
    print(f"[salvage] xd_port: still-open={len(xd_buckets.get('still-open', []))} "
          f"already-applied={len(xd_buckets.get('already-applied', []))} "
          f"already-conflict={len(xd_buckets.get('already-conflict', []))}")
    print(f"[salvage] wrote structural_ports.json (+{struct_added}), "
          "structural_port_salvage.md, xd_port.json "
          f"(+{ported_added}), xd_port.md, xd_vocabulary.json")


if __name__ == "__main__":
    main()
