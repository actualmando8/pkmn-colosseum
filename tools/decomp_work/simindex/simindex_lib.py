"""simindex_lib: shared parser / normalizer / minhash / index IO for the
asm-similarity retrieval index (v0, structural).

Corpora:
  colo: build/GC6E01/asm/**/*.s      (dtk disassembly of the full DOL)
  xd:   tools/symbolmap/xd_ref/GXXE01/_xdsplit/asm/**/*.s

Matched-status comes from build/GC6E01/report.json (fuzzy_match_percent).
"""

import json
import os
import pickle
import re
import zlib

import numpy as np

REPO = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
COLO_ASM_DIR = os.path.join(REPO, "build", "GC6E01", "asm")
XD_ASM_DIR = os.path.join(REPO, "tools", "symbolmap", "xd_ref", "GXXE01", "_xdsplit", "asm")
REPORT_JSON = os.path.join(REPO, "build", "GC6E01", "report.json")
INDEX_DIR = os.path.join(REPO, "build", "simindex")
INDEX_PATH = os.path.join(INDEX_DIR, "index.pkl")

# ---------------------------------------------------------------- parser ---

FN_RE = re.compile(r"^\.fn\s+([^\s,]+)")
ENDFN_RE = re.compile(r"^\.endfn")
INSN_RE = re.compile(
    r"^/\* ([0-9A-F]{8}) [0-9A-F]{8}\s+(?:[0-9A-F]{2} ){4}\*/\s+(\S+)\s*(.*)$"
)
LABEL_RE = re.compile(r"^(\.L_[0-9A-Fa-f]+):")
SIZE_CMT_RE = re.compile(r"^#\s+\.\w+:0x[0-9A-Fa-f]+ \| (0x[0-9A-Fa-f]+) \| size: (0x[0-9A-Fa-f]+)")


def parse_dtk_asm(path):
    """Yield dicts: name, addr, size, insns=[(mnemonic, operands_str)],
    labels={label_name: insn_index}."""
    cur = None
    pend_addr = pend_size = None
    with open(path, "r", errors="replace") as f:
        for line in f:
            line = line.rstrip("\n")
            m = SIZE_CMT_RE.match(line)
            if m:
                pend_addr, pend_size = int(m.group(1), 16), int(m.group(2), 16)
                continue
            m = FN_RE.match(line)
            if m:
                cur = {"name": m.group(1), "addr": pend_addr, "size": pend_size,
                       "insns": [], "labels": {}}
                continue
            if cur is None:
                continue
            if ENDFN_RE.match(line):
                if cur["addr"] is None and cur["insns"]:
                    cur["addr"] = cur["_first_addr"]
                if cur["size"] is None:
                    cur["size"] = 4 * len(cur["insns"])
                yield cur
                cur = None
                pend_addr = pend_size = None
                continue
            m = LABEL_RE.match(line)
            if m:
                cur["labels"][m.group(1)] = len(cur["insns"])
                continue
            m = INSN_RE.match(line)
            if m:
                if not cur["insns"]:
                    cur["_first_addr"] = int(m.group(1), 16)
                cur["insns"].append((m.group(2), m.group(3)))


# ------------------------------------------------------------ normalizer ---

TOK_RE = re.compile(
    r"\.L_[0-9A-Fa-f]+"           # local branch label
    r"|-?0x[0-9A-Fa-f]+"          # hex immediate
    r"|-?\d+"                     # decimal immediate
    r"|[A-Za-z_$@.][A-Za-z0-9_$@.]*"  # identifier (may carry @l/@ha/@sda21)
    r"|[(),]"                     # punctuation
)
GPR_RE = re.compile(r"^r\d+$")
FPR_RE = re.compile(r"^f\d+$")
KEEP_RE = re.compile(r"^(cr\d|qr\d|lt|gt|eq|so|un)$")


def _imm_bucket(v, disp=False):
    """disp=True: memory-operand displacement d(rN) — 0 merges into small,
    since struct offset 0x0 vs 0x4 is the same codegen shape."""
    if v == 0 and not disp:
        return "0"
    a = abs(v)
    if a < 16:
        return "S"
    if a < 0x8000:
        return "M"
    if a >= 0x80000000:
        return "A"  # address-like
    return "L"


def normalize(insns, labels):
    """Canonicalize a function body to a list of normalized instruction
    strings. Registers renumbered by first use, immediates bucketed,
    symbols -> SYM, branch labels -> FWD/BACK. Mnemonics verbatim."""
    gmap, fmap = {}, {}
    out = []
    for idx, (mnem, ops) in enumerate(insns):
        raw = TOK_RE.findall(ops)
        toks = []
        for i, t in enumerate(raw):
            if t in "(),":
                toks.append(t)
            elif t.startswith(".L_"):
                tgt = labels.get(t, len(insns))
                toks.append("FWD" if tgt > idx else "BACK")
            elif GPR_RE.match(t):
                toks.append("G%d" % gmap.setdefault(t, len(gmap)))
            elif FPR_RE.match(t):
                toks.append("F%d" % fmap.setdefault(t, len(fmap)))
            elif KEEP_RE.match(t):
                toks.append(t)
            elif t[0] in "-0123456789":
                disp = i + 1 < len(raw) and raw[i + 1] == "("
                try:
                    toks.append(_imm_bucket(int(t, 0), disp=disp))
                except ValueError:
                    toks.append("SYM")
            else:
                at = t.find("@")
                toks.append("SYM" + t[at:] if at >= 0 else "SYM")
        out.append(mnem + " " + "".join(toks))
    return out


# --------------------------------------------------------------- minhash ---

N_PERMS = 128
N_BANDS = 32          # 32 bands x 4 rows
ROWS = N_PERMS // N_BANDS
_PRIME = (1 << 61) - 1
_rng = np.random.RandomState(0x5EED)
_A = _rng.randint(1, _PRIME, size=N_PERMS, dtype=np.uint64)
_B = _rng.randint(0, _PRIME, size=N_PERMS, dtype=np.uint64)


def shingles(norm_seq, n=4):
    """4-gram shingle hashes of the normalized instruction stream.
    Functions shorter than n yield one whole-body shingle."""
    if len(norm_seq) < n:
        return [zlib.crc32("\n".join(norm_seq).encode())]
    joined = ["\n".join(norm_seq[i:i + n]) for i in range(len(norm_seq) - n + 1)]
    return [zlib.crc32(s.encode()) for s in joined]


def minhash_sig(sh):
    h = np.asarray(sorted(set(sh)), dtype=np.uint64)
    # (a*h + b) mod prime, elementwise min over shingles
    v = (_A[:, None] * h[None, :] + _B[:, None]) % _PRIME
    return v.min(axis=1)


def lsh_keys(sig):
    for b in range(N_BANDS):
        yield (b, sig[b * ROWS:(b + 1) * ROWS].tobytes())


# ----------------------------------------------------------------- index ---

def load_report():
    """addr(int) -> dict(fuzzy, unit, src_path) from report.json."""
    with open(REPORT_JSON) as f:
        rep = json.load(f)
    out = {}
    for unit in rep["units"]:
        meta = unit.get("metadata") or {}
        src = meta.get("source_path")
        for fn in unit.get("functions") or []:
            va = int(fn["metadata"]["virtual_address"])
            out[va] = {
                "fuzzy": fn.get("fuzzy_match_percent", 0.0),
                "unit": unit["name"],
                "src_path": src,
            }
    return out


def iter_asm_files(root):
    for dirpath, _dirs, files in os.walk(root):
        for fname in sorted(files):
            if fname.endswith(".s"):
                yield os.path.join(dirpath, fname)


def save_index(index, path=INDEX_PATH):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    tmp = path + ".tmp"
    with open(tmp, "wb") as f:
        pickle.dump(index, f, protocol=pickle.HIGHEST_PROTOCOL)
    os.replace(tmp, path)


def load_index(path=INDEX_PATH):
    with open(path, "rb") as f:
        return pickle.load(f)


def resolve(index, key, corpus=None):
    """Resolve an addr (0x... / hex) or name to a function id."""
    fns = index["functions"]
    key = key.strip()
    ids = None
    try:
        addr = int(key, 16) if key.lower().startswith("0x") else int(key, 16) if re.fullmatch(r"[0-9A-Fa-f]{8}", key) else None
    except ValueError:
        addr = None
    if addr is not None:
        ids = index["by_addr"].get(addr)
    if ids is None:
        ids = index["by_name"].get(key)
    if not ids:
        return None
    if corpus:
        pref = [i for i in ids if fns[i]["corpus"] == corpus]
        if pref:
            ids = pref
    # prefer colo on ambiguity
    ids = sorted(ids, key=lambda i: fns[i]["corpus"] != "colo")
    return ids[0]


def sig_agreement(index, fid):
    """Fraction of matching minhash positions vs every fn (est. Jaccard)."""
    M = index["sig_matrix"]
    return (M == M[fid]).mean(axis=1)
