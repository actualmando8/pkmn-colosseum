#!/usr/bin/env python3
"""Create or repair a worker worktree with local original-game-file access."""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_ORIG_SYS = ROOT / "orig" / "GC6E01" / "sys"


def run(cmd: list[str], cwd: Path = ROOT) -> None:
    print("+", " ".join(cmd), flush=True)
    subprocess.run(cmd, cwd=cwd, check=True)


def git(args: list[str], cwd: Path = ROOT) -> None:
    run(["git", *args], cwd=cwd)


def is_git_worktree(path: Path) -> bool:
    if not path.exists():
        return False
    result = subprocess.run(
        ["git", "-C", str(path), "rev-parse", "--is-inside-work-tree"],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    return result.returncode == 0


def require_orig_sys(orig_sys: Path) -> Path:
    try:
        resolved = orig_sys.resolve(strict=True)
    except FileNotFoundError:
        sys.exit(f"missing original game sys directory: {orig_sys}")

    main_dol = resolved / "main.dol"
    if not main_dol.is_file():
        sys.exit(f"missing original DOL: {main_dol}")
    return resolved


def ensure_orig_link(worktree: Path, orig_sys: Path, replace: bool) -> None:
    target_dir = worktree / "orig" / "GC6E01"
    link = target_dir / "sys"
    target_dir.mkdir(parents=True, exist_ok=True)

    if link.exists() or link.is_symlink():
        if link.is_symlink():
            current = link.resolve(strict=False)
            if current == orig_sys:
                print(f"orig link already ok: {link} -> {orig_sys}")
                return
            if not replace:
                sys.exit(
                    f"orig link points to {current}; rerun with --replace-orig-sys"
                )
            link.unlink()
        elif (link / "main.dol").is_file():
            print(f"orig directory already has main.dol: {link}")
            return
        else:
            sys.exit(f"refusing to replace non-symlink path: {link}")

    try:
        link.symlink_to(orig_sys, target_is_directory=True)
        print(f"created orig link: {link} -> {orig_sys}")
    except OSError as exc:
        # Windows refuses symlinks without Administrator or Developer Mode
        # (WinError 1314). The tree is ~4 MB across 6 files, so copying is a
        # perfectly good fallback and needs no elevated privileges.
        if getattr(exc, "winerror", None) != 1314:
            raise
        import shutil

        print(
            f"symlink not permitted ({exc.strerror}); copying instead. "
            "Enable Developer Mode to get symlinks."
        )
        link.parent.mkdir(parents=True, exist_ok=True)
        shutil.copytree(orig_sys, link)
        print(f"copied orig files: {orig_sys} -> {link}")


def git_ref_exists(ref: str) -> bool:
    result = subprocess.run(
        ["git", "show-ref", "--verify", "--quiet", f"refs/heads/{ref}"],
        cwd=ROOT,
    )
    return result.returncode == 0


def add_worktree(path: Path, branch: str | None, base: str, detach: bool) -> None:
    if is_git_worktree(path):
        print(f"existing git worktree: {path}")
        return
    if path.exists() and any(path.iterdir()):
        sys.exit(f"path exists and is not an empty git worktree: {path}")

    if branch and detach:
        sys.exit("--branch and --detach are mutually exclusive")
    if branch:
        if git_ref_exists(branch):
            git(["worktree", "add", str(path), branch])
        else:
            git(["worktree", "add", "-b", branch, str(path), base])
    elif detach:
        git(["worktree", "add", "--detach", str(path), base])
    else:
        git(["worktree", "add", str(path), base])


def validate(worktree: Path) -> None:
    run(["python3", "configure.py", "--no-progress"], cwd=worktree)
    run(["ninja", "all_source", "build/GC6E01/report.json"], cwd=worktree)
    run(["python3", "tools/update_readme_progress.py", "--check"], cwd=worktree)
    run(["git", "diff", "--check"], cwd=worktree)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Create/repair a worker worktree and link orig/GC6E01/sys."
    )
    parser.add_argument("path", type=Path, help="worker worktree path")
    parser.add_argument(
        "--branch",
        help="create this worker branch from --base, or use it if it exists",
    )
    parser.add_argument(
        "--base",
        default="origin/master",
        help="base revision for new worktrees (default: origin/master)",
    )
    parser.add_argument(
        "--detach",
        action="store_true",
        help="create a detached worktree instead of a branch",
    )
    parser.add_argument(
        "--orig-sys",
        type=Path,
        default=DEFAULT_ORIG_SYS,
        help="path to local orig/GC6E01/sys directory",
    )
    parser.add_argument(
        "--replace-orig-sys",
        action="store_true",
        help="replace an existing incorrect orig/GC6E01/sys symlink",
    )
    parser.add_argument(
        "--validate",
        action="store_true",
        help="run the standard worker validation sequence after setup",
    )
    args = parser.parse_args()

    orig_sys = require_orig_sys(args.orig_sys)
    worktree = args.path.resolve()

    add_worktree(worktree, args.branch, args.base, args.detach)
    ensure_orig_link(worktree, orig_sys, args.replace_orig_sys)

    if args.validate:
        validate(worktree)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
