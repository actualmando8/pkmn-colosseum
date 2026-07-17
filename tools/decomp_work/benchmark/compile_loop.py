#!/usr/bin/env python3
"""Reproducible compile-in-the-loop benchmark for decompilation models.

The runner consumes fidelity-gated work units produced by
tools/decomp_work/permuter/gen_workunits.py.  It never edits the source tree:
each run copies its inputs below build/model_benchmark, compiles candidates with
the work unit's exact MWCC command, and scores the resulting object with
objdiff-cli.

Secrets are read after process start and used only for the request header and
in-memory error redaction. They are never accepted as command-line arguments or
placed in the environment.
"""

from __future__ import annotations

import argparse
import datetime as dt
import hashlib
import json
import os
import re
import shutil
import stat
import subprocess
import sys
import tempfile
import time
import urllib.error
import urllib.request
from dataclasses import dataclass, replace
from pathlib import Path
from typing import Any, Iterable


REPO = Path(__file__).resolve().parents[3]
DEFAULT_SUITE = Path(__file__).with_name("suite_pilot.json")
DEFAULT_KEY_FILE = Path.home() / ".config" / "decomp-keys" / "openrouter.txt"
OBJDIFF = REPO / "build" / "tools" / "objdiff-cli"
RUN_ROOT = REPO / "build" / "model_benchmark"
SEMANTIC_DRIVER = REPO / "tools" / "decomp_work" / "semantic_oracle" / "driver.py"
SEMANTIC_PINS = (
    REPO / "tools" / "decomp_work" / "semantic_oracle" / "moderngekko_pins.json"
)
SEMANTIC_DOL = REPO / "orig" / "GC6E01" / "sys" / "main.dol"
SEMANTIC_PROFILE_FUNCTIONS = {
    "GStextureLockImage-v2": "GStextureLockImage",
}
SEMANTIC_ENGINE = "dolphin-interpreter-from-moderngekko-tree"
SEMANTIC_NATIVE_ENGINE = "moderngekko-dolrecomp-native-original"
SEMANTIC_PROFILE_AUTHORITIES = {
    "GStextureLockImage-v2": {
        "virtual_address": "0x800ef548",
        "size": 0x30,
        "dol_sha1": "870e8b9693ca780782d80f22a6a4572d8ba9458f",
    },
}
SEMANTIC_MAX_TEXT_SIZE = 4096
SEMANTIC_MAX_REPORT_SIZE = 8 * 1024 * 1024
SOURCE_BASELINE_PATHS = ("src", "include", "config", "configure.py")
FULL_DOL_ARTIFACT = "build/GC6E01/main.dol"
FULL_DOL_SHA1_AUTHORITY = "config/GC6E01/build.sha1"
WORKUNIT_FINGERPRINT_FILES = (
    "base.c",
    "compile.sh",
    "compile_cmd.json",
    "meta.json",
    "settings.toml",
    "target.o",
)

PROVIDERS: dict[str, dict[str, str]] = {
    "moonshot": {
        "base_url": "https://api.moonshot.ai/v1",
        "key_label": "kimi",
    },
    "deepseek": {
        "base_url": "https://api.deepseek.com/v1",
        "key_label": "deepseek",
    },
    "glm": {
        "base_url": "https://api.z.ai/api/paas/v4",
        "key_label": "glm5.2",
    },
    "openrouter": {
        "base_url": "https://openrouter.ai/api/v1",
        "key_label": "openrouter",
    },
}

SYSTEM_PROMPT = """You are matching a GameCube PowerPC function with Metrowerks C.
Return one complete replacement C function in a single ```c fenced block.
Do not emit includes, assembly, inline assembly, .inc references, wrappers, or
changes outside the named function. Preserve the supplied ABI and declarations.
Use compiler/objdiff feedback literally. A higher match percentage is better;
100.0% is exact. Do not claim success unless the reported score is 100.0%.
Behavioral fixture feedback is diagnostic: it never replaces objdiff, and an
isolated exact result still requires full-DOL validation before it can be banked.
Reserve at least 512 output tokens for the C function; stop private reasoning
before the output limit so that every round produces a compilable proposal."""


class BenchError(RuntimeError):
    """Expected benchmark failure with a safe user-facing message."""


class SemanticError(BenchError):
    """Semantic-oracle infrastructure or authority failure."""


@dataclass(frozen=True)
class SemanticOracleConfig:
    profile: str
    function: str
    checkout: Path
    sidecar: Path
    attestation: Path
    native_sidecar: Path
    native_attestation: Path
    native_manifest: Path
    driver: Path
    pins: Path
    dol: Path
    objcopy: Path
    readelf: Path
    fixture_count: int
    seed: int
    timeout: int
    mismatch_limit: int
    expected_sidecar_sha256: str | None = None
    expected_attestation_sha256: str | None = None
    expected_native_sidecar_sha256: str | None = None
    expected_native_attestation_sha256: str | None = None
    expected_native_manifest_sha256: str | None = None
    expected_driver_sha256: str | None = None
    expected_pins_sha256: str | None = None
    expected_dol_sha256: str | None = None
    expected_pin_report_sha256: str | None = None
    expected_provenance: tuple[tuple[str, str], ...] = ()


@dataclass
class SemanticScore:
    evaluated: bool
    equal: bool | None
    mismatch_count: int | None
    feedback: str
    report_sha256: str | None
    reference_elf_sha256: str
    candidate_elf_sha256: str
    report: dict[str, Any] | None = None


@dataclass
class CompileScore:
    compile_ok: bool
    match_percent: float
    object_size: int | None
    feedback: str
    diff: dict[str, Any] | None = None
    semantic: SemanticScore | None = None


@dataclass
class ApiReply:
    assistant: dict[str, Any]
    served_model: str
    usage: dict[str, Any]
    elapsed_seconds: float
    finish_reason: str

    @property
    def content(self) -> str:
        value = self.assistant.get("content")
        return value if isinstance(value, str) else ""


def safe_slug(value: str) -> str:
    return re.sub(r"[^A-Za-z0-9_.-]+", "_", value).strip("._") or "run"


def display_path(path: Path) -> str:
    try:
        return str(path.resolve().relative_to(REPO))
    except ValueError:
        return str(path.resolve())


def failure_category(error: BenchError) -> str:
    message = str(error).lower()
    if "credential" in message:
        return "credential"
    if "provider" in message:
        return "provider"
    if "semantic" in message or "sidecar" in message:
        return "semantic_oracle"
    if "baseline" in message or "work unit" in message or "work-unit" in message:
        return "benchmark_input"
    if "compile" in message or "objdiff" in message or "objdump" in message:
        return "toolchain"
    return "benchmark"


def run_command(
    argv: list[str], *, cwd: Path, timeout: int, max_output: int = 12_000
) -> subprocess.CompletedProcess[str]:
    try:
        result = subprocess.run(
            argv,
            cwd=cwd,
            text=True,
            capture_output=True,
            timeout=timeout,
            check=False,
        )
    except subprocess.TimeoutExpired as exc:
        raise BenchError(f"command timed out after {timeout}s: {Path(argv[0]).name}") from exc
    result.stdout = result.stdout[-max_output:]
    result.stderr = result.stderr[-max_output:]
    return result


def _skip_quoted_or_comment(text: str, index: int) -> int | None:
    """Return index after a quoted string/comment, or None if not at one."""
    if text.startswith("//", index):
        end = text.find("\n", index + 2)
        return len(text) if end < 0 else end + 1
    if text.startswith("/*", index):
        end = text.find("*/", index + 2)
        if end < 0:
            raise BenchError("unterminated block comment in candidate")
        return end + 2
    if text[index : index + 1] in {'"', "'"}:
        quote = text[index]
        index += 1
        while index < len(text):
            if text[index] == "\\":
                index += 2
            elif text[index] == quote:
                return index + 1
            else:
                index += 1
        raise BenchError("unterminated quoted string in candidate")
    return None


def _balanced_end(text: str, start: int, opening: str, closing: str) -> int:
    if start >= len(text) or text[start] != opening:
        raise BenchError(f"expected {opening!r} while locating function")
    depth = 0
    index = start
    while index < len(text):
        skipped = _skip_quoted_or_comment(text, index)
        if skipped is not None:
            index = skipped
            continue
        char = text[index]
        if char == opening:
            depth += 1
        elif char == closing:
            depth -= 1
            if depth == 0:
                return index + 1
        index += 1
    raise BenchError(f"unterminated {opening}{closing} block")


def find_function_span(text: str, function: str) -> tuple[int, int]:
    """Locate a function definition in preprocessed C without parsing its types."""
    pattern = re.compile(rf"\b{re.escape(function)}\s*\(")
    for match in pattern.finditer(text):
        open_paren = text.find("(", match.start())
        try:
            after_params = _balanced_end(text, open_paren, "(", ")")
        except BenchError:
            continue
        cursor = after_params
        while cursor < len(text):
            skipped = _skip_quoted_or_comment(text, cursor)
            if skipped is not None:
                cursor = skipped
                continue
            if text[cursor].isspace():
                cursor += 1
                continue
            break
        if cursor >= len(text) or text[cursor] != "{":
            continue
        end = _balanced_end(text, cursor, "{", "}")
        # Work-unit target signatures are emitted on one logical line.  Include
        # indentation but not preceding declarations or pragmas.
        start = text.rfind("\n", 0, match.start()) + 1
        return start, end
    raise BenchError(f"function definition not found: {function}")


FORBIDDEN_CANDIDATE = [
    (re.compile(r"#\s*include\b"), "#include"),
    (re.compile(r"\b(?:__asm__|__asm|asm)\b"), "inline assembly"),
    (re.compile(r"\.inc\b", re.IGNORECASE), ".inc reference"),
]


def extract_candidate(response: str, function: str) -> str:
    blocks = re.findall(r"```(?:c|C)?\s*\n(.*?)```", response, flags=re.DOTALL)
    search = blocks if blocks else [response]
    last_error: Exception | None = None
    for block in search:
        try:
            start, end = find_function_span(block, function)
            candidate = block[start:end].strip() + "\n"
            for pattern, label in FORBIDDEN_CANDIDATE:
                if pattern.search(candidate):
                    raise BenchError(f"candidate rejected: {label} is forbidden")
            return candidate
        except BenchError as exc:
            last_error = exc
    raise BenchError(str(last_error or BenchError("response contains no C function")))


def splice_function(base: str, function: str, candidate: str) -> str:
    candidate_start, candidate_end = find_function_span(candidate, function)
    clean_candidate = candidate[candidate_start:candidate_end].strip()
    base_start, base_end = find_function_span(base, function)
    return base[:base_start] + clean_candidate + base[base_end:]


def find_symbol(diff_side: dict[str, Any], function: str) -> dict[str, Any] | None:
    for symbol in diff_side.get("symbols", []):
        if symbol.get("name") == function:
            return symbol
    return None


def instruction_summary(diff: dict[str, Any], function: str, limit: int = 36) -> str:
    lines: list[str] = []
    for side_name in ("left", "right"):
        symbol = find_symbol(diff.get(side_name, {}), function)
        if not symbol:
            continue
        label = "target" if side_name == "left" else "candidate"
        mismatches = [row for row in symbol.get("instructions", []) if row.get("diff_kind")]
        lines.append(f"{label} mismatches ({len(mismatches)}):")
        for row in mismatches[:limit]:
            inst = row.get("instruction") or {}
            address = int(inst.get("address", "0"))
            formatted = inst.get("formatted", "<missing>")
            lines.append(f"  +0x{address:04X} {row.get('diff_kind')}: {formatted}")
        if len(mismatches) > limit:
            lines.append(f"  ... {len(mismatches) - limit} more")
    return "\n".join(lines) if lines else "No instruction-level diff was available."


def score_object(workunit: Path, candidate_object: Path, function: str) -> CompileScore:
    if not OBJDIFF.is_file():
        raise BenchError(f"missing objdiff tool: {OBJDIFF}")
    target = workunit / "target.o"
    if not target.is_file():
        raise BenchError(f"missing target object: {target}")
    result = run_command(
        [
            str(OBJDIFF),
            "diff",
            "-1",
            str(target),
            "-2",
            str(candidate_object),
            "-o",
            "-",
            "--format",
            "json",
            function,
        ],
        cwd=REPO,
        timeout=30,
        max_output=8_000_000,
    )
    if result.returncode != 0:
        detail = (result.stderr or result.stdout).strip()
        raise BenchError(f"objdiff failed ({result.returncode}): {detail[-1200:]}")
    try:
        diff = json.loads(result.stdout)
    except json.JSONDecodeError as exc:
        raise BenchError("objdiff returned invalid JSON") from exc
    symbol = find_symbol(diff.get("left", {}), function)
    if symbol is None:
        raise BenchError(f"objdiff did not find target symbol: {function}")
    percent = float(symbol.get("match_percent", 0.0))
    size_value = symbol.get("size")
    object_size = int(size_value) if size_value is not None else None
    return CompileScore(
        compile_ok=True,
        match_percent=percent,
        object_size=object_size,
        feedback=instruction_summary(diff, function),
        diff=diff,
    )


def _semantic_text_size(
    config: SemanticOracleConfig,
    elf: Path,
    *,
    output: Path,
    label: str,
) -> tuple[int | None, str | None]:
    relocation_check = run_command(
        [str(config.readelf), "-rW", str(elf)], cwd=REPO, timeout=30
    )
    if relocation_check.returncode != 0:
        detail = (relocation_check.stderr or relocation_check.stdout).strip()
        raise SemanticError(
            f"semantic readelf failed for {label} ({relocation_check.returncode}): "
            f"{detail[-1000:]}"
        )
    if "there are no relocations in this file" not in relocation_check.stdout.lower():
        return None, f"{label} ELF contains relocations"

    output.unlink(missing_ok=True)
    extraction = run_command(
        [
            str(config.objcopy),
            "-O",
            "binary",
            "--only-section=.text",
            str(elf),
            str(output),
        ],
        cwd=REPO,
        timeout=30,
    )
    try:
        if extraction.returncode != 0 or not output.is_file() or output.is_symlink():
            detail = (extraction.stderr or extraction.stdout).strip()
            raise SemanticError(
                f"semantic objcopy failed for {label} ({extraction.returncode}): "
                f"{detail[-1000:]}"
            )
        size = output.stat().st_size
        if size <= 0 or size % 4 or size > SEMANTIC_MAX_TEXT_SIZE:
            return (
                None,
                f"{label} .text size must be non-empty, word-aligned, and at most "
                f"{SEMANTIC_MAX_TEXT_SIZE} bytes: {size}",
            )
        return size, None
    finally:
        output.unlink(missing_ok=True)


def _semantic_feedback(report: dict[str, Any], *, mismatch_limit: int) -> str:
    fixture_count = report.get("fixture_count")
    fixture_text = (
        str(fixture_count)
        if isinstance(fixture_count, int) and not isinstance(fixture_count, bool)
        else "configured"
    )
    equal = report["equal"]
    mismatch_count = report["mismatch_count"]
    if equal:
        return f"PASS: all {fixture_text} deterministic fixtures matched."

    lines = [
        f"FAIL: {mismatch_count} observable mismatch(es) across "
        f"{fixture_text} deterministic fixtures."
    ]
    raw_mismatches = report["mismatches"]
    for mismatch in raw_mismatches[:mismatch_limit]:
        compact = " ".join(mismatch.split())[:320]
        lines.append(f"- {compact}")
    omitted = report.get("omitted_mismatches", 0)
    if isinstance(omitted, int) and not isinstance(omitted, bool) and omitted > 0:
        lines.append(f"- ... {omitted} additional mismatch(es) omitted")
    return "\n".join(lines)[:2400]


def _semantic_report(
    report_file: Path,
    *,
    config: SemanticOracleConfig,
    returncode: int,
) -> dict[str, Any]:
    if not report_file.is_file() or report_file.is_symlink():
        raise SemanticError("semantic driver did not create a regular report file")
    if report_file.stat().st_size > SEMANTIC_MAX_REPORT_SIZE:
        raise SemanticError("semantic driver report exceeded the size limit")
    try:
        report = json.loads(report_file.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        raise SemanticError("semantic driver returned an invalid report") from exc
    if not isinstance(report, dict) or report.get("schema_version") != 1:
        raise SemanticError("semantic driver returned an unsupported report schema")
    if report.get("profile") != config.profile:
        raise SemanticError("semantic driver report named the wrong profile")
    if report.get("function") != config.function:
        raise SemanticError("semantic driver report named the wrong function")
    expected_hashes = {
        "driver": config.expected_driver_sha256,
        "sidecar": config.expected_sidecar_sha256,
        "build attestation": config.expected_attestation_sha256,
        "native sidecar": config.expected_native_sidecar_sha256,
        "native build attestation": config.expected_native_attestation_sha256,
        "native generated manifest": config.expected_native_manifest_sha256,
        "pin manifest": config.expected_pins_sha256,
        "original DOL": config.expected_dol_sha256,
        "pin report": config.expected_pin_report_sha256,
    }
    if any(
        not isinstance(value, str) or not re.fullmatch(r"[0-9a-f]{64}", value)
        for value in expected_hashes.values()
    ):
        raise SemanticError("semantic runtime fingerprints were not bound at run start")
    if report.get("driver_sha256") != config.expected_driver_sha256:
        raise SemanticError("semantic driver report has the wrong driver fingerprint")
    if report.get("sidecar_sha256") != config.expected_sidecar_sha256:
        raise SemanticError("semantic driver report has the wrong sidecar fingerprint")
    if report.get("build_attestation_sha256") != config.expected_attestation_sha256:
        raise SemanticError("semantic driver report has the wrong build attestation fingerprint")
    if not re.fullmatch(
        r"[0-9a-f]{64}", str(report.get("build_attestation_state_sha256") or "")
    ):
        raise SemanticError("semantic driver report has an invalid attested build state")
    if report.get("engine") != SEMANTIC_ENGINE:
        raise SemanticError("semantic driver report has an untrusted engine identity")
    native = report.get("native_qualification")
    if not isinstance(native, dict):
        raise SemanticError("semantic driver report omitted native-original qualification")
    if native.get("engine") != SEMANTIC_NATIVE_ENGINE:
        raise SemanticError(
            "semantic driver report has an untrusted native-original engine identity"
        )
    if native.get("sidecar_sha256") != config.expected_native_sidecar_sha256:
        raise SemanticError(
            "semantic driver report has the wrong native sidecar fingerprint"
        )
    if (
        native.get("build_attestation_sha256")
        != config.expected_native_attestation_sha256
    ):
        raise SemanticError(
            "semantic driver report has the wrong native build attestation fingerprint"
        )
    if (
        native.get("generated_manifest_sha256")
        != config.expected_native_manifest_sha256
    ):
        raise SemanticError(
            "semantic driver report has the wrong native generated manifest fingerprint"
        )
    if not re.fullmatch(
        r"[0-9a-f]{64}",
        str(native.get("build_attestation_state_sha256") or ""),
    ):
        raise SemanticError(
            "semantic driver report has an invalid native attested build state"
        )
    native_equal = native.get("equal")
    native_mismatch_count = native.get("mismatch_count")
    if native_equal is not True:
        raise SemanticError("native-original qualification did not match Dolphin")
    if (
        isinstance(native_mismatch_count, bool)
        or not isinstance(native_mismatch_count, int)
        or native_mismatch_count != 0
    ):
        raise SemanticError(
            "native-original qualification has a nonzero or invalid mismatch count"
        )
    expected_provenance = dict(config.expected_provenance)
    if report.get("provenance") != expected_provenance:
        raise SemanticError("semantic driver report has the wrong pinned provenance")
    pins = report.get("pins")
    if not isinstance(pins, dict):
        raise SemanticError("semantic driver report omitted checkout authority")
    canonical_pins = json.dumps(pins, separators=(",", ":"), sort_keys=True).encode(
        "utf-8"
    )
    if hashlib.sha256(canonical_pins).hexdigest() != config.expected_pin_report_sha256:
        raise SemanticError("semantic driver report has the wrong checkout authority")
    if pins.get("manifest_sha256") != config.expected_pins_sha256:
        raise SemanticError("semantic driver report has the wrong pin manifest")
    authority = report.get("reference_authority")
    expected_authority = SEMANTIC_PROFILE_AUTHORITIES.get(config.profile)
    if not isinstance(authority, dict) or expected_authority is None:
        raise SemanticError("semantic driver report omitted reference authority")
    if (
        authority.get("virtual_address") != expected_authority["virtual_address"]
        or authority.get("size") != expected_authority["size"]
        or authority.get("dol_sha1") != expected_authority["dol_sha1"]
        or authority.get("dol_sha256") != config.expected_dol_sha256
        or not re.fullmatch(r"[0-9a-f]{64}", str(authority.get("text_sha256") or ""))
    ):
        raise SemanticError("semantic driver report has the wrong reference authority")
    if report.get("fixture_count") != config.fixture_count:
        raise SemanticError("semantic driver report has the wrong fixture count")
    if report.get("fixture_seed") != f"0x{config.seed:08x}":
        raise SemanticError("semantic driver report has the wrong fixture seed")
    fixture_corpus_sha256 = report.get("fixture_corpus_sha256")
    if not isinstance(fixture_corpus_sha256, str) or not re.fullmatch(
        r"[0-9a-f]{64}", fixture_corpus_sha256
    ):
        raise SemanticError("semantic driver report has an invalid fixture fingerprint")
    equal = report.get("equal")
    mismatch_count = report.get("mismatch_count")
    mismatches = report.get("mismatches")
    if not isinstance(equal, bool):
        raise SemanticError("semantic driver report omitted a Boolean equal result")
    if (
        isinstance(mismatch_count, bool)
        or not isinstance(mismatch_count, int)
        or mismatch_count < 0
    ):
        raise SemanticError("semantic driver report has an invalid mismatch count")
    if (
        not isinstance(mismatches, list)
        or len(mismatches) > 4096
        or any(not isinstance(value, str) for value in mismatches)
    ):
        raise SemanticError("semantic driver report has invalid mismatch details")
    if equal != (returncode == 0):
        raise SemanticError("semantic driver exit status disagrees with its report")
    if equal != (mismatch_count == 0):
        raise SemanticError("semantic driver equality disagrees with its mismatch count")
    report_file.chmod(0o600)
    return report


def run_semantic_oracle(
    config: SemanticOracleConfig,
    *,
    reference_elf: Path,
    candidate_elf: Path,
    report_file: Path,
    strict: bool,
) -> SemanticScore:
    reference_sha256 = file_sha256(reference_elf)
    candidate_sha256 = file_sha256(candidate_elf)
    if reference_sha256 is None or candidate_sha256 is None:
        raise SemanticError("semantic oracle input object disappeared before execution")

    report_file.unlink(missing_ok=True)
    scratch_stem = f".{report_file.stem}"
    reference_size, reference_reason = _semantic_text_size(
        config,
        reference_elf,
        output=report_file.parent / f"{scratch_stem}.reference.text.bin",
        label="reference",
    )
    if reference_reason is not None or reference_size is None:
        raise SemanticError(f"semantic reference is ineligible: {reference_reason}")
    _candidate_size, candidate_reason = _semantic_text_size(
        config,
        candidate_elf,
        output=report_file.parent / f"{scratch_stem}.candidate.text.bin",
        label="candidate",
    )
    if candidate_reason is not None:
        if strict:
            raise SemanticError(f"semantic baseline is ineligible: {candidate_reason}")
        report = {
            "schema_version": 1,
            "function": config.function,
            "profile": config.profile,
            "state": "candidate_ineligible",
            "reason": candidate_reason,
            "reference_elf_sha256": reference_sha256,
            "candidate_elf_sha256": candidate_sha256,
            "sidecar_sha256": config.expected_sidecar_sha256,
            "build_attestation_sha256": config.expected_attestation_sha256,
            "native_sidecar_sha256": config.expected_native_sidecar_sha256,
            "native_build_attestation_sha256": (
                config.expected_native_attestation_sha256
            ),
            "native_generated_manifest_sha256": (
                config.expected_native_manifest_sha256
            ),
        }
        write_json(report_file, report)
        return SemanticScore(
            evaluated=False,
            equal=None,
            mismatch_count=None,
            feedback=f"NOT EVALUATED: {candidate_reason}.",
            report_sha256=file_sha256(report_file),
            reference_elf_sha256=reference_sha256,
            candidate_elf_sha256=candidate_sha256,
            report=report,
        )

    if (
        config.expected_sidecar_sha256 is None
        or config.expected_attestation_sha256 is None
        or config.expected_native_sidecar_sha256 is None
        or config.expected_native_attestation_sha256 is None
        or config.expected_native_manifest_sha256 is None
    ):
        raise SemanticError("semantic runtime fingerprints were not bound at run start")
    result = run_command(
        [
            sys.executable,
            str(config.driver),
            "run",
            "--profile",
            config.profile,
            "--checkout",
            str(config.checkout),
            "--pins",
            str(config.pins),
            "--sidecar",
            str(config.sidecar),
            "--attestation",
            str(config.attestation),
            "--expected-sidecar-sha256",
            config.expected_sidecar_sha256,
            "--expected-attestation-sha256",
            config.expected_attestation_sha256,
            "--native-sidecar",
            str(config.native_sidecar),
            "--native-attestation",
            str(config.native_attestation),
            "--native-manifest",
            str(config.native_manifest),
            "--expected-native-sidecar-sha256",
            config.expected_native_sidecar_sha256,
            "--expected-native-attestation-sha256",
            config.expected_native_attestation_sha256,
            "--expected-native-manifest-sha256",
            config.expected_native_manifest_sha256,
            "--reference-elf",
            str(reference_elf),
            "--candidate-elf",
            str(candidate_elf),
            "--dol",
            str(config.dol),
            "--objcopy",
            str(config.objcopy),
            "--readelf",
            str(config.readelf),
            "--fixture-count",
            str(config.fixture_count),
            "--seed",
            f"0x{config.seed:08x}",
            "--timeout",
            str(config.timeout),
            "--mismatch-limit",
            str(config.mismatch_limit),
            "--report-file",
            str(report_file),
        ],
        cwd=REPO,
        timeout=config.timeout * 3 + 150,
        max_output=8000,
    )
    if result.returncode not in (0, 1):
        detail = (result.stderr or result.stdout).strip()
        raise SemanticError(
            f"semantic driver failed ({result.returncode}): {detail[-2000:]}"
        )
    report = _semantic_report(report_file, config=config, returncode=result.returncode)
    score = SemanticScore(
        evaluated=True,
        equal=bool(report["equal"]),
        mismatch_count=int(report["mismatch_count"]),
        feedback=_semantic_feedback(report, mismatch_limit=config.mismatch_limit),
        report_sha256=file_sha256(report_file),
        reference_elf_sha256=reference_sha256,
        candidate_elf_sha256=candidate_sha256,
        report=report,
    )
    return score


def compile_and_score(
    workunit: Path,
    source: Path,
    function: str,
    round_name: str,
    *,
    semantic: SemanticOracleConfig | None = None,
    semantic_strict: bool = False,
) -> CompileScore:
    output = workunit / f"{safe_slug(round_name)}.o"
    output.unlink(missing_ok=True)
    compiler = workunit / "compile.sh"
    if not compiler.is_file():
        raise BenchError(f"missing compile script: {compiler}")
    source_argument = source.name if source.parent.resolve() == workunit.resolve() else str(source)
    result = run_command(
        [str(compiler), source_argument, "-o", output.name],
        cwd=workunit,
        timeout=90,
    )
    if result.returncode != 0 or not output.is_file():
        detail = (result.stderr or result.stdout).strip()
        output.unlink(missing_ok=True)
        return CompileScore(
            compile_ok=False,
            match_percent=0.0,
            object_size=None,
            feedback=f"MWCC compile failed ({result.returncode}):\n{detail[-6000:]}",
        )
    try:
        score = score_object(workunit, output, function)
        if semantic is not None:
            score.semantic = run_semantic_oracle(
                semantic,
                reference_elf=workunit / "target.o",
                candidate_elf=output,
                report_file=workunit / f"{safe_slug(round_name)}.semantic.json",
                strict=semantic_strict,
            )
            if (
                score.match_percent >= 100.0
                and score.semantic.evaluated
                and score.semantic.equal is False
            ):
                raise SemanticError(
                    "semantic oracle disagreed with an objdiff-exact candidate; "
                    "benchmark invariant failed"
                )
        return score
    finally:
        output.unlink(missing_ok=True)


def ppc_objdump_path() -> Path:
    configured = os.environ.get("PPC_OBJDUMP")
    if configured:
        path = Path(configured).expanduser()
        return path.resolve() if path.is_absolute() else (REPO / path).resolve()
    return (
        Path.home()
        / ".cache"
        / "pkmn-permuter-tools"
        / "ppc-binutils"
        / "powerpc-eabi-objdump"
    )


def ppc_objcopy_path() -> Path:
    configured = os.environ.get("PPC_OBJCOPY")
    if configured:
        path = Path(configured).expanduser()
        return path.resolve() if path.is_absolute() else (REPO / path).resolve()
    return ppc_objdump_path().with_name("powerpc-eabi-objcopy")


def ppc_readelf_path() -> Path:
    configured = os.environ.get("PPC_READELF")
    if configured:
        path = Path(configured).expanduser()
        return path.resolve() if path.is_absolute() else (REPO / path).resolve()
    return ppc_objdump_path().with_name("powerpc-eabi-readelf")


def target_assembly(workunit: Path, function: str) -> str:
    objdump = ppc_objdump_path()
    if not objdump.is_file():
        raise BenchError(f"missing powerpc-eabi-objdump: {objdump}")
    result = run_command(
        [
            str(objdump),
            "-dr",
            "-EB",
            "-mpowerpc",
            "-M",
            "broadway",
            f"--disassemble={function}",
            "target.o",
        ],
        cwd=workunit,
        timeout=30,
        max_output=80_000,
    )
    if result.returncode != 0:
        raise BenchError(f"powerpc-eabi-objdump failed with exit {result.returncode}")
    return result.stdout.strip()


def load_key_file(path: Path, label: str) -> str:
    if not path.is_file():
        raise BenchError(f"credential file not found: {path}")
    mode = stat.S_IMODE(path.stat().st_mode)
    if mode & 0o077:
        raise BenchError(f"refusing credential file with mode {mode:o}; require 600: {path}")
    values: dict[str, str] = {}
    bare_values: list[str] = []
    for raw in path.read_text(encoding="utf-8").splitlines():
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        if ":" not in line:
            bare_values.append(line)
            continue
        name, value = line.split(":", 1)
        values[name.strip().lower()] = value.strip()
    value = bare_values[-1] if label == "@last" and bare_values else values.get(label.lower(), "")
    if not value:
        raise BenchError(f"credential label not present: {label}")
    return value


def provider_payload(
    provider: str, model: str, messages: list[dict[str, Any]], max_tokens: int
) -> dict[str, Any]:
    payload: dict[str, Any] = {
        "model": model,
        "messages": messages,
        "stream": True,
        "stream_options": {"include_usage": True},
    }
    lower = model.lower()
    if lower == "kimi-k3":
        payload.update(
            {
                "max_completion_tokens": max_tokens,
                "temperature": 1.0,
                "top_p": 0.95,
                "reasoning_effort": "max",
            }
        )
    elif provider == "moonshot":
        payload.update(
            {
                "max_completion_tokens": max_tokens,
                "temperature": 1.0,
                "top_p": 0.95,
                "thinking": {"type": "enabled"},
            }
        )
    elif provider == "deepseek":
        payload.update(
            {
                "max_tokens": max_tokens,
                "temperature": 1.0,
                "top_p": 0.95,
                "reasoning_effort": "high",
                "thinking": {"type": "enabled"},
            }
        )
    elif provider == "glm":
        payload.update(
            {
                "max_tokens": max_tokens,
                "temperature": 1.0,
                "top_p": 0.95,
                "thinking": {"type": "enabled"},
            }
        )
    else:
        payload.update({"max_tokens": max_tokens, "temperature": 1.0, "top_p": 0.95})
    return payload


def _merge_delta(target: dict[str, Any], delta: dict[str, Any]) -> None:
    for key, value in delta.items():
        if value is None:
            continue
        if key == "role":
            target[key] = value
        elif isinstance(value, str):
            target[key] = str(target.get(key, "")) + value
        elif isinstance(value, list):
            target.setdefault(key, []).extend(value)
        else:
            target[key] = value


class NoRedirectHandler(urllib.request.HTTPRedirectHandler):
    """Never forward an API credential to a redirected origin."""

    def redirect_request(
        self,
        req: urllib.request.Request,
        fp: Any,
        code: int,
        msg: str,
        headers: Any,
        newurl: str,
    ) -> None:
        return None


def safe_provider_field(value: Any, key: str) -> str:
    rendered = str(value or "").replace(key, "[REDACTED]")
    rendered = re.sub(r"\b(?:sk-|mk-)[A-Za-z0-9_-]{12,}\b", "[REDACTED]", rendered)
    return rendered[:600]


def parse_openai_stream(
    lines: Iterable[bytes],
    *,
    key: str,
    requested_model: str,
    deadline: float | None = None,
) -> tuple[dict[str, Any], str, dict[str, Any], str]:
    assistant: dict[str, Any] = {"role": "assistant", "content": ""}
    usage: dict[str, Any] = {}
    served_model = requested_model
    finish_reason = ""
    saw_delta = False
    saw_done = False
    for raw in lines:
        if deadline is not None and time.monotonic() > deadline:
            raise BenchError("provider exceeded the request wall-time budget")
        line = raw.decode("utf-8", errors="replace").strip()
        if not line.startswith("data:"):
            continue
        data = line[5:].strip()
        if not data:
            continue
        if data == "[DONE]":
            saw_done = True
            continue
        try:
            event = json.loads(data)
        except json.JSONDecodeError as exc:
            raise BenchError("provider returned malformed streaming JSON") from exc
        if not isinstance(event, dict):
            raise BenchError("provider returned a malformed streaming event")
        if isinstance(event.get("error"), dict):
            error = event["error"]
            error_type = safe_provider_field(error.get("type"), key)[:120]
            error_message = safe_provider_field(error.get("message"), key)
            detail = ": ".join(value for value in (error_type, error_message) if value)
            suffix = f" ({detail})" if detail else ""
            raise BenchError(f"provider returned a streaming error{suffix}")
        if event.get("model"):
            served_model = str(event["model"])
        if isinstance(event.get("usage"), dict):
            usage = event["usage"]
        choices = event.get("choices") or []
        if not isinstance(choices, list) or (choices and not isinstance(choices[0], dict)):
            raise BenchError("provider returned malformed streaming choices")
        if choices and isinstance(choices[0].get("delta"), dict):
            _merge_delta(assistant, choices[0]["delta"])
            saw_delta = saw_delta or bool(choices[0]["delta"])
        if choices and choices[0].get("finish_reason"):
            finish_reason = str(choices[0]["finish_reason"])
    if not saw_delta:
        raise BenchError("provider stream contained no assistant delta")
    if not saw_done and not finish_reason:
        raise BenchError("provider stream ended before a completion marker")
    return assistant, served_model, usage, finish_reason


def call_openai_compatible(
    *,
    provider: str,
    model: str,
    messages: list[dict[str, Any]],
    key_file: Path,
    key_label: str | None,
    max_tokens: int,
    timeout: int,
) -> ApiReply:
    config = PROVIDERS[provider]
    label = key_label or config["key_label"]
    key = load_key_file(key_file, label)
    payload = provider_payload(provider, model, messages, max_tokens)
    request = urllib.request.Request(
        config["base_url"].rstrip("/") + "/chat/completions",
        data=json.dumps(payload, separators=(",", ":")).encode("utf-8"),
        headers={
            "Authorization": f"Bearer {key}",
            "Content-Type": "application/json",
            "Accept": "text/event-stream",
            "User-Agent": "pkmn-colosseum-compile-loop/1",
        },
        method="POST",
    )
    started = time.monotonic()
    opener = urllib.request.build_opener(NoRedirectHandler())
    try:
        with opener.open(request, timeout=timeout) as response:
            assistant, served_model, usage, finish_reason = parse_openai_stream(
                response, key=key, requested_model=model, deadline=started + timeout
            )
    except urllib.error.HTTPError as exc:
        # Parse only the provider's structured error fields. Never echo a raw
        # body or headers; either can be unsafe when a proxy reflects a request.
        error_type = ""
        error_message = ""
        try:
            body = json.loads(exc.read(16_384).decode("utf-8", errors="replace"))
            error = body.get("error") if isinstance(body, dict) else None
            if isinstance(error, dict):
                error_type = safe_provider_field(error.get("type"), key)[:120]
                error_message = safe_provider_field(error.get("message"), key)
        except (ValueError, OSError):
            pass
        detail = ": ".join(value for value in (error_type, error_message) if value)
        suffix = f" ({detail})" if detail else ""
        raise BenchError(f"provider returned HTTP {exc.code}{suffix}") from exc
    except urllib.error.URLError as exc:
        raise BenchError(f"provider connection failed: {type(exc.reason).__name__}") from exc
    except TimeoutError as exc:
        raise BenchError(f"provider timed out after {timeout}s") from exc
    return ApiReply(
        assistant=assistant,
        served_model=served_model,
        usage=usage,
        elapsed_seconds=time.monotonic() - started,
        finish_reason=finish_reason,
    )


def semantic_prompt_block(score: CompileScore) -> str:
    if score.semantic is None:
        return ""
    return (
        "\nDolphin-interpreter behavioral feedback (diagnostic only):\n"
        "```\n"
        f"{score.semantic.feedback}\n"
        "```\n"
    )


def initial_prompt(
    *,
    function: str,
    meta: dict[str, Any],
    base: str,
    assembly: str,
    baseline: CompileScore,
    source_commit: str,
) -> str:
    semantic_block = semantic_prompt_block(baseline)
    return f"""Match `{function}` using the supplied work-unit C and target assembly.

Pinned source baseline: {source_commit}
Compiler: {meta.get('mw_version', 'unknown')}
Flags: {meta.get('cflags', 'unknown')}
Baseline match: {baseline.match_percent:.5f}%
{semantic_block}

Current preprocessed work-unit C:
```c
{base.rstrip()}
```

Target assembly:
```asm
{assembly}
```

Return only the complete replacement definition for `{function}`."""


def feedback_prompt(
    *,
    function: str,
    score: CompileScore,
    attempted_candidate: str,
    best_percent: float,
    best_candidate: str,
) -> str:
    status = (
        f"compiled; objdiff match {score.match_percent:.5f}%"
        if score.compile_ok
        else "did not compile"
    )
    semantic_block = semantic_prompt_block(score)
    return f"""Round result for `{function}`: {status}.
Best match so far: {best_percent:.5f}%.

Compiler/objdiff feedback:
```
{score.feedback}
```
{semantic_block}

Function that produced this feedback:
```c
{attempted_candidate.rstrip() if attempted_candidate else "<no function was extracted>"}
```

Best known function to revise:
```c
{best_candidate.rstrip()}
```

Return one revised complete C definition. Respect the no-assembly/no-.inc rules."""


def git_head() -> str:
    result = run_command(["git", "rev-parse", "HEAD"], cwd=REPO, timeout=10)
    if result.returncode != 0:
        return "unknown"
    return result.stdout.strip()


def git_dirty() -> bool:
    result = run_command(
        ["git", "status", "--porcelain=v1", "--untracked-files=all"],
        cwd=REPO,
        timeout=10,
    )
    return result.returncode != 0 or bool(result.stdout.strip())


def validate_source_baseline(commit: str) -> None:
    if not re.fullmatch(r"[0-9a-fA-F]{40}", commit):
        raise BenchError("suite source_commit must be a full 40-character Git hash")
    ancestor = run_command(
        ["git", "merge-base", "--is-ancestor", commit, "HEAD"], cwd=REPO, timeout=10
    )
    if ancestor.returncode != 0:
        raise BenchError(f"suite source baseline is not an ancestor of HEAD: {commit}")
    committed = run_command(
        ["git", "diff", "--quiet", commit, "HEAD", "--", *SOURCE_BASELINE_PATHS],
        cwd=REPO,
        timeout=10,
    )
    if committed.returncode == 1:
        raise BenchError(
            "active source/config changed after the suite baseline; "
            "regenerate the suite deliberately"
        )
    if committed.returncode != 0:
        raise BenchError("could not compare HEAD with the suite source baseline")
    working = run_command(
        [
            "git",
            "status",
            "--porcelain=v1",
            "--untracked-files=all",
            "--",
            *SOURCE_BASELINE_PATHS,
        ],
        cwd=REPO,
        timeout=10,
    )
    if working.returncode != 0:
        raise BenchError("could not inspect active source/config state")
    if working.stdout.strip():
        raise BenchError("active source/config has working-tree changes; benchmark refused")


def workunit_sha256(workunit: Path) -> str:
    digest = hashlib.sha256()
    for name in WORKUNIT_FINGERPRINT_FILES:
        path = workunit / name
        if not path.is_file():
            raise BenchError(f"work unit fingerprint input is missing: {path}")
        digest.update(name.encode("utf-8"))
        digest.update(b"\0")
        digest.update(path.read_bytes())
        digest.update(b"\0")
    return digest.hexdigest()


def file_sha256(path: Path) -> str | None:
    if not path.is_file():
        return None
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def execution_tool_fingerprints(
    workunits: list[Path],
    semantic_configs: Iterable[SemanticOracleConfig] = (),
) -> dict[str, str | None]:
    tools: dict[str, Path] = {
        "objdiff-cli": OBJDIFF,
        "powerpc-eabi-objdump": ppc_objdump_path(),
        "wibo": REPO / "build" / "tools" / "wibo",
        "sjiswrap.exe": REPO / "build" / "tools" / "sjiswrap.exe",
    }
    for workunit in workunits:
        meta = json.loads((workunit / "meta.json").read_text(encoding="utf-8"))
        version = str(meta.get("mw_version") or "")
        if version:
            tools[f"mwcceppc:{version}"] = (
                REPO / "build" / "compilers" / version / "mwcceppc.exe"
            )
    for config in semantic_configs:
        tools.update(
            {
                "semantic-driver": config.driver,
                "semantic-pin-manifest": config.pins,
                "semantic-sidecar": config.sidecar,
                "semantic-sidecar-attestation": config.attestation,
                "semantic-native-sidecar": config.native_sidecar,
                "semantic-native-sidecar-attestation": config.native_attestation,
                "semantic-native-manifest": config.native_manifest,
                "semantic-original-dol": config.dol,
                "semantic-powerpc-eabi-objcopy": config.objcopy,
                "semantic-powerpc-eabi-readelf": config.readelf,
            }
        )
    return {name: file_sha256(path) for name, path in sorted(tools.items())}


def load_suite(path: Path) -> tuple[dict[str, Any], list[dict[str, Any]]]:
    try:
        suite = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise BenchError(f"invalid suite: {path}") from exc
    targets = suite.get("targets")
    if not isinstance(targets, list) or not targets:
        raise BenchError("suite must contain a non-empty targets list")
    return suite, targets


def _semantic_integer(
    value: Any,
    *,
    field: str,
    minimum: int,
    maximum: int,
) -> int:
    if isinstance(value, bool):
        raise BenchError(f"semantic {field} must be an integer")
    try:
        parsed = int(value, 0) if isinstance(value, str) else int(value)
    except (TypeError, ValueError) as exc:
        raise BenchError(f"semantic {field} must be an integer") from exc
    if not minimum <= parsed <= maximum:
        raise BenchError(
            f"semantic {field} must be between {minimum} and {maximum}"
        )
    return parsed


def resolve_semantic_config(
    target: dict[str, Any], args: argparse.Namespace
) -> SemanticOracleConfig | None:
    raw = target.get("semantic_oracle")
    if raw is None:
        return None
    if not isinstance(raw, dict):
        raise BenchError("semantic_oracle target configuration must be an object")
    allowed = {"profile", "fixture_count", "seed", "timeout", "mismatch_limit"}
    unknown = sorted(set(raw) - allowed)
    if unknown:
        raise BenchError(f"unknown semantic_oracle setting(s): {', '.join(unknown)}")
    profile = str(raw.get("profile") or "")
    expected_function = SEMANTIC_PROFILE_FUNCTIONS.get(profile)
    function = str(target.get("function") or "")
    if expected_function is None:
        raise BenchError(f"unsupported semantic oracle profile: {profile or '<missing>'}")
    if function != expected_function:
        raise BenchError(
            f"semantic profile {profile} is for {expected_function}, not {function}"
        )
    checkout_value = getattr(args, "semantic_checkout", None)
    sidecar_value = getattr(args, "semantic_sidecar", None)
    native_sidecar_value = getattr(args, "semantic_native_sidecar", None)
    if not checkout_value or not sidecar_value or not native_sidecar_value:
        raise BenchError(
            "selected target requires --semantic-checkout, --semantic-sidecar, "
            "and --semantic-native-sidecar"
        )
    checkout = Path(checkout_value).expanduser().resolve()
    sidecar = Path(sidecar_value).expanduser().resolve()
    attestation = sidecar.with_name(sidecar.name + ".attestation.json")
    native_sidecar = Path(native_sidecar_value).expanduser().resolve()
    native_attestation = native_sidecar.with_name(
        native_sidecar.name + ".attestation.json"
    )
    native_manifest = native_sidecar.with_name(
        native_sidecar.name + ".generated-manifest.json"
    )
    if not checkout.is_dir():
        raise BenchError(f"semantic checkout is not a directory: {checkout}")
    if not sidecar.is_file() or not os.access(sidecar, os.X_OK):
        raise BenchError(f"semantic sidecar is not executable: {sidecar}")
    if not attestation.is_file() or attestation.is_symlink():
        raise BenchError(f"semantic sidecar build attestation is missing: {attestation}")
    if not native_sidecar.is_file() or not os.access(native_sidecar, os.X_OK):
        raise BenchError(f"semantic native sidecar is not executable: {native_sidecar}")
    if not native_attestation.is_file() or native_attestation.is_symlink():
        raise BenchError(
            "semantic native sidecar build attestation is missing: "
            f"{native_attestation}"
        )
    if not native_manifest.is_file() or native_manifest.is_symlink():
        raise BenchError(
            f"semantic native sidecar generated manifest is missing: {native_manifest}"
        )
    return SemanticOracleConfig(
        profile=profile,
        function=function,
        checkout=checkout,
        sidecar=sidecar,
        attestation=attestation,
        native_sidecar=native_sidecar,
        native_attestation=native_attestation,
        native_manifest=native_manifest,
        driver=SEMANTIC_DRIVER,
        pins=SEMANTIC_PINS,
        dol=SEMANTIC_DOL,
        objcopy=ppc_objcopy_path(),
        readelf=ppc_readelf_path(),
        fixture_count=_semantic_integer(
            raw.get("fixture_count", 1000),
            field="fixture_count",
            minimum=1,
            maximum=10000,
        ),
        seed=_semantic_integer(
            raw.get("seed", "0x6c6f7373"),
            field="seed",
            minimum=0,
            maximum=0xFFFFFFFF,
        ),
        timeout=_semantic_integer(
            raw.get("timeout", 120), field="timeout", minimum=1, maximum=600
        ),
        mismatch_limit=_semantic_integer(
            raw.get("mismatch_limit", 8),
            field="mismatch_limit",
            minimum=1,
            maximum=64,
        ),
    )


def verify_semantic_checkout(config: SemanticOracleConfig) -> dict[str, Any]:
    result = run_command(
        [
            sys.executable,
            str(config.driver),
            "verify-pins",
            "--checkout",
            str(config.checkout),
            "--pins",
            str(config.pins),
        ],
        cwd=REPO,
        timeout=90,
        max_output=1024 * 1024,
    )
    if result.returncode != 0:
        detail = (result.stderr or result.stdout).strip()
        raise SemanticError(
            f"semantic checkout pin verification failed ({result.returncode}): "
            f"{detail[-2000:]}"
        )
    try:
        report = json.loads(result.stdout)
    except json.JSONDecodeError as exc:
        raise SemanticError("semantic checkout verifier returned invalid JSON") from exc
    if not isinstance(report, dict) or not isinstance(report.get("components"), list):
        raise SemanticError("semantic checkout verifier returned an invalid report")
    manifest_sha256 = file_sha256(config.pins)
    if report.get("manifest_sha256") != manifest_sha256:
        raise SemanticError("semantic checkout report named the wrong pin manifest")
    return report


def bind_semantic_runtime(
    config: SemanticOracleConfig,
    *,
    tool_hashes: dict[str, str | None],
    pin_report: dict[str, Any],
) -> SemanticOracleConfig:
    required = {
        "expected_sidecar_sha256": "semantic-sidecar",
        "expected_attestation_sha256": "semantic-sidecar-attestation",
        "expected_native_sidecar_sha256": "semantic-native-sidecar",
        "expected_native_attestation_sha256": "semantic-native-sidecar-attestation",
        "expected_native_manifest_sha256": "semantic-native-manifest",
        "expected_driver_sha256": "semantic-driver",
        "expected_pins_sha256": "semantic-pin-manifest",
        "expected_dol_sha256": "semantic-original-dol",
    }
    values: dict[str, str] = {}
    for field, key in required.items():
        digest = tool_hashes.get(key)
        if not isinstance(digest, str) or not re.fullmatch(r"[0-9a-f]{64}", digest):
            raise SemanticError(f"semantic runtime has no valid run-start fingerprint: {key}")
        values[field] = digest
    components = pin_report.get("components")
    if not isinstance(components, list):
        raise SemanticError("semantic checkout verifier omitted components")
    provenance: list[tuple[str, str]] = []
    for component in components:
        if not isinstance(component, dict):
            raise SemanticError("semantic checkout verifier returned an invalid component")
        name = component.get("name")
        commit = component.get("commit")
        if (
            not isinstance(name, str)
            or not isinstance(commit, str)
            or not re.fullmatch(r"[0-9a-f]{40}", commit)
        ):
            raise SemanticError("semantic checkout verifier returned invalid provenance")
        provenance.append((name, commit))
    if (
        len(provenance) != 3
        or {name for name, _ in provenance}
        != {"ModernGekko", "RecompCore", "DolRecomp"}
    ):
        raise SemanticError("semantic checkout verifier returned incomplete provenance")
    canonical_pin_report = json.dumps(
        pin_report, separators=(",", ":"), sort_keys=True
    ).encode("utf-8")
    return replace(
        config,
        **values,
        expected_pin_report_sha256=hashlib.sha256(canonical_pin_report).hexdigest(),
        expected_provenance=tuple(sorted(provenance)),
    )


def semantic_config_record(
    config: SemanticOracleConfig, pin_report: dict[str, Any]
) -> dict[str, Any]:
    canonical_pin_report = json.dumps(
        pin_report, separators=(",", ":"), sort_keys=True
    ).encode("utf-8")
    return {
        "profile": config.profile,
        "function": config.function,
        "fixture_count": config.fixture_count,
        "seed": f"0x{config.seed:08x}",
        "timeout": config.timeout,
        "mismatch_limit": config.mismatch_limit,
        "checkout": display_path(config.checkout),
        "sidecar": display_path(config.sidecar),
        "attestation": display_path(config.attestation),
        "native_sidecar": display_path(config.native_sidecar),
        "native_attestation": display_path(config.native_attestation),
        "native_manifest": display_path(config.native_manifest),
        "driver": display_path(config.driver),
        "pins": display_path(config.pins),
        "dol": display_path(config.dol),
        "objcopy": display_path(config.objcopy),
        "readelf": display_path(config.readelf),
        "pin_report": pin_report,
        "pin_report_sha256": hashlib.sha256(canonical_pin_report).hexdigest(),
        "expected_sidecar_sha256": config.expected_sidecar_sha256,
        "expected_attestation_sha256": config.expected_attestation_sha256,
        "expected_native_sidecar_sha256": config.expected_native_sidecar_sha256,
        "expected_native_attestation_sha256": (
            config.expected_native_attestation_sha256
        ),
        "expected_native_manifest_sha256": config.expected_native_manifest_sha256,
        "expected_driver_sha256": config.expected_driver_sha256,
        "expected_pins_sha256": config.expected_pins_sha256,
        "expected_dol_sha256": config.expected_dol_sha256,
    }


def resolve_workunit(suite: dict[str, Any], target: dict[str, Any]) -> Path:
    root_value = target.get("workunit_root", suite.get("workunit_root"))
    if not root_value:
        raise BenchError("suite has no workunit_root")
    root = Path(root_value)
    if not root.is_absolute():
        root = REPO / root
    function = str(target["function"])
    workunit = root / function
    if not workunit.is_dir():
        raise BenchError(
            f"work unit not found for {function}: {workunit}; regenerate the suite work units"
        )
    meta_path = workunit / "meta.json"
    try:
        meta = json.loads(meta_path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise BenchError(f"invalid work-unit metadata: {meta_path}") from exc
    if meta.get("fn") != function:
        raise BenchError(f"work-unit function mismatch for {function}")
    if meta.get("fidelity") != "isolated-equals-full-tu":
        raise BenchError(f"work unit is not fidelity-gated: {function}")
    try:
        extract_candidate((workunit / "base.c").read_text(encoding="utf-8"), function)
    except (OSError, BenchError) as exc:
        raise BenchError(f"work-unit incumbent violates source guardrails: {function}") from exc
    try:
        expected_score = float(target["baseline_match_percent"])
        metadata_score = float(meta["pct"])
    except (KeyError, TypeError, ValueError) as exc:
        raise BenchError(f"invalid baseline metadata for {function}") from exc
    if abs(expected_score - metadata_score) > 0.001:
        raise BenchError(f"suite and work-unit baseline metadata disagree for {function}")
    expected_hash = str(target.get("workunit_sha256") or "").lower()
    if not re.fullmatch(r"[0-9a-f]{64}", expected_hash):
        raise BenchError(f"suite has no valid workunit_sha256 for {function}")
    actual_hash = workunit_sha256(workunit)
    if actual_hash != expected_hash:
        raise BenchError(
            f"work-unit fingerprint changed for {function}: "
            f"expected {expected_hash}, got {actual_hash}"
        )
    return workunit


def copy_workunit(source: Path, destination: Path) -> None:
    if destination.exists():
        shutil.rmtree(destination)
    destination.mkdir(mode=0o700, parents=True)
    for name in WORKUNIT_FINGERPRINT_FILES:
        source_path = source / name
        destination_path = destination / name
        shutil.copy2(source_path, destination_path)
        executable = bool(source_path.stat().st_mode & stat.S_IXUSR)
        destination_path.chmod(0o700 if executable else 0o600)


def write_private_text(path: Path, value: str) -> None:
    path.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
    descriptor, temporary = tempfile.mkstemp(prefix=f".{path.name}.", dir=path.parent, text=True)
    try:
        os.fchmod(descriptor, 0o600)
        with os.fdopen(descriptor, "w", encoding="utf-8") as stream:
            stream.write(value)
            stream.flush()
            os.fsync(stream.fileno())
        os.replace(temporary, path)
        path.chmod(0o600)
    finally:
        try:
            os.unlink(temporary)
        except FileNotFoundError:
            pass


def write_json(path: Path, value: Any) -> None:
    write_private_text(path, json.dumps(value, indent=2, sort_keys=True) + "\n")


def run_target(
    *,
    source_workunit: Path,
    run_directory: Path,
    function: str,
    provider: str,
    model: str,
    rounds: int,
    max_tokens: int,
    timeout: int,
    key_file: Path,
    key_label: str | None,
    source_commit: str,
    runner_commit: str,
    expected_baseline: float,
    expected_workunit_sha256: str,
    allow_reasoning_salvage: bool,
    semantic_config: SemanticOracleConfig | None = None,
) -> dict[str, Any]:
    copy_workunit(source_workunit, run_directory)
    copied_sha256 = workunit_sha256(run_directory)
    if copied_sha256 != expected_workunit_sha256:
        raise BenchError(f"work unit changed while copying: {function}")
    base_path = run_directory / "base.c"
    meta_path = run_directory / "meta.json"
    base = base_path.read_text(encoding="utf-8")
    meta = json.loads(meta_path.read_text(encoding="utf-8")) if meta_path.is_file() else {}
    base_start, base_end = find_function_span(base, function)
    baseline_candidate = base[base_start:base_end].strip() + "\n"
    baseline = compile_and_score(
        run_directory,
        base_path,
        function,
        "baseline",
        semantic=semantic_config,
        semantic_strict=True,
    )
    if not baseline.compile_ok:
        raise BenchError(f"fidelity-gated baseline stopped compiling for {function}")
    if abs(baseline.match_percent - expected_baseline) > 0.001:
        raise BenchError(
            f"baseline score changed for {function}: expected {expected_baseline:.6f}, "
            f"got {baseline.match_percent:.6f}"
        )

    initial_user_prompt = initial_prompt(
        function=function,
        meta=meta,
        base=base,
        assembly=target_assembly(run_directory, function),
        baseline=baseline,
        source_commit=source_commit,
    )
    messages: list[dict[str, Any]] = [
        {"role": "system", "content": SYSTEM_PROMPT},
        {"role": "user", "content": initial_user_prompt},
    ]
    write_json(run_directory / "initial_messages.json", messages)
    initial_messages_sha256 = hashlib.sha256(
        json.dumps(messages, separators=(",", ":"), sort_keys=True).encode("utf-8")
    ).hexdigest()
    best_percent = baseline.match_percent
    best_candidate = baseline_candidate
    salvage_best_percent: float | None = None
    round_results: list[dict[str, Any]] = []
    aggregate_usage: dict[str, int] = {}
    started = time.monotonic()

    for round_index in range(1, rounds + 1):
        request_payload_sha256 = hashlib.sha256(
            json.dumps(
                provider_payload(provider, model, messages, max_tokens),
                separators=(",", ":"),
                sort_keys=True,
            ).encode("utf-8")
        ).hexdigest()
        reply = call_openai_compatible(
            provider=provider,
            model=model,
            messages=messages,
            key_file=key_file,
            key_label=key_label,
            max_tokens=max_tokens,
            timeout=timeout,
        )
        messages.append(reply.assistant)
        for key, value in reply.usage.items():
            if isinstance(value, int):
                aggregate_usage[key] = aggregate_usage.get(key, 0) + value

        candidate_error = ""
        candidate = ""
        candidate_channel = "content"
        try:
            try:
                candidate = extract_candidate(reply.content, function)
            except BenchError:
                if not allow_reasoning_salvage:
                    raise
                candidate = extract_candidate(
                    str(reply.assistant.get("reasoning_content") or ""), function
                )
                candidate_channel = "reasoning_salvage"
            candidate_source = splice_function(base, function, candidate)
            candidate_path = run_directory / f"round_{round_index:02d}.c"
            write_private_text(candidate_path, candidate_source)
            score = compile_and_score(
                run_directory,
                candidate_path,
                function,
                f"round_{round_index:02d}",
                semantic=(semantic_config if candidate_channel == "content" else None),
            )
        except SemanticError:
            raise
        except BenchError as exc:
            candidate_error = str(exc)
            score = CompileScore(False, 0.0, None, candidate_error)

        primary_candidate = candidate_channel == "content" and bool(candidate)
        improved = primary_candidate and score.compile_ok and score.match_percent > best_percent
        salvage_improved = (
            candidate_channel == "reasoning_salvage"
            and score.compile_ok
            and (salvage_best_percent is None or score.match_percent > salvage_best_percent)
        )
        if salvage_improved:
            salvage_best_percent = score.match_percent
        if improved:
            best_percent = score.match_percent
            best_candidate = candidate
            write_private_text(run_directory / "best_function.c", best_candidate)

        round_record = {
            "round": round_index,
            "served_model": reply.served_model,
            "request_payload_sha256": request_payload_sha256,
            "elapsed_seconds": round(reply.elapsed_seconds, 3),
            "finish_reason": reply.finish_reason,
            "usage": reply.usage,
            "compile_ok": score.compile_ok,
            "primary_compile_ok": primary_candidate and score.compile_ok,
            "salvage_compile_ok": candidate_channel == "reasoning_salvage" and score.compile_ok,
            "match_percent": round(score.match_percent, 6),
            "improved": improved,
            "salvage_improved": salvage_improved,
            "candidate_error": candidate_error,
            "candidate_channel": candidate_channel if candidate else "none",
            "response": reply.content,
            "reasoning_characters": len(str(reply.assistant.get("reasoning_content") or "")),
            "feedback": score.feedback,
            "semantic_evaluated": (
                score.semantic.evaluated if score.semantic is not None else None
            ),
            "semantic_equal": score.semantic.equal if score.semantic is not None else None,
            "semantic_mismatch_count": (
                score.semantic.mismatch_count if score.semantic is not None else None
            ),
            "semantic_feedback": (
                score.semantic.feedback if score.semantic is not None else None
            ),
            "semantic_report": (
                f"round_{round_index:02d}.semantic.json"
                if score.semantic is not None and score.semantic.report_sha256 is not None
                else None
            ),
            "semantic_report_sha256": (
                score.semantic.report_sha256 if score.semantic is not None else None
            ),
            "semantic_reference_elf_sha256": (
                score.semantic.reference_elf_sha256 if score.semantic is not None else None
            ),
            "semantic_candidate_elf_sha256": (
                score.semantic.candidate_elf_sha256 if score.semantic is not None else None
            ),
        }
        write_json(run_directory / f"round_{round_index:02d}.json", round_record)
        round_results.append(round_record)

        if primary_candidate and score.compile_ok and score.match_percent >= 100.0:
            break
        feedback_score = score
        feedback_candidate = candidate
        if candidate_channel == "reasoning_salvage":
            feedback_score = CompileScore(
                False,
                0.0,
                None,
                "No complete function appeared in final content; hidden-reasoning salvage "
                "is excluded from primary feedback.",
            )
            feedback_candidate = ""
        messages.append(
            {
                "role": "user",
                "content": feedback_prompt(
                    function=function,
                    score=feedback_score,
                    attempted_candidate=feedback_candidate,
                    best_percent=best_percent,
                    best_candidate=best_candidate,
                ),
            }
        )

    if not (run_directory / "best_function.c").exists():
        write_private_text(run_directory / "best_function.c", best_candidate)
    isolated_objdiff_exact = best_percent >= 100.0
    return {
        "function": function,
        "provider": provider,
        "requested_model": model,
        "source_commit": source_commit,
        "runner_commit": runner_commit,
        "workunit_sha256": copied_sha256,
        "initial_messages_sha256": initial_messages_sha256,
        "workunit": display_path(source_workunit),
        "baseline_match_percent": round(baseline.match_percent, 6),
        "semantic_oracle_profile": (
            semantic_config.profile if semantic_config is not None else None
        ),
        "baseline_semantic_evaluated": (
            baseline.semantic.evaluated if baseline.semantic is not None else None
        ),
        "baseline_semantic_equal": (
            baseline.semantic.equal if baseline.semantic is not None else None
        ),
        "baseline_semantic_mismatch_count": (
            baseline.semantic.mismatch_count if baseline.semantic is not None else None
        ),
        "baseline_semantic_feedback": (
            baseline.semantic.feedback if baseline.semantic is not None else None
        ),
        "baseline_semantic_report": (
            "baseline.semantic.json"
            if baseline.semantic is not None and baseline.semantic.report_sha256 is not None
            else None
        ),
        "baseline_semantic_report_sha256": (
            baseline.semantic.report_sha256 if baseline.semantic is not None else None
        ),
        "baseline_semantic_reference_elf_sha256": (
            baseline.semantic.reference_elf_sha256
            if baseline.semantic is not None
            else None
        ),
        "baseline_semantic_candidate_elf_sha256": (
            baseline.semantic.candidate_elf_sha256
            if baseline.semantic is not None
            else None
        ),
        "best_match_percent": round(best_percent, 6),
        "improvement_points": round(best_percent - baseline.match_percent, 6),
        "isolated_objdiff_exact": isolated_objdiff_exact,
        # Compatibility alias for benchmark consumers written before campaign
        # acceptance was made explicit. This is never a full-DOL claim.
        "exact": isolated_objdiff_exact,
        "campaign_bankable": False,
        "campaign_acceptance_status": (
            "requires-source-integration-and-full-dol-validation"
            if isolated_objdiff_exact
            else "not-isolated-objdiff-exact"
        ),
        "full_dol_validation": {
            "performed": False,
            "passed": None,
            "artifact": FULL_DOL_ARTIFACT,
            "sha1_authority": FULL_DOL_SHA1_AUTHORITY,
        },
        "reasoning_salvage_best_match_percent": (
            round(salvage_best_percent, 6) if salvage_best_percent is not None else None
        ),
        "reasoning_salvage_improvement_points": (
            round(salvage_best_percent - baseline.match_percent, 6)
            if salvage_best_percent is not None
            else None
        ),
        "reasoning_salvage_exact": (
            salvage_best_percent is not None and salvage_best_percent >= 100.0
        ),
        "rounds_completed": len(round_results),
        "elapsed_seconds": round(time.monotonic() - started, 3),
        "usage": aggregate_usage,
        "rounds": round_results,
    }


def command_score(args: argparse.Namespace) -> int:
    workunit = Path(args.workunit).resolve()
    meta = json.loads((workunit / "meta.json").read_text(encoding="utf-8"))
    function = args.function or meta.get("fn")
    if not function:
        raise BenchError("function is required when meta.json has no fn")
    source = Path(args.source).resolve() if args.source else workunit / "base.c"
    score = compile_and_score(workunit, source, function, "score_probe")
    print(
        json.dumps(
            {
                "function": function,
                "compile_ok": score.compile_ok,
                "match_percent": round(score.match_percent, 6),
                "feedback": score.feedback,
            },
            indent=2,
        )
    )
    return 0 if score.compile_ok else 1


def command_prepare(args: argparse.Namespace) -> int:
    workunit = Path(args.workunit).resolve()
    meta = json.loads((workunit / "meta.json").read_text(encoding="utf-8"))
    function = args.function or meta.get("fn")
    if not function:
        raise BenchError("function is required when meta.json has no fn")
    base_path = workunit / "base.c"
    baseline = compile_and_score(workunit, base_path, function, "prepare_probe")
    if not baseline.compile_ok:
        raise BenchError(f"baseline stopped compiling for {function}")
    prompt = initial_prompt(
        function=function,
        meta=meta,
        base=base_path.read_text(encoding="utf-8"),
        assembly=target_assembly(workunit, function),
        baseline=baseline,
        source_commit=git_head(),
    )
    if args.output:
        write_private_text(Path(args.output), prompt + "\n")
    else:
        print(prompt)
    return 0


def command_run(args: argparse.Namespace) -> int:
    suite_path = Path(args.suite).resolve()
    suite, targets = load_suite(suite_path)
    source_commit = str(suite.get("source_commit") or "")
    validate_source_baseline(source_commit)
    if args.only:
        wanted = set(args.only)
        targets = [target for target in targets if target.get("function") in wanted]
    if args.limit is not None:
        targets = targets[: args.limit]
    if not targets:
        raise BenchError("no suite targets selected")

    resolved_targets = [
        (
            target,
            resolve_workunit(suite, target),
            resolve_semantic_config(target, args),
        )
        for target in targets
    ]
    semantic_configs = [
        config for _, _, config in resolved_targets if config is not None
    ]
    runner_commit = git_head()
    runner_dirty = git_dirty()
    key_file = Path(args.key_file).expanduser().resolve()
    payload_shape = provider_payload(args.provider, args.model, [], args.max_tokens)
    payload_shape.pop("messages", None)
    tool_hashes = execution_tool_fingerprints(
        [workunit for _, workunit, _ in resolved_targets], semantic_configs
    )
    missing_tools = [name for name, digest in tool_hashes.items() if digest is None]
    if missing_tools:
        raise BenchError(f"missing benchmark execution tools: {', '.join(missing_tools)}")
    semantic_runtime_reports: dict[tuple[str, ...], dict[str, Any]] = {}
    semantic_records: list[dict[str, Any]] = []
    bound_targets: list[
        tuple[dict[str, Any], Path, SemanticOracleConfig | None]
    ] = []
    for target, workunit, config in resolved_targets:
        if config is None:
            bound_targets.append((target, workunit, None))
            continue
        runtime_key = tuple(
            str(path)
            for path in (
                config.driver,
                config.checkout,
                config.pins,
                config.sidecar,
                config.attestation,
                config.native_sidecar,
                config.native_attestation,
                config.native_manifest,
            )
        )
        pin_report = semantic_runtime_reports.get(runtime_key)
        if pin_report is None:
            pin_report = verify_semantic_checkout(config)
            semantic_runtime_reports[runtime_key] = pin_report
        bound_config = bind_semantic_runtime(
            config, tool_hashes=tool_hashes, pin_report=pin_report
        )
        semantic_records.append(semantic_config_record(bound_config, pin_report))
        bound_targets.append((target, workunit, bound_config))
    resolved_targets = bound_targets

    timestamp = dt.datetime.now(dt.timezone.utc).strftime("%Y%m%dT%H%M%S.%fZ")
    run_directory = RUN_ROOT / f"{timestamp}_{safe_slug(args.provider)}_{safe_slug(args.model)}"
    RUN_ROOT.mkdir(parents=True, exist_ok=True)
    RUN_ROOT.chmod(0o700)
    run_directory.mkdir(mode=0o700, exist_ok=False)
    write_json(
        run_directory / "run_config.json",
        {
            "suite": display_path(suite_path),
            "provider": args.provider,
            "model": args.model,
            "rounds": args.rounds,
            "max_tokens": args.max_tokens,
            "timeout": args.timeout,
            "allow_reasoning_salvage": args.allow_reasoning_salvage,
            "source_commit": source_commit,
            "runner_commit": runner_commit,
            "runner_dirty": runner_dirty,
            "runner_script_sha256": file_sha256(Path(__file__).resolve()),
            "suite_sha256": file_sha256(suite_path),
            "system_prompt": SYSTEM_PROMPT,
            "request_parameters": payload_shape,
            "execution_tool_sha256": tool_hashes,
            "semantic_oracles": semantic_records,
            "targets": [
                {
                    "function": target.get("function"),
                    "workunit_sha256": target.get("workunit_sha256"),
                    "baseline_match_percent": target.get("baseline_match_percent"),
                    "class": target.get("class"),
                    "semantic_oracle": target.get("semantic_oracle"),
                }
                for target, _, _ in resolved_targets
            ],
        },
    )

    results: list[dict[str, Any]] = []
    results_jsonl = run_directory / "results.jsonl"
    run_status: dict[str, Any] = {
        "state": "running",
        "selected": len(resolved_targets),
        "completed": 0,
        "failed": 0,
        "failures": [],
    }
    write_json(run_directory / "run_status.json", run_status)
    for index, (target, source_workunit, semantic_config) in enumerate(
        resolved_targets, 1
    ):
        function = str(target["function"])
        print(f"[{index}/{len(targets)}] {function}", flush=True)
        try:
            expected_baseline = float(target["baseline_match_percent"])
        except (KeyError, TypeError, ValueError) as exc:
            raise BenchError(f"suite has no valid baseline score for {function}") from exc
        try:
            result = run_target(
                source_workunit=source_workunit,
                run_directory=run_directory / safe_slug(function),
                function=function,
                provider=args.provider,
                model=args.model,
                rounds=args.rounds,
                max_tokens=args.max_tokens,
                timeout=args.timeout,
                key_file=key_file,
                key_label=args.key_label,
                source_commit=source_commit,
                runner_commit=runner_commit,
                expected_baseline=expected_baseline,
                expected_workunit_sha256=str(target["workunit_sha256"]),
                allow_reasoning_salvage=args.allow_reasoning_salvage,
                semantic_config=semantic_config,
            )
        except BenchError as exc:
            run_status.update({"state": "failed", "failed": 1})
            run_status["failures"] = [
                {
                    "function": function,
                    "category": failure_category(exc),
                    "message": str(exc),
                }
            ]
            write_json(run_directory / "run_status.json", run_status)
            raise
        except Exception as exc:
            run_status.update({"state": "failed", "failed": 1})
            run_status["failures"] = [
                {
                    "function": function,
                    "category": "internal",
                    "message": type(exc).__name__,
                }
            ]
            write_json(run_directory / "run_status.json", run_status)
            raise
        result["class"] = target.get("class")
        results.append(result)
        with results_jsonl.open("a", encoding="utf-8") as output:
            output.write(json.dumps(result, sort_keys=True) + "\n")
        results_jsonl.chmod(0o600)
        run_status["completed"] = len(results)
        write_json(run_directory / "run_status.json", run_status)
        print(
            f"  {result['baseline_match_percent']:.3f}% -> "
            f"{result['best_match_percent']:.3f}% "
            f"({result['rounds_completed']} rounds)",
            flush=True,
        )

    summary = {
        "provider": args.provider,
        "model": args.model,
        "source_commit": source_commit,
        "runner_commit": runner_commit,
        "runner_dirty": runner_dirty,
        "functions": len(results),
        "compile_at_1": sum(
            bool(result["rounds"] and result["rounds"][0]["primary_compile_ok"])
            for result in results
        ),
        "reasoning_salvage_compile_at_1": sum(
            bool(result["rounds"] and result["rounds"][0]["salvage_compile_ok"])
            for result in results
        ),
        "reasoning_salvage_exact": sum(result["reasoning_salvage_exact"] for result in results),
        "isolated_objdiff_exact": sum(
            result["isolated_objdiff_exact"] for result in results
        ),
        # Compatibility alias; see the per-target acceptance fields.
        "exact": sum(result["isolated_objdiff_exact"] for result in results),
        "campaign_bankable": sum(result["campaign_bankable"] for result in results),
        "mean_baseline_match_percent": round(
            sum(result["baseline_match_percent"] for result in results) / len(results), 6
        ),
        "mean_best_match_percent": round(
            sum(result["best_match_percent"] for result in results) / len(results), 6
        ),
        "run_directory": str(run_directory.relative_to(REPO)),
    }
    write_json(run_directory / "summary.json", summary)
    run_status["state"] = "completed"
    write_json(run_directory / "run_status.json", run_status)
    print(json.dumps(summary, indent=2))
    return 0


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    score = subparsers.add_parser("score", help="compile and objdiff one local candidate")
    score.add_argument("workunit")
    score.add_argument("--source", help="candidate translation unit; defaults to base.c")
    score.add_argument("--function")
    score.set_defaults(handler=command_score)

    prepare = subparsers.add_parser("prepare", help="render the initial user prompt")
    prepare.add_argument("workunit")
    prepare.add_argument("--function")
    prepare.add_argument("-o", "--output")
    prepare.set_defaults(handler=command_prepare)

    run = subparsers.add_parser("run", help="run a provider/model through the suite")
    run.add_argument("--provider", choices=sorted(PROVIDERS), required=True)
    run.add_argument("--model", required=True)
    run.add_argument("--suite", default=str(DEFAULT_SUITE))
    run.add_argument("--rounds", type=int, default=3)
    run.add_argument("--max-tokens", type=int, default=16_384)
    run.add_argument(
        "--timeout",
        type=int,
        default=900,
        help="socket timeout and approximate wall budget, checked between stream events",
    )
    run.add_argument("--limit", type=int)
    run.add_argument("--only", action="append", help="function to include; repeatable")
    run.add_argument("--key-file", default=str(DEFAULT_KEY_FILE))
    run.add_argument("--key-label", help="override the provider's credential label")
    run.add_argument(
        "--semantic-checkout",
        help="external exact-pinned ModernGekko checkout; required by semantic targets",
    )
    run.add_argument(
        "--semantic-sidecar",
        help="external GPL semantic-sidecar executable; required by semantic targets",
    )
    run.add_argument(
        "--semantic-native-sidecar",
        help=(
            "external pinned ModernGekko/DolRecomp native-original sidecar; "
            "required by semantic targets"
        ),
    )
    run.add_argument(
        "--allow-reasoning-salvage",
        action="store_true",
        help="diagnostically compile C found in reasoning; excluded from primary metrics",
    )
    run.set_defaults(handler=command_run)
    return parser


def main(argv: Iterable[str] | None = None) -> int:
    args = build_parser().parse_args(list(argv) if argv is not None else None)
    try:
        if getattr(args, "rounds", 1) < 1:
            raise BenchError("rounds must be at least 1")
        if getattr(args, "max_tokens", 1) < 1:
            raise BenchError("max-tokens must be at least 1")
        if getattr(args, "timeout", 1) < 1:
            raise BenchError("timeout must be at least 1")
        if getattr(args, "limit", None) is not None and args.limit < 1:
            raise BenchError("limit must be at least 1")
        return int(args.handler(args))
    except BenchError as exc:
        print(f"benchmark error: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
