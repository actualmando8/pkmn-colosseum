"""prefilter.py — Phase-3 compiler-surrogate pre-filter for decomp-permuter.

Env-gated exactly like trainlog: without PERM_PREFILTER_MODEL the permuter is
byte-identical to upstream (PreFilter.create returns None).

For each freshly-generated candidate (BEFORE the C compile), builds the SAME
features train_surrogate.py used — hashed 1-2grams over the trainlog window-diff
changed lines (2^15), passes multi-hot over pass_vocab, and the 7 dense feats —
scores P(improve) with the XGBoost booster, and decides:
  prob >= threshold          -> compile   (accepted)
  else, w.p. audit_rate       -> audit     (compile anyway, to measure the
                                            skipped-but-would-improve rate)
  else                        -> skip      (no compile: the throughput win)
Any featurize/predict error -> compile (FAIL-OPEN; never skip on error).

Featurizer parity is anchored to featurizer.pkl (hv_params, pass_vocab,
dense_names) + the identical window-diff/diff_feats/text-prep pipeline.

Env:
  PERM_PREFILTER_MODEL       xgb json (enables the filter)
  PERM_PREFILTER_FEATURIZER  featurizer.pkl
  PERM_PREFILTER_THRESHOLD   prob cutoff (default 0.38148 = pooled top-30%)
  PERM_PREFILTER_AUDIT       audit sample rate on rejects (default 0.05)
  PERM_PREFILTER_WARMUP      first N candidates always compiled (default 30)
  PERM_PREFILTER_LOG         decision-log dir; <dir>/<fn>/<worker>.jsonl
  PERM_PREFILTER_WORKER      worker tag (default pid<pid>)
"""
import atexit
import difflib
import json
import os
import pickle
import random
import time
from typing import List, Optional, Sequence, Tuple

FAIL = 10**9


class PreFilter:
    @staticmethod
    def create(fn_name: str, base_score: int, base_source: str) -> Optional["PreFilter"]:
        if not os.environ.get("PERM_PREFILTER_MODEL"):
            return None
        if base_score <= 0:
            return None  # nothing to improve; don't filter
        try:
            return PreFilter(fn_name, base_score, base_source)
        except Exception as e:  # any load failure -> disable (fail open)
            print(f"[prefilter] disabled (load error: {e})")
            return None

    def __init__(self, fn_name: str, base_score: int, base_source: str) -> None:
        import numpy as np
        import scipy.sparse as sp
        import xgboost as xgb
        from sklearn.feature_extraction.text import HashingVectorizer

        self._np = np
        self._sp = sp
        self._xgb = xgb

        with open(os.environ["PERM_PREFILTER_FEATURIZER"], "rb") as f:
            spec = pickle.load(f)
        self._hv = HashingVectorizer(**spec["hv_params"])
        self._pass_vocab = spec["pass_vocab"]
        self._pv_idx = {p: i for i, p in enumerate(self._pass_vocab)}
        self._dense_names = spec["dense_names"]

        self._bst = xgb.Booster()
        self._bst.load_model(os.environ["PERM_PREFILTER_MODEL"])
        # CRITICAL for online use: default multi-thread predict spawns a thread
        # pool per call (~90-130ms under load); nthread=1 + inplace_predict on
        # the sparse row is ~0.6ms. Without this the filter costs more than the
        # compile it saves and destroys throughput.
        self._bst.set_param({"nthread": 1})

        self.fn = fn_name
        self.base_score = base_score
        self.base_lines = base_source.split("\n")
        self.log_base_score = float(np.log1p(base_score))

        self.threshold = float(os.environ.get("PERM_PREFILTER_THRESHOLD", "0.38148"))
        self.audit_rate = float(os.environ.get("PERM_PREFILTER_AUDIT", "0.05"))
        self.warmup = int(os.environ.get("PERM_PREFILTER_WARMUP", "30"))
        self._rng = random.Random(0xC0FFEE ^ hash(fn_name) & 0xFFFFFFFF)
        self._n = 0
        self._pending = None

        self._fh = None
        logdir = os.environ.get("PERM_PREFILTER_LOG")
        if logdir:
            worker = os.environ.get("PERM_PREFILTER_WORKER", f"pid{os.getpid()}")
            d = os.path.join(logdir, fn_name)
            os.makedirs(d, exist_ok=True)
            self._fh = open(os.path.join(d, f"{worker}.jsonl"), "a", encoding="utf-8")
            self._buf: List[str] = []
            atexit.register(self.close)

    # ---- diff parity with trainlog._window_diff + parse_corpus.diff_feats ----
    def _window_diff(self, cand_lines: Sequence[str]) -> Tuple[int, str]:
        a, b = self.base_lines, cand_lines
        na, nb = len(a), len(b)
        lo = 0
        hi_max = min(na, nb)
        while lo < hi_max and a[lo] == b[lo]:
            lo += 1
        ta, tb = na, nb
        while ta > lo and tb > lo and a[ta - 1] == b[tb - 1]:
            ta -= 1
            tb -= 1
        if lo == ta and lo == tb:
            return lo, ""
        s = max(0, lo - 2)
        diff = difflib.unified_diff(
            a[s:ta + 2 if ta + 2 <= na else na],
            b[s:tb + 2 if tb + 2 <= nb else nb],
            n=2, lineterm="",
        )
        return s, "\n".join(diff)

    @staticmethod
    def _diff_feats(diff: str):
        n_hunk = n_add = n_del = 0
        changed = []
        for line in diff.split("\n"):
            if line.startswith("@@"):
                n_hunk += 1
            elif line.startswith("+") and not line.startswith("+++"):
                n_add += 1
                changed.append("A> " + line[1:].strip())
            elif line.startswith("-") and not line.startswith("---"):
                n_del += 1
                changed.append("D> " + line[1:].strip())
        return n_hunk, n_add, n_del, len(diff), "\n".join(changed)

    def _prob(self, cand_source: str, passes: Optional[Sequence[str]]) -> float:
        np, sp = self._np, self._sp
        _, diff = self._window_diff(cand_source.split("\n"))
        n_hunk, n_add, n_del, diff_len, ctext = self._diff_feats(diff)
        ctext = ctext.replace("A> ", "addln ").replace("D> ", "delln ")
        X_text = self._hv.transform([ctext]).astype(np.float32)
        pl = list(passes)[-64:] if passes else []
        X_pass = sp.lil_matrix((1, len(self._pass_vocab)), dtype=np.float32)
        for p in pl:
            j = self._pv_idx.get(p)
            if j is not None:
                X_pass[0, j] += 1.0
        n_pass = len(pl)
        D = np.array([[n_hunk, n_add, n_del, n_pass, self.log_base_score,
                       float(np.log1p(diff_len)), n_add - n_del]], dtype=np.float32)
        X = sp.hstack([X_text, X_pass.tocsr(), sp.csr_matrix(D)], format="csr")
        # inplace_predict on the csr row: same output as predict(DMatrix), 100x faster
        return float(self._bst.inplace_predict(X)[0])

    # ---- decision API -------------------------------------------------------
    def pre(self, cand_source: str, passes: Optional[Sequence[str]]) -> str:
        """Return action; store pending meta for post()."""
        self._n += 1
        try:
            prob = self._prob(cand_source, passes)
        except Exception as e:
            self._pending = {"action": "error", "prob": None, "err": str(e)[:120]}
            return "compile"  # FAIL OPEN
        if self._n <= self.warmup:
            action = "warmup"
        elif prob >= self.threshold:
            action = "compile"
        elif self._rng.random() < self.audit_rate:
            action = "audit"
        else:
            action = "skip"
        self._pending = {"action": action, "prob": prob}
        if action == "skip":
            self._emit(self._pending, None)
        return action

    def post(self, score: int) -> None:
        """Record the compiled outcome for the last accepted/audited/warmup cand."""
        m = self._pending
        if m is None or m["action"] == "skip":
            return
        self._emit(m, score)
        self._pending = None

    def _emit(self, meta: dict, score) -> None:
        if self._fh is None:
            return
        rec = {"fn": self.fn, "ts": int(time.time()), "n": self._n,
               "action": meta["action"], "prob": meta.get("prob"),
               "base_score": self.base_score}
        if score is not None:
            rec["score"] = score
            rec["improved"] = bool(score < self.base_score and score < FAIL)
            rec["fail"] = bool(score >= FAIL)
        self._buf.append(json.dumps(rec, separators=(",", ":")))
        if len(self._buf) >= 50:
            self._flush()

    def _flush(self) -> None:
        if self._fh and self._buf:
            self._fh.write("\n".join(self._buf) + "\n")
            self._fh.flush()
            self._buf = []

    def close(self) -> None:
        try:
            self._flush()
            if self._fh:
                self._fh.close()
        except Exception:
            pass
