#!/usr/bin/env python3
from __future__ import annotations

import importlib.util
import os
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


MODULE_PATH = Path(__file__).with_name("owner_extract.py")
SPEC = importlib.util.spec_from_file_location("owner_extract", MODULE_PATH)
assert SPEC and SPEC.loader
owner_extract = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = owner_extract
SPEC.loader.exec_module(owner_extract)


def tool(name: str) -> Path | None:
    configured = os.environ.get(f"PPC_{name.upper()}")
    if configured:
        path = Path(configured).expanduser()
        return path if path.is_file() else None
    path = (
        Path.home()
        / ".cache"
        / "pkmn-permuter-tools"
        / "ppc-binutils"
        / f"powerpc-eabi-{name}"
    )
    if path.is_file():
        return path
    discovered = shutil.which(f"powerpc-eabi-{name}")
    return Path(discovered) if discovered else None


class OwnerExtractTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.assembler = tool("as")
        cls.objcopy = tool("objcopy")
        cls.readelf = tool("readelf")
        if not all((cls.assembler, cls.objcopy, cls.readelf)):
            raise unittest.SkipTest("PowerPC binutils are unavailable")

    def assemble(self, root: Path, name: str, source: str) -> Path:
        source_path = root / f"{name}.s"
        object_path = root / f"{name}.o"
        source_path.write_text(source, encoding="utf-8")
        result = subprocess.run(
            [
                str(self.assembler),
                "-mgekko",
                "-mregnames",
                "-be",
                "-o",
                str(object_path),
                str(source_path),
            ],
            capture_output=True,
            text=True,
        )
        self.assertEqual(result.returncode, 0, result.stderr)
        return object_path

    @staticmethod
    def owner_source(
        *,
        target_nops: int = 0,
        before_value: int = 1,
        after_call: str = "ext_after",
        compiler_local: str = "@12",
        data_value: int = 0,
    ) -> str:
        nops = "\n".join("  nop" for _ in range(target_nops))
        return f"""
.text
.globl before
.type before,@function
before:
  li 3,{before_value}
  blr
.size before,.-before

.globl target
.type target,@function
target:
  bl ext_target
{nops}
  blr
.size target,.-target

.globl after
.type after,@function
after:
  bl {after_call}
  blr
.size after,.-after

.data
.local "{compiler_local}"
.type "{compiler_local}",@object
"{compiler_local}":
  .long {data_value}
.size "{compiler_local}",4

.globl owner_data
.type owner_data,@object
owner_data:
  .long target
  .long "{compiler_local}"
.size owner_data,8
"""

    def test_extracts_one_function_and_preserves_target_relocation(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            baseline = self.assemble(root, "baseline", self.owner_source())
            candidate = self.assemble(
                root, "candidate", self.owner_source(target_nops=1)
            )
            output = root / "target.o"
            report = owner_extract.extract(
                baseline_path=baseline,
                candidate_path=candidate,
                function="target",
                output_path=output,
                objcopy=self.objcopy,
                readelf=self.readelf,
                expected_baseline_fingerprint=owner_extract.target_record(
                    owner_extract.read_elf(baseline),
                    owner_extract.find_target(owner_extract.read_elf(baseline), "target"),
                )["fingerprint_sha256"],
            )
            emitted = owner_extract.read_elf(output)
            target = owner_extract.find_target(emitted, "target")
            functions = [s.name for s in emitted.symbols if s.type == owner_extract.STT_FUNC]
            self.assertEqual(functions, ["target"])
            self.assertEqual(target.symbol.size, 12)
            relocations = owner_extract.target_relocations(emitted, target)
            self.assertEqual(len(relocations), 1)
            self.assertEqual(emitted.symbols[relocations[0].symbol_index].name, "ext_target")
            self.assertEqual(report["sibling_audit"]["post_target_shift"], 4)
            self.assertEqual(
                report["candidate"]["fingerprint_sha256"],
                report["output"]["fingerprint_sha256"],
            )

    def test_rejects_sibling_text_drift(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            baseline = self.assemble(root, "baseline", self.owner_source())
            candidate = self.assemble(
                root, "candidate", self.owner_source(before_value=2)
            )
            with self.assertRaisesRegex(owner_extract.ExtractError, "text before"):
                owner_extract.extract(
                    baseline_path=baseline,
                    candidate_path=candidate,
                    function="target",
                    output_path=root / "target.o",
                    objcopy=self.objcopy,
                    readelf=self.readelf,
                )

    def test_rejects_sibling_relocation_drift(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            baseline = self.assemble(root, "baseline", self.owner_source())
            candidate = self.assemble(
                root, "candidate", self.owner_source(after_call="other_after")
            )
            with self.assertRaisesRegex(owner_extract.ExtractError, "sibling relocation"):
                owner_extract.extract(
                    baseline_path=baseline,
                    candidate_path=candidate,
                    function="target",
                    output_path=root / "target.o",
                    objcopy=self.objcopy,
                    readelf=self.readelf,
                )

    def test_compiler_local_renumber_uses_stable_identity(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            baseline = self.assemble(
                root, "baseline", self.owner_source(compiler_local="@12")
            )
            candidate = self.assemble(
                root, "candidate", self.owner_source(compiler_local="@34")
            )
            report = owner_extract.extract(
                baseline_path=baseline,
                candidate_path=candidate,
                function="target",
                output_path=root / "target.o",
                objcopy=self.objcopy,
                readelf=self.readelf,
            )
            self.assertEqual(report["sibling_audit"]["state"], "passed")

    def test_non_ascii_digits_are_named_symbols_not_compiler_locals(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            baseline = self.assemble(
                root, "baseline", self.owner_source(compiler_local="@١")
            )
            candidate = self.assemble(
                root, "candidate", self.owner_source(compiler_local="@２")
            )
            with self.assertRaises(owner_extract.ExtractError):
                owner_extract.extract(
                    baseline_path=baseline,
                    candidate_path=candidate,
                    function="target",
                    output_path=root / "target.o",
                    objcopy=self.objcopy,
                    readelf=self.readelf,
                )

    def test_rejects_allocatable_data_state_drift(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            baseline = self.assemble(root, "baseline", self.owner_source(data_value=0))
            candidate = self.assemble(root, "candidate", self.owner_source(data_value=1))
            with self.assertRaisesRegex(owner_extract.ExtractError, "allocatable section"):
                owner_extract.extract(
                    baseline_path=baseline,
                    candidate_path=candidate,
                    function="target",
                    output_path=root / "target.o",
                    objcopy=self.objcopy,
                    readelf=self.readelf,
                )

    def test_rejects_target_relocation_to_sibling_definition(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            source = self.owner_source().replace("bl ext_target", ".long after")
            owner = self.assemble(root, "owner", source)
            with self.assertRaisesRegex(
                owner_extract.ExtractError, "definition outside the target"
            ):
                owner_extract.extract(
                    baseline_path=owner,
                    candidate_path=owner,
                    function="target",
                    output_path=root / "target.o",
                    objcopy=self.objcopy,
                    readelf=self.readelf,
                )

    def test_expected_baseline_fingerprint_fails_closed(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            owner = self.assemble(root, "owner", self.owner_source())
            with self.assertRaisesRegex(owner_extract.ExtractError, "fingerprint drifted"):
                owner_extract.extract(
                    baseline_path=owner,
                    candidate_path=owner,
                    function="target",
                    output_path=root / "target.o",
                    objcopy=self.objcopy,
                    readelf=self.readelf,
                    expected_baseline_fingerprint="0" * 64,
                )


if __name__ == "__main__":
    unittest.main()
