#!/usr/bin/env python3
"""Pinned, deterministic semantic-oracle driver for integer leaf profiles.

The GPL runtimes remain external executables. This driver verifies their source
checkout and attestations, qualifies pinned DolRecomp-native original chunks
against Dolphin, then compares relocation-free candidate PPC under Dolphin
using only explicit ABI observables.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import stat
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Iterable


REPO = Path(__file__).resolve().parents[3]
DEFAULT_PINS = Path(__file__).with_name("moderngekko_pins.json")
DEFAULT_BINUTILS = Path.home() / ".cache" / "pkmn-permuter-tools" / "ppc-binutils"
DEFAULT_OBJCOPY = Path(os.environ.get("PPC_OBJCOPY", DEFAULT_BINUTILS / "powerpc-eabi-objcopy"))
DEFAULT_READELF = Path(os.environ.get("PPC_READELF", DEFAULT_BINUTILS / "powerpc-eabi-readelf"))
TRANSIENT_ROOT = REPO / "build" / "semantic_oracle"
DEFAULT_DOL = REPO / "orig" / "GC6E01" / "sys" / "main.dol"
SIDECAR_SOURCE_DIR = Path(__file__).with_name("gpl_sidecar")
SIDECAR_BUILD_INPUTS = (
    "CMakeLists.txt",
    "dolphin_oracle.cpp",
    "generate_native.py",
    "native_oracle.cpp",
)
SIDECAR_ATTESTATION_SUFFIX = ".attestation.json"
NATIVE_MANIFEST_SUFFIX = ".generated-manifest.json"

SCHEMA_VERSION = 1
BUILD_ATTESTATION_KIND = "moderngekko-dolphin-oracle-build"
EXPECTED_ENGINE = "dolphin-interpreter-from-moderngekko-tree"
NATIVE_BUILD_ATTESTATION_KIND = "moderngekko-dolrecomp-native-oracle-build"
NATIVE_EXPECTED_ENGINE = "moderngekko-dolrecomp-native-original"
FUNCTION_NAME = "msgctrlWait"
ENTRY_PC = 0x80132454
TARGET_TEXT_SIZE = 0x78
ORIGINAL_DOL_SHA1 = "870e8b9693ca780782d80f22a6a4572d8ba9458f"
ORIGINAL_DOL_SHA256 = "7e6c00a3bd632126d5466cbab856c63ac3c52a5da20b60dba7e756538152c9f4"
NATIVE_GENERATED_TREE_SHA256 = (
    "438b92e6109ac5263860dcbcee24152148c139a34c60bf17933b4253b7309048"
)
NATIVE_GENERATED_FILE_COUNT = 157
NATIVE_GENERATED_SHA256_RECIPE = (
    "sha256(concat(u64be(relative_path_utf8_length),relative_path_utf8,"
    "sha256(file_bytes))) for files sorted by relative POSIX path"
)
NATIVE_SELECTED_GENERATED_SHA256 = {
    "chunks/chunk_0059_text1_800ED5E0.c": (
        "8ece25c2b66ea205047e465d3b1f24b7694930677deecf92842f30305faf48d9"
    ),
    "chunks/chunk_0076_text1_801315E0.c": (
        "2755b985feecd5645aebc69b9708796459a365bc95c6986ef49af5dfaa32c794"
    ),
    "chunks/chunk_0105_text1_801A55E0.c": (
        "e63d85dfff5588fc1a71fa921dd79f5270ff91ea35411ba081be02d99e90a39f"
    ),
    "generated.h": "4a7a53d36da85ee05c7e2a6db98efebb90662e26c53580103b17cc4daa8de71e",
}
NATIVE_SELECTED_GENERATED_FILES = set(NATIVE_SELECTED_GENERATED_SHA256)
MAX_NATIVE_GENERATED_FILE_SIZE = 8 * 1024 * 1024
MAX_INSTRUCTIONS = 64
DEFAULT_PROFILE = "msgctrlWait-v1"
TEXTURE_PROFILE = "GStextureLockImage-v2"
TEXTURE_FUNCTION_NAME = "GStextureLockImage"
TEXTURE_ENTRY_PC = 0x800EF548
TEXTURE_TARGET_TEXT_SIZE = 0x30
TEXTURE_MAX_INSTRUCTIONS = 32
TEXTURE_OBJECT_BASE = 0x80410000
TEXTURE_OBJECT_SIZE = 0x60
MOBJ_ADD_TOBJ_PROFILE = "fn_801A6DA0-v1"
MOBJ_ADD_TOBJ_FUNCTION_NAME = "fn_801A6DA0"
MOBJ_ADD_TOBJ_ENTRY_PC = 0x801A6DA0
MOBJ_ADD_TOBJ_TARGET_TEXT_SIZE = 0x24
MOBJ_ADD_TOBJ_MAX_INSTRUCTIONS = 32
MOBJ_BASE = 0x80420000
MOBJ_SIZE = 0x20
TOBJ_BASE = 0x80421000
TOBJ_SIZE = 0xAC
OLD_TOBJ_BASE = 0x80422000
OBJECT_BASE = 0x80010000
OBJECT_SIZE = 0x50
OBJECT_STRIDE = 0x100
STREAM_OFFSET = 0x80
STREAM_SIZE = 0x10
STACK_POINTER = 0x817FF000
RETURN_PC = 0x817FFF00
DEFAULT_FIXTURE_COUNT = 1000
DEFAULT_SEED = 0x6C6F7373
MAX_FIXTURES = 10000
MAX_TEXT_SIZE = 4096
MAX_RESULT_SIZE = 64 * 1024 * 1024
ALLOWED_STATUSES = {
    "returned",
    "exception",
    "pc_out_of_range",
    "step_limit",
    "alert",
    "code_modified",
}


class OracleError(RuntimeError):
    """Expected, safely reportable semantic-oracle failure."""


@dataclass(frozen=True)
class FunctionSpec:
    name: str
    virtual_address: int
    size: int
    original_dol_sha1: str


@dataclass(frozen=True)
class OracleProfile:
    identifier: str
    spec: FunctionSpec
    max_instructions: int
    memory_contract: tuple[str, ...]


PILOT_SPEC = FunctionSpec(
    name=FUNCTION_NAME,
    virtual_address=ENTRY_PC,
    size=TARGET_TEXT_SIZE,
    original_dol_sha1=ORIGINAL_DOL_SHA1,
)

MSGCTRL_PROFILE = OracleProfile(
    identifier=DEFAULT_PROFILE,
    spec=PILOT_SPEC,
    max_instructions=MAX_INSTRUCTIONS,
    memory_contract=("object[0x00:0x50]", "stream[0x00:0x10]"),
)
GSTEXTURE_PROFILE = OracleProfile(
    identifier=TEXTURE_PROFILE,
    spec=FunctionSpec(
        name=TEXTURE_FUNCTION_NAME,
        virtual_address=TEXTURE_ENTRY_PC,
        size=TEXTURE_TARGET_TEXT_SIZE,
        original_dol_sha1=ORIGINAL_DOL_SHA1,
    ),
    max_instructions=TEXTURE_MAX_INSTRUCTIONS,
    memory_contract=("texture[0x00:0x60]",),
)
MOBJ_ADD_TOBJ_ORACLE_PROFILE = OracleProfile(
    identifier=MOBJ_ADD_TOBJ_PROFILE,
    spec=FunctionSpec(
        name=MOBJ_ADD_TOBJ_FUNCTION_NAME,
        virtual_address=MOBJ_ADD_TOBJ_ENTRY_PC,
        size=MOBJ_ADD_TOBJ_TARGET_TEXT_SIZE,
        original_dol_sha1=ORIGINAL_DOL_SHA1,
    ),
    max_instructions=MOBJ_ADD_TOBJ_MAX_INSTRUCTIONS,
    memory_contract=(
        "mobj[0x00:0x20] (tobj at +0x08)",
        "tobj[0x00:0xac] (next at +0x08)",
    ),
)
PROFILES = {
    MSGCTRL_PROFILE.identifier: MSGCTRL_PROFILE,
    GSTEXTURE_PROFILE.identifier: GSTEXTURE_PROFILE,
    MOBJ_ADD_TOBJ_ORACLE_PROFILE.identifier: MOBJ_ADD_TOBJ_ORACLE_PROFILE,
}


@dataclass(frozen=True)
class RamChange:
    address: int
    before: bytes
    after: bytes


@dataclass(frozen=True)
class RamSummary:
    digest: int
    changed_bytes: int
    truncated: bool
    changes: tuple[RamChange, ...]


@dataclass(frozen=True)
class Comparison:
    equal: bool
    mismatch_count: int
    mismatches: list[str]
    omitted_mismatches: int


class XorShift32:
    """Small version-stable PRNG used only for deterministic fixture bytes."""

    def __init__(self, seed: int):
        self.state = seed & 0xFFFFFFFF or 0xA341316C

    def next_u32(self) -> int:
        value = self.state
        value ^= (value << 13) & 0xFFFFFFFF
        value ^= value >> 17
        value ^= (value << 5) & 0xFFFFFFFF
        self.state = value & 0xFFFFFFFF
        return self.state

    def next_byte(self) -> int:
        return self.next_u32() & 0xFF

    def bytes(self, size: int) -> bytes:
        return bytes(self.next_byte() for _ in range(size))


def u32(value: Any, *, field: str) -> int:
    if isinstance(value, bool):
        raise OracleError(f"{field} must be a 32-bit integer")
    try:
        parsed = int(value, 0) if isinstance(value, str) else int(value)
    except (TypeError, ValueError) as exc:
        raise OracleError(f"{field} must be a 32-bit integer") from exc
    if not 0 <= parsed <= 0xFFFFFFFF:
        raise OracleError(f"{field} is outside the 32-bit range")
    return parsed


def u64_hex(value: Any, *, field: str) -> int:
    if not isinstance(value, str) or not re.fullmatch(r"0x[0-9a-fA-F]{16}", value):
        raise OracleError(f"{field} must be a 16-digit hexadecimal string")
    return int(value, 16)


def hex32(value: int) -> str:
    return f"0x{value & 0xFFFFFFFF:08x}"


def hex64(value: int) -> str:
    return f"0x{value & 0xFFFFFFFFFFFFFFFF:016x}"


def resolve_profile(identifier: str) -> OracleProfile:
    try:
        return PROFILES[identifier]
    except KeyError as exc:
        raise OracleError(f"unknown semantic-oracle profile: {identifier}") from exc


def sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def canonical_sha256(value: Any) -> str:
    payload = json.dumps(value, separators=(",", ":"), sort_keys=True).encode("utf-8")
    return hashlib.sha256(payload).hexdigest()


def sha1_file(path: Path) -> str:
    digest = hashlib.new("sha1")
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def read_dol_text(dol: Path, *, address: int, size: int) -> bytes:
    dol = dol.expanduser().resolve()
    if not dol.is_file():
        raise OracleError(f"original DOL not found: {dol}")
    if address & 3 or size <= 0 or size & 3:
        raise OracleError("DOL authority range must be non-empty and word-aligned")
    try:
        file_size = dol.stat().st_size
        with dol.open("rb") as stream:
            header = stream.read(0xD8)
            if len(header) != 0xD8:
                raise OracleError(f"truncated DOL header: {dol}")
            offsets = struct.unpack(">18I", header[0x00:0x48])
            addresses = struct.unpack(">18I", header[0x48:0x90])
            sizes = struct.unpack(">18I", header[0x90:0xD8])
            matches: list[int] = []
            requested_end = address + size
            for index, (offset, base, section_size) in enumerate(
                zip(offsets, addresses, sizes)
            ):
                if section_size == 0:
                    continue
                section_end = base + section_size
                file_end = offset + section_size
                if section_end > 0x100000000 or file_end > file_size:
                    raise OracleError(f"invalid DOL section {index}: {dol}")
                if index < 7 and base <= address and requested_end <= section_end:
                    matches.append(offset + address - base)
            if len(matches) != 1:
                raise OracleError(
                    f"{hex32(address)}+0x{size:x} is not in exactly one DOL text section"
                )
            stream.seek(matches[0])
            result = stream.read(size)
    except OSError as exc:
        raise OracleError(f"cannot read original DOL: {dol}") from exc
    if len(result) != size:
        raise OracleError(f"truncated DOL text at {hex32(address)}")
    return result


def verify_reference_authority(
    reference_code: bytes, dol: Path, spec: FunctionSpec = PILOT_SPEC
) -> dict[str, Any]:
    dol = dol.expanduser().resolve()
    if len(reference_code) != spec.size:
        raise OracleError(
            f"reference .text size mismatch for {spec.name}: "
            f"expected {spec.size}, got {len(reference_code)}"
        )
    if not re.fullmatch(r"[0-9a-f]{40}", spec.original_dol_sha1):
        raise OracleError(f"invalid original-DOL pin for {spec.name}")
    if not dol.is_file():
        raise OracleError(f"original DOL not found: {dol}")
    actual_dol_sha1 = sha1_file(dol)
    if actual_dol_sha1 != spec.original_dol_sha1:
        raise OracleError(
            f"original DOL SHA-1 mismatch: expected {spec.original_dol_sha1}, "
            f"got {actual_dol_sha1}"
        )
    original_code = read_dol_text(dol, address=spec.virtual_address, size=spec.size)
    if reference_code != original_code:
        first_difference = next(
            index
            for index, (reference_byte, original_byte) in enumerate(
                zip(reference_code, original_code)
            )
            if reference_byte != original_byte
        )
        raise OracleError(
            f"reference .text is not authoritative {spec.name} DOL code "
            f"(first mismatch at +0x{first_difference:x})"
        )
    return {
        "dol": display_path(dol),
        "dol_sha1": actual_dol_sha1,
        "dol_sha256": sha256_file(dol),
        "virtual_address": hex32(spec.virtual_address),
        "size": spec.size,
        "text_sha256": hashlib.sha256(original_code).hexdigest(),
    }


def display_path(path: Path) -> str:
    resolved = path.resolve()
    try:
        return str(resolved.relative_to(REPO))
    except ValueError:
        return str(resolved)


def run_command(
    argv: list[str], *, cwd: Path, timeout: int, max_output: int = 12000
) -> subprocess.CompletedProcess[str]:
    try:
        result = subprocess.run(
            argv,
            cwd=cwd,
            stdin=subprocess.DEVNULL,
            text=True,
            capture_output=True,
            timeout=timeout,
            check=False,
        )
    except subprocess.TimeoutExpired as exc:
        raise OracleError(f"command timed out after {timeout}s: {Path(argv[0]).name}") from exc
    result.stdout = result.stdout[-max_output:]
    result.stderr = result.stderr[-max_output:]
    return result


def write_private_text(path: Path, value: str) -> None:
    ensure_transient_path(path)
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


def write_private_json(path: Path, value: Any) -> None:
    write_private_text(path, json.dumps(value, indent=2, sort_keys=True) + "\n")


def read_json(path: Path, *, label: str, max_size: int = MAX_RESULT_SIZE) -> Any:
    if not path.is_file():
        raise OracleError(f"missing {label}: {path}")
    if path.stat().st_size > max_size:
        raise OracleError(f"{label} exceeds {max_size} bytes: {path}")
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        raise OracleError(f"invalid {label}: {path}") from exc


def ensure_transient_path(path: Path) -> None:
    resolved = path.expanduser().resolve()
    try:
        resolved.relative_to(REPO)
    except ValueError:
        return
    try:
        resolved.relative_to(REPO / "build")
    except ValueError as exc:
        raise OracleError(
            f"generated oracle artifacts must stay under build/ or outside the repo: {path}"
        ) from exc


def require_executable(path: Path, *, label: str) -> Path:
    resolved = path.expanduser().resolve()
    if not resolved.is_file() or not os.access(resolved, os.X_OK):
        raise OracleError(f"{label} is not an executable file: {resolved}")
    return resolved


def _opened_file_identity(info: os.stat_result) -> tuple[int, int, int, int, int]:
    return (info.st_dev, info.st_ino, info.st_size, info.st_mtime_ns, info.st_ctime_ns)


def read_regular_bytes(path: Path, *, label: str, max_size: int) -> bytes:
    """Read one stable regular-file inode and reject concurrent in-place writes."""

    resolved = path.expanduser().resolve()
    flags = os.O_RDONLY | getattr(os, "O_CLOEXEC", 0)
    flags |= getattr(os, "O_NOFOLLOW", 0)
    try:
        descriptor = os.open(resolved, flags)
    except OSError as exc:
        raise OracleError(f"cannot open {label}: {resolved}") from exc
    try:
        before = os.fstat(descriptor)
        if not stat.S_ISREG(before.st_mode):
            raise OracleError(f"{label} is not a regular file: {resolved}")
        if before.st_size > max_size:
            raise OracleError(f"{label} exceeds {max_size} bytes: {resolved}")
        with os.fdopen(descriptor, "rb", closefd=False) as stream:
            payload = stream.read(max_size + 1)
        after = os.fstat(descriptor)
    except OSError as exc:
        raise OracleError(f"cannot read {label}: {resolved}") from exc
    finally:
        os.close(descriptor)
    if len(payload) > max_size:
        raise OracleError(f"{label} exceeds {max_size} bytes: {resolved}")
    if _opened_file_identity(before) != _opened_file_identity(after):
        raise OracleError(f"{label} changed while it was being read: {resolved}")
    return payload


def snapshot_executable(
    source: Path,
    destination: Path,
    *,
    expected_sha256: str | None = None,
) -> tuple[Path, str]:
    """Copy one executable inode to a private path and hash the copied bytes."""

    source = require_executable(source, label="semantic sidecar")
    destination = destination.expanduser().resolve()
    ensure_transient_path(destination)
    destination.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
    flags = os.O_RDONLY | getattr(os, "O_CLOEXEC", 0)
    flags |= getattr(os, "O_NOFOLLOW", 0)
    try:
        source_fd = os.open(source, flags)
    except OSError as exc:
        raise OracleError(f"cannot open semantic sidecar: {source}") from exc
    output_fd, temporary = tempfile.mkstemp(
        prefix=f".{destination.name}.", dir=destination.parent
    )
    digest = hashlib.sha256()
    try:
        before = os.fstat(source_fd)
        if not stat.S_ISREG(before.st_mode) or not before.st_mode & stat.S_IXUSR:
            raise OracleError(f"semantic sidecar is not an executable file: {source}")
        with (
            os.fdopen(source_fd, "rb", closefd=False) as input_stream,
            os.fdopen(output_fd, "wb", closefd=False) as output_stream,
        ):
            for chunk in iter(lambda: input_stream.read(1024 * 1024), b""):
                digest.update(chunk)
                output_stream.write(chunk)
            output_stream.flush()
            os.fsync(output_stream.fileno())
        after = os.fstat(source_fd)
        if _opened_file_identity(before) != _opened_file_identity(after):
            raise OracleError("semantic sidecar changed while it was being snapshotted")
        actual_sha256 = digest.hexdigest()
        if expected_sha256 is not None and actual_sha256 != expected_sha256:
            raise OracleError(
                "semantic sidecar does not match the expected run-start fingerprint"
            )
        os.fchmod(output_fd, 0o500)
        os.replace(temporary, destination)
        return destination, actual_sha256
    except OSError as exc:
        raise OracleError(f"cannot snapshot semantic sidecar: {source}") from exc
    finally:
        os.close(source_fd)
        os.close(output_fd)
        try:
            os.unlink(temporary)
        except FileNotFoundError:
            pass


def normalize_repository(value: str) -> str:
    normalized = value.strip().replace("\\", "/")
    if normalized.startswith("git@github.com:"):
        normalized = "https://github.com/" + normalized[len("git@github.com:") :]
    normalized = normalized.removesuffix(".git").rstrip("/").lower()
    return normalized


def load_pins(path: Path = DEFAULT_PINS) -> dict[str, Any]:
    manifest = read_json(path, label="pin manifest", max_size=1024 * 1024)
    if not isinstance(manifest, dict) or manifest.get("schema_version") != SCHEMA_VERSION:
        raise OracleError("unsupported pin-manifest schema")
    components = manifest.get("components")
    if not isinstance(components, list) or not components:
        raise OracleError("pin manifest has no components")
    return manifest


def manifest_provenance(manifest: dict[str, Any]) -> dict[str, str]:
    provenance: dict[str, str] = {}
    for component in manifest.get("components", []):
        if not isinstance(component, dict):
            raise OracleError("invalid component in pin manifest")
        name = component.get("name")
        commit = component.get("commit")
        if (
            not isinstance(name, str)
            or not name
            or name in provenance
            or not isinstance(commit, str)
            or not re.fullmatch(r"[0-9a-f]{40}", commit)
        ):
            raise OracleError("invalid provenance component in pin manifest")
        provenance[name] = commit
    if set(provenance) != {"ModernGekko", "RecompCore", "DolRecomp"}:
        raise OracleError("pin manifest provenance must name ModernGekko, RecompCore, and DolRecomp")
    return provenance


def git_stdout(repository: Path, *args: str) -> str:
    result = run_command(["git", "-C", str(repository), *args], cwd=REPO, timeout=20)
    if result.returncode != 0:
        detail = (result.stderr or result.stdout).strip()
        raise OracleError(f"git {' '.join(args)} failed for {repository}: {detail[-800:]}")
    return result.stdout.strip()


def git_component_clean(repository: Path) -> bool:
    """Reject every staged, tracked, untracked, or nested-submodule change."""

    result = run_command(
        [
            "git",
            "-C",
            str(repository),
            "status",
            "--porcelain=v1",
            "--untracked-files=all",
            "--ignore-submodules=none",
        ],
        cwd=REPO,
        timeout=30,
        max_output=1024 * 1024,
    )
    if result.returncode != 0:
        detail = (result.stderr or result.stdout).strip()
        raise OracleError(f"git status failed for {repository}: {detail[-800:]}")
    # Porcelain v1 quotes unusual paths.  Treat the output as opaque rather
    # than parsing or reusing a nested-submodule path in another command.
    return not result.stdout


def verify_recursive_submodules(checkout: Path) -> int:
    result = run_command(
        ["git", "-C", str(checkout), "submodule", "status", "--recursive"],
        cwd=REPO,
        timeout=60,
        max_output=1024 * 1024,
    )
    if result.returncode != 0:
        detail = (result.stderr or result.stdout).strip()
        raise OracleError(f"cannot inspect recursive submodules: {detail[-1000:]}")
    lines = [line for line in result.stdout.splitlines() if line]
    drifted = [line for line in lines if line[0] != " "]
    if drifted:
        detail = "; ".join(line[:160] for line in drifted[:5])
        raise OracleError(f"recursive submodule checkout is incomplete or drifted: {detail}")
    return len(lines)


def verify_checkout_pins(checkout: Path, manifest_path: Path = DEFAULT_PINS) -> dict[str, Any]:
    checkout = checkout.expanduser().resolve()
    manifest_path = manifest_path.expanduser().resolve()
    if not checkout.is_dir():
        raise OracleError(f"ModernGekko checkout not found: {checkout}")
    manifest = load_pins(manifest_path)
    # Validate the complete, duplicate-free component set before trusting or
    # touching any manifest-provided paths.
    manifest_provenance(manifest)
    reports: list[dict[str, str]] = []
    for raw_component in manifest["components"]:
        if not isinstance(raw_component, dict):
            raise OracleError("invalid component in pin manifest")
        name = str(raw_component.get("name") or "")
        relative = Path(str(raw_component.get("path") or ""))
        expected_commit = str(raw_component.get("commit") or "").lower()
        expected_repository = str(raw_component.get("repository") or "")
        if not name or relative.is_absolute() or ".." in relative.parts:
            raise OracleError(f"invalid component path in pin manifest: {name or '<unnamed>'}")
        if not re.fullmatch(r"[0-9a-f]{40}", expected_commit):
            raise OracleError(f"invalid pinned commit for {name}")
        component = (checkout / relative).resolve()
        try:
            component.relative_to(checkout)
        except ValueError as exc:
            raise OracleError(f"component escapes checkout: {name}") from exc
        if not component.is_dir():
            raise OracleError(f"pinned component is not checked out: {name} ({component})")
        actual_commit = git_stdout(component, "rev-parse", "HEAD").lower()
        if actual_commit != expected_commit:
            raise OracleError(
                f"{name} HEAD mismatch: expected {expected_commit}, got {actual_commit}"
            )
        origin = git_stdout(component, "remote", "get-url", "origin")
        if normalize_repository(origin) != normalize_repository(expected_repository):
            raise OracleError(
                f"{name} origin mismatch: expected {expected_repository}, got {origin}"
            )
        if not git_component_clean(component):
            raise OracleError(
                f"{name} checkout is not clean (tracked, untracked, or submodule changes)"
            )
        parent_value = raw_component.get("gitlink_parent")
        gitlink_value = raw_component.get("gitlink_path")
        if parent_value is not None or gitlink_value is not None:
            parent_relative = Path(str(parent_value or ""))
            if parent_relative.is_absolute() or ".." in parent_relative.parts or not gitlink_value:
                raise OracleError(f"invalid gitlink metadata for {name}")
            parent = (checkout / parent_relative).resolve()
            gitlink = git_stdout(parent, "rev-parse", f"HEAD:{gitlink_value}").lower()
            if gitlink != expected_commit:
                raise OracleError(
                    f"{name} gitlink mismatch: expected {expected_commit}, got {gitlink}"
                )
        reports.append(
            {
                "name": name,
                "path": relative.as_posix(),
                "repository": expected_repository,
                "commit": actual_commit,
            }
        )
    recursive_submodules = verify_recursive_submodules(checkout)
    return {
        "manifest_sha256": sha256_file(manifest_path),
        "checkout": display_path(checkout),
        "recursive_submodules": recursive_submodules,
        "components": reports,
    }


def pin_identity(pin_report: dict[str, Any]) -> dict[str, Any]:
    """Return location-independent pinned source identity for an attestation."""

    manifest_sha256 = pin_report.get("manifest_sha256")
    recursive_submodules = pin_report.get("recursive_submodules")
    components = pin_report.get("components")
    if (
        not isinstance(manifest_sha256, str)
        or not re.fullmatch(r"[0-9a-f]{64}", manifest_sha256)
        or isinstance(recursive_submodules, bool)
        or not isinstance(recursive_submodules, int)
        or recursive_submodules < 0
        or not isinstance(components, list)
    ):
        raise OracleError("invalid checkout pin report for build attestation")
    return {
        "manifest_sha256": manifest_sha256,
        "recursive_submodules": recursive_submodules,
        "components": components,
    }


def sidecar_input_fingerprints() -> list[dict[str, str]]:
    inputs: list[dict[str, str]] = []
    for name in SIDECAR_BUILD_INPUTS:
        path = SIDECAR_SOURCE_DIR / name
        if not path.is_file() or path.is_symlink():
            raise OracleError(f"sidecar build input is not a regular file: {path}")
        inputs.append({"path": name, "sha256": sha256_file(path)})
    return inputs


def sidecar_build_state(pin_report: dict[str, Any]) -> dict[str, Any]:
    return {
        "schema_version": SCHEMA_VERSION,
        "attestation_tool_sha256": sha256_file(Path(__file__).resolve()),
        "sidecar_inputs": sidecar_input_fingerprints(),
        "pins": pin_identity(pin_report),
    }


def sidecar_attestation_path(sidecar: Path) -> Path:
    sidecar = sidecar.expanduser().resolve()
    return sidecar.with_name(sidecar.name + SIDECAR_ATTESTATION_SUFFIX)


def native_manifest_path(sidecar: Path) -> Path:
    sidecar = sidecar.expanduser().resolve()
    return sidecar.with_name(sidecar.name + NATIVE_MANIFEST_SUFFIX)


def hash_native_generated_tree(generated_root: Path) -> dict[str, Any]:
    """Recompute the exact, canonical-LF DolRecomp source-tree identity."""

    raw_root = generated_root.expanduser()
    if raw_root.is_symlink():
        raise OracleError(f"native generated root must not be a symlink: {raw_root}")
    try:
        root = raw_root.resolve(strict=True)
    except OSError as exc:
        raise OracleError(f"native generated root not found: {raw_root}") from exc
    if not root.is_dir():
        raise OracleError(f"native generated root is not a directory: {root}")

    files: list[tuple[str, Path]] = []
    for candidate in root.rglob("*"):
        if candidate.is_symlink():
            raise OracleError(f"symlink in native generated source tree: {candidate}")
        if candidate.is_dir():
            continue
        try:
            mode = candidate.stat().st_mode
        except OSError as exc:
            raise OracleError(f"cannot inspect native generated source: {candidate}") from exc
        if not stat.S_ISREG(mode):
            raise OracleError(f"non-regular native generated source: {candidate}")
        files.append((candidate.relative_to(root).as_posix(), candidate))
    files.sort(key=lambda item: item[0])

    combined = hashlib.sha256()
    file_hashes: dict[str, str] = {}
    for relative_path, path in files:
        payload = read_regular_bytes(
            path,
            label=f"native generated source {relative_path}",
            max_size=MAX_NATIVE_GENERATED_FILE_SIZE,
        )
        if b"\r" in payload:
            raise OracleError(
                f"native generated source is not canonical LF text: {relative_path}"
            )
        digest = hashlib.sha256(payload).hexdigest()
        encoded_path = relative_path.encode("utf-8")
        combined.update(len(encoded_path).to_bytes(8, byteorder="big"))
        combined.update(encoded_path)
        combined.update(bytes.fromhex(digest))
        file_hashes[relative_path] = digest

    try:
        selected_hashes = {
            relative_path: file_hashes[relative_path]
            for relative_path in NATIVE_SELECTED_GENERATED_FILES
        }
    except KeyError as exc:
        raise OracleError(f"required native generated source is missing: {exc.args[0]}") from exc
    return {
        "relative_file_count": len(files),
        "combined_sha256": combined.hexdigest(),
        "selected_file_sha256": selected_hashes,
    }


def verify_native_generation_manifest(
    payload: bytes,
    pin_report: dict[str, Any],
    *,
    generated_root: Path | None = None,
) -> dict[str, Any]:
    try:
        manifest = json.loads(payload.decode("utf-8"))
    except (UnicodeError, json.JSONDecodeError) as exc:
        raise OracleError("invalid native generated manifest") from exc
    expected_commits = {
        component["name"]: component["commit"] for component in pin_report["components"]
    }
    if (
        not isinstance(manifest, dict)
        or manifest.get("schema_version") != SCHEMA_VERSION
        or manifest.get("commits") != expected_commits
    ):
        raise OracleError("native generated manifest has the wrong pins or schema")
    dol = manifest.get("dol")
    if (
        not isinstance(dol, dict)
        or dol.get("game_id") != "GC6E01"
        or dol.get("sha1") != ORIGINAL_DOL_SHA1
        or dol.get("sha256") != ORIGINAL_DOL_SHA256
    ):
        raise OracleError("native generated manifest has the wrong original DOL")
    dolrecomp = manifest.get("dolrecomp")
    if (
        not isinstance(dolrecomp, dict)
        or not re.fullmatch(r"[0-9a-f]{64}", str(dolrecomp.get("sha256") or ""))
    ):
        raise OracleError("native generated manifest has an invalid DolRecomp binary hash")
    generated = manifest.get("generated")
    if not isinstance(generated, dict):
        raise OracleError("native generated manifest omitted generated-source identity")
    selected = generated.get("selected_file_sha256")
    file_count = generated.get("relative_file_count")
    if (
        generated.get("cpu") != "gekko"
        or generated.get("platform") != "gamecube"
        or generated.get("root") != "generated"
        or generated.get("combined_sha256") != NATIVE_GENERATED_TREE_SHA256
        or generated.get("combined_sha256_recipe")
        != NATIVE_GENERATED_SHA256_RECIPE
        or generated.get("text_newlines") != "lf"
        or isinstance(file_count, bool)
        or file_count != NATIVE_GENERATED_FILE_COUNT
        or selected != NATIVE_SELECTED_GENERATED_SHA256
    ):
        raise OracleError("native generated manifest has untrusted generated-source identity")
    if generated_root is not None:
        actual = hash_native_generated_tree(generated_root)
        expected = {
            "relative_file_count": file_count,
            "combined_sha256": generated.get("combined_sha256"),
            "selected_file_sha256": selected,
        }
        if actual != expected:
            raise OracleError(
                "native generated source tree does not match its generation manifest"
            )
    return manifest


def expected_native_binary_identity(pin_report: dict[str, Any]) -> dict[str, Any]:
    return {
        "schema_version": SCHEMA_VERSION,
        "engine": NATIVE_EXPECTED_ENGINE,
        "generated_tree_sha256": NATIVE_GENERATED_TREE_SHA256,
        "provenance": {
            component["name"]: component["commit"]
            for component in pin_report["components"]
        },
    }


def verify_native_binary_identity(
    binary: Path, pin_report: dict[str, Any]
) -> dict[str, Any]:
    result = run_command(
        [str(binary), "--identity"], cwd=REPO, timeout=10, max_output=64 * 1024
    )
    if result.returncode != 0:
        detail = (result.stderr or result.stdout).strip()
        raise OracleError(f"native sidecar identity probe failed: {detail[-800:]}")
    try:
        identity = json.loads(result.stdout)
    except json.JSONDecodeError as exc:
        raise OracleError("native sidecar identity probe returned invalid JSON") from exc
    if identity != expected_native_binary_identity(pin_report):
        raise OracleError("native sidecar embedded identity does not match its build inputs")
    return identity


def verify_build_attestation(
    attestation_path: Path,
    *,
    sidecar_sha256: str,
    pin_report: dict[str, Any],
    expected_kind: str,
    expected_binary_name: str,
    expected_attestation_sha256: str | None = None,
    generated_manifest_sha256: str | None = None,
) -> tuple[dict[str, Any], str]:
    payload = read_regular_bytes(
        attestation_path,
        label="sidecar build attestation",
        max_size=1024 * 1024,
    )
    attestation_sha256 = hashlib.sha256(payload).hexdigest()
    if (
        expected_attestation_sha256 is not None
        and attestation_sha256 != expected_attestation_sha256
    ):
        raise OracleError(
            "sidecar build attestation does not match the expected run-start fingerprint"
        )
    try:
        attestation = json.loads(payload.decode("utf-8"))
    except (UnicodeError, json.JSONDecodeError) as exc:
        raise OracleError("invalid sidecar build attestation") from exc
    if (
        not isinstance(attestation, dict)
        or attestation.get("schema_version") != SCHEMA_VERSION
        or attestation.get("kind") != expected_kind
    ):
        raise OracleError("unsupported sidecar build attestation")
    build_state = attestation.get("build_state")
    current_state = sidecar_build_state(pin_report)
    if build_state != current_state:
        raise OracleError("sidecar build attestation inputs or pins no longer match")
    state_sha256 = canonical_sha256(current_state)
    if (
        attestation.get("pre_state_sha256") != state_sha256
        or attestation.get("post_state_sha256") != state_sha256
    ):
        raise OracleError("sidecar build attestation pre/post state is invalid")
    binary = attestation.get("binary")
    if (
        not isinstance(binary, dict)
        or binary.get("name") != expected_binary_name
        or binary.get("sha256") != sidecar_sha256
    ):
        raise OracleError("sidecar binary does not match its build attestation")
    manifest = attestation.get("generated_manifest")
    binary_identity = attestation.get("binary_identity")
    if generated_manifest_sha256 is None:
        if manifest is not None or binary_identity is not None:
            raise OracleError("unexpected generated manifest in sidecar build attestation")
    elif (
        not isinstance(manifest, dict)
        or manifest.get("sha256") != generated_manifest_sha256
        or binary_identity != expected_native_binary_identity(pin_report)
    ):
        raise OracleError(
            "native generated manifest or binary identity does not match its build attestation"
        )
    return attestation, attestation_sha256


def verify_sidecar_attestation(
    attestation_path: Path,
    *,
    sidecar_sha256: str,
    pin_report: dict[str, Any],
    expected_attestation_sha256: str | None = None,
) -> tuple[dict[str, Any], str]:
    return verify_build_attestation(
        attestation_path,
        sidecar_sha256=sidecar_sha256,
        pin_report=pin_report,
        expected_kind=BUILD_ATTESTATION_KIND,
        expected_binary_name="moderngekko-dolphin-oracle",
        expected_attestation_sha256=expected_attestation_sha256,
    )


def verify_native_attestation(
    attestation_path: Path,
    *,
    sidecar_sha256: str,
    generated_manifest_sha256: str,
    pin_report: dict[str, Any],
    expected_attestation_sha256: str | None = None,
) -> tuple[dict[str, Any], str]:
    return verify_build_attestation(
        attestation_path,
        sidecar_sha256=sidecar_sha256,
        pin_report=pin_report,
        expected_kind=NATIVE_BUILD_ATTESTATION_KIND,
        expected_binary_name="moderngekko-native-oracle",
        expected_attestation_sha256=expected_attestation_sha256,
        generated_manifest_sha256=generated_manifest_sha256,
    )


def command_attest_pre(args: argparse.Namespace) -> int:
    pin_report = verify_checkout_pins(Path(args.checkout), Path(args.pins))
    state = sidecar_build_state(pin_report)
    write_private_json(Path(args.output).expanduser().resolve(), state)
    print(json.dumps(state, indent=2, sort_keys=True))
    return 0


def command_attest_finalize(args: argparse.Namespace) -> int:
    pre_state = read_json(
        Path(args.pre_state), label="sidecar pre-build state", max_size=1024 * 1024
    )
    pin_report = verify_checkout_pins(Path(args.checkout), Path(args.pins))
    post_state = sidecar_build_state(pin_report)
    if pre_state != post_state:
        raise OracleError("sidecar build inputs or pinned checkout changed during the build")
    binary = require_executable(Path(args.binary), label="semantic sidecar")
    output_path = Path(args.output).expanduser().resolve()
    ensure_transient_path(output_path)
    output_path.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
    native = args.kind == "native"
    if (
        native != bool(args.generated_manifest)
        or native != bool(args.generated_root)
        or native != bool(args.dolrecomp)
    ):
        raise OracleError(
            "native attestation requires exactly one generated manifest, source root, "
            "and DolRecomp executable"
        )
    with tempfile.TemporaryDirectory(
        prefix="attest-", dir=output_path.parent
    ) as temporary_value:
        temporary = Path(temporary_value)
        binary_snapshot, binary_sha256 = snapshot_executable(
            binary, temporary / binary.name
        )
        state_sha256 = canonical_sha256(post_state)
        attestation = {
            "schema_version": SCHEMA_VERSION,
            "kind": NATIVE_BUILD_ATTESTATION_KIND if native else BUILD_ATTESTATION_KIND,
            "binary": {
                "name": (
                    "moderngekko-native-oracle"
                    if native
                    else "moderngekko-dolphin-oracle"
                ),
                "sha256": binary_sha256,
            },
            "build_state": post_state,
            "pre_state_sha256": state_sha256,
            "post_state_sha256": state_sha256,
        }
        if native:
            manifest_path = Path(args.generated_manifest).expanduser().resolve()
            manifest_payload = read_regular_bytes(
                manifest_path,
                label="native generated manifest",
                max_size=4 * 1024 * 1024,
            )
            manifest = verify_native_generation_manifest(
                manifest_payload,
                pin_report,
                generated_root=Path(args.generated_root),
            )
            dolrecomp = require_executable(Path(args.dolrecomp), label="DolRecomp")
            _, dolrecomp_sha256 = snapshot_executable(
                dolrecomp, temporary / dolrecomp.name
            )
            if manifest["dolrecomp"]["sha256"] != dolrecomp_sha256:
                raise OracleError(
                    "native generated manifest does not match the DolRecomp executable"
                )
            attestation["binary_identity"] = verify_native_binary_identity(
                binary_snapshot, pin_report
            )
            attestation["generated_manifest"] = {
                "name": manifest_path.name,
                "sha256": hashlib.sha256(manifest_payload).hexdigest(),
            }
    write_private_json(output_path, attestation)
    print(json.dumps(attestation, indent=2, sort_keys=True))
    return 0


def signed16(value: int) -> int:
    value &= 0xFFFF
    return value - 0x10000 if value & 0x8000 else value


EDGE_CASES: tuple[tuple[str, int, int, int], ...] = (
    ("edge-active-one", 1, 7, 0x11),
    ("edge-active-ff", 0xFF, -1, 0xFF),
    ("edge-zero-counter-stream-zero", 0, 0, 0),
    ("edge-zero-counter-stream-one", 0, 0, 1),
    ("edge-zero-counter-stream-ff", 0, 0, 0xFF),
    ("edge-counter-one", 0, 1, 0x41),
    ("edge-counter-two", 0, 2, 0x42),
    ("edge-counter-negative-one", 0, -1, 0x43),
    ("edge-counter-min", 0, -0x8000, 0x44),
    ("edge-counter-max", 0, 0x7FFF, 0x45),
)


def make_fixture(
    *, index: int, identifier: str, active: int, counter: int, stream_first: int, rng: XorShift32
) -> dict[str, Any]:
    object_address = OBJECT_BASE + index * OBJECT_STRIDE
    stream_address = object_address + STREAM_OFFSET
    if stream_address + STREAM_SIZE > 0x81800000:
        raise OracleError("fixture addresses exceed GameCube MEM1")
    object_data = bytearray(rng.bytes(OBJECT_SIZE))
    stream_data = bytearray(rng.bytes(STREAM_SIZE))
    object_data[1] = active & 0xFF
    object_data[0x30:0x34] = stream_address.to_bytes(4, "big")
    object_data[0x48:0x4A] = (counter & 0xFFFF).to_bytes(2, "big")
    stream_data[0] = stream_first & 0xFF
    return {
        "id": identifier,
        "initial": {
            "pc": hex32(ENTRY_PC),
            "gpr": {"1": hex32(STACK_POINTER), "3": hex32(object_address)},
            "lr": hex32(RETURN_PC),
            "memory": [
                {"address": hex32(object_address), "data_hex": object_data.hex()},
                {"address": hex32(stream_address), "data_hex": stream_data.hex()},
            ],
        },
        "observe": {
            "gpr": [3],
            "memory": [
                {"address": hex32(object_address), "size": OBJECT_SIZE},
                {"address": hex32(stream_address), "size": STREAM_SIZE},
            ],
        },
    }


def generate_fixtures(count: int = DEFAULT_FIXTURE_COUNT, seed: int = DEFAULT_SEED) -> list[dict[str, Any]]:
    if not 1 <= count <= MAX_FIXTURES:
        raise OracleError(f"fixture count must be between 1 and {MAX_FIXTURES}")
    rng = XorShift32(seed)
    fixtures: list[dict[str, Any]] = []
    for index in range(count):
        if index < len(EDGE_CASES):
            identifier, active, counter, stream_first = EDGE_CASES[index]
        else:
            identifier = f"random-{index - len(EDGE_CASES):04d}"
            active_roll = rng.next_u32() & 3
            active = 0 if active_roll < 3 else (rng.next_byte() or 1)
            counter = signed16(rng.next_u32())
            stream_first = rng.next_byte()
        fixtures.append(
            make_fixture(
                index=index,
                identifier=identifier,
                active=active,
                counter=counter,
                stream_first=stream_first,
                rng=rng,
            )
        )
    return fixtures


TEXTURE_EDGE_CASES: tuple[tuple[str, int, int], ...] = (
    ("edge-level-zero", 0x00000000, 0x0000),
    ("edge-level-one", 0x00000001, 0x0001),
    ("edge-level-seven", 0x00000007, 0x7FFF),
    ("edge-level-eight", 0x00000008, 0x8000),
    ("edge-level-ff", 0x000000FF, 0xFFFF),
    ("edge-upper-bits-zero", 0xA5A50000, 0xFFFE),
    ("edge-upper-bits-seven", 0x5A5A0007, 0xFFFF),
    ("edge-upper-bits-eight", 0xFFFF0008, 0x1234),
)


def make_texture_fixture(
    *, index: int, identifier: str, raw_level: int, ref_count: int, rng: XorShift32
) -> dict[str, Any]:
    object_data = bytearray(rng.bytes(TEXTURE_OBJECT_SIZE))
    for level in range(8):
        pointer = 0x80500000 + (index * 8 + level) * 0x20
        object_data[0x28 + level * 4 : 0x2C + level * 4] = pointer.to_bytes(4, "big")
    object_data[0x50:0x52] = (ref_count & 0xFFFF).to_bytes(2, "big")
    object_data[0x54:0x56] = ((ref_count ^ 0x5A5A) & 0xFFFF).to_bytes(2, "big")
    return {
        "id": identifier,
        "initial": {
            "pc": hex32(TEXTURE_ENTRY_PC),
            "gpr": {
                "1": hex32(STACK_POINTER),
                "3": hex32(TEXTURE_OBJECT_BASE),
                "4": hex32(raw_level),
            },
            "lr": hex32(RETURN_PC),
            "memory": [
                {"address": hex32(TEXTURE_OBJECT_BASE), "data_hex": object_data.hex()},
            ],
        },
        "observe": {
            "gpr": [3],
            "memory": [
                {"address": hex32(TEXTURE_OBJECT_BASE), "size": TEXTURE_OBJECT_SIZE},
            ],
        },
    }


def generate_texture_fixtures(
    count: int = DEFAULT_FIXTURE_COUNT, seed: int = DEFAULT_SEED
) -> list[dict[str, Any]]:
    if not 1 <= count <= MAX_FIXTURES:
        raise OracleError(f"fixture count must be between 1 and {MAX_FIXTURES}")
    rng = XorShift32(seed)
    fixtures: list[dict[str, Any]] = []
    for index in range(count):
        if index < len(TEXTURE_EDGE_CASES):
            identifier, raw_level, ref_count = TEXTURE_EDGE_CASES[index]
        else:
            identifier = f"random-{index - len(TEXTURE_EDGE_CASES):04d}"
            random_index = index - len(TEXTURE_EDGE_CASES)
            upper_bits = rng.next_u32() & 0xFFFFFF00
            if random_index & 1:
                level = 8 + (rng.next_u32() % (0x100 - 8))
            else:
                # Deliberately balance the valid/invalid branch and cycle all
                # eight valid levels; a uniform u8 would make 31/32 fixtures
                # take the uninteresting NULL path.
                level = (random_index // 2) & 7
            raw_level = upper_bits | level
            ref_count = rng.next_u32() & 0xFFFF
        fixtures.append(
            make_texture_fixture(
                index=index,
                identifier=identifier,
                raw_level=raw_level,
                ref_count=ref_count,
                rng=rng,
            )
        )
    return fixtures


MOBJ_ADD_TOBJ_EDGE_CASES: tuple[tuple[str, bool, bool, int], ...] = (
    ("edge-mobj-null", False, True, OLD_TOBJ_BASE),
    ("edge-tobj-null", True, False, OLD_TOBJ_BASE),
    ("edge-valid-old-head-null", True, True, 0),
    ("edge-valid-old-head-non-null", True, True, OLD_TOBJ_BASE),
)


def make_mobj_add_tobj_fixture(
    *,
    index: int,
    identifier: str,
    mobj_present: bool,
    tobj_present: bool,
    old_head: int,
    rng: XorShift32,
) -> dict[str, Any]:
    mobj_data = bytearray(rng.bytes(MOBJ_SIZE))
    tobj_data = bytearray(rng.bytes(TOBJ_SIZE))
    mobj_data[0x08:0x0C] = old_head.to_bytes(4, "big")
    old_next = OLD_TOBJ_BASE + 0x1000 + ((index & 0xFF) * 0x20)
    tobj_data[0x08:0x0C] = old_next.to_bytes(4, "big")
    return {
        "id": identifier,
        "initial": {
            "pc": hex32(MOBJ_ADD_TOBJ_ENTRY_PC),
            "gpr": {
                "1": hex32(STACK_POINTER),
                "3": hex32(MOBJ_BASE if mobj_present else 0),
                "4": hex32(TOBJ_BASE if tobj_present else 0),
            },
            "lr": hex32(RETURN_PC),
            "memory": [
                {"address": hex32(MOBJ_BASE), "data_hex": mobj_data.hex()},
                {"address": hex32(TOBJ_BASE), "data_hex": tobj_data.hex()},
            ],
        },
        "observe": {
            # This void function's volatile register aftermath is not part of
            # the contract. Both pointer writes are covered by RAM instead.
            "gpr": [],
            "memory": [
                {"address": hex32(MOBJ_BASE), "size": MOBJ_SIZE},
                {"address": hex32(TOBJ_BASE), "size": TOBJ_SIZE},
            ],
        },
    }


def generate_mobj_add_tobj_fixtures(
    count: int = DEFAULT_FIXTURE_COUNT, seed: int = DEFAULT_SEED
) -> list[dict[str, Any]]:
    if not 1 <= count <= MAX_FIXTURES:
        raise OracleError(f"fixture count must be between 1 and {MAX_FIXTURES}")
    rng = XorShift32(seed)
    fixtures: list[dict[str, Any]] = []
    for index in range(count):
        if index < len(MOBJ_ADD_TOBJ_EDGE_CASES):
            identifier, mobj_present, tobj_present, old_head = (
                MOBJ_ADD_TOBJ_EDGE_CASES[index]
            )
        else:
            random_index = index - len(MOBJ_ADD_TOBJ_EDGE_CASES)
            mode = random_index & 3
            identifier = f"random-{random_index:04d}-mode-{mode}"
            mobj_present = mode != 0
            tobj_present = mode != 1
            old_head = 0
            if mode in (0, 1, 3):
                old_head = OLD_TOBJ_BASE + ((rng.next_u32() & 0xFF) * 0x20)
        fixtures.append(
            make_mobj_add_tobj_fixture(
                index=index,
                identifier=identifier,
                mobj_present=mobj_present,
                tobj_present=tobj_present,
                old_head=old_head,
                rng=rng,
            )
        )
    return fixtures


def generate_profile_fixtures(
    profile: OracleProfile, count: int = DEFAULT_FIXTURE_COUNT, seed: int = DEFAULT_SEED
) -> list[dict[str, Any]]:
    if profile.identifier == MSGCTRL_PROFILE.identifier:
        return generate_fixtures(count, seed)
    if profile.identifier == GSTEXTURE_PROFILE.identifier:
        return generate_texture_fixtures(count, seed)
    if profile.identifier == MOBJ_ADD_TOBJ_ORACLE_PROFILE.identifier:
        return generate_mobj_add_tobj_fixtures(count, seed)
    raise OracleError(f"profile has no fixture generator: {profile.identifier}")


def build_request(
    code: bytes,
    fixtures: list[dict[str, Any]],
    profile: OracleProfile = MSGCTRL_PROFILE,
) -> dict[str, Any]:
    if not code or len(code) % 4 or len(code) > MAX_TEXT_SIZE:
        raise OracleError("PowerPC .text must be non-empty, word-aligned, and at most 4096 bytes")
    return {
        "schema_version": SCHEMA_VERSION,
        "function": {
            "name": profile.spec.name,
            "entry_pc": hex32(profile.spec.virtual_address),
            "code_hex": code.hex(),
            "max_instructions": profile.max_instructions,
            "original": {
                "virtual_address": hex32(profile.spec.virtual_address),
                "size": profile.spec.size,
                "dol_sha1": profile.spec.original_dol_sha1,
            },
        },
        "fixtures": fixtures,
    }


def extract_text(elf: Path, *, objcopy: Path, readelf: Path, output: Path) -> bytes:
    elf = elf.expanduser().resolve()
    if not elf.is_file():
        raise OracleError(f"ELF object not found: {elf}")
    objcopy = require_executable(objcopy, label="powerpc-eabi-objcopy")
    readelf = require_executable(readelf, label="powerpc-eabi-readelf")
    relocation_check = run_command([str(readelf), "-rW", str(elf)], cwd=REPO, timeout=30)
    if relocation_check.returncode != 0:
        detail = (relocation_check.stderr or relocation_check.stdout).strip()
        raise OracleError(f"readelf failed for {elf}: {detail[-1000:]}")
    if "there are no relocations in this file" not in relocation_check.stdout.lower():
        raise OracleError(f"ELF contains relocations and is unsafe for direct execution: {elf}")
    output = output.expanduser().resolve()
    ensure_transient_path(output)
    output.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
    output.unlink(missing_ok=True)
    extraction = run_command(
        [str(objcopy), "-O", "binary", "--only-section=.text", str(elf), str(output)],
        cwd=REPO,
        timeout=30,
    )
    if extraction.returncode != 0 or not output.is_file() or output.is_symlink():
        detail = (extraction.stderr or extraction.stdout).strip()
        raise OracleError(f"objcopy failed for {elf}: {detail[-1000:]}")
    output.chmod(0o600)
    code = output.read_bytes()
    if not code or len(code) % 4 or len(code) > MAX_TEXT_SIZE:
        raise OracleError(
            f"invalid extracted .text size for {elf}: {len(code)} bytes (limit {MAX_TEXT_SIZE})"
        )
    return code


def invoke_sidecar(
    executable: Path,
    *,
    request: dict[str, Any],
    request_file: Path,
    result_file: Path,
    timeout: int,
) -> dict[str, Any]:
    executable = require_executable(executable, label="semantic sidecar")
    request_file = request_file.expanduser().resolve()
    result_file = result_file.expanduser().resolve()
    ensure_transient_path(request_file)
    ensure_transient_path(result_file)
    if request_file == result_file:
        raise OracleError("sidecar request and result paths must differ")
    write_private_json(request_file, request)
    result_file.parent.mkdir(mode=0o700, parents=True, exist_ok=True)
    result_file.unlink(missing_ok=True)
    result = run_command(
        [
            str(executable),
            "--request-file",
            str(request_file),
            "--result-file",
            str(result_file),
        ],
        cwd=executable.parent,
        timeout=timeout,
        max_output=8000,
    )
    if result.returncode != 0:
        detail = (result.stderr or result.stdout).strip()
        raise OracleError(f"semantic sidecar failed ({result.returncode}): {detail[-2000:]}")
    if not result_file.is_file() or result_file.is_symlink():
        raise OracleError("semantic sidecar did not create a regular result file")
    result_file.chmod(0o600)
    parsed = read_json(result_file, label="semantic sidecar result")
    if not isinstance(parsed, dict):
        raise OracleError("semantic sidecar result must be a JSON object")
    return parsed


def fixture_index(fixtures: list[dict[str, Any]]) -> dict[str, dict[str, Any]]:
    indexed: dict[str, dict[str, Any]] = {}
    for fixture in fixtures:
        identifier = str(fixture.get("id") or "")
        if not identifier or identifier in indexed:
            raise OracleError(f"duplicate or empty fixture id: {identifier!r}")
        indexed[identifier] = fixture
    return indexed


def result_index(
    result: dict[str, Any],
    fixtures: list[dict[str, Any]],
    *,
    label: str,
    expected_provenance: dict[str, str],
    expected_function: str,
    expected_engine: str = EXPECTED_ENGINE,
) -> dict[str, dict[str, Any]]:
    if result.get("schema_version") != SCHEMA_VERSION:
        raise OracleError(f"{label} result has an unsupported schema")
    if result.get("engine") != expected_engine:
        raise OracleError(f"{label} result has an untrusted engine identity")
    if result.get("code_sandbox_bytes") != MAX_TEXT_SIZE:
        raise OracleError(f"{label} result has the wrong code-sandbox size")
    provenance = result.get("provenance")
    if provenance != expected_provenance:
        raise OracleError(f"{label} result provenance does not match the pin manifest")
    if expected_engine == NATIVE_EXPECTED_ENGINE:
        if result.get("generated_tree_sha256") != NATIVE_GENERATED_TREE_SHA256:
            raise OracleError(
                f"{label} result has the wrong embedded generated-tree identity"
            )
    if result.get("function") != expected_function:
        raise OracleError(f"{label} result names the wrong function")
    rows = result.get("results")
    if not isinstance(rows, list):
        raise OracleError(f"{label} result has no results list")
    expected = fixture_index(fixtures)
    indexed: dict[str, dict[str, Any]] = {}
    for row in rows:
        if not isinstance(row, dict):
            raise OracleError(f"{label} result contains a non-object row")
        identifier = str(row.get("id") or "")
        if not identifier or identifier in indexed:
            raise OracleError(f"{label} result has a duplicate or empty fixture id")
        status = row.get("status")
        if status not in ALLOWED_STATUSES:
            raise OracleError(f"{label} result has invalid status for {identifier}: {status!r}")
        indexed[identifier] = row
    missing = sorted(set(expected) - set(indexed))
    extra = sorted(set(indexed) - set(expected))
    if missing or extra:
        raise OracleError(
            f"{label} result fixture mismatch: missing={missing[:5]} extra={extra[:5]}"
        )
    return indexed


def final_state(row: dict[str, Any], *, label: str, identifier: str) -> dict[str, Any]:
    final = row.get("final")
    if not isinstance(final, dict):
        raise OracleError(f"{label} result has no final state for {identifier}")
    return final


def observed_gpr(final: dict[str, Any], register: int, *, label: str, identifier: str) -> int:
    registers = final.get("gpr")
    if not isinstance(registers, dict):
        raise OracleError(f"{label} result has no GPR map for {identifier}")
    key = str(register)
    if key not in registers:
        raise OracleError(f"{label} result omitted r{register} for {identifier}")
    return u32(registers[key], field=f"{label}.{identifier}.r{register}")


def observed_memory(final: dict[str, Any], *, label: str, identifier: str) -> dict[int, bytes]:
    rows = final.get("memory")
    if not isinstance(rows, list):
        raise OracleError(f"{label} result has no memory list for {identifier}")
    memory: dict[int, bytes] = {}
    for row in rows:
        if not isinstance(row, dict):
            raise OracleError(f"{label} result has malformed memory for {identifier}")
        address = u32(row.get("address"), field=f"{label}.{identifier}.memory.address")
        data_hex = row.get("data_hex")
        if not isinstance(data_hex, str) or len(data_hex) % 2 or not re.fullmatch(r"[0-9a-fA-F]*", data_hex):
            raise OracleError(f"{label} result has malformed memory bytes for {identifier}")
        if address in memory:
            raise OracleError(f"{label} result duplicates memory address {hex32(address)}")
        memory[address] = bytes.fromhex(data_hex)
    return memory


def ram_summary(row: dict[str, Any], *, label: str, identifier: str) -> RamSummary:
    digest = u64_hex(row.get("ram_digest"), field=f"{label}.{identifier}.ram_digest")
    raw_changed_bytes = row.get("ram_changed_bytes")
    if (
        isinstance(raw_changed_bytes, bool)
        or not isinstance(raw_changed_bytes, int)
        or not 0 <= raw_changed_bytes <= 0x01800000
    ):
        raise OracleError(f"{label} result has invalid ram_changed_bytes for {identifier}")
    truncated = row.get("ram_changes_truncated")
    if not isinstance(truncated, bool):
        raise OracleError(f"{label} result has invalid ram_changes_truncated for {identifier}")
    raw_changes = row.get("ram_changes")
    if not isinstance(raw_changes, list) or len(raw_changes) > 32:
        raise OracleError(f"{label} result has invalid ram_changes for {identifier}")
    changes: list[RamChange] = []
    reported_bytes = 0
    previous_end = 0x80000000
    for raw_change in raw_changes:
        if not isinstance(raw_change, dict):
            raise OracleError(f"{label} result has malformed ram_changes for {identifier}")
        address = u32(
            raw_change.get("address"), field=f"{label}.{identifier}.ram_changes.address"
        )
        before_hex = raw_change.get("before_hex")
        after_hex = raw_change.get("after_hex")
        if (
            not isinstance(before_hex, str)
            or not isinstance(after_hex, str)
            or not before_hex
            or len(before_hex) != len(after_hex)
            or len(before_hex) % 2
            or not re.fullmatch(r"[0-9a-fA-F]+", before_hex)
            or not re.fullmatch(r"[0-9a-fA-F]+", after_hex)
        ):
            raise OracleError(f"{label} result has malformed RAM change bytes for {identifier}")
        before = bytes.fromhex(before_hex)
        after = bytes.fromhex(after_hex)
        end = address + len(before)
        if (
            before == after
            or any(left == right for left, right in zip(before, after))
            or address < previous_end
            or not 0x80000000 <= address < end <= 0x81800000
        ):
            raise OracleError(f"{label} result has invalid RAM change span for {identifier}")
        reported_bytes += len(before)
        if reported_bytes > 512:
            raise OracleError(f"{label} result reports too many RAM change bytes for {identifier}")
        changes.append(RamChange(address=address, before=before, after=after))
        previous_end = end
    if reported_bytes > raw_changed_bytes or (not truncated and reported_bytes != raw_changed_bytes):
        raise OracleError(f"{label} result has inconsistent RAM change counts for {identifier}")
    return RamSummary(
        digest=digest,
        changed_bytes=raw_changed_bytes,
        truncated=truncated,
        changes=tuple(changes),
    )


def compact_first_change(summary: RamSummary) -> str:
    if not summary.changes:
        return "none"
    change = summary.changes[0]
    shown = min(len(change.before), 8)
    suffix = ".." if shown < len(change.before) else ""
    return (
        f"{hex32(change.address)}+{len(change.before)}:"
        f"{change.before[:shown].hex()}{suffix}>{change.after[:shown].hex()}{suffix}"
    )


def append_mismatch(lines: list[str], *, limit: int, line: str) -> None:
    if len(lines) < limit:
        lines.append(line)


def compare_results(
    reference: dict[str, Any],
    candidate: dict[str, Any],
    fixtures: list[dict[str, Any]],
    *,
    mismatch_limit: int = 64,
    expected_provenance: dict[str, str] | None = None,
    profile: OracleProfile = MSGCTRL_PROFILE,
    reference_engine: str = EXPECTED_ENGINE,
    candidate_engine: str = EXPECTED_ENGINE,
) -> Comparison:
    if mismatch_limit < 1:
        raise OracleError("mismatch limit must be positive")
    if expected_provenance is None:
        expected_provenance = manifest_provenance(load_pins())
    reference_rows = result_index(
        reference,
        fixtures,
        label="reference",
        expected_provenance=expected_provenance,
        expected_function=profile.spec.name,
        expected_engine=reference_engine,
    )
    candidate_rows = result_index(
        candidate,
        fixtures,
        label="candidate",
        expected_provenance=expected_provenance,
        expected_function=profile.spec.name,
        expected_engine=candidate_engine,
    )
    mismatches: list[str] = []
    mismatch_count = 0
    for fixture in fixtures:
        identifier = str(fixture["id"])
        reference_row = reference_rows[identifier]
        candidate_row = candidate_rows[identifier]
        reference_status = str(reference_row["status"])
        candidate_status = str(candidate_row["status"])
        if reference_status != "returned":
            raise OracleError(
                f"reference oracle did not return for {identifier}: {reference_status}"
            )
        if candidate_status != reference_status:
            mismatch_count += 1
            append_mismatch(
                mismatches,
                limit=mismatch_limit,
                line=f"{identifier} status ref={reference_status} cand={candidate_status}",
            )
            continue
        reference_final = final_state(reference_row, label="reference", identifier=identifier)
        candidate_final = final_state(candidate_row, label="candidate", identifier=identifier)
        reference_pc = u32(reference_final.get("pc"), field=f"reference.{identifier}.pc")
        candidate_pc = u32(candidate_final.get("pc"), field=f"candidate.{identifier}.pc")
        initial = fixture.get("initial")
        if not isinstance(initial, dict):
            raise OracleError(f"fixture has no initial state: {identifier}")
        expected_return_pc = u32(initial.get("lr"), field=f"fixture.{identifier}.lr")
        if reference_pc != expected_return_pc:
            raise OracleError(
                f"reference result returned at the wrong PC for {identifier}: "
                f"expected {hex32(expected_return_pc)}, got {hex32(reference_pc)}"
            )
        if candidate_pc != expected_return_pc:
            raise OracleError(
                f"candidate result returned at the wrong PC for {identifier}: "
                f"expected {hex32(expected_return_pc)}, got {hex32(candidate_pc)}"
            )
        if reference_pc != candidate_pc:
            mismatch_count += 1
            append_mismatch(
                mismatches,
                limit=mismatch_limit,
                line=f"{identifier} pc ref={hex32(reference_pc)} cand={hex32(candidate_pc)}",
            )
        reference_ram = ram_summary(reference_row, label="reference", identifier=identifier)
        candidate_ram = ram_summary(candidate_row, label="candidate", identifier=identifier)
        if (
            reference_ram.digest != candidate_ram.digest
            or reference_ram.changed_bytes != candidate_ram.changed_bytes
        ):
            mismatch_count += 1
            append_mismatch(
                mismatches,
                limit=mismatch_limit,
                line=(
                    f"{identifier} ram_digest ref={hex64(reference_ram.digest)} "
                    f"cand={hex64(candidate_ram.digest)} "
                    f"changed={reference_ram.changed_bytes}/{candidate_ram.changed_bytes} "
                    f"first_ref={compact_first_change(reference_ram)} "
                    f"first_cand={compact_first_change(candidate_ram)}"
                ),
            )
        observe = fixture.get("observe")
        if not isinstance(observe, dict):
            raise OracleError(f"fixture has no observe object: {identifier}")
        registers = observe.get("gpr")
        if not isinstance(registers, list):
            raise OracleError(f"fixture has no observed GPR list: {identifier}")
        for raw_register in registers:
            register = int(raw_register)
            reference_value = observed_gpr(
                reference_final, register, label="reference", identifier=identifier
            )
            candidate_value = observed_gpr(
                candidate_final, register, label="candidate", identifier=identifier
            )
            if reference_value != candidate_value:
                mismatch_count += 1
                append_mismatch(
                    mismatches,
                    limit=mismatch_limit,
                    line=(
                        f"{identifier} r{register} ref={hex32(reference_value)} "
                        f"cand={hex32(candidate_value)}"
                    ),
                )
        reference_memory = observed_memory(
            reference_final, label="reference", identifier=identifier
        )
        candidate_memory = observed_memory(
            candidate_final, label="candidate", identifier=identifier
        )
        watches = observe.get("memory")
        if not isinstance(watches, list):
            raise OracleError(f"fixture has no memory watches: {identifier}")
        for watch in watches:
            if not isinstance(watch, dict):
                raise OracleError(f"fixture has malformed memory watch: {identifier}")
            address = u32(watch.get("address"), field=f"fixture.{identifier}.memory.address")
            size = int(watch.get("size", -1))
            if not 0 <= size <= 65536:
                raise OracleError(f"fixture has invalid memory-watch size: {identifier}")
            if address not in reference_memory or address not in candidate_memory:
                raise OracleError(
                    f"sidecar omitted watched memory {hex32(address)} for {identifier}"
                )
            reference_bytes = reference_memory[address]
            candidate_bytes = candidate_memory[address]
            if len(reference_bytes) != size or len(candidate_bytes) != size:
                raise OracleError(
                    f"sidecar returned the wrong memory size at {hex32(address)} for {identifier}"
                )
            different = [
                offset
                for offset, (left, right) in enumerate(zip(reference_bytes, candidate_bytes))
                if left != right
            ]
            cursor = 0
            while cursor < len(different):
                start = different[cursor]
                end = start + 1
                cursor += 1
                while cursor < len(different) and different[cursor] == end and end - start < 8:
                    end += 1
                    cursor += 1
                mismatch_count += 1
                append_mismatch(
                    mismatches,
                    limit=mismatch_limit,
                    line=(
                        f"{identifier} ram[{hex32(address + start)}+{end - start}] "
                        f"ref={reference_bytes[start:end].hex()} "
                        f"cand={candidate_bytes[start:end].hex()}"
                    ),
                )
    return Comparison(
        equal=mismatch_count == 0,
        mismatch_count=mismatch_count,
        mismatches=mismatches,
        omitted_mismatches=max(0, mismatch_count - len(mismatches)),
    )


def observed_gpr_contract(fixtures: list[dict[str, Any]]) -> list[int]:
    """Return the GPRs compared by at least one fixture in this corpus."""
    registers: set[int] = set()
    for fixture in fixtures:
        observe = fixture.get("observe")
        if not isinstance(observe, dict) or not isinstance(observe.get("gpr"), list):
            identifier = fixture.get("id", "<unknown>")
            raise OracleError(f"fixture has no observed GPR list: {identifier}")
        registers.update(int(register) for register in observe["gpr"])
    return sorted(registers)


def configured_io_paths(args: argparse.Namespace, temporary: Path) -> tuple[Path, Path, Path, Path]:
    option_names = (
        "reference_request_file",
        "reference_result_file",
        "candidate_request_file",
        "candidate_result_file",
    )
    provided = [getattr(args, name) for name in option_names]
    if any(provided) and not all(provided):
        raise OracleError("provide all four request/result file options or none")
    if all(provided):
        paths = tuple(Path(value).expanduser().resolve() for value in provided)
    else:
        paths = (
            temporary / "reference.request.json",
            temporary / "reference.result.json",
            temporary / "candidate.request.json",
            temporary / "candidate.result.json",
        )
    if len(set(paths)) != 4:
        raise OracleError("request/result paths must be distinct")
    for path in paths:
        ensure_transient_path(path)
    reference_request, reference_result, candidate_request, candidate_result = paths
    return reference_request, reference_result, candidate_request, candidate_result


def command_verify_pins(args: argparse.Namespace) -> int:
    report = verify_checkout_pins(Path(args.checkout), Path(args.pins))
    print(json.dumps(report, indent=2, sort_keys=True))
    return 0


def command_fixtures(args: argparse.Namespace) -> int:
    profile = resolve_profile(args.profile)
    fixtures = generate_profile_fixtures(profile, args.count, args.seed)
    payload = {
        "schema_version": SCHEMA_VERSION,
        "profile": profile.identifier,
        "function": profile.spec.name,
        "seed": hex32(args.seed),
        "fixture_corpus_sha256": hashlib.sha256(
            json.dumps(fixtures, separators=(",", ":"), sort_keys=True).encode("utf-8")
        ).hexdigest(),
        "fixtures": fixtures,
    }
    write_private_json(Path(args.output).expanduser().resolve(), payload)
    return 0


def command_run(args: argparse.Namespace) -> int:
    profile = resolve_profile(args.profile)
    pin_report = verify_checkout_pins(Path(args.checkout), Path(args.pins))
    expected_provenance = {
        component["name"]: component["commit"] for component in pin_report["components"]
    }
    sidecar = require_executable(Path(args.sidecar), label="semantic sidecar")
    native_sidecar = require_executable(
        Path(args.native_sidecar), label="native semantic sidecar"
    )
    for label, value in (
        ("sidecar", args.expected_sidecar_sha256),
        ("sidecar attestation", args.expected_attestation_sha256),
        ("native sidecar", args.expected_native_sidecar_sha256),
        ("native sidecar attestation", args.expected_native_attestation_sha256),
        ("native generated manifest", args.expected_native_manifest_sha256),
    ):
        if value is not None and not re.fullmatch(r"[0-9a-f]{64}", value):
            raise OracleError(f"invalid expected {label} SHA-256")
    fixtures = generate_profile_fixtures(profile, args.fixture_count, args.seed)
    TRANSIENT_ROOT.mkdir(mode=0o700, parents=True, exist_ok=True)
    TRANSIENT_ROOT.chmod(0o700)
    with tempfile.TemporaryDirectory(prefix="run-", dir=TRANSIENT_ROOT) as temporary_value:
        temporary = Path(temporary_value)
        sidecar_snapshot, sidecar_sha256 = snapshot_executable(
            sidecar,
            temporary / "moderngekko-dolphin-oracle.snapshot",
            expected_sha256=args.expected_sidecar_sha256,
        )
        attestation_path = (
            Path(args.attestation).expanduser().resolve()
            if args.attestation
            else sidecar_attestation_path(sidecar)
        )
        attestation, attestation_sha256 = verify_sidecar_attestation(
            attestation_path,
            sidecar_sha256=sidecar_sha256,
            pin_report=pin_report,
            expected_attestation_sha256=args.expected_attestation_sha256,
        )
        native_snapshot, native_sidecar_sha256 = snapshot_executable(
            native_sidecar,
            temporary / "moderngekko-native-oracle.snapshot",
            expected_sha256=args.expected_native_sidecar_sha256,
        )
        native_attestation_path = (
            Path(args.native_attestation).expanduser().resolve()
            if args.native_attestation
            else sidecar_attestation_path(native_sidecar)
        )
        native_generated_manifest_path = (
            Path(args.native_manifest).expanduser().resolve()
            if args.native_manifest
            else native_manifest_path(native_sidecar)
        )
        native_manifest_payload = read_regular_bytes(
            native_generated_manifest_path,
            label="native generated manifest",
            max_size=4 * 1024 * 1024,
        )
        verify_native_generation_manifest(native_manifest_payload, pin_report)
        native_manifest_sha256 = hashlib.sha256(native_manifest_payload).hexdigest()
        if (
            args.expected_native_manifest_sha256 is not None
            and native_manifest_sha256 != args.expected_native_manifest_sha256
        ):
            raise OracleError(
                "native generated manifest does not match the expected run-start fingerprint"
            )
        native_attestation, native_attestation_sha256 = verify_native_attestation(
            native_attestation_path,
            sidecar_sha256=native_sidecar_sha256,
            generated_manifest_sha256=native_manifest_sha256,
            pin_report=pin_report,
            expected_attestation_sha256=args.expected_native_attestation_sha256,
        )
        reference_code = extract_text(
            Path(args.reference_elf),
            objcopy=Path(args.objcopy),
            readelf=Path(args.readelf),
            output=temporary / "reference.text.bin",
        )
        reference_authority = verify_reference_authority(
            reference_code, Path(args.dol), profile.spec
        )
        candidate_code = extract_text(
            Path(args.candidate_elf),
            objcopy=Path(args.objcopy),
            readelf=Path(args.readelf),
            output=temporary / "candidate.text.bin",
        )
        reference_request_file, reference_result_file, candidate_request_file, candidate_result_file = (
            configured_io_paths(args, temporary)
        )
        reference_request = build_request(reference_code, fixtures, profile)
        candidate_request = build_request(candidate_code, fixtures, profile)
        reference_result = invoke_sidecar(
            sidecar_snapshot,
            request=reference_request,
            request_file=reference_request_file,
            result_file=reference_result_file,
            timeout=args.timeout,
        )
        if sha256_file(sidecar_snapshot) != sidecar_sha256:
            raise OracleError("semantic sidecar snapshot changed after the reference run")
        native_result = invoke_sidecar(
            native_snapshot,
            request=reference_request,
            request_file=temporary / "native-reference.request.json",
            result_file=temporary / "native-reference.result.json",
            timeout=args.timeout,
        )
        if sha256_file(native_snapshot) != native_sidecar_sha256:
            raise OracleError("native semantic sidecar snapshot changed after execution")
        native_qualification = compare_results(
            reference_result,
            native_result,
            fixtures,
            mismatch_limit=args.mismatch_limit,
            expected_provenance=expected_provenance,
            profile=profile,
            reference_engine=EXPECTED_ENGINE,
            candidate_engine=NATIVE_EXPECTED_ENGINE,
        )
        if not native_qualification.equal:
            details = "; ".join(native_qualification.mismatches[:3]) or "unknown mismatch"
            raise OracleError(
                "native ModernGekko qualification disagreed with Dolphin; "
                f"semantic feedback refused ({details})"
            )
        candidate_result = invoke_sidecar(
            sidecar_snapshot,
            request=candidate_request,
            request_file=candidate_request_file,
            result_file=candidate_result_file,
            timeout=args.timeout,
        )
        if sha256_file(sidecar_snapshot) != sidecar_sha256:
            raise OracleError("semantic sidecar snapshot changed after the candidate run")
        comparison = compare_results(
            reference_result,
            candidate_result,
            fixtures,
            mismatch_limit=args.mismatch_limit,
            expected_provenance=expected_provenance,
            profile=profile,
        )
        report = {
            "schema_version": SCHEMA_VERSION,
            "profile": profile.identifier,
            "function": profile.spec.name,
            "driver_sha256": sha256_file(Path(__file__).resolve()),
            "engine": EXPECTED_ENGINE,
            "provenance": expected_provenance,
            "entry_pc": hex32(profile.spec.virtual_address),
            "fixture_count": len(fixtures),
            "fixture_seed": hex32(args.seed),
            "fixture_corpus_sha256": hashlib.sha256(
                json.dumps(fixtures, separators=(",", ":"), sort_keys=True).encode("utf-8")
            ).hexdigest(),
            "observable_contract": {
                "gpr": observed_gpr_contract(fixtures),
                "memory": list(profile.memory_contract),
                "whole_ram": "FNV-1a-64 excluding the validated 4096-byte code sandbox",
                "control": ["status", "return_pc"],
            },
            "reference_authority": reference_authority,
            "pins": pin_report,
            "sidecar_sha256": sidecar_sha256,
            "build_attestation_sha256": attestation_sha256,
            "build_attestation_state_sha256": attestation["post_state_sha256"],
            "native_qualification": {
                "engine": NATIVE_EXPECTED_ENGINE,
                "equal": native_qualification.equal,
                "mismatch_count": native_qualification.mismatch_count,
                "mismatches": native_qualification.mismatches,
                "omitted_mismatches": native_qualification.omitted_mismatches,
                "sidecar_sha256": native_sidecar_sha256,
                "build_attestation_sha256": native_attestation_sha256,
                "build_attestation_state_sha256": native_attestation[
                    "post_state_sha256"
                ],
                "generated_manifest_sha256": native_manifest_sha256,
            },
            "reference": {
                "elf": display_path(Path(args.reference_elf)),
                "text_size": len(reference_code),
                "text_sha256": hashlib.sha256(reference_code).hexdigest(),
            },
            "candidate": {
                "elf": display_path(Path(args.candidate_elf)),
                "text_size": len(candidate_code),
                "text_sha256": hashlib.sha256(candidate_code).hexdigest(),
            },
            "equal": comparison.equal,
            "mismatch_count": comparison.mismatch_count,
            "mismatches": comparison.mismatches,
            "omitted_mismatches": comparison.omitted_mismatches,
        }
        if args.report_file:
            write_private_json(Path(args.report_file).expanduser().resolve(), report)
        print(json.dumps(report, indent=2, sort_keys=True))
        return 0 if comparison.equal else 1


def integer(value: str) -> int:
    try:
        return int(value, 0)
    except ValueError as exc:
        raise argparse.ArgumentTypeError(f"invalid integer: {value}") from exc


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    verify = subparsers.add_parser("verify-pins", help="verify an external ModernGekko checkout")
    verify.add_argument("--checkout", required=True)
    verify.add_argument("--pins", default=str(DEFAULT_PINS))
    verify.set_defaults(handler=command_verify_pins)

    attest_pre = subparsers.add_parser(
        "attest-build-pre", help="record clean pinned sidecar inputs before a build"
    )
    attest_pre.add_argument("--checkout", required=True)
    attest_pre.add_argument("--pins", default=str(DEFAULT_PINS))
    attest_pre.add_argument("--output", required=True)
    attest_pre.set_defaults(handler=command_attest_pre)

    attest_finalize = subparsers.add_parser(
        "attest-build-finalize", help="verify post-build inputs and attest the sidecar"
    )
    attest_finalize.add_argument("--checkout", required=True)
    attest_finalize.add_argument("--pins", default=str(DEFAULT_PINS))
    attest_finalize.add_argument("--pre-state", required=True)
    attest_finalize.add_argument("--binary", required=True)
    attest_finalize.add_argument("--kind", choices=("dolphin", "native"), default="dolphin")
    attest_finalize.add_argument("--generated-manifest")
    attest_finalize.add_argument("--generated-root")
    attest_finalize.add_argument("--dolrecomp")
    attest_finalize.add_argument("--output", required=True)
    attest_finalize.set_defaults(handler=command_attest_finalize)

    fixtures = subparsers.add_parser("fixtures", help="write deterministic profile fixtures")
    fixtures.add_argument("--profile", choices=sorted(PROFILES), default=DEFAULT_PROFILE)
    fixtures.add_argument("--output", required=True)
    fixtures.add_argument("--count", type=int, default=DEFAULT_FIXTURE_COUNT)
    fixtures.add_argument("--seed", type=integer, default=DEFAULT_SEED)
    fixtures.set_defaults(handler=command_fixtures)

    run = subparsers.add_parser("run", help="compare reference and candidate PPC objects")
    run.add_argument("--profile", choices=sorted(PROFILES), default=DEFAULT_PROFILE)
    run.add_argument("--checkout", required=True, help="external pinned ModernGekko checkout")
    run.add_argument("--pins", default=str(DEFAULT_PINS))
    run.add_argument("--sidecar", required=True)
    run.add_argument("--attestation")
    run.add_argument("--expected-sidecar-sha256")
    run.add_argument("--expected-attestation-sha256")
    run.add_argument("--native-sidecar", required=True)
    run.add_argument("--native-attestation")
    run.add_argument("--native-manifest")
    run.add_argument("--expected-native-sidecar-sha256")
    run.add_argument("--expected-native-attestation-sha256")
    run.add_argument("--expected-native-manifest-sha256")
    run.add_argument("--reference-elf", required=True)
    run.add_argument("--candidate-elf", required=True)
    run.add_argument("--dol", default=str(DEFAULT_DOL), help="original GC6E01 main.dol")
    run.add_argument("--objcopy", default=str(DEFAULT_OBJCOPY))
    run.add_argument("--readelf", default=str(DEFAULT_READELF))
    run.add_argument("--fixture-count", type=int, default=DEFAULT_FIXTURE_COUNT)
    run.add_argument("--seed", type=integer, default=DEFAULT_SEED)
    run.add_argument("--timeout", type=int, default=120)
    run.add_argument("--mismatch-limit", type=int, default=64)
    run.add_argument("--reference-request-file")
    run.add_argument("--reference-result-file")
    run.add_argument("--candidate-request-file")
    run.add_argument("--candidate-result-file")
    run.add_argument("--report-file")
    run.set_defaults(handler=command_run)
    return parser


def main(argv: Iterable[str] | None = None) -> int:
    args = build_parser().parse_args(list(argv) if argv is not None else None)
    try:
        if getattr(args, "timeout", 1) < 1:
            raise OracleError("timeout must be at least 1 second")
        if getattr(args, "mismatch_limit", 1) < 1:
            raise OracleError("mismatch limit must be at least 1")
        return int(args.handler(args))
    except OracleError as exc:
        print(f"semantic-oracle error: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
