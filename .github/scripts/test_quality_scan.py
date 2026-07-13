#!/usr/bin/env python3
"""Focused tests for the quality gate's asm exceptions."""

import contextlib
import io
import unittest

import quality_scan


def body_ok(body: str, path: str, func: str) -> bool:
    with contextlib.redirect_stdout(io.StringIO()):
        return quality_scan.asm_body_ok(body, path, func)


def source_ok(source: str, path: str, added_lines: list[tuple[int, str]]) -> bool:
    with contextlib.redirect_stdout(io.StringIO()):
        return quality_scan.scan_source(path, source, added_lines)


class QualityScanAllowlistTests(unittest.TestCase):
    def test_hardware_primitive_still_allowed_globally(self) -> None:
        self.assertTrue(body_ok("mfmsr r3\nblr", "src/dolphin/os/OS.c", "PPCMfmsr"))

    def test_paired_single_allowed_for_named_dolphin_sdk_function(self) -> None:
        body = """
            nofralloc
            stwu r1, -64(r1)
            psq_l f0, 0(r3), 0, qr0
            ps_madds1 f1, f2, f3, f4
            stfd f14, 8(r1)
            blr
        """
        self.assertTrue(body_ok(body, quality_scan.DOLPHIN_PAIRED_SINGLE_PATH, "PSMTXConcat"))

    def test_paired_single_rejected_outside_dolphin(self) -> None:
        self.assertFalse(body_ok("ps_add f1, f2, f3", "src/game/fight.c", "PSMTXConcat"))

    def test_paired_single_rejected_for_unknown_symbol(self) -> None:
        self.assertFalse(body_ok("ps_add f1, f2, f3", quality_scan.DOLPHIN_PAIRED_SINGLE_PATH, "PSMTXNew"))

    def test_calls_remain_forbidden_in_paired_single_function(self) -> None:
        self.assertFalse(body_ok("ps_add f1, f2, f3\nbl helper", quality_scan.DOLPHIN_PAIRED_SINGLE_PATH, "PSVECNormalize"))

    def test_general_gpr_load_remains_forbidden(self) -> None:
        self.assertFalse(body_ok("ps_mul f1, f2, f3\nlwz r3, 0(r4)", quality_scan.DOLPHIN_PAIRED_SINGLE_PATH, "PSMTXInverse"))

    def test_paired_exception_requires_paired_instruction(self) -> None:
        self.assertFalse(body_ok("fres f1, f2", quality_scan.DOLPHIN_PAIRED_SINGLE_PATH, "PSMTXInverse"))

    def test_vendor_c_wrappers_are_not_allowlisted(self) -> None:
        self.assertNotIn("PSMTXRotRad", quality_scan.DOLPHIN_PAIRED_SINGLE_FUNCTIONS)
        self.assertNotIn("PSMTXRotAxisRad", quality_scan.DOLPHIN_PAIRED_SINGLE_FUNCTIONS)
        self.assertEqual(len(quality_scan.DOLPHIN_PAIRED_SINGLE_FUNCTIONS), 26)

    def test_changed_instruction_inside_existing_inline_asm_is_scanned(self) -> None:
        source = """void PSVECNormalize(void)
{
    asm {
        psq_l f1, 0(r3), 0, qr0
        frsqrte f2, f1
    }
}
"""
        self.assertTrue(source_ok(
            source,
            quality_scan.DOLPHIN_PAIRED_SINGLE_PATH,
            [(5, "        frsqrte f2, f1")],
        ))

    def test_whole_asm_function_is_mapped_to_its_symbol(self) -> None:
        source = """asm void PSMTXCopy(void)
{
    psq_l f0, 0(r3), 0, qr0
    psq_st f0, 0(r4), 0, qr0
    blr
}
"""
        self.assertTrue(source_ok(
            source,
            quality_scan.DOLPHIN_PAIRED_SINGLE_PATH,
            [(1, "asm void PSMTXCopy(void)")],
        ))

    def test_changed_inline_asm_rejects_forbidden_instruction(self) -> None:
        source = """void PSVECNormalize(void)
{
    asm {
        psq_l f1, 0(r3), 0, qr0
        lwz r3, 0(r4)
    }
}
"""
        self.assertFalse(source_ok(
            source,
            quality_scan.DOLPHIN_PAIRED_SINGLE_PATH,
            [(5, "        lwz r3, 0(r4)")],
        ))

    def test_unbalanced_inline_asm_fails_closed(self) -> None:
        source = """void PSVECNormalize(void)
{
    asm {
        psq_l f1, 0(r3), 0, qr0
}
"""
        self.assertFalse(source_ok(
            source,
            quality_scan.DOLPHIN_PAIRED_SINGLE_PATH,
            [(3, "    asm {")],
        ))

    def test_zero_context_diff_tracks_added_head_line_numbers(self) -> None:
        diff = """diff --git a/src/example.c b/src/example.c
--- a/src/example.c
+++ b/src/example.c
@@ -4,0 +5,2 @@
+    asm {
+        ps_add f1, f2, f3
"""
        self.assertEqual(
            quality_scan.added_lines_from_diff(diff),
            {
                "src/example.c": [
                    (5, "    asm {"),
                    (6, "        ps_add f1, f2, f3"),
                ],
            },
        )


if __name__ == "__main__":
    unittest.main()
