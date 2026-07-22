#!/usr/bin/env python3
from __future__ import annotations

import hashlib
import importlib.util
import json
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock


MODULE_PATH = Path(__file__).with_name("compile_loop.py")
SPEC = importlib.util.spec_from_file_location("compile_loop", MODULE_PATH)
assert SPEC and SPEC.loader
compile_loop = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = compile_loop
SPEC.loader.exec_module(compile_loop)


OWNER_TARGET_DEFINITION = """int msgctrlWait(EffectUtilCommandObj* obj) {
    u8* stream;
    short counter;
    if (obj->activeFlag == 0) {
        if (obj->waitCounter == 0) {
            stream = obj->stream;
            obj->waitCounter = (short)((short)stream[0] + 1);
        }
        counter = obj->waitCounter;
        counter = (short)(counter - 1);
        obj->waitCounter = counter;
        if (counter <= 0) {
            obj->waitCounter = 0;
        } else {
            stream = obj->stream;
            obj->stream = stream - 3;
            return 1;
        }
    }
    stream = obj->stream;
    obj->stream = stream + 1;
    return 0;
}"""

OWNER_TEST_BASE = """typedef unsigned char u8;
typedef struct EffectUtilCommandObj {
    u8 activeFlag;
    short waitCounter;
    u8* stream;
} EffectUtilCommandObj;
int sibling(void) { return 7; }
""" + OWNER_TARGET_DEFINITION + "\n"


class CompileLoopTests(unittest.TestCase):
    def make_workunit(
        self,
        root: Path,
        *,
        function: str = "target",
        pct: float = 90.0,
        fidelity: str = "isolated-equals-full-tu",
    ) -> Path:
        unit = root / function
        unit.mkdir(parents=True)
        (unit / "base.c").write_text(
            f"int {function}(int x) {{ return x; }}\n", encoding="utf-8"
        )
        (unit / "compile.sh").write_text("#!/bin/sh\nexit 0\n", encoding="utf-8")
        (unit / "compile.sh").chmod(0o700)
        (unit / "compile_cmd.json").write_text("{}\n", encoding="utf-8")
        (unit / "meta.json").write_text(
            json.dumps(
                {
                    "fn": function,
                    "pct": pct,
                    "fidelity": fidelity,
                    "mw_version": "GC/1.2.5n",
                }
            )
            + "\n",
            encoding="utf-8",
        )
        (unit / "settings.toml").write_text("# test\n", encoding="utf-8")
        (unit / "target.o").write_bytes(b"target-object")
        return unit

    def make_owner_workunit(self, root: Path) -> tuple[Path, str]:
        function = "msgctrlWait"
        unit = self.make_workunit(
            root, function=function, fidelity=compile_loop.OWNER_FIDELITY
        )
        (unit / "base.c").write_text(OWNER_TEST_BASE, encoding="utf-8")
        files = unit / "attested"
        files.mkdir()
        paths = {
            name: files / name
            for name in (
                "generator",
                "objcopy",
                "readelf",
                "seed",
                "owner-source",
                "live-owner",
            )
        }
        for name, path in paths.items():
            path.write_bytes(name.encode("ascii"))
        # The production schema requires the owner source itself to be one of
        # the preprocessing inputs and to resolve inside the repository.
        paths["owner-source"] = compile_loop.REPO / "src/game/msgctrl.c"
        clang = shutil.which("clang")
        if not clang:
            raise unittest.SkipTest("Clang is unavailable")
        paths.update(
            {
                "sanitizer": Path(clang).resolve(),
                "source_guard": compile_loop.PERMUTER_TOOLS / "owner_source.py",
                "extractor": compile_loop.PERMUTER_TOOLS / "owner_extract.py",
                "python": Path(sys.executable).resolve(),
                "compiler": compile_loop.REPO
                / "build/compilers/GC/1.3/mwcceppc.exe",
                "wibo": compile_loop.REPO / "build/tools/wibo",
                "sjiswrap": compile_loop.REPO / "build/tools/sjiswrap.exe",
            }
        )
        version_result = subprocess.run(
            [str(paths["sanitizer"]), "--version"],
            capture_output=True,
            text=True,
            env={"LANG": "C", "LC_ALL": "C", "PATH": "/usr/bin:/bin"},
        )
        self.assertEqual(version_result.returncode, 0)
        version = version_result.stdout.strip()

        def attest(name: str) -> dict[str, str]:
            path = paths[name]
            digest = compile_loop.file_sha256(path)
            assert digest is not None
            return {"path": str(path), "sha256": digest}

        shaped = {
            "start": 4,
            "size": 4,
            "bytes_sha256": "b" * 64,
            "relocations_sha256": "c" * 64,
            "fingerprint_sha256": "a" * 64,
        }
        clean = {
            "start": 4,
            "size": 4,
            "bytes_sha256": "e" * 64,
            "relocations_sha256": "f" * 64,
            "fingerprint_sha256": "d" * 64,
        }
        audit = {
            "state": "passed",
            "sibling_functions": 89,
            "sibling_relocations": 386,
            "allocatable_non_text_sections": 2,
            "sibling_relocations_sha256": "7" * 64,
        }
        base_sha = compile_loop.file_sha256(unit / "base.c")
        assert base_sha is not None
        meta = {
            "fn": function,
            "pct": 90.0,
            "size": 4,
            "mode": "full-owner",
            "fidelity": compile_loop.OWNER_FIDELITY,
            "mw_version": "GC/1.3",
            "owner": {
                "schema": 2,
                "source": "src/game/msgctrl.c",
                "seed": attest("seed"),
                "clean_base_sha256": base_sha,
                "candidate_policy": {
                    "schema": 1,
                    "function": function,
                    "parser": str(paths["sanitizer"]),
                    "parser_mode": compile_loop.OWNER_PARSER_MODE,
                    "intrinsic_allowlist": [],
                },
                "context_transform": {
                    "schema": 1,
                    "policy": "msgctrlWait-pragma-clean-v1",
                    "function": function,
                    "removed_pragmas": ["optimization_level 4", "peephole off"],
                    "inserted_pragmas": ["peephole on"],
                    "following_state_restore": "peephole on retained",
                    "shaped_source_sha256": "4" * 64,
                    "clean_source_sha256": base_sha,
                    "following_source_sha256": "5" * 64,
                },
                "sanitizer": {
                    **attest("sanitizer"),
                    "version": version,
                    "version_sha256": hashlib.sha256(version.encode()).hexdigest(),
                    "argv": ["-E", "-P"],
                    "inputs": [attest("owner-source")],
                },
                "generator": attest("generator"),
                "source_guard": attest("source_guard"),
                "extractor": attest("extractor"),
                "objcopy": attest("objcopy"),
                "readelf": attest("readelf"),
                "python": attest("python"),
                "compiler": attest("compiler"),
                "wibo": attest("wibo"),
                "sjiswrap": attest("sjiswrap"),
                "live_owner": {**attest("live-owner"), "target": dict(shaped)},
                "shaped_owner": {
                    "source_sha256": "4" * 64,
                    "sha256": "8" * 64,
                    "target": dict(shaped),
                    "sibling_audit": dict(audit),
                },
                "clean_owner": {
                    "source_sha256": base_sha,
                    "sha256": "9" * 64,
                    "target": dict(clean),
                },
                "retail_target": {
                    "size": 4,
                    "bytes_sha256": "1" * 64,
                    "relocations_sha256": "2" * 64,
                    "fingerprint_sha256": "3" * 64,
                    "elf_sha256": compile_loop.file_sha256(unit / "target.o"),
                },
                "extracted_baseline": {
                    **clean,
                    "elf_sha256": "6" * 64,
                    "functions": [function],
                },
                "sibling_audit": dict(audit),
            },
        }
        bound_values = [
            str(paths[name].resolve())
            for name in ("objcopy", "readelf", "python", "sanitizer")
        ]
        repo_bindings = (
            "tools/decomp_work/permuter/owner_source.py",
            "tools/decomp_work/permuter/owner_extract.py",
            "build/tools/wibo",
            "build/tools/sjiswrap.exe",
            "build/compilers/GC/1.3/mwcceppc.exe",
        )
        (unit / "compile.sh").write_text(
            "#!/bin/sh\n# "
            + " ".join(bound_values)
            + " "
            + " ".join(repo_bindings)
            + " --parser "
            + "d" * 64
            + "\nexit 0\n",
            encoding="utf-8",
        )
        (unit / "compile.sh").chmod(0o700)
        (unit / "meta.json").write_text(json.dumps(meta) + "\n", encoding="utf-8")
        return unit, version

    def make_semantic_config(
        self,
        root: Path,
        *,
        function: str = "GStextureLockImage",
    ) -> compile_loop.SemanticOracleConfig:
        root.mkdir(parents=True, exist_ok=True)
        checkout = root / "checkout"
        checkout.mkdir(exist_ok=True)
        paths = {
            name: root / name
            for name in (
                "sidecar",
                "native-sidecar",
                "driver.py",
                "pins.json",
                "main.dol",
                "objcopy",
                "readelf",
            )
        }
        for name, path in paths.items():
            path.write_bytes(name.encode("ascii"))
        for name in ("sidecar", "native-sidecar", "objcopy", "readelf"):
            paths[name].chmod(0o700)
        attestation = root / "sidecar.attestation.json"
        attestation.write_text("{}\n", encoding="utf-8")
        native_attestation = root / "native-sidecar.attestation.json"
        native_attestation.write_text("{}\n", encoding="utf-8")
        native_manifest = root / "native-sidecar.generated-manifest.json"
        native_manifest.write_text("{}\n", encoding="utf-8")
        provenance = (
            ("DolRecomp", "3" * 40),
            ("ModernGekko", "1" * 40),
            ("RecompCore", "2" * 40),
        )
        pin_report = {
            "manifest_sha256": compile_loop.file_sha256(paths["pins.json"]),
            "checkout": str(checkout.resolve()),
            "recursive_submodules": 0,
            "components": [
                {"name": name, "commit": commit} for name, commit in provenance
            ],
        }
        return compile_loop.SemanticOracleConfig(
            profile="GStextureLockImage-v2",
            function=function,
            checkout=checkout,
            sidecar=paths["sidecar"],
            attestation=attestation,
            native_sidecar=paths["native-sidecar"],
            native_attestation=native_attestation,
            native_manifest=native_manifest,
            driver=paths["driver.py"],
            pins=paths["pins.json"],
            dol=paths["main.dol"],
            objcopy=paths["objcopy"],
            readelf=paths["readelf"],
            fixture_count=32,
            seed=0x1234,
            timeout=10,
            mismatch_limit=2,
            expected_sidecar_sha256=compile_loop.file_sha256(paths["sidecar"]),
            expected_attestation_sha256=compile_loop.file_sha256(attestation),
            expected_native_sidecar_sha256=compile_loop.file_sha256(
                paths["native-sidecar"]
            ),
            expected_native_attestation_sha256=compile_loop.file_sha256(
                native_attestation
            ),
            expected_native_manifest_sha256=compile_loop.file_sha256(native_manifest),
            expected_driver_sha256=compile_loop.file_sha256(paths["driver.py"]),
            expected_pins_sha256=compile_loop.file_sha256(paths["pins.json"]),
            expected_dol_sha256=compile_loop.file_sha256(paths["main.dol"]),
            expected_pin_report_sha256=hashlib.sha256(
                json.dumps(pin_report, separators=(",", ":"), sort_keys=True).encode()
            ).hexdigest(),
            expected_provenance=tuple(sorted(provenance)),
        )

    def semantic_pin_report(
        self, config: compile_loop.SemanticOracleConfig
    ) -> dict[str, object]:
        return {
            "manifest_sha256": config.expected_pins_sha256,
            "checkout": str(config.checkout.resolve()),
            "recursive_submodules": 0,
            "components": [
                {"name": name, "commit": commit}
                for name, commit in config.expected_provenance
            ],
        }

    def semantic_report_payload(
        self,
        config: compile_loop.SemanticOracleConfig,
        *,
        equal: bool,
        mismatch_count: int,
        mismatches: list[str],
    ) -> dict[str, object]:
        return {
            "schema_version": 1,
            "profile": config.profile,
            "function": config.function,
            "driver_sha256": config.expected_driver_sha256,
            "sidecar_sha256": config.expected_sidecar_sha256,
            "build_attestation_sha256": config.expected_attestation_sha256,
            "build_attestation_state_sha256": "f" * 64,
            "engine": compile_loop.SEMANTIC_ENGINE,
            "native_qualification": {
                "engine": compile_loop.SEMANTIC_NATIVE_ENGINE,
                "equal": True,
                "mismatch_count": 0,
                "sidecar_sha256": config.expected_native_sidecar_sha256,
                "build_attestation_sha256": (
                    config.expected_native_attestation_sha256
                ),
                "build_attestation_state_sha256": "d" * 64,
                "generated_manifest_sha256": config.expected_native_manifest_sha256,
            },
            "provenance": dict(config.expected_provenance),
            "pins": self.semantic_pin_report(config),
            "reference_authority": {
                "virtual_address": "0x800ef548",
                "size": 0x30,
                "dol_sha1": "870e8b9693ca780782d80f22a6a4572d8ba9458f",
                "dol_sha256": config.expected_dol_sha256,
                "text_sha256": "e" * 64,
            },
            "fixture_count": config.fixture_count,
            "fixture_seed": f"0x{config.seed:08x}",
            "fixture_corpus_sha256": "a" * 64,
            "equal": equal,
            "mismatch_count": mismatch_count,
            "mismatches": mismatches,
            "omitted_mismatches": 0,
        }

    def semantic_score(
        self, *, equal: bool = True, evaluated: bool = True
    ) -> compile_loop.SemanticScore:
        return compile_loop.SemanticScore(
            evaluated=evaluated,
            equal=equal if evaluated else None,
            mismatch_count=(0 if equal else 1) if evaluated else None,
            feedback=("PASS: fixtures matched." if equal else "FAIL: r3 differed."),
            report_sha256="a" * 64,
            reference_elf_sha256="b" * 64,
            candidate_elf_sha256="c" * 64,
            report={"equal": equal} if evaluated else None,
        )

    def test_find_and_splice_function(self) -> None:
        source = "int before(void);\nint target(int x) { return x + 1; }\nint after(void);\n"
        candidate = "int target(int x)\n{\n  return x * 2;\n}\n"
        result = compile_loop.splice_function(source, "target", candidate)
        self.assertIn("return x * 2", result)
        self.assertNotIn("return x + 1", result)
        self.assertIn("int before(void);", result)
        self.assertIn("int after(void);", result)

    def test_extract_fenced_candidate(self) -> None:
        response = "analysis\n```c\nvoid target(void) { /* } */ return; }\n```"
        candidate = compile_loop.extract_candidate(response, "target")
        self.assertEqual(candidate, "void target(void) { /* } */ return; }\n")

    def test_reject_inline_assembly(self) -> None:
        response = "```c\nvoid target(void) { asm(\"nop\"); }\n```"
        with self.assertRaises(compile_loop.BenchError):
            compile_loop.extract_candidate(response, "target")

    def test_reject_qualified_inline_assembly(self) -> None:
        response = '```c\nvoid target(void) { __asm__ volatile("nop"); }\n```'
        with self.assertRaises(compile_loop.BenchError):
            compile_loop.extract_candidate(response, "target")

    def test_reject_inc_reference(self) -> None:
        response = "```c\nvoid target(void) { use(\"thing.inc\"); }\n```"
        with self.assertRaises(compile_loop.BenchError):
            compile_loop.extract_candidate(response, "target")

    def test_workunit_fingerprint_and_fidelity(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            unit = self.make_workunit(root)
            target = {
                "function": "target",
                "baseline_match_percent": 90.0,
                "workunit_sha256": compile_loop.workunit_sha256(unit),
            }
            suite = {"workunit_root": str(root)}
            self.assertEqual(compile_loop.resolve_workunit(suite, target), unit)

            (unit / "base.c").write_text("int target(int x) { return x + 1; }\n")
            with self.assertRaisesRegex(compile_loop.BenchError, "fingerprint changed"):
                compile_loop.resolve_workunit(suite, target)

            meta = json.loads((unit / "meta.json").read_text())
            meta["fidelity"] = "unknown"
            (unit / "meta.json").write_text(json.dumps(meta) + "\n")
            target["workunit_sha256"] = compile_loop.workunit_sha256(unit)
            with self.assertRaisesRegex(compile_loop.BenchError, "not fidelity-gated"):
                compile_loop.resolve_workunit(suite, target)

    def test_workunit_incumbent_guardrails(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            unit = self.make_workunit(root)
            (unit / "base.c").write_text(
                'int target(int x) { __asm__ volatile("nop"); return x; }\n'
            )
            target = {
                "function": "target",
                "baseline_match_percent": 90.0,
                "workunit_sha256": compile_loop.workunit_sha256(unit),
            }
            with self.assertRaisesRegex(compile_loop.BenchError, "source guardrails"):
                compile_loop.resolve_workunit({"workunit_root": str(root)}, target)

    def test_full_owner_candidate_guard_rejects_shapers(self) -> None:
        accepted = "int target(int x) { return x ? x : 0; }"
        clang = shutil.which("clang")
        if not clang:
            self.skipTest("Clang is unavailable")
        self.assertEqual(
            compile_loop.extract_candidate(
                accepted,
                "target",
                full_owner=True,
                owner_parser=Path(clang),
                owner_context=accepted,
            ),
            accepted + "\n",
        )
        rejected = {
            "goto": "int target(int x) { goto done; done: return x; }",
            "pragma": "int target(int x) {\n#pragma peephole off\nreturn x; }",
            "self-assignment": "int target(int x) { x = x; return x; }",
            "register": "int target(int x) { register int y = x; return y; }",
        }
        for label, source in rejected.items():
            with self.subTest(label=label):
                with self.assertRaisesRegex(
                    compile_loop.BenchError, "full-owner target rejected"
                ):
                    compile_loop.extract_candidate(
                        source,
                        "target",
                        full_owner=True,
                        owner_parser=Path(clang),
                        owner_context=source,
                    )
        with self.assertRaisesRegex(compile_loop.BenchError, "signature drifted"):
            compile_loop.validate_owner_signature(
                "int target(int x) { return x; }",
                "long target(int x) { return x; }",
                "target",
            )

    def test_full_owner_workunit_attestation_and_six_file_hash(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            unit, version = self.make_owner_workunit(root)
            initial_hash = compile_loop.workunit_sha256(unit)
            (unit / "not-admitted.txt").write_text("ignored by fingerprint\n")
            self.assertEqual(compile_loop.workunit_sha256(unit), initial_hash)
            target = {
                "function": "msgctrlWait",
                "baseline_match_percent": 90.0,
                "workunit_sha256": initial_hash,
            }
            completed = subprocess.CompletedProcess(
                ["sanitizer", "--version"], 0, version + "\n", ""
            )
            with mock.patch.object(
                compile_loop, "run_command", return_value=completed
            ):
                self.assertEqual(
                    compile_loop.resolve_workunit(
                        {"workunit_root": str(root)}, target
                    ),
                    unit,
                )
                hashes = compile_loop.execution_tool_fingerprints([unit])
            self.assertEqual(
                hashes["full-owner:msgctrlWait:sanitizer-version"],
                hashlib.sha256(version.encode()).hexdigest(),
            )
            self.assertEqual(
                hashes["full-owner:msgctrlWait:extractor"],
                compile_loop.file_sha256(compile_loop.PERMUTER_TOOLS / "owner_extract.py"),
            )

            meta = json.loads((unit / "meta.json").read_text())
            meta["owner"]["sibling_audit"]["state"] = "failed"
            (unit / "meta.json").write_text(json.dumps(meta) + "\n")
            target["workunit_sha256"] = compile_loop.workunit_sha256(unit)
            with mock.patch.object(
                compile_loop, "run_command", return_value=completed
            ):
                with self.assertRaisesRegex(
                    compile_loop.BenchError, "sibling audit did not pass"
                ):
                    compile_loop.resolve_workunit(
                        {"workunit_root": str(root)}, target
                    )

    def test_full_owner_score_source_rejects_context_before_compiler(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            unit, version = self.make_owner_workunit(root)
            injected = unit / "injected.c"
            injected.write_text(
                (unit / "base.c").read_text().replace("return 7", "return 8"),
                encoding="utf-8",
            )
            completed = subprocess.CompletedProcess(
                ["sanitizer", "--version"], 0, version + "\n", ""
            )
            with mock.patch.object(
                compile_loop, "run_command", return_value=completed
            ) as runner:
                with self.assertRaisesRegex(
                    compile_loop.BenchError, "changed context before the target"
                ):
                    compile_loop.compile_and_score(
                        unit, injected, "msgctrlWait", "injected"
                    )
            # The only subprocess was the attested Clang version check.  The
            # compile wrapper/MWCC was never reached.
            self.assertEqual(runner.call_count, 1)

    def test_full_owner_source_must_be_an_attested_sanitizer_input(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            unit, version = self.make_owner_workunit(root)
            meta = json.loads((unit / "meta.json").read_text())
            meta["owner"]["sanitizer"]["inputs"] = [meta["owner"]["generator"]]
            (unit / "meta.json").write_text(json.dumps(meta) + "\n")
            target = {
                "function": "msgctrlWait",
                "baseline_match_percent": 90.0,
                "workunit_sha256": compile_loop.workunit_sha256(unit),
            }
            completed = subprocess.CompletedProcess(
                ["sanitizer", "--version"], 0, version + "\n", ""
            )
            with mock.patch.object(
                compile_loop, "run_command", return_value=completed
            ):
                with self.assertRaisesRegex(
                    compile_loop.BenchError, "not an attested sanitizer input"
                ):
                    compile_loop.resolve_workunit(
                        {"workunit_root": str(root)}, target
                    )

    def test_source_baseline_rejects_committed_source_change(self) -> None:
        commit = "a" * 40
        ok = subprocess.CompletedProcess(["git"], 0, "", "")
        changed = subprocess.CompletedProcess(["git"], 1, "", "")
        with mock.patch.object(compile_loop, "run_command", side_effect=[ok, changed]):
            with self.assertRaisesRegex(compile_loop.BenchError, "source/config changed"):
                compile_loop.validate_source_baseline(commit)

    def test_stream_parser_valid_malformed_and_truncated(self) -> None:
        event = {
            "model": "served",
            "choices": [{"delta": {"content": "ok"}, "finish_reason": "stop"}],
        }
        lines = [f"data: {json.dumps(event)}\n".encode(), b"data: [DONE]\n"]
        assistant, model, _, finish = compile_loop.parse_openai_stream(
            lines, key="secret", requested_model="requested"
        )
        self.assertEqual(assistant["content"], "ok")
        self.assertEqual(model, "served")
        self.assertEqual(finish, "stop")

        with self.assertRaisesRegex(compile_loop.BenchError, "malformed streaming JSON"):
            compile_loop.parse_openai_stream(
                [b"data: {bad json}\n"], key="secret", requested_model="requested"
            )
        delta_only = [b'data: {"choices":[{"delta":{"content":"partial"}}]}\n']
        with self.assertRaisesRegex(compile_loop.BenchError, "before a completion marker"):
            compile_loop.parse_openai_stream(
                delta_only, key="secret", requested_model="requested"
            )

    def test_stream_error_redacts_exact_key(self) -> None:
        key = "replacement-key-that-must-not-escape"
        event = {"error": {"type": key, "message": "prefix " + key + " suffix"}}
        with self.assertRaises(compile_loop.BenchError) as caught:
            compile_loop.parse_openai_stream(
                [f"data: {json.dumps(event)}\n".encode()],
                key=key,
                requested_model="requested",
            )
        self.assertNotIn(key, str(caught.exception))
        self.assertIn("[REDACTED]", str(caught.exception))

    def test_redirects_are_refused(self) -> None:
        handler = compile_loop.NoRedirectHandler()
        self.assertIsNone(handler.redirect_request(mock.Mock(), None, 302, "", {}, "https://x"))

    def test_stale_compile_output_is_not_accepted(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            unit = self.make_workunit(Path(temporary))
            stale = unit / "score_probe.o"
            stale.write_bytes(b"stale")
            completed = subprocess.CompletedProcess(["compile.sh"], 0, "", "")
            with mock.patch.object(compile_loop, "run_command", return_value=completed):
                score = compile_loop.compile_and_score(
                    unit, unit / "base.c", "target", "score_probe"
                )
            self.assertFalse(score.compile_ok)
            self.assertFalse(stale.exists())

    def test_semantic_mismatch_exit_is_valid_and_feedback_is_bounded(self) -> None:
        with tempfile.TemporaryDirectory() as temporary_value:
            root = Path(temporary_value)
            config = self.make_semantic_config(root)
            reference = root / "target.o"
            candidate = root / "candidate.o"
            report_file = root / "candidate.semantic.json"
            reference.write_bytes(b"reference")
            candidate.write_bytes(b"candidate")

            def fake_driver(argv: list[str], **_: object) -> subprocess.CompletedProcess[str]:
                self.assertEqual(
                    argv[argv.index("--expected-sidecar-sha256") + 1],
                    config.expected_sidecar_sha256,
                )
                self.assertEqual(
                    argv[argv.index("--expected-attestation-sha256") + 1],
                    config.expected_attestation_sha256,
                )
                self.assertEqual(
                    argv[argv.index("--native-sidecar") + 1],
                    str(config.native_sidecar),
                )
                self.assertEqual(
                    argv[argv.index("--native-attestation") + 1],
                    str(config.native_attestation),
                )
                self.assertEqual(
                    argv[argv.index("--native-manifest") + 1],
                    str(config.native_manifest),
                )
                self.assertEqual(
                    argv[argv.index("--expected-native-sidecar-sha256") + 1],
                    config.expected_native_sidecar_sha256,
                )
                self.assertEqual(
                    argv[argv.index("--expected-native-attestation-sha256") + 1],
                    config.expected_native_attestation_sha256,
                )
                self.assertEqual(
                    argv[argv.index("--expected-native-manifest-sha256") + 1],
                    config.expected_native_manifest_sha256,
                )
                output = Path(argv[argv.index("--report-file") + 1])
                payload = self.semantic_report_payload(
                    config,
                    equal=False,
                    mismatch_count=3,
                    mismatches=[
                        "fixture-0 r3 ref=0x00000000 cand=0x00000001 " + "x" * 500,
                        "fixture-1 ram ref=00 cand=01",
                        "fixture-2 status ref=returned cand=step_limit",
                    ],
                )
                payload["omitted_mismatches"] = 1
                output.write_text(
                    json.dumps(payload),
                    encoding="utf-8",
                )
                return subprocess.CompletedProcess(argv, 1, "", "")

            with (
                mock.patch.object(
                    compile_loop,
                    "_semantic_text_size",
                    side_effect=[(0x30, None), (0x34, None)],
                ),
                mock.patch.object(compile_loop, "run_command", side_effect=fake_driver),
            ):
                score = compile_loop.run_semantic_oracle(
                    config,
                    reference_elf=reference,
                    candidate_elf=candidate,
                    report_file=report_file,
                    strict=False,
                )
            self.assertTrue(score.evaluated)
            self.assertFalse(score.equal)
            self.assertEqual(score.mismatch_count, 3)
            self.assertIn("fixture-0", score.feedback)
            self.assertIn("fixture-1", score.feedback)
            self.assertNotIn("fixture-2", score.feedback)
            self.assertLessEqual(len(score.feedback), 2400)
            self.assertEqual(score.report_sha256, compile_loop.file_sha256(report_file))
            self.assertEqual(report_file.stat().st_mode & 0o777, 0o600)

    def test_candidate_semantic_ineligibility_is_feedback_but_baseline_is_strict(self) -> None:
        with tempfile.TemporaryDirectory() as temporary_value:
            root = Path(temporary_value)
            config = self.make_semantic_config(root)
            reference = root / "target.o"
            candidate = root / "candidate.o"
            reference.write_bytes(b"reference")
            candidate.write_bytes(b"candidate")
            report_file = root / "candidate.semantic.json"
            eligibility = [(0x48, None), (None, "candidate ELF contains relocations")]
            with mock.patch.object(
                compile_loop, "_semantic_text_size", side_effect=eligibility
            ), mock.patch.object(compile_loop, "run_command") as driver:
                score = compile_loop.run_semantic_oracle(
                    config,
                    reference_elf=reference,
                    candidate_elf=candidate,
                    report_file=report_file,
                    strict=False,
                )
            self.assertFalse(score.evaluated)
            self.assertIsNone(score.equal)
            self.assertIn("contains relocations", score.feedback)
            self.assertFalse(driver.called)
            persisted = json.loads(report_file.read_text(encoding="utf-8"))
            self.assertEqual(persisted["state"], "candidate_ineligible")

            with mock.patch.object(
                compile_loop,
                "_semantic_text_size",
                side_effect=[(0x48, None), (None, "candidate .text is empty")],
            ):
                with self.assertRaisesRegex(
                    compile_loop.SemanticError, "baseline is ineligible"
                ):
                    compile_loop.run_semantic_oracle(
                        config,
                        reference_elf=reference,
                        candidate_elf=candidate,
                        report_file=report_file,
                        strict=True,
                    )

    def test_semantic_runs_before_round_object_deletion_and_exact_disagreement_fails(self) -> None:
        with tempfile.TemporaryDirectory() as temporary_value:
            root = Path(temporary_value)
            unit = self.make_workunit(root)
            config = self.make_semantic_config(root / "semantic", function="target")

            def compile_command(
                argv: list[str], *, cwd: Path, **_: object
            ) -> subprocess.CompletedProcess[str]:
                (cwd / argv[-1]).write_bytes(b"candidate-object")
                return subprocess.CompletedProcess(argv, 0, "", "")

            def semantic_while_present(
                _: compile_loop.SemanticOracleConfig,
                *,
                candidate_elf: Path,
                **__: object,
            ) -> compile_loop.SemanticScore:
                self.assertTrue(candidate_elf.is_file())
                return self.semantic_score()

            with (
                mock.patch.object(compile_loop, "run_command", side_effect=compile_command),
                mock.patch.object(
                    compile_loop,
                    "score_object",
                    return_value=compile_loop.CompileScore(True, 99.0, 4, "diff"),
                ),
                mock.patch.object(
                    compile_loop, "run_semantic_oracle", side_effect=semantic_while_present
                ),
            ):
                score = compile_loop.compile_and_score(
                    unit,
                    unit / "base.c",
                    "target",
                    "semantic_probe",
                    semantic=config,
                )
            self.assertTrue(score.semantic and score.semantic.equal)
            self.assertFalse((unit / "semantic_probe.o").exists())

            with (
                mock.patch.object(compile_loop, "run_command", side_effect=compile_command),
                mock.patch.object(
                    compile_loop,
                    "score_object",
                    return_value=compile_loop.CompileScore(True, 100.0, 4, "exact"),
                ),
                mock.patch.object(
                    compile_loop,
                    "run_semantic_oracle",
                    return_value=self.semantic_score(equal=False),
                ),
            ):
                with self.assertRaisesRegex(
                    compile_loop.SemanticError, "benchmark invariant failed"
                ):
                    compile_loop.compile_and_score(
                        unit,
                        unit / "base.c",
                        "target",
                        "exact_probe",
                        semantic=config,
                    )
            self.assertFalse((unit / "exact_probe.o").exists())

    def test_semantic_report_errors_fail_closed(self) -> None:
        with tempfile.TemporaryDirectory() as temporary_value:
            root = Path(temporary_value)
            config = self.make_semantic_config(root)
            report = root / "report.json"
            payload = self.semantic_report_payload(
                config, equal=True, mismatch_count=1, mismatches=[]
            )
            report.write_text(
                json.dumps(payload),
                encoding="utf-8",
            )
            with self.assertRaisesRegex(
                compile_loop.SemanticError, "equality disagrees"
            ):
                compile_loop._semantic_report(report, config=config, returncode=0)
            payload = self.semantic_report_payload(
                config, equal=True, mismatch_count=0, mismatches=[]
            )
            payload["sidecar_sha256"] = "0" * 64
            report.write_text(json.dumps(payload), encoding="utf-8")
            with self.assertRaisesRegex(compile_loop.SemanticError, "sidecar fingerprint"):
                compile_loop._semantic_report(report, config=config, returncode=0)
            payload = self.semantic_report_payload(
                config, equal=True, mismatch_count=0, mismatches=[]
            )
            payload["native_qualification"]["equal"] = False
            payload["native_qualification"]["mismatch_count"] = 1
            report.write_text(json.dumps(payload), encoding="utf-8")
            with self.assertRaisesRegex(
                compile_loop.SemanticError, "native-original qualification"
            ):
                compile_loop._semantic_report(report, config=config, returncode=0)
            payload = self.semantic_report_payload(
                config, equal=True, mismatch_count=0, mismatches=[]
            )
            payload["native_qualification"]["generated_manifest_sha256"] = "0" * 64
            report.write_text(json.dumps(payload), encoding="utf-8")
            with self.assertRaisesRegex(
                compile_loop.SemanticError, "native generated manifest fingerprint"
            ):
                compile_loop._semantic_report(report, config=config, returncode=0)
            payload = self.semantic_report_payload(
                config, equal=True, mismatch_count=0, mismatches=[]
            )
            payload["native_qualification"]["build_attestation_state_sha256"] = "bad"
            report.write_text(json.dumps(payload), encoding="utf-8")
            with self.assertRaisesRegex(
                compile_loop.SemanticError, "native attested build state"
            ):
                compile_loop._semantic_report(report, config=config, returncode=0)
            payload = self.semantic_report_payload(
                config, equal=True, mismatch_count=0, mismatches=[]
            )
            payload["engine"] = "untrusted-engine"
            report.write_text(json.dumps(payload), encoding="utf-8")
            with self.assertRaisesRegex(compile_loop.SemanticError, "engine identity"):
                compile_loop._semantic_report(report, config=config, returncode=0)
            payload = self.semantic_report_payload(
                config, equal=True, mismatch_count=0, mismatches=[]
            )
            payload["profile"] = "wrong-profile-v1"
            report.write_text(
                json.dumps(payload),
                encoding="utf-8",
            )
            with self.assertRaisesRegex(compile_loop.SemanticError, "wrong profile"):
                compile_loop._semantic_report(report, config=config, returncode=0)

    def test_target_assembly_is_path_stable(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            objdump = root / "objdump"
            objdump.write_text("tool")
            first = root / "first"
            second = root / "second"
            first.mkdir()
            second.mkdir()
            completed = subprocess.CompletedProcess(
                [str(objdump)], 0, "target.o: file format elf32-powerpc\n  blr\n", ""
            )
            with (
                mock.patch.object(compile_loop, "ppc_objdump_path", return_value=objdump),
                mock.patch.object(compile_loop, "run_command", return_value=completed) as run,
            ):
                first_assembly = compile_loop.target_assembly(first, "target")
                second_assembly = compile_loop.target_assembly(second, "target")
            self.assertEqual(first_assembly, second_assembly)
            self.assertEqual(run.call_args_list[0].args[0][-1], "target.o")
            self.assertEqual(run.call_args_list[1].args[0][-1], "target.o")

    def test_semantic_target_config_is_explicit_and_tools_are_fingerprinted(self) -> None:
        with tempfile.TemporaryDirectory() as temporary_value:
            root = Path(temporary_value)
            config_files = self.make_semantic_config(root / "files")
            target = {
                "function": "GStextureLockImage",
                "semantic_oracle": {
                    "profile": "GStextureLockImage-v2",
                    "fixture_count": 64,
                    "seed": "0x89abcdef",
                    "timeout": 20,
                    "mismatch_limit": 4,
                },
            }
            args = mock.Mock(
                semantic_checkout=str(config_files.checkout),
                semantic_sidecar=str(config_files.sidecar),
                semantic_native_sidecar=str(config_files.native_sidecar),
            )
            with (
                mock.patch.object(compile_loop, "SEMANTIC_DRIVER", config_files.driver),
                mock.patch.object(compile_loop, "SEMANTIC_PINS", config_files.pins),
                mock.patch.object(compile_loop, "SEMANTIC_DOL", config_files.dol),
                mock.patch.object(
                    compile_loop, "ppc_objcopy_path", return_value=config_files.objcopy
                ),
                mock.patch.object(
                    compile_loop, "ppc_readelf_path", return_value=config_files.readelf
                ),
            ):
                config = compile_loop.resolve_semantic_config(target, args)
            assert config is not None
            self.assertEqual(config.fixture_count, 64)
            self.assertEqual(config.seed, 0x89ABCDEF)
            hashes = compile_loop.execution_tool_fingerprints([], [config])
            self.assertEqual(
                hashes["semantic-sidecar"], compile_loop.file_sha256(config.sidecar)
            )
            self.assertEqual(
                hashes["semantic-sidecar-attestation"],
                compile_loop.file_sha256(config.attestation),
            )
            self.assertEqual(
                hashes["semantic-native-sidecar"],
                compile_loop.file_sha256(config.native_sidecar),
            )
            self.assertEqual(
                hashes["semantic-native-sidecar-attestation"],
                compile_loop.file_sha256(config.native_attestation),
            )
            self.assertEqual(
                hashes["semantic-native-manifest"],
                compile_loop.file_sha256(config.native_manifest),
            )
            self.assertEqual(
                hashes["semantic-original-dol"], compile_loop.file_sha256(config.dol)
            )
            self.assertEqual(
                hashes["semantic-powerpc-eabi-readelf"],
                compile_loop.file_sha256(config.readelf),
            )
            pin_report = {
                "manifest_sha256": compile_loop.file_sha256(config.pins),
                "checkout": str(config.checkout),
                "recursive_submodules": 39,
                "components": [
                    {"name": "ModernGekko", "commit": "1" * 40},
                    {"name": "RecompCore", "commit": "2" * 40},
                    {"name": "DolRecomp", "commit": "3" * 40},
                ],
            }
            completed = subprocess.CompletedProcess(
                ["driver.py"], 0, json.dumps(pin_report), ""
            )
            with mock.patch.object(
                compile_loop, "run_command", return_value=completed
            ) as verify:
                verified = compile_loop.verify_semantic_checkout(config)
            self.assertEqual(verified, pin_report)
            self.assertIn("verify-pins", verify.call_args.args[0])
            bound = compile_loop.bind_semantic_runtime(
                config, tool_hashes=hashes, pin_report=verified
            )
            self.assertEqual(
                bound.expected_sidecar_sha256, hashes["semantic-sidecar"]
            )
            self.assertEqual(
                bound.expected_attestation_sha256,
                hashes["semantic-sidecar-attestation"],
            )
            self.assertEqual(
                bound.expected_native_sidecar_sha256,
                hashes["semantic-native-sidecar"],
            )
            self.assertEqual(
                bound.expected_native_attestation_sha256,
                hashes["semantic-native-sidecar-attestation"],
            )

            self.assertEqual(
                bound.expected_native_manifest_sha256,
                hashes["semantic-native-manifest"],
            )
            record = compile_loop.semantic_config_record(bound, verified)
            self.assertRegex(record["pin_report_sha256"], r"^[0-9a-f]{64}$")
            self.assertEqual(
                record["expected_native_manifest_sha256"],
                hashes["semantic-native-manifest"],
            )

            wrong_target = dict(target)
            wrong_target["function"] = "differentFunction"
            with self.assertRaisesRegex(compile_loop.BenchError, "is for"):
                compile_loop.resolve_semantic_config(wrong_target, args)

    def test_mobj_semantic_profile_is_bound_to_its_function_and_authority(self) -> None:
        self.assertEqual(
            compile_loop.SEMANTIC_PROFILE_FUNCTIONS["fn_801A6DA0-v1"],
            "fn_801A6DA0",
        )
        self.assertEqual(
            compile_loop.SEMANTIC_PROFILE_AUTHORITIES["fn_801A6DA0-v1"],
            {
                "virtual_address": "0x801a6da0",
                "size": 0x24,
                "dol_sha1": "870e8b9693ca780782d80f22a6a4572d8ba9458f",
            },
        )

    def test_msgctrl_wait_semantic_profile_is_bound_to_owner_target(self) -> None:
        self.assertEqual(
            compile_loop.SEMANTIC_PROFILE_FUNCTIONS["msgctrlWait-v1"],
            "msgctrlWait",
        )
        self.assertEqual(
            compile_loop.SEMANTIC_PROFILE_AUTHORITIES["msgctrlWait-v1"],
            {
                "virtual_address": "0x80132454",
                "size": 0x78,
                "dol_sha1": "870e8b9693ca780782d80f22a6a4572d8ba9458f",
            },
        )

    def test_semantic_feedback_does_not_replace_objdiff_ranking_or_termination(self) -> None:
        with tempfile.TemporaryDirectory() as temporary_value:
            root = Path(temporary_value)
            unit = self.make_workunit(root / "source")
            destination = root / "run"
            config = self.make_semantic_config(root / "semantic", function="target")
            reply = compile_loop.ApiReply(
                assistant={
                    "role": "assistant",
                    "content": "```c\nint target(int x) { return x + 1; }\n```",
                },
                served_model="test-model",
                usage={},
                elapsed_seconds=0.1,
                finish_reason="stop",
            )
            scores = [
                compile_loop.CompileScore(
                    True, 90.0, 4, "baseline", semantic=self.semantic_score()
                ),
                compile_loop.CompileScore(
                    True,
                    95.0,
                    4,
                    "candidate",
                    semantic=self.semantic_score(equal=False),
                ),
                compile_loop.CompileScore(
                    True, 95.0, 4, "candidate", semantic=self.semantic_score()
                ),
            ]
            outbound_messages: list[list[dict[str, object]]] = []

            def record_api_call(**kwargs: object) -> compile_loop.ApiReply:
                outbound_messages.append(
                    json.loads(json.dumps(kwargs["messages"]))
                )
                return reply

            with (
                mock.patch.object(compile_loop, "target_assembly", return_value="blr"),
                mock.patch.object(
                    compile_loop,
                    "call_openai_compatible",
                    side_effect=record_api_call,
                ),
                mock.patch.object(compile_loop, "compile_and_score", side_effect=scores),
            ):
                result = compile_loop.run_target(
                    source_workunit=unit,
                    run_directory=destination,
                    function="target",
                    provider="moonshot",
                    model="kimi-k3",
                    rounds=2,
                    max_tokens=100,
                    timeout=10,
                    key_file=root / "unused-key",
                    key_label=None,
                    source_commit="a" * 40,
                    runner_commit="b" * 40,
                    expected_baseline=90.0,
                    expected_workunit_sha256=compile_loop.workunit_sha256(unit),
                    allow_reasoning_salvage=False,
                    semantic_config=config,
                )
            self.assertEqual(result["rounds_completed"], 2)
            self.assertEqual(result["best_match_percent"], 95.0)
            self.assertFalse(result["exact"])
            self.assertFalse(result["isolated_objdiff_exact"])
            self.assertFalse(result["campaign_bankable"])
            self.assertEqual(
                result["campaign_acceptance_status"],
                "not-isolated-objdiff-exact",
            )
            self.assertEqual(
                result["full_dol_validation"],
                {
                    "performed": False,
                    "passed": None,
                    "artifact": "build/GC6E01/main.dol",
                    "sha1_authority": "config/GC6E01/build.sha1",
                },
            )
            self.assertFalse(result["rounds"][0]["semantic_equal"])
            self.assertEqual(len(outbound_messages), 2)
            self.assertIn(
                "FAIL: r3 differed.",
                outbound_messages[1][-1]["content"],
            )
            initial = (destination / "initial_messages.json").read_text(encoding="utf-8")
            self.assertIn("Dolphin-interpreter behavioral feedback", initial)
            prompt = compile_loop.feedback_prompt(
                function="target",
                score=scores[1],
                attempted_candidate="int target(int x) { return x + 1; }",
                best_percent=95.0,
                best_candidate="int target(int x) { return x + 1; }",
            )
            self.assertIn("diagnostic only", prompt)

    def test_semantic_pass_objdiff_exact_stops_but_is_not_bankable(self) -> None:
        with tempfile.TemporaryDirectory() as temporary_value:
            root = Path(temporary_value)
            unit = self.make_workunit(root / "source")
            config = self.make_semantic_config(root / "semantic", function="target")
            reply = compile_loop.ApiReply(
                assistant={
                    "role": "assistant",
                    "content": "```c\nint target(int x) { return x + 1; }\n```",
                },
                served_model="test-model",
                usage={},
                elapsed_seconds=0.1,
                finish_reason="stop",
            )
            scores = [
                compile_loop.CompileScore(
                    True, 90.0, 4, "baseline", semantic=self.semantic_score()
                ),
                compile_loop.CompileScore(
                    True, 100.0, 4, "exact", semantic=self.semantic_score()
                ),
            ]
            with (
                mock.patch.object(compile_loop, "target_assembly", return_value="blr"),
                mock.patch.object(
                    compile_loop, "call_openai_compatible", return_value=reply
                ) as api,
                mock.patch.object(compile_loop, "compile_and_score", side_effect=scores),
            ):
                result = compile_loop.run_target(
                    source_workunit=unit,
                    run_directory=root / "run",
                    function="target",
                    provider="moonshot",
                    model="kimi-k3",
                    rounds=3,
                    max_tokens=100,
                    timeout=10,
                    key_file=root / "unused-key",
                    key_label=None,
                    source_commit="a" * 40,
                    runner_commit="b" * 40,
                    expected_baseline=90.0,
                    expected_workunit_sha256=compile_loop.workunit_sha256(unit),
                    allow_reasoning_salvage=False,
                    semantic_config=config,
                )
            self.assertEqual(result["rounds_completed"], 1)
            self.assertEqual(result["best_match_percent"], 100.0)
            self.assertTrue(result["isolated_objdiff_exact"])
            self.assertTrue(result["exact"])
            self.assertFalse(result["campaign_bankable"])
            self.assertEqual(
                result["campaign_acceptance_status"],
                "requires-source-integration-and-full-dol-validation",
            )
            self.assertFalse(result["full_dol_validation"]["performed"])
            self.assertIsNone(result["full_dol_validation"]["passed"])
            api.assert_called_once()

    def test_full_owner_exactness_is_reported_separately(self) -> None:
        with tempfile.TemporaryDirectory() as temporary_value:
            root = Path(temporary_value)
            unit, _version = self.make_owner_workunit(root / "source")
            reply = compile_loop.ApiReply(
                assistant={
                    "role": "assistant",
                    "content": f"```c\n{OWNER_TARGET_DEFINITION}\n```",
                },
                served_model="test-model",
                usage={},
                elapsed_seconds=0.1,
                finish_reason="stop",
            )
            scores = [
                compile_loop.CompileScore(True, 90.0, 4, "baseline"),
                compile_loop.CompileScore(True, 100.0, 4, "exact"),
            ]
            with (
                mock.patch.object(compile_loop, "target_assembly", return_value="blr"),
                mock.patch.object(
                    compile_loop, "call_openai_compatible", return_value=reply
                ),
                mock.patch.object(
                    compile_loop, "compile_and_score", side_effect=scores
                ),
            ):
                result = compile_loop.run_target(
                    source_workunit=unit,
                    run_directory=root / "run",
                    function="msgctrlWait",
                    provider="moonshot",
                    model="kimi-k3",
                    rounds=1,
                    max_tokens=100,
                    timeout=10,
                    key_file=root / "unused-key",
                    key_label=None,
                    source_commit="a" * 40,
                    runner_commit="b" * 40,
                    expected_baseline=90.0,
                    expected_workunit_sha256=compile_loop.workunit_sha256(unit),
                    allow_reasoning_salvage=False,
                )
            self.assertEqual(
                result["comparison_mode"], "full-owner-clean-context-extracted"
            )
            self.assertTrue(result["workunit_objdiff_exact"])
            self.assertFalse(result["isolated_objdiff_exact"])
            self.assertTrue(result["full_owner_clean_context_objdiff_exact"])
            self.assertTrue(result["full_owner_extracted_objdiff_exact"])
            self.assertTrue(result["exact"])
            self.assertFalse(result["campaign_bankable"])

    def test_semantic_infrastructure_failure_aborts_target(self) -> None:
        with tempfile.TemporaryDirectory() as temporary_value:
            root = Path(temporary_value)
            unit = self.make_workunit(root / "source")
            config = self.make_semantic_config(root / "semantic", function="target")
            reply = compile_loop.ApiReply(
                assistant={
                    "role": "assistant",
                    "content": "```c\nint target(int x) { return x + 1; }\n```",
                },
                served_model="test-model",
                usage={},
                elapsed_seconds=0.1,
                finish_reason="stop",
            )
            with (
                mock.patch.object(compile_loop, "target_assembly", return_value="blr"),
                mock.patch.object(compile_loop, "call_openai_compatible", return_value=reply),
                mock.patch.object(
                    compile_loop,
                    "compile_and_score",
                    side_effect=[
                        compile_loop.CompileScore(
                            True, 90.0, 4, "baseline", semantic=self.semantic_score()
                        ),
                        compile_loop.SemanticError("semantic driver failed"),
                    ],
                ),
            ):
                with self.assertRaisesRegex(
                    compile_loop.SemanticError, "semantic driver failed"
                ):
                    compile_loop.run_target(
                        source_workunit=unit,
                        run_directory=root / "run",
                        function="target",
                        provider="moonshot",
                        model="kimi-k3",
                        rounds=1,
                        max_tokens=100,
                        timeout=10,
                        key_file=root / "unused-key",
                        key_label=None,
                        source_commit="a" * 40,
                        runner_commit="b" * 40,
                        expected_baseline=90.0,
                        expected_workunit_sha256=compile_loop.workunit_sha256(unit),
                        allow_reasoning_salvage=False,
                        semantic_config=config,
                    )

    def test_reasoning_salvage_is_private_and_excluded_from_primary(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            unit = self.make_workunit(root / "source")
            destination = root / "run"
            config = self.make_semantic_config(root / "semantic", function="target")
            unit_hash = compile_loop.workunit_sha256(unit)
            reply = compile_loop.ApiReply(
                assistant={
                    "role": "assistant",
                    "content": "",
                    "reasoning_content": (
                        "PRIVATE SCRATCH\n```c\nint target(int x) { return x + 1; }\n```"
                    ),
                },
                served_model="test-model",
                usage={"completion_tokens": 10},
                elapsed_seconds=0.1,
                finish_reason="length",
            )
            scores = [
                compile_loop.CompileScore(True, 90.0, 4, "baseline"),
                compile_loop.CompileScore(True, 95.0, 4, "candidate"),
            ]
            with (
                mock.patch.object(compile_loop, "target_assembly", return_value="blr"),
                mock.patch.object(compile_loop, "call_openai_compatible", return_value=reply),
                mock.patch.object(
                    compile_loop, "compile_and_score", side_effect=scores
                ) as compile_score,
            ):
                result = compile_loop.run_target(
                    source_workunit=unit,
                    run_directory=destination,
                    function="target",
                    provider="moonshot",
                    model="kimi-k3",
                    rounds=1,
                    max_tokens=100,
                    timeout=10,
                    key_file=root / "unused-key",
                    key_label=None,
                    source_commit="a" * 40,
                    runner_commit="b" * 40,
                    expected_baseline=90.0,
                    expected_workunit_sha256=unit_hash,
                    allow_reasoning_salvage=True,
                    semantic_config=config,
                )
            self.assertEqual(result["best_match_percent"], 90.0)
            self.assertEqual(result["reasoning_salvage_best_match_percent"], 95.0)
            self.assertFalse(result["rounds"][0]["primary_compile_ok"])
            self.assertTrue(result["rounds"][0]["salvage_compile_ok"])
            persisted = (destination / "round_01.json").read_text()
            self.assertNotIn("reasoning_content", persisted)
            self.assertNotIn("PRIVATE SCRATCH", persisted)
            self.assertEqual((destination / "round_01.json").stat().st_mode & 0o777, 0o600)
            self.assertIs(compile_score.call_args_list[0].kwargs["semantic"], config)
            self.assertIsNone(compile_score.call_args_list[1].kwargs["semantic"])


if __name__ == "__main__":
    unittest.main()
