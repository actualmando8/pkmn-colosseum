#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Generate and attest the small DolRecomp input set used by the native oracle."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import secrets
import shutil
import stat
import subprocess
import sys
import tempfile
from typing import Iterable


EXPECTED_DOL_SHA1 = "870e8b9693ca780782d80f22a6a4572d8ba9458f"
EXPECTED_DOL_SHA256 = (
    "7e6c00a3bd632126d5466cbab856c63ac3c52a5da20b60dba7e756538152c9f4"
)

PINNED_COMMITS = {
    "ModernGekko": "11237c119a5d8e907a20e9cae1c357df149aaa47",
    "RecompCore": "1873066167f3d03b39771b547f280d2b970427b6",
    "DolRecomp": "a2b02e5a515fc8971cc551ad51c9e26a9815daad",
}

REPOSITORY_PATHS = {
    "ModernGekko": Path("."),
    "RecompCore": Path("vendor/dolphin"),
    "DolRecomp": Path("vendor/dolphin/DolRecomp"),
}

SELECTED_GENERATED_FILES = (
    "generated.h",
    "chunks/chunk_0059_text1_800ED5E0.c",
    "chunks/chunk_0076_text1_801315E0.c",
    "chunks/chunk_0105_text1_801A55E0.c",
)

SELECTED_CHUNK_MARKERS = {
    "chunks/chunk_0059_text1_800ED5E0.c": (
        "void func_800ED5E0(CPUState* ctx)",
        "case 0x800EF548u: goto label_800EF548;",
        "label_800EF548:",
    ),
    "chunks/chunk_0076_text1_801315E0.c": (
        "void func_801315E0(CPUState* ctx)",
        "case 0x80132454u: goto label_80132454;",
        "label_80132454:",
    ),
    "chunks/chunk_0105_text1_801A55E0.c": (
        "void func_801A55E0(CPUState* ctx)",
        "case 0x801A6DA0u: goto label_801A6DA0;",
        "label_801A6DA0:",
    ),
}

GENERATED_HEADER_MARKERS = (
    "// cpu: gekko",
    "#define DOLRECOMP_CPU_GEKKO 1",
    '#define DOLRECOMP_CPU_NAME "gekko"',
    '#include "cpu/cpu.h"',
)

COMBINED_SHA256_RECIPE = (
    "sha256(concat(u64be(relative_path_utf8_length),relative_path_utf8,"
    "sha256(file_bytes))) for files sorted by relative POSIX path"
)


class GenerationError(RuntimeError):
    """An input, generation, or attestation invariant was not satisfied."""


def _positive_jobs(value: str) -> int:
    try:
        jobs = int(value, 10)
    except ValueError as exc:
        raise argparse.ArgumentTypeError("jobs must be an integer") from exc
    if not 1 <= jobs <= 256:
        raise argparse.ArgumentTypeError("jobs must be between 1 and 256")
    return jobs


def _resolve_regular_file(raw_path: str, label: str, *, executable: bool) -> Path:
    path = Path(raw_path).expanduser().resolve(strict=True)
    mode = path.stat().st_mode
    if not stat.S_ISREG(mode):
        raise GenerationError(f"{label} is not a regular file: {path}")
    if executable and not os.access(path, os.X_OK):
        raise GenerationError(f"{label} is not executable: {path}")
    return path


def _resolve_checkout(raw_path: str) -> Path:
    checkout = Path(raw_path).expanduser().resolve(strict=True)
    if not checkout.is_dir():
        raise GenerationError(f"ModernGekko checkout is not a directory: {checkout}")
    return checkout


def _run_git(repository: Path, *arguments: str) -> str:
    try:
        completed = subprocess.run(
            ["git", "-C", str(repository), *arguments],
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
    except (OSError, subprocess.CalledProcessError) as exc:
        detail = ""
        if isinstance(exc, subprocess.CalledProcessError) and exc.stderr:
            detail = f": {exc.stderr.strip()}"
        raise GenerationError(f"cannot inspect git repository {repository}{detail}") from exc
    return completed.stdout.strip()


def _verify_pinned_heads(checkout: Path) -> dict[str, str]:
    heads: dict[str, str] = {}
    for name, relative_path in REPOSITORY_PATHS.items():
        repository = (checkout / relative_path).resolve(strict=True)
        try:
            repository.relative_to(checkout)
        except ValueError as exc:
            raise GenerationError(f"{name} repository escapes the checkout: {repository}") from exc
        if not repository.is_dir():
            raise GenerationError(f"{name} repository is not a directory: {repository}")

        top_level = Path(_run_git(repository, "rev-parse", "--show-toplevel")).resolve(
            strict=True
        )
        if top_level != repository:
            raise GenerationError(
                f"{name} path is not its git worktree root: {repository} (root {top_level})"
            )

        head = _run_git(repository, "rev-parse", "--verify", "HEAD^{commit}")
        expected = PINNED_COMMITS[name]
        if head != expected:
            raise GenerationError(
                f"{name} pin mismatch: expected {expected}, got {head or '<empty>'}"
            )
        heads[name] = head
    return heads


def _hash_file(path: Path, algorithms: Iterable[str]) -> dict[str, str]:
    hashers = {name: hashlib.new(name) for name in algorithms}
    before = path.stat()
    with path.open("rb") as source:
        while chunk := source.read(1024 * 1024):
            for hasher in hashers.values():
                hasher.update(chunk)
    after = path.stat()
    identity_before = (before.st_dev, before.st_ino, before.st_size, before.st_mtime_ns)
    identity_after = (after.st_dev, after.st_ino, after.st_size, after.st_mtime_ns)
    if identity_before != identity_after:
        raise GenerationError(f"input changed while it was being hashed: {path}")
    return {name: hasher.hexdigest() for name, hasher in hashers.items()}


def _is_relative_to(path: Path, parent: Path) -> bool:
    try:
        path.relative_to(parent)
    except ValueError:
        return False
    return True


def _prepare_output_root(raw_path: str, checkout: Path) -> Path:
    output_root = Path(raw_path).expanduser().resolve(strict=False)
    if len(output_root.parts) < 3:
        raise GenerationError(f"refusing unsafe, shallow output root: {output_root}")

    home = Path.home().resolve(strict=True)
    if output_root == Path(output_root.anchor) or output_root == home:
        raise GenerationError(f"refusing unsafe output root: {output_root}")
    if _is_relative_to(output_root, checkout) or _is_relative_to(checkout, output_root):
        raise GenerationError(
            f"output root must be separate from the pinned checkout: {output_root}"
        )

    output_root.mkdir(parents=True, exist_ok=True)
    output_root = output_root.resolve(strict=True)
    if not output_root.is_dir():
        raise GenerationError(f"output root is not a directory: {output_root}")
    return output_root


def _assert_safe_generated_tree(path: Path, output_root: Path) -> None:
    if path.parent != output_root:
        raise GenerationError(f"generated tree is outside its output root: {path}")
    if path.is_symlink():
        raise GenerationError(f"refusing symlinked generated tree: {path}")
    if not path.is_dir():
        raise GenerationError(f"generated path is not a directory: {path}")
    if os.path.ismount(path):
        raise GenerationError(f"refusing mounted generated tree: {path}")

    for directory, directory_names, file_names in os.walk(path, followlinks=False):
        directory_path = Path(directory)
        if os.path.ismount(directory_path) and directory_path != path:
            raise GenerationError(f"refusing nested mount in generated tree: {directory_path}")
        for name in directory_names:
            child = directory_path / name
            if child.is_symlink():
                raise GenerationError(f"refusing symlink in generated tree: {child}")
        for name in file_names:
            child = directory_path / name
            if child.is_symlink() or not stat.S_ISREG(child.stat().st_mode):
                raise GenerationError(f"refusing non-regular generated entry: {child}")


def _remove_generated_tree(path: Path, output_root: Path) -> None:
    if not path.exists() and not path.is_symlink():
        return
    _assert_safe_generated_tree(path, output_root)
    shutil.rmtree(path)


def _validate_generated_output(generated: Path, output_root: Path) -> None:
    _assert_safe_generated_tree(generated, output_root)
    for relative_path in SELECTED_GENERATED_FILES:
        selected = generated / relative_path
        if selected.is_symlink() or not selected.is_file():
            raise GenerationError(f"required generated file is missing: {relative_path}")
        if selected.stat().st_size == 0:
            raise GenerationError(f"required generated file is empty: {relative_path}")

    header = (generated / "generated.h").read_text(encoding="utf-8")
    missing = [marker for marker in GENERATED_HEADER_MARKERS if marker not in header]
    if missing:
        raise GenerationError(
            "generated header does not identify the Gekko/current runtime include; "
            f"missing {missing!r}"
        )

    for relative_path, markers in SELECTED_CHUNK_MARKERS.items():
        chunk = (generated / relative_path).read_text(encoding="utf-8")
        missing = [marker for marker in markers if marker not in chunk]
        if missing:
            raise GenerationError(
                f"selected chunk {relative_path} lacks its expected entry; "
                f"missing {missing!r}"
            )


def _normalize_generated_text(generated: Path, output_root: Path) -> None:
    """Canonicalize DolRecomp's text-mode output to LF on every host."""

    _assert_safe_generated_tree(generated, output_root)
    for candidate in sorted(generated.rglob("*")):
        if candidate.is_dir():
            continue
        if candidate.is_symlink() or not stat.S_ISREG(candidate.stat().st_mode):
            raise GenerationError(f"refusing non-regular generated entry: {candidate}")
        payload = candidate.read_bytes()
        if b"\0" in payload:
            raise GenerationError(f"generated output is not text: {candidate}")
        try:
            payload.decode("utf-8")
        except UnicodeDecodeError as exc:
            raise GenerationError(f"generated output is not UTF-8 text: {candidate}") from exc
        normalized = payload.replace(b"\r\n", b"\n")
        if b"\r" in normalized:
            raise GenerationError(f"generated output contains a bare CR: {candidate}")
        if normalized == payload:
            continue

        mode = stat.S_IMODE(candidate.stat().st_mode)
        descriptor, temporary_name = tempfile.mkstemp(
            dir=candidate.parent, prefix=f".{candidate.name}.", suffix=".tmp"
        )
        temporary = Path(temporary_name)
        try:
            with os.fdopen(descriptor, "wb") as output:
                output.write(normalized)
                output.flush()
                os.fsync(output.fileno())
            os.chmod(temporary, mode)
            os.replace(temporary, candidate)
        finally:
            if temporary.exists():
                temporary.unlink()


def _hash_generated_tree(generated: Path) -> tuple[int, str, dict[str, str]]:
    files: list[tuple[str, Path]] = []
    for candidate in generated.rglob("*"):
        if candidate.is_symlink():
            raise GenerationError(f"refusing symlink in generated output: {candidate}")
        if candidate.is_dir():
            continue
        if not stat.S_ISREG(candidate.stat().st_mode):
            raise GenerationError(f"refusing non-regular generated output: {candidate}")
        files.append((candidate.relative_to(generated).as_posix(), candidate))
    files.sort(key=lambda item: item[0])
    if not files:
        raise GenerationError("DolRecomp produced no generated files")

    combined = hashlib.sha256()
    file_hashes: dict[str, str] = {}
    for relative_path, path in files:
        digest = _hash_file(path, ("sha256",))["sha256"]
        encoded_path = relative_path.encode("utf-8")
        combined.update(len(encoded_path).to_bytes(8, byteorder="big"))
        combined.update(encoded_path)
        combined.update(bytes.fromhex(digest))
        file_hashes[relative_path] = digest

    selected_hashes = {
        relative_path: file_hashes[relative_path]
        for relative_path in SELECTED_GENERATED_FILES
    }
    return len(files), combined.hexdigest(), selected_hashes


def _atomic_write_json(path: Path, document: dict[str, object]) -> None:
    path = path.expanduser().resolve(strict=False)
    if path.exists() and path.is_dir():
        raise GenerationError(f"manifest path is a directory: {path}")
    parent = path.parent
    parent.mkdir(parents=True, exist_ok=True)
    parent = parent.resolve(strict=True)

    descriptor, temporary_name = tempfile.mkstemp(
        dir=parent, prefix=f".{path.name}.", suffix=".tmp"
    )
    temporary = Path(temporary_name)
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", newline="\n") as output:
            json.dump(document, output, indent=2, sort_keys=True)
            output.write("\n")
            output.flush()
            os.fsync(output.fileno())
        os.replace(temporary, path)
    finally:
        if temporary.exists():
            temporary.unlink()


def _build_manifest(
    *,
    heads: dict[str, str],
    dol_hashes: dict[str, str],
    dolrecomp_sha256: str,
    file_count: int,
    combined_sha256: str,
    selected_hashes: dict[str, str],
) -> dict[str, object]:
    return {
        "schema_version": 1,
        "commits": heads,
        "dol": {
            "game_id": "GC6E01",
            "sha1": dol_hashes["sha1"],
            "sha256": dol_hashes["sha256"],
        },
        "dolrecomp": {
            "sha256": dolrecomp_sha256,
        },
        "generated": {
            "combined_sha256": combined_sha256,
            "combined_sha256_recipe": COMBINED_SHA256_RECIPE,
            "cpu": "gekko",
            "platform": "gamecube",
            "relative_file_count": file_count,
            "root": "generated",
            "selected_file_sha256": selected_hashes,
            "text_newlines": "lf",
        },
    }


def generate(arguments: argparse.Namespace) -> dict[str, object]:
    checkout = _resolve_checkout(arguments.checkout)
    dolrecomp = _resolve_regular_file(
        arguments.dolrecomp, "DolRecomp executable", executable=True
    )
    dol = _resolve_regular_file(arguments.dol, "GC6E01 DOL", executable=False)
    output_root = _prepare_output_root(arguments.output_root, checkout)
    generated = output_root / "generated"

    manifest = Path(arguments.manifest).expanduser().resolve(strict=False)
    if _is_relative_to(manifest, generated):
        raise GenerationError("manifest must not be written inside generated output")

    heads_before = _verify_pinned_heads(checkout)
    dol_hashes_before = _hash_file(dol, ("sha1", "sha256"))
    if dol_hashes_before["sha1"] != EXPECTED_DOL_SHA1:
        raise GenerationError(
            "GC6E01 DOL SHA-1 mismatch: "
            f"expected {EXPECTED_DOL_SHA1}, got {dol_hashes_before['sha1']}"
        )
    if dol_hashes_before["sha256"] != EXPECTED_DOL_SHA256:
        raise GenerationError(
            "GC6E01 DOL SHA-256 mismatch: "
            f"expected {EXPECTED_DOL_SHA256}, got {dol_hashes_before['sha256']}"
        )
    dolrecomp_hash_before = _hash_file(dolrecomp, ("sha256",))["sha256"]

    backup: Path | None = None
    if generated.exists() or generated.is_symlink():
        _assert_safe_generated_tree(generated, output_root)
        backup = output_root / (
            f".generated.backup-{os.getpid()}-{secrets.token_hex(8)}"
        )
        generated.rename(backup)

    try:
        command = [
            str(dolrecomp),
            f"-j{arguments.jobs}",
            "--cpu",
            "gekko",
            "--gamecube",
            str(dol),
            str(output_root),
        ]
        try:
            subprocess.run(command, check=True)
        except (OSError, subprocess.CalledProcessError) as exc:
            raise GenerationError(f"DolRecomp generation failed: {exc}") from exc

        _normalize_generated_text(generated, output_root)
        _validate_generated_output(generated, output_root)
        file_count, combined_sha256, selected_hashes = _hash_generated_tree(generated)

        heads_after = _verify_pinned_heads(checkout)
        if heads_after != heads_before:
            raise GenerationError("pinned git HEADs changed during generation")
        dol_hashes_after = _hash_file(dol, ("sha1", "sha256"))
        if dol_hashes_after != dol_hashes_before:
            raise GenerationError("GC6E01 DOL changed during generation")
        dolrecomp_hash_after = _hash_file(dolrecomp, ("sha256",))["sha256"]
        if dolrecomp_hash_after != dolrecomp_hash_before:
            raise GenerationError("DolRecomp executable changed during generation")

        document = _build_manifest(
            heads=heads_after,
            dol_hashes=dol_hashes_after,
            dolrecomp_sha256=dolrecomp_hash_after,
            file_count=file_count,
            combined_sha256=combined_sha256,
            selected_hashes=selected_hashes,
        )
        _atomic_write_json(manifest, document)
    except Exception as exc:
        try:
            _remove_generated_tree(generated, output_root)
            if backup is not None and backup.exists():
                backup.rename(generated)
        except Exception as rollback_error:
            raise GenerationError(
                f"{exc}; additionally failed to restore prior generated output: "
                f"{rollback_error}"
            ) from exc
        raise

    if backup is not None:
        _remove_generated_tree(backup, output_root)
    return document


def _parse_arguments(argv: list[str] | None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--checkout", required=True, help="pinned ModernGekko root")
    parser.add_argument("--dolrecomp", required=True, help="DolRecomp executable")
    parser.add_argument("--dol", required=True, help="exact GC6E01 main.dol")
    parser.add_argument("--output-root", required=True, help="DolRecomp output directory")
    parser.add_argument("--manifest", required=True, help="output JSON manifest")
    parser.add_argument(
        "--jobs",
        type=_positive_jobs,
        default=max(1, min(os.cpu_count() or 1, 64)),
        help="DolRecomp worker count (default: host CPUs, capped at 64)",
    )
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    try:
        document = generate(_parse_arguments(argv))
    except (GenerationError, OSError, UnicodeError, ValueError) as exc:
        print(f"generate_native.py: error: {exc}", file=sys.stderr)
        return 1
    generated = document["generated"]
    assert isinstance(generated, dict)
    print(
        "generated and attested "
        f"{generated['relative_file_count']} files "
        f"({generated['combined_sha256']})"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
