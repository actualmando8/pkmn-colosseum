#!/usr/bin/env python3
"""Unit tests for the dependency-free semantic-oracle Python driver."""

from __future__ import annotations

import copy
import hashlib
import json
import stat
import struct
import subprocess
import sys
import tempfile
import textwrap
import unittest
from pathlib import Path
from unittest import mock


sys.path.insert(0, str(Path(__file__).resolve().parent))
import driver  # noqa: E402


def write_executable(path: Path, source: str) -> Path:
    path.write_text(textwrap.dedent(source).lstrip(), encoding="utf-8")
    path.chmod(path.stat().st_mode | stat.S_IXUSR)
    return path


def make_fake_sidecar(path: Path) -> Path:
    return write_executable(
        path,
        r"""
        #!/usr/bin/env python3
        import argparse
        import hashlib
        import json
        from pathlib import Path

        def number(value):
            return int(value, 0) if isinstance(value, str) else int(value)

        def observed_bytes(fixture, address, size):
            result = bytearray(size)
            for patch in fixture["initial"].get("memory", []):
                patch_address = number(patch["address"])
                patch_bytes = bytes.fromhex(patch["data_hex"])
                begin = max(address, patch_address)
                end = min(address + size, patch_address + len(patch_bytes))
                if begin < end:
                    result[begin - address:end - address] = patch_bytes[
                        begin - patch_address:end - patch_address
                    ]
            return bytes(result)

        parser = argparse.ArgumentParser()
        parser.add_argument("--request-file", required=True)
        parser.add_argument("--result-file", required=True)
        args = parser.parse_args()
        request = json.loads(Path(args.request_file).read_text(encoding="utf-8"))
        code_tag = int(request["function"]["code_hex"][:2], 16)
        rows = []
        for fixture in request["fixtures"]:
            initial = fixture["initial"]
            registers = {}
            for register in fixture["observe"]["gpr"]:
                key = str(register)
                value = number(initial.get("gpr", {}).get(key, 0))
                if register == 3:
                    value = (value + code_tag) & 0xffffffff
                registers[key] = f"0x{value:08x}"
            memory = []
            for watch in fixture["observe"]["memory"]:
                address = number(watch["address"])
                size = number(watch["size"])
                memory.append(
                    {
                        "address": f"0x{address:08x}",
                        "data_hex": observed_bytes(fixture, address, size).hex(),
                    }
                )
            return_pc = number(initial["lr"])
            digest_input = b"".join(
                bytes.fromhex(patch["data_hex"])
                for patch in sorted(initial.get("memory", []), key=lambda row: number(row["address"]))
            )
            ram_digest = int.from_bytes(hashlib.sha256(digest_input).digest()[:8], "big")
            rows.append(
                {
                    "id": fixture["id"],
                    "status": "returned",
                    "instructions": 1,
                    "ram_digest": f"0x{ram_digest:016x}",
                    "ram_changed_bytes": 0,
                    "ram_changes_truncated": False,
                    "ram_changes": [],
                    "final": {
                        "gpr": registers,
                        "pc": f"0x{return_pc:08x}",
                        "lr": f"0x{return_pc:08x}",
                        "ctr": "0x00000000",
                        "cr": "0x00000000",
                        "xer": "0x00000000",
                        "memory": memory,
                    },
                }
            )
        result = {
            "schema_version": request["schema_version"],
            "engine": "dolphin-interpreter-from-moderngekko-tree",
            "code_sandbox_bytes": 4096,
            "provenance": {
                "ModernGekko": "11237c119a5d8e907a20e9cae1c357df149aaa47",
                "RecompCore": "1873066167f3d03b39771b547f280d2b970427b6",
                "DolRecomp": "a2b02e5a515fc8971cc551ad51c9e26a9815daad",
            },
            "function": request["function"]["name"],
            "results": rows,
        }
        Path(args.result_file).write_text(json.dumps(result), encoding="utf-8")
        """,
    )


class FixtureTests(unittest.TestCase):
    def test_generation_is_deterministic_and_has_expected_layout(self) -> None:
        fixtures = driver.generate_fixtures(12, 0x12345678)
        self.assertEqual(fixtures, driver.generate_fixtures(12, 0x12345678))
        self.assertNotEqual(fixtures, driver.generate_fixtures(12, 0x12345679))
        self.assertEqual(len({row["id"] for row in fixtures}), len(fixtures))

        first = fixtures[0]
        object_patch, stream_patch = first["initial"]["memory"]
        object_bytes = bytes.fromhex(object_patch["data_hex"])
        stream_bytes = bytes.fromhex(stream_patch["data_hex"])
        self.assertEqual(first["id"], "edge-active-one")
        self.assertEqual(object_bytes[1], 1)
        self.assertEqual(
            int.from_bytes(object_bytes[0x30:0x34], "big"),
            driver.OBJECT_BASE + driver.STREAM_OFFSET,
        )
        self.assertEqual(int.from_bytes(object_bytes[0x48:0x4A], "big"), 7)
        self.assertEqual(stream_bytes[0], 0x11)
        self.assertEqual(first["observe"]["gpr"], [3])
        self.assertEqual(first["initial"]["lr"], driver.hex32(driver.RETURN_PC))

    def test_request_rejects_unaligned_text(self) -> None:
        with self.assertRaisesRegex(driver.OracleError, "word-aligned"):
            driver.build_request(b"abc", driver.generate_fixtures(1))

    def test_request_carries_pinned_original_address_and_size(self) -> None:
        request = driver.build_request(b"\x00\x00\x00\x00", driver.generate_fixtures(1))
        self.assertEqual(
            request["function"]["original"],
            {
                "virtual_address": "0x80132454",
                "size": 0x78,
                "dol_sha1": "870e8b9693ca780782d80f22a6a4572d8ba9458f",
            },
        )

    def test_texture_profile_is_deterministic_and_supports_different_text_lengths(self) -> None:
        profile = driver.resolve_profile("GStextureLockImage-v2")
        fixtures = driver.generate_profile_fixtures(profile, 12, 0x12345678)
        self.assertEqual(
            fixtures,
            driver.generate_profile_fixtures(profile, 12, 0x12345678),
        )
        first = fixtures[0]
        object_data = bytes.fromhex(first["initial"]["memory"][0]["data_hex"])
        self.assertEqual(first["id"], "edge-level-zero")
        self.assertEqual(first["initial"]["gpr"]["4"], "0x00000000")
        self.assertEqual(int.from_bytes(object_data[0x28:0x2C], "big"), 0x80500000)
        self.assertEqual(int.from_bytes(object_data[0x50:0x52], "big"), 0)
        self.assertEqual(first["observe"]["gpr"], [3])

        reference = driver.build_request(b"\x00" * 48, fixtures, profile)
        candidate = driver.build_request(b"\x01" * 52, fixtures, profile)
        self.assertEqual(reference["function"]["name"], "GStextureLockImage")
        self.assertEqual(reference["function"]["entry_pc"], "0x800ef548")
        self.assertEqual(reference["function"]["original"]["size"], 0x30)
        self.assertEqual(len(candidate["function"]["code_hex"]), 104)

        thousand = driver.generate_texture_fixtures(1000, driver.DEFAULT_SEED)
        levels = [int(row["initial"]["gpr"]["4"], 0) & 0xFF for row in thousand]
        self.assertGreaterEqual(sum(level < 8 for level in levels), 500)
        self.assertTrue(all(level in levels for level in range(8)))


class PinTests(unittest.TestCase):
    def test_repository_manifest_contains_exact_reviewed_pins(self) -> None:
        manifest = driver.load_pins()
        commits = {row["name"]: row["commit"] for row in manifest["components"]}
        self.assertEqual(
            commits,
            {
                "ModernGekko": "11237c119a5d8e907a20e9cae1c357df149aaa47",
                "RecompCore": "1873066167f3d03b39771b547f280d2b970427b6",
                "DolRecomp": "a2b02e5a515fc8971cc551ad51c9e26a9815daad",
            },
        )

    def test_checkout_rejects_incomplete_or_duplicate_manifest_before_git(self) -> None:
        base = {
            "name": "ModernGekko",
            "path": ".",
            "repository": "https://github.com/example/ModernGekko.git",
            "commit": "a" * 40,
        }
        with tempfile.TemporaryDirectory() as temporary_value:
            temporary = Path(temporary_value)
            checkout = temporary / "checkout"
            checkout.mkdir()
            manifest_path = temporary / "pins.json"
            for components in ([base], [base, dict(base)]):
                manifest_path.write_text(
                    json.dumps({"schema_version": 1, "components": components}),
                    encoding="utf-8",
                )
                with self.subTest(count=len(components)), mock.patch.object(
                    driver, "git_stdout"
                ) as git:
                    with self.assertRaises(driver.OracleError):
                        driver.verify_checkout_pins(checkout, manifest_path)
                    git.assert_not_called()

    def test_checkout_verifier_checks_heads_origins_gitlinks_and_cleanliness(self) -> None:
        commits = {"ModernGekko": "a" * 40, "RecompCore": "b" * 40, "DolRecomp": "c" * 40}
        repositories = {
            "ModernGekko": "https://github.com/example/ModernGekko.git",
            "RecompCore": "https://github.com/example/RecompCore.git",
            "DolRecomp": "https://github.com/example/DolRecomp.git",
        }
        with tempfile.TemporaryDirectory() as temporary_value:
            temporary = Path(temporary_value)
            checkout = temporary / "checkout"
            recomp = checkout / "vendor" / "dolphin"
            dolrecomp = recomp / "DolRecomp"
            dolrecomp.mkdir(parents=True)
            manifest_path = temporary / "pins.json"
            manifest_path.write_text(
                json.dumps(
                    {
                        "schema_version": 1,
                        "components": [
                            {
                                "name": "ModernGekko",
                                "path": ".",
                                "repository": repositories["ModernGekko"],
                                "commit": commits["ModernGekko"],
                            },
                            {
                                "name": "RecompCore",
                                "path": "vendor/dolphin",
                                "repository": repositories["RecompCore"],
                                "commit": commits["RecompCore"],
                                "gitlink_parent": ".",
                                "gitlink_path": "vendor/dolphin",
                            },
                            {
                                "name": "DolRecomp",
                                "path": "vendor/dolphin/DolRecomp",
                                "repository": repositories["DolRecomp"],
                                "commit": commits["DolRecomp"],
                                "gitlink_parent": "vendor/dolphin",
                                "gitlink_path": "DolRecomp",
                            },
                        ],
                    }
                ),
                encoding="utf-8",
            )
            component_data = {
                checkout.resolve(): (commits["ModernGekko"], repositories["ModernGekko"]),
                recomp.resolve(): (commits["RecompCore"], repositories["RecompCore"]),
                dolrecomp.resolve(): (commits["DolRecomp"], repositories["DolRecomp"]),
            }

            def fake_git(repository: Path, *arguments: str) -> str:
                repository = repository.resolve()
                if arguments == ("rev-parse", "HEAD"):
                    return component_data[repository][0]
                if arguments == ("remote", "get-url", "origin"):
                    return component_data[repository][1]
                if repository == checkout.resolve() and arguments == (
                    "rev-parse",
                    "HEAD:vendor/dolphin",
                ):
                    return commits["RecompCore"]
                if repository == recomp.resolve() and arguments == (
                    "rev-parse",
                    "HEAD:DolRecomp",
                ):
                    return commits["DolRecomp"]
                self.fail(f"unexpected git invocation: {repository} {arguments}")

            with mock.patch.object(driver, "git_stdout", side_effect=fake_git), mock.patch.object(
                driver, "git_component_clean", return_value=True
            ) as clean, mock.patch.object(driver, "verify_recursive_submodules", return_value=42):
                report = driver.verify_checkout_pins(checkout, manifest_path)
            self.assertEqual([row["name"] for row in report["components"]], list(commits))
            self.assertEqual(report["recursive_submodules"], 42)
            self.assertEqual(clean.call_count, 3)

            with mock.patch.object(driver, "git_stdout", side_effect=fake_git), mock.patch.object(
                driver, "git_component_clean", return_value=False
            ), mock.patch.object(driver, "verify_recursive_submodules", return_value=42):
                with self.assertRaisesRegex(driver.OracleError, "checkout is not clean"):
                    driver.verify_checkout_pins(checkout, manifest_path)

    def test_cleanliness_check_includes_untracked_and_nested_submodule_state(self) -> None:
        completed = subprocess.CompletedProcess(["git"], 0, "", "")
        with mock.patch.object(driver, "run_command", return_value=completed) as run:
            self.assertTrue(driver.git_component_clean(Path("checkout")))
        arguments = run.call_args.args[0]
        self.assertIn("--porcelain=v1", arguments)
        self.assertIn("--untracked-files=all", arguments)
        self.assertIn("--ignore-submodules=none", arguments)

        for status in ("?? scratch.txt\n", " M vendor/dolphin\n", "A  staged.txt\n"):
            dirty = subprocess.CompletedProcess(["git"], 0, status, "")
            with self.subTest(status=status), mock.patch.object(
                driver, "run_command", return_value=dirty
            ):
                self.assertFalse(driver.git_component_clean(Path("checkout")))

    def test_recursive_submodule_verifier_rejects_uninitialized_or_drifted_heads(self) -> None:
        good = subprocess.CompletedProcess(
            ["git"],
            0,
            " 1111111111111111111111111111111111111111 vendor/dolphin\n",
            "",
        )
        with mock.patch.object(driver, "run_command", return_value=good):
            self.assertEqual(driver.verify_recursive_submodules(Path("checkout")), 1)

        bad = subprocess.CompletedProcess(
            ["git"],
            0,
            "-2222222222222222222222222222222222222222 vendor/missing\n"
            "+3333333333333333333333333333333333333333 vendor/drifted\n",
            "",
        )
        with mock.patch.object(driver, "run_command", return_value=bad):
            with self.assertRaisesRegex(driver.OracleError, "incomplete or drifted"):
                driver.verify_recursive_submodules(Path("checkout"))


class ExtractionTests(unittest.TestCase):
    def test_reference_must_equal_pinned_original_dol_slice(self) -> None:
        code = bytes.fromhex("7c0802a64e800020")
        blob = bytearray(0x100 + len(code))
        struct.pack_into(">I", blob, 0x00, 0x100)
        struct.pack_into(">I", blob, 0x48, driver.ENTRY_PC)
        struct.pack_into(">I", blob, 0x90, len(code))
        blob[0x100:] = code
        dol_sha1 = hashlib.sha1(blob).hexdigest()
        spec = driver.FunctionSpec("fixture", driver.ENTRY_PC, len(code), dol_sha1)
        with tempfile.TemporaryDirectory() as temporary_value:
            dol = Path(temporary_value) / "main.dol"
            dol.write_bytes(blob)
            report = driver.verify_reference_authority(code, dol, spec)
            self.assertEqual(report["virtual_address"], driver.hex32(driver.ENTRY_PC))
            self.assertEqual(report["size"], len(code))
            self.assertEqual(report["dol_sha1"], dol_sha1)

            with self.assertRaisesRegex(driver.OracleError, "not authoritative"):
                driver.verify_reference_authority(b"\x00" + code[1:], dol, spec)
            wrong_pin = driver.FunctionSpec("fixture", driver.ENTRY_PC, len(code), "0" * 40)
            with self.assertRaisesRegex(driver.OracleError, "SHA-1 mismatch"):
                driver.verify_reference_authority(code, dol, wrong_pin)

    def test_extracts_text_with_configured_tools(self) -> None:
        with tempfile.TemporaryDirectory() as temporary_value:
            temporary = Path(temporary_value)
            elf = temporary / "candidate.o"
            elf.write_bytes(b"fake ELF")
            readelf = write_executable(
                temporary / "powerpc-eabi-readelf",
                """
                #!/usr/bin/env python3
                print("There are no relocations in this file.")
                """,
            )
            objcopy = write_executable(
                temporary / "powerpc-eabi-objcopy",
                """
                #!/usr/bin/env python3
                import sys
                from pathlib import Path
                Path(sys.argv[-1]).write_bytes(bytes.fromhex("7c0802a64e800020"))
                """,
            )
            output = temporary / "nested" / "candidate.text.bin"
            code = driver.extract_text(elf, objcopy=objcopy, readelf=readelf, output=output)
            self.assertEqual(code, bytes.fromhex("7c0802a64e800020"))
            self.assertEqual(stat.S_IMODE(output.stat().st_mode), 0o600)

    def test_rejects_any_relocation_before_objcopy(self) -> None:
        with tempfile.TemporaryDirectory() as temporary_value:
            temporary = Path(temporary_value)
            elf = temporary / "candidate.o"
            elf.write_bytes(b"fake ELF")
            readelf = write_executable(
                temporary / "powerpc-eabi-readelf",
                """
                #!/usr/bin/env python3
                print("Relocation section '.rela.text' contains 1 entry")
                """,
            )
            marker = temporary / "objcopy-ran"
            objcopy = write_executable(
                temporary / "powerpc-eabi-objcopy",
                f"""
                #!/usr/bin/env python3
                from pathlib import Path
                Path({str(marker)!r}).touch()
                """,
            )
            with self.assertRaisesRegex(driver.OracleError, "contains relocations"):
                driver.extract_text(
                    elf,
                    objcopy=objcopy,
                    readelf=readelf,
                    output=temporary / "candidate.text.bin",
                )
            self.assertFalse(marker.exists())


class SidecarTests(unittest.TestCase):
    def test_one_executable_snapshot_survives_source_replacement(self) -> None:
        fixtures = driver.generate_fixtures(1, 0x1234)
        with tempfile.TemporaryDirectory() as temporary_value:
            temporary = Path(temporary_value)
            original = make_fake_sidecar(temporary / "fake-sidecar")
            original_sha256 = driver.sha256_file(original)
            snapshot, snapshot_sha256 = driver.snapshot_executable(
                original,
                temporary / "private" / "sidecar.snapshot",
                expected_sha256=original_sha256,
            )
            request = driver.build_request(b"\x00\x00\x00\x00", fixtures)
            reference = driver.invoke_sidecar(
                snapshot,
                request=request,
                request_file=temporary / "reference.request.json",
                result_file=temporary / "reference.result.json",
                timeout=10,
            )
            write_executable(original, "#!/bin/sh\nexit 99\n")
            candidate = driver.invoke_sidecar(
                snapshot,
                request=request,
                request_file=temporary / "candidate.request.json",
                result_file=temporary / "candidate.result.json",
                timeout=10,
            )
            self.assertEqual(reference, candidate)
            self.assertEqual(snapshot_sha256, original_sha256)
            self.assertEqual(stat.S_IMODE(snapshot.stat().st_mode), 0o500)
            with self.assertRaisesRegex(driver.OracleError, "run-start fingerprint"):
                driver.snapshot_executable(
                    original,
                    temporary / "wrong.snapshot",
                    expected_sha256=original_sha256,
                )

    def test_build_attestation_binds_binary_inputs_and_clean_pins(self) -> None:
        pin_report = {
            "manifest_sha256": "a" * 64,
            "checkout": "/external/ModernGekko",
            "recursive_submodules": 39,
            "components": [
                {"name": "ModernGekko", "commit": "1" * 40},
                {"name": "RecompCore", "commit": "2" * 40},
                {"name": "DolRecomp", "commit": "3" * 40},
            ],
        }
        with tempfile.TemporaryDirectory() as temporary_value:
            temporary = Path(temporary_value)
            binary = write_executable(temporary / "moderngekko-dolphin-oracle", "#!/bin/sh\n")
            pre_state = temporary / "pre.json"
            pre_state.write_text(
                json.dumps(driver.sidecar_build_state(pin_report)), encoding="utf-8"
            )
            output = binary.with_name(binary.name + driver.SIDECAR_ATTESTATION_SUFFIX)
            args = mock.Mock(
                checkout=str(temporary / "checkout"),
                pins=str(temporary / "pins.json"),
                pre_state=str(pre_state),
                binary=str(binary),
                output=str(output),
            )
            with (
                mock.patch.object(driver, "verify_checkout_pins", return_value=pin_report),
                mock.patch("builtins.print"),
            ):
                self.assertEqual(driver.command_attest_finalize(args), 0)
            attestation_sha256 = driver.sha256_file(output)
            attestation, verified_sha256 = driver.verify_sidecar_attestation(
                output,
                sidecar_sha256=driver.sha256_file(binary),
                pin_report=pin_report,
                expected_attestation_sha256=attestation_sha256,
            )
            self.assertEqual(verified_sha256, attestation_sha256)
            self.assertEqual(
                attestation["binary"]["sha256"], driver.sha256_file(binary)
            )
            self.assertEqual(
                {row["path"] for row in attestation["build_state"]["sidecar_inputs"]},
                set(driver.SIDECAR_BUILD_INPUTS),
            )

            drifted = copy.deepcopy(pin_report)
            drifted["components"][0]["commit"] = "0" * 40
            with self.assertRaisesRegex(driver.OracleError, "inputs or pins"):
                driver.verify_sidecar_attestation(
                    output,
                    sidecar_sha256=driver.sha256_file(binary),
                    pin_report=drifted,
                )

    def test_fake_sidecar_round_trip_and_register_mismatch(self) -> None:
        fixtures = driver.generate_fixtures(2, 0x1234)
        with tempfile.TemporaryDirectory() as temporary_value:
            temporary = Path(temporary_value)
            sidecar = make_fake_sidecar(temporary / "fake-sidecar")
            reference_request = driver.build_request(b"\x00\x00\x00\x00", fixtures)
            reference = driver.invoke_sidecar(
                sidecar,
                request=reference_request,
                request_file=temporary / "reference" / "request.json",
                result_file=temporary / "reference" / "result.json",
                timeout=10,
            )
            second_reference = driver.invoke_sidecar(
                sidecar,
                request=reference_request,
                request_file=temporary / "second" / "request.json",
                result_file=temporary / "second" / "result.json",
                timeout=10,
            )
            self.assertEqual(reference, second_reference)
            self.assertTrue(driver.compare_results(reference, second_reference, fixtures).equal)
            self.assertEqual(
                stat.S_IMODE((temporary / "reference" / "request.json").stat().st_mode), 0o600
            )
            self.assertEqual(
                stat.S_IMODE((temporary / "reference" / "result.json").stat().st_mode), 0o600
            )

            candidate = driver.invoke_sidecar(
                sidecar,
                request=driver.build_request(b"\x01\x00\x00\x00", fixtures),
                request_file=temporary / "candidate" / "request.json",
                result_file=temporary / "candidate" / "result.json",
                timeout=10,
            )
            comparison = driver.compare_results(reference, candidate, fixtures)
            self.assertFalse(comparison.equal)
            self.assertEqual(comparison.mismatch_count, 2)
            self.assertRegex(
                comparison.mismatches[0],
                r"edge-active-one r3 ref=0x[0-9a-f]{8} cand=0x[0-9a-f]{8}",
            )

    def test_ram_digest_mismatch_includes_first_change(self) -> None:
        fixtures = driver.generate_fixtures(1, 0x1234)
        with tempfile.TemporaryDirectory() as temporary_value:
            temporary = Path(temporary_value)
            sidecar = make_fake_sidecar(temporary / "fake-sidecar")
            request = driver.build_request(b"\x00\x00\x00\x00", fixtures)
            reference = driver.invoke_sidecar(
                sidecar,
                request=request,
                request_file=temporary / "request.json",
                result_file=temporary / "result.json",
                timeout=10,
            )
        candidate = copy.deepcopy(reference)
        candidate_row = candidate["results"][0]
        candidate_row.update(
            {
                "ram_digest": "0x0123456789abcdef",
                "ram_changed_bytes": 3,
                "ram_changes_truncated": False,
                "ram_changes": [
                    {
                        "address": "0x80010120",
                        "before_hex": "001122",
                        "after_hex": "ffeedd",
                    }
                ],
            }
        )
        comparison = driver.compare_results(reference, candidate, fixtures)
        self.assertFalse(comparison.equal)
        self.assertEqual(comparison.mismatch_count, 1)
        self.assertRegex(
            comparison.mismatches[0],
            r"ram_digest ref=0x[0-9a-f]{16} cand=0x0123456789abcdef .*"
            r"first_ref=none first_cand=0x80010120\+3:001122>ffeedd",
        )

    def test_rejects_wrong_engine_or_embedded_provenance(self) -> None:
        fixtures = driver.generate_fixtures(1, 0x1234)
        with tempfile.TemporaryDirectory() as temporary_value:
            temporary = Path(temporary_value)
            sidecar = make_fake_sidecar(temporary / "fake-sidecar")
            request = driver.build_request(b"\x00\x00\x00\x00", fixtures)
            trusted = driver.invoke_sidecar(
                sidecar,
                request=request,
                request_file=temporary / "request.json",
                result_file=temporary / "result.json",
                timeout=10,
            )

        wrong_engine = copy.deepcopy(trusted)
        wrong_engine["engine"] = "dolphin-interpreter"
        with self.assertRaisesRegex(driver.OracleError, "untrusted engine identity"):
            driver.compare_results(trusted, wrong_engine, fixtures)

        wrong_provenance = copy.deepcopy(trusted)
        wrong_provenance["provenance"]["RecompCore"] = "0" * 40
        with self.assertRaisesRegex(driver.OracleError, "provenance does not match"):
            driver.compare_results(trusted, wrong_provenance, fixtures)

        wrong_sandbox = copy.deepcopy(trusted)
        wrong_sandbox["code_sandbox_bytes"] = 8192
        with self.assertRaisesRegex(driver.OracleError, "wrong code-sandbox size"):
            driver.compare_results(trusted, wrong_sandbox, fixtures)

    def test_returned_status_must_end_at_fixture_lr(self) -> None:
        fixtures = driver.generate_fixtures(1, 0x1234)
        with tempfile.TemporaryDirectory() as temporary_value:
            temporary = Path(temporary_value)
            sidecar = make_fake_sidecar(temporary / "fake-sidecar")
            request = driver.build_request(b"\x00\x00\x00\x00", fixtures)
            trusted = driver.invoke_sidecar(
                sidecar,
                request=request,
                request_file=temporary / "request.json",
                result_file=temporary / "result.json",
                timeout=10,
            )

        wrong_pc = copy.deepcopy(trusted)
        wrong_pc["results"][0]["final"]["pc"] = "0x817fff04"
        with self.assertRaisesRegex(driver.OracleError, "returned at the wrong PC"):
            driver.compare_results(trusted, wrong_pc, fixtures)

    def test_ram_differences_are_grouped_and_nonobservables_are_ignored(self) -> None:
        fixtures = driver.generate_fixtures(1, 0x1234)
        with tempfile.TemporaryDirectory() as temporary_value:
            temporary = Path(temporary_value)
            sidecar = make_fake_sidecar(temporary / "fake-sidecar")
            request = driver.build_request(b"\x00\x00\x00\x00", fixtures)
            reference = driver.invoke_sidecar(
                sidecar,
                request=request,
                request_file=temporary / "request.json",
                result_file=temporary / "result.json",
                timeout=10,
            )

        ignored = copy.deepcopy(reference)
        ignored_final = ignored["results"][0]["final"]
        ignored_final.update(
            {
                "lr": "0x11111111",
                "ctr": "0x22222222",
                "cr": "0x33333333",
                "xer": "0x44444444",
            }
        )
        ignored_final["gpr"]["4"] = "0x55555555"
        self.assertTrue(driver.compare_results(reference, ignored, fixtures).equal)

        candidate = copy.deepcopy(ignored)
        memory = bytearray.fromhex(candidate["results"][0]["final"]["memory"][0]["data_hex"])
        memory[4] ^= 0xFF
        memory[5] ^= 0xFF
        memory[14] ^= 0xFF
        candidate["results"][0]["final"]["memory"][0]["data_hex"] = memory.hex()
        comparison = driver.compare_results(reference, candidate, fixtures, mismatch_limit=1)
        self.assertFalse(comparison.equal)
        self.assertEqual(comparison.mismatch_count, 2)
        self.assertEqual(comparison.omitted_mismatches, 1)
        self.assertRegex(
            comparison.mismatches[0],
            r"edge-active-one ram\[0x80010004\+2\] ref=[0-9a-f]{4} cand=[0-9a-f]{4}",
        )


if __name__ == "__main__":
    unittest.main()
