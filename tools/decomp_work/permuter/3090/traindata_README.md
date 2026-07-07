# Permuter training data (compiler-surrogate Phase 1)

One JSONL record per **freshly compiled** permuter candidate (cache-hit
re-encounters of an identical source are deduplicated at the source; the
final in-memory buffer of a run — ≤200 records — is lost if the permuter is
SIGTERM'd at budget expiry, which is the normal end of a run).

Layout: `traindata/<fn>/<worker>.jsonl`, rotated at 64 MiB to
`<name>.jsonl.<epoch>.gz`. Total dir capped at 50 GiB by `traindata_gc.sh`
(hourly cron; oldest gz first). Producer: `src/trainlog.py` in the deployed
decomp-permuter clone (env-gated by `PERM_TRAINLOG_DIR`).

## Record schema

| field      | type        | meaning                                                            |
|------------|-------------|--------------------------------------------------------------------|
| fn         | str         | target function symbol                                             |
| ts         | int         | unix seconds at scoring time                                       |
| iter       | int         | 1-based fresh-compile counter within one permuter run              |
| score      | int         | permuter scorer result; 0 = byte-exact match; 1000000000 = compile failure |
| base_score | int         | score of the unmutated base source for that run                    |
| seed       | [int,int]?  | [perm_seed, rng_seed]; with the run's base.c + weight config this reproduces the candidate deterministically |
| passes     | [str]?      | cumulative randomization-pass names applied to the candidate lineage (mutation-class labels, last 64; null if unavailable) |
| win_start  | int         | 0-based line offset into the canonicalized base source where `diff` begins (common prefix/suffix trimmed) |
| diff       | str         | unified diff (2 context lines), candidate vs base, both pycparser-canonical; "" = candidate identical to base |

## Reconstruction notes for Phase 2

- The base source for a record is the **canonical (pycparser-reprinted)**
  form of the run's `dirs/<fn>/base.c`, NOT the raw file: reprint it with the
  permuter's `ast_util.to_c()` before applying `diff` at `win_start`.
- `base.c` for a fn changes across queue refreshes (snapshot re-syncs).
  Partition training examples by (`fn`, `base_score`, first record `ts`) or
  re-derive base hashes from the farm dirs when exactness matters.
- Compile-failure records (score = 1e9) are the negative class for a
  "will it compile" auxiliary head; they carry the same diff features.
- The mwcc invocation per fn (exact flags) is `dirs/<fn>/compile.sh`;
  the target objects are `dirs/<fn>/target.o`.
