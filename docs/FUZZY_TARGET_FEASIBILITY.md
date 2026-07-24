# Fuzzy-match target feasibility (2026-07-24)

Baseline for this analysis: master `dae9b326`, fuzzy **57.81%**.

## How `fuzzy_match_percent` is actually computed

The headline number in `build/GC6E01/report.json` is weighted by **code size
only**. Verified exactly against the reported value:

```
fuzzy_match_percent = Σ(unit.total_code × unit.fuzzy_match_percent) / Σ(unit.total_code)
```

Reproduce:

```bash
python3 -c "
import json
r=json.load(open('build/GC6E01/report.json'))
n=d=0
for u in r['units']:
    m=u['measures']; c=int(m.get('total_code',0))
    n+=c*m.get('fuzzy_match_percent',0)/100.0; d+=c
print('%.6f'%(100*n/d))
"
```

**Consequence: data match contributes nothing to the headline fuzzy number.**
Data is already at 97.29% and is worth 0.00pp of further fuzzy. Any effort
spent closing the remaining ~59.6KB data gap does not move fuzzy at all. Aim
code-side only.

Note the per-function percentages live at `unit['functions']`, *not*
`unit['sections'][n]['functions']` (which is always empty). `fleet/verify_fn.sh`
reads the correct location.

## Where the remaining headroom lives

Total remaining headroom across the whole project is **42.19pp**. Distribution
by current per-function fuzzy band:

| band | fns | headroom | value | ceiling if fully closed |
|---|---|---|---|---|
| 95–100% | 227 | 3.4 KB | +0.14pp | 57.97% |
| 80–95% | 316 | 17.0 KB | +0.68pp | 58.65% |
| 50–80% | 320 | 74.9 KB | +3.00pp | 61.65% |
| 10–50% | 229 | 154.8 KB | +6.20pp | 67.85% |
| 0–10% | 1,190 | 802.2 KB | +32.15pp | 100.00% |

### The decisive fact

**Perfecting every partially-decompiled function in the project — all 1,092
functions above 10% fuzzy, every one taken to byte-exact — yields only 67.85%.**

An 80% target is therefore not reachable by finishing near-misses, closing the
one-away frontier, or grinding the register-coloring walls. It requires
byte-exact decompilation of roughly **800 of the 1,190 functions currently below
10% fuzzy** — code that is essentially untouched, averaging ~670 bytes each.

### Where that hard tail sits

| subsystem | units | headroom | current fuzzy |
|---|---|---|---|
| game (core) | 853 | 20.92pp | 64.3% |
| game/menu | 77 | 8.15pp | 37.5% |
| hsd | 128 | 4.48pp | 45.2% |
| game/people | 30 | 2.39pp | 23.9% |
| musyx | 124 | 1.02pp | 80.0% |
| everything else (dolphin/trk/crt) | — | ~5.2pp | — |

31.5pp of the 42.2pp is proprietary Colosseum game logic (`game`, `game/menu`,
`game/people`) with no public reference implementation. Dolphin SDK, MusyX, HSD,
TRK and CRT combined hold only ~10.7pp, and much of that is already high.

Some of the hard tail is **permanently unmatchable by policy**: `OSDisableInterrupts`,
`OSEnableInterrupts`, `OSRestoreInterrupts`, `OSSaveFPUContext`, `OSGetTick`,
`OSGetStackPointer`, `__DBExceptionDestination` and similar are MSR/SPR
manipulation that cannot be expressed in C, and `AGENTS.md` correctly forbids
counting asm wrappers as progress.

## Observed campaign rate

From README history, with the full multi-agent fleet running and ~30 PRs merged
(each linking ~30 exact functions):

| date | fuzzy |
|---|---|
| 2026-07-18 | 57.39% |
| 2026-07-19 | 57.41% |
| 2026-07-21 | 57.81% |
| 2026-07-22 | 57.81% |

That is **+0.42pp over 4 days**, best single day +0.40pp. At the sustained rate,
+22.19pp is on the order of 200 days of continuous fleet operation; at the
best-ever day rate, ~55 days. Neither is a single-session or single-week target.

## Recommended re-scoping

Ordered by pp-per-unit-effort, not by function count:

1. **Stop counting functions, start counting bytes.** A 30-function PR of small
   accessors moves fuzzy ~0.01pp. The metric is byte-weighted — one 4KB function
   is worth ~100 small ones. Queue construction should sort by
   `size × (100 − fuzzy)`, not by perceived tractability.
2. **Zero data-side work.** It is worth exactly 0.00pp.
3. **Highest-value single units** (headroom, pp): `menu/pda_range_80037158`
   (76.9KB, 3.08pp), `people/people` (51.3KB, 2.05pp),
   `hsd_texp_candidate_801B4300` (26.3KB, 1.05pp),
   `menu/menu_range_8007109C` (23.2KB, 0.93pp), `menu/cardesavedata`
   (21.5KB, 0.86pp). These five alone are worth 7.97pp — more than the entire
   50–100% frontier combined.
4. **Realistic near-term goal: 62–65%.** That is the 50%+ frontier plus
   meaningful progress into the 10–50% band, and it is achievable with the
   existing fleet workflow. 80% needs a different plan — most plausibly a
   reference-guided port of the `game/menu` and `people` subsystems rather than
   per-function permuter grinding.

## Known walls (do not re-grind)

`fn_800C46B0`, `fn_8000CF68`, `fn_8017B5A4`, `fn_800D36B4`, `fn_80080ED8`,
`fn_8011487C`, `fn_8019C3C4`, `fn_80230568`, `fn_80238060`.
