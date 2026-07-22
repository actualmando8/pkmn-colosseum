# simindex — asm-similarity retrieval index (v0, structural)

For any unmatched PowerPC function, retrieve the most similar ALREADY-MATCHED
functions (byte-exact C exists) as few-shot exemplars, and cluster the
unmatched pool into template families ("solve one, sweep twenty").

## Corpora

| corpus | source | fns |
|--------|--------|-----|
| `colo` | `build/GC6E01/asm/**/*.s` (dtk disasm, deduped by address — the tree contains overlapping `auto_*` split files and per-unit files) | ~8.6k |
| `xd`   | `tools/symbolmap/xd_ref/GXXE01/_xdsplit/asm/**/*.s` (Pokémon XD reference: no C, but 13k named fns with near-identical codegen — naming-by-analogy) | ~13.2k |
| `ext`  | `~/decomp-corpus/*.pairs.jsonl` — byte-exact-matched functions extracted from *other* same-compiler-family GameCube decomps (Mario Party 4: 10.7k, Star Fox Adventures: 8.9k, Mario Strikers: 8.0k, Metroid Prime: 0.8k), each rebuilt from source with its own pinned mwcc so the disassembly is authoritative even though we never touch that game's disc. Every `ext` fn carries a real C `src_path` in a foreign repo — a genuine cross-game exemplar, not naming-by-analogy. MP4/Strikers/Prime each include a complete matched MusyX@GC/1.3.2 runtime (`extern/musyx/src/musyx/runtime/*`), which covers ~98% of Colosseum's unmatched `people_field` pool at seq >= 0.9. See "External corpora" below. | ~28.4k |

Matched status (fuzzy_match_percent, unit, src path) comes from
`build/GC6E01/report.json` for `colo`. Index artifacts live in `build/simindex/index.pkl`.

## Usage

```sh
# build/refresh (run after report.json / asm changes; ~10 s)
python3 tools/decomp_work/simindex/build_index.py

# ...with external corpora folded in (repeatable flag; ~17s with all four)
python3 tools/decomp_work/simindex/build_index.py \
    --ext-corpus ~/decomp-corpus/mariopartyrd-marioparty4.pairs.jsonl \
    --ext-corpus ~/decomp-corpus/zcanann-SFA-Decomp.pairs.jsonl \
    --ext-corpus ~/decomp-corpus/yannicksuter-smstrikers-decomp.pairs.jsonl \
    --ext-corpus ~/decomp-corpus/PrimeDecomp-prime.pairs.jsonl
# or point at the whole dir; it globs every *.pairs.jsonl inside
python3 tools/decomp_work/simindex/build_index.py --ext-corpus ~/decomp-corpus

# top-k similar functions for an address or name
python3 tools/decomp_work/simindex/query.py fn_80253B78 -k 5 --matched-only
python3 tools/decomp_work/simindex/query.py 0x8015210C --corpus xd
python3 tools/decomp_work/simindex/query.py hwSetPitch --corpus ext

# template families in the unmatched pool, ranked by sweep payoff
python3 tools/decomp_work/simindex/families.py --min-size 3 --unmatched-only
```

`query.py` flags: `-k N`, `--corpus colo|xd|ext|all`, `--matched-only`
(restrict to fns with byte-exact C: Colosseum fns at 100%, or any `ext` fn —
`ext` is matched by construction, see below).
Output columns: `score` = SequenceMatcher ratio x size-ratio prior,
`seq` = raw ratio, `mh` = minhash agreement (est. Jaccard of 4-gram shingles).

### External corpora

`ext` entries come from `~/decomp-corpus/<repo>.pairs.jsonl`, one JSON object
per matched function:

```json
{"game": "marioparty4", "repo": "mariopartyrd/marioparty4",
 "unit": "...", "src_path": "src/foo/bar.c", "fn_name": "hwSetPitch",
 "mwcc_version": "GC/1.2.5n", "addr": 2149580000, "size": 120,
 "insns": [["mflr", "r0"], ...], "labels": {".L_...": 3}}
```

`insns`/`labels` mirror `parse_dtk_asm()`'s per-function shape exactly, so
`build_index.py` runs the identical `normalize()` over `colo`/`xd`/`ext` --
no separate codepath, no drift. The principle behind these pairs: a matched
function means the repo's pinned mwcc reproduces the original game bytes
from its C source, so disassembling a locally-rebuilt `.o` is authoritative
disassembly of the *real game* even without ever touching that game's disc
-- only the CI-generated `report.json` (fetched as a GitHub Actions artifact,
since generating it locally needs the real ROM to diff against) is needed to
know which functions qualify. `families.py` intentionally ignores `ext`
(it already filters to `corpus == "colo"`) -- template clustering stays a
pure Colosseum-pool concern; `ext` is retrieval-only.

`families.py` flags: `--min-size`, `--unmatched-only`, `--min-insns` (default 4),
`--threshold` (default 0.45, similarity vs cluster representative), `--top`.
Payoff = member_count x avg_fn_size.

## How it works

1. **Parse** dtk `.s` files into per-function `(mnemonic, operands)` streams.
2. **Normalize**: registers renumbered by first-use order (`r31,r3,r0` ->
   `G0,G1,G2`; floats `F*`), immediates bucketed (`0 / S<16 / M<0x8000 / L /
   A>=0x80000000`; memory-operand displacements merge 0 into S so struct
   offset 0x0 vs 0x4 is the same shape), symbol refs -> `SYM[@l/@ha/...]`,
   branch targets -> `FWD`/`BACK`, mnemonics verbatim (incl. cr fields).
3. **Index**: 4-gram shingles over the normalized stream -> 128-perm MinHash
   -> LSH (32 bands x 4 rows) for candidates; a full sig matrix enables exact
   agreement scans (~21k fns, cheap in numpy).
4. **Rank**: candidates re-ranked by `difflib.SequenceMatcher` on the full
   normalized sequences x a size-ratio prior.
5. **Families**: candidate pairs from short 2-row LSH bands (high recall);
   union-find where merges must verify against the cluster *representative*
   (prevents transitive drift into mega-clusters). Template family members
   are similar, not identical (WazaHit-style pairs sit at seq 0.5-0.8).

## Known limits (v0 is structural)

- Families that are *semantic* (same role, divergent codegen — e.g. the
  MusyX `mcmd*` handlers, max pairwise seq 0.44) do not cluster structurally.
  They ARE still served by the XD corpus (identically-named twin scores
  ~1.0), and now better by `ext`: the same query returns MP4/Strikers/Prime's
  matched twin at seq 1.0 *with its C source path* in `musyx/runtime/`.
- Tiny functions (<4 insns) collapse to one shingle; generic stub/getter
  families are real but low-value — payoff ranking demotes them.

## v1 plan

- Embeddings on an available accelerator: per-function encoder (e.g. a small
  transformer over normalized instruction tokens, contrastively trained with
  compile-time augmentations: register permutations, scheduling jitter,
  immediate perturbation) -> FAISS index. Captures semantic-level similarity
  (same C template, different codegen) that 4-gram shingles miss — this is
  exactly the mcmd-family gap.
- Cross-title alignment: joint colo+XD embedding space to propagate XD names
  onto unnamed Colosseum fns at scale (call-graph-consistency reranking).
- Incremental rebuilds keyed on report.json mtime; per-unit invalidation.
- ~~External corpora~~ done (see "External corpora" above) — `ext` gives
  real matched-C exemplars beyond XD's naming-by-analogy; next step is
  growing per-repo coverage (more units per repo, more repos) and folding
  `ext` into the v1 embedding space once it exists.
