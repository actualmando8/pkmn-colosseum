#!/usr/bin/env python3
from __future__ import annotations

import importlib.util
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


MODULE_PATH = Path(__file__).with_name("owner_source.py")
SPEC = importlib.util.spec_from_file_location("owner_source", MODULE_PATH)
assert SPEC and SPEC.loader
owner_source = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = owner_source
SPEC.loader.exec_module(owner_source)


NATURAL = """s32 msgctrlWait(EffectUtilCommandObj* obj) {
    u8* stream;
    s16 counter;
    if (obj->activeFlag == 0) {
        if (obj->waitCounter == 0) {
            stream = obj->stream;
            obj->waitCounter = (s16)((s16)stream[0] + 1);
        }
        counter = obj->waitCounter;
        counter = (s16)(counter - 1);
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


CONTEXT = """
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef struct EffectUtilCommandObj {
    u8 field_00;
    u8 activeFlag;
    u8 field_02;
    u8 field_03;
    f32 field_04;
    f32 field_08;
    f32 field_0C;
    f32 field_10;
    u8 pad_14[0x0C];
    u16 commandValue;
    u8 pad_22;
    u8 field_23;
    u32 colorRgba;
    u8 pad_28[4];
    u8* savedStream;
    u8* stream;
    u8 pad_34[0x0D];
    u8 field_41;
    u8 field_42;
    u8 field_43;
    u8 flags;
    u8 pendingFlag;
    u8 doneFlag;
    u8 pad_47;
    s16 waitCounter;
    u8 alignMode;
    u8 field_4B;
    u8 pad_4C[0x18];
    f32 field_64;
} EffectUtilCommandObj;
typedef struct EffectTraceFxEntry {
    s32 value;
} EffectTraceFxEntry;
void menuClose(u32);
extern EffectTraceFxEntry lbl_80363B88[];
int before(void) { return 7; }
#pragma peephole on
""" + NATURAL + """
#pragma peephole on
int after(void) { return 9; }
"""


class OwnerSourceTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        discovered = shutil.which("clang")
        if not discovered:
            raise unittest.SkipTest("Clang is unavailable")
        cls.clang = Path(discovered).resolve()

    def translation_unit(self, definition: str) -> str:
        start, end = owner_source._definition_span(CONTEXT, "msgctrlWait")
        return CONTEXT[:start] + definition + CONTEXT[end:]

    def assert_rejected(self, definition: str, pattern: str = "rejected") -> None:
        candidate = self.translation_unit(definition)
        with self.assertRaisesRegex(owner_source.OwnerSourceError, pattern):
            owner_source.validate_candidate_translation_unit(
                CONTEXT,
                candidate,
                "msgctrlWait",
                parser=self.clang,
            )

    def test_natural_c_and_reviewed_register_cast_variant_are_accepted(self) -> None:
        owner_source.validate_candidate_translation_unit(
            CONTEXT,
            CONTEXT,
            "msgctrlWait",
            parser=self.clang,
        )
        reviewed_variant = """s32 msgctrlWait(EffectUtilCommandObj* obj) {
            u8* cursor;
            s32 count;
            if (obj->activeFlag == 0) {
                if (obj->waitCounter == 0) {
                    cursor = obj->stream;
                    obj->waitCounter = cursor[0] + 1;
                }
                count = obj->waitCounter;
                count = count - 1;
                obj->waitCounter = count;
                if (count <= 0) {
                    obj->waitCounter = 0;
                } else {
                    cursor = obj->stream;
                    obj->stream = cursor - 3;
                    return 1;
                }
            }
            cursor = obj->stream;
            obj->stream = cursor + 1;
            return 0;
        }"""
        owner_source.validate_candidate_translation_unit(
            CONTEXT,
            self.translation_unit(reviewed_variant),
            "msgctrlWait",
            parser=self.clang,
        )

    def test_msgctrlwait_normal_form_rejects_structural_shapers(self) -> None:
        candidates = {
            "commuted outer condition": NATURAL.replace(
                "obj->activeFlag == 0", "0 == obj->activeFlag", 1
            ),
            "nested affine decrement": NATURAL.replace(
                "counter - 1", "counter + 1 - 2", 1
            ),
            "zero pointer delta": NATURAL.replace("stream + 1", "stream + 0", 1),
            "commuted array subscript": NATURAL.replace("stream[0]", "0[stream]", 1),
            "integer suffix": NATURAL.replace("stream + 1", "stream + 1u", 1),
            "redundant signed-short cast": NATURAL.replace(
                "counter = obj->waitCounter;",
                "counter = (s16)obj->waitCounter;",
                1,
            ),
            "extra dead branch": NATURAL.replace(
                "stream = obj->stream;\n    obj->stream = stream + 1;",
                "if (obj->activeFlag == 0) { }\n"
                "    stream = obj->stream;\n    obj->stream = stream + 1;",
                1,
            ),
            "aggregate local": NATURAL.replace(
                "u8* stream;",
                "EffectUtilCommandObj copy = *obj;\n    u8* stream;",
                1,
            ),
        }
        for label, definition in candidates.items():
            with self.subTest(label=label):
                self.assert_rejected(definition, "normal form")

    def test_intrinsic_pragma_translation_and_layout_tricks_reject(self) -> None:
        bodies = {
            "rlwimi": "return __rlwimi(1, 2, 3, 4);",
            "cntlzw": "return __cntlzw(1);",
            "sync": "__sync(); return 0;",
            "option": "return __option(1);",
            "unknown intrinsic": "return __anything(1);",
            "pragma operator": '_Pragma("peephole off") return 0;',
            "hash pragma": "\n#pragma peephole off\nreturn 0;",
            "digraph pragma": "\n%:pragma peephole off\nreturn 0;",
            "brace digraph": "<% return 0; %> return 0;",
            "trigraph pragma": "\n??=pragma peephole off\nreturn 0;",
            "line splice": "go\\\nto done; done: return 0;",
            "asm": 'asm("nop"); return 0;',
            "goto": "goto done; done: return 0;",
            "restrict": "u8 * restrict p = obj->stream; return p[0];",
        }
        for label, body in bodies.items():
            with self.subTest(label=label):
                self.assert_rejected(
                    f"s32 msgctrlWait(EffectUtilCommandObj* obj) {{ {body} }}"
                )

    def test_local_type_comma_alias_and_unknown_call_tricks_reject(self) -> None:
        bodies = {
            "typedef": "typedef s32 T; T value = 0; return value;",
            "named struct": "struct S { s32 x; }; struct S value = {0}; return value.x;",
            "anonymous struct": "struct { s32 x; } value = {0}; return value.x;",
            "union": "union U { s32 x; u32 y; } value; value.x = 0; return value.x;",
            "enum": "enum E { E_ZERO }; return E_ZERO;",
            "comma": "s32 value = (obj->activeFlag, 1); return value;",
            "pointer integer": "return ((u8*)(u32)obj)[0];",
            "pointer bitcast": "void *p = obj->stream; u32 *q = p; return q[0];",
            "address": "u8 **p = &obj->stream; return (*p)[0];",
            "unknown call": "return totallyUnknown(1);",
        }
        for label, body in bodies.items():
            with self.subTest(label=label):
                self.assert_rejected(
                    f"s32 msgctrlWait(EffectUtilCommandObj* obj) {{ {body} }}"
                )

    def test_uninitialized_mask_shift_and_self_assignment_tricks_reject(self) -> None:
        bodies = {
            "direct uninitialized": "s16 x; return x;",
            "masked uninitialized": "s16 x; return x & 0xFF;",
            "short circuit uninitialized": "s16 x; return 0 && x;",
            "conditional uninitialized": "s16 x; return obj ? 0 : x;",
            "initialized mask": "s16 x = obj->waitCounter; x = x & 0; return x;",
            "logical-and-zero": "return obj->waitCounter && 0;",
            "logical-or-one": "return obj->waitCounter || 1;",
            "modulo-one": "return obj->waitCounter % 1;",
            "comparison-times-zero": (
                "return (obj->waitCounter == 0) * 0;"
            ),
            "nonlinear cancellation": (
                "return obj->waitCounter * obj->waitCounter - "
                "obj->waitCounter * obj->waitCounter;"
            ),
            "shift": "return obj->waitCounter << 1;",
            "member self": "obj->waitCounter = (obj->waitCounter); return 0;",
            "cast member self": "obj->waitCounter = (s16)obj->waitCounter; return 0;",
            "plus zero member self": "obj->waitCounter = obj->waitCounter + 0; return 0;",
            "zero plus member self": "obj->waitCounter = 0 + obj->waitCounter; return 0;",
            "minus zero member self": "obj->waitCounter = obj->waitCounter - 0; return 0;",
            "times one member self": "obj->waitCounter = obj->waitCounter * 1; return 0;",
            "divide one member self": "obj->waitCounter = obj->waitCounter / 1; return 0;",
            "double negate member self": (
                "obj->waitCounter = -(-obj->waitCounter); return 0;"
            ),
            "canceling arithmetic member self": (
                "obj->waitCounter = (obj->waitCounter + 1) - 1; return 0;"
            ),
            "wrapping arithmetic member self": (
                "obj->waitCounter = obj->waitCounter + 65536; return 0;"
            ),
            "casted wrapping constant member self": (
                "obj->waitCounter = obj->waitCounter + (u8)256; return 0;"
            ),
            "conditional member self": (
                "obj->waitCounter = obj->activeFlag ? "
                "obj->waitCounter : obj->waitCounter; return 0;"
            ),
            "zero subscript member alias": (
                "obj[0].waitCounter = obj->waitCounter; return 0;"
            ),
            "zero pointer arithmetic member alias": (
                "(obj + 0)->waitCounter = obj->waitCounter; return 0;"
            ),
            "compound plus zero": "obj->waitCounter += 0; return 0;",
            "compound nonzero": "obj->waitCounter += 1; return 0;",
            "array self": "u8 *p = obj->stream; p[0] = p[0]; return 0;",
            "commuted array self": (
                "u8 *p = obj->stream; p[0] = 0[p]; return 0;"
            ),
            "variable-index array self": (
                "u8 *p = obj->stream; s16 i = obj->waitCounter; "
                "p[i] = p[i]; return 0;"
            ),
            "direct variable-index array self": (
                "obj->stream[obj->waitCounter] = "
                "obj->stream[obj->waitCounter]; return 0;"
            ),
            "variable-index plus-zero alias": (
                "u8 *p = obj->stream; s16 i = obj->waitCounter; "
                "p[i + 0] = p[i]; return 0;"
            ),
            "cancelled-index dereference alias": (
                "u8 *p = obj->stream; s16 i = obj->waitCounter; "
                "p[0] = *(p + i - i); return 0;"
            ),
            "wrapping pointer self": (
                "u8 *p = obj->stream; p = p + 4294967296ULL; return p[0];"
            ),
            "deref self": "u8 *p = obj->stream; *p = *p; return 0;",
            "save write restore": (
                "s16 saved = obj->waitCounter; obj->waitCounter = 7; "
                "obj->waitCounter = saved; return 0;"
            ),
            "transitive save write restore": (
                "s16 saved = obj->waitCounter; s16 copy = saved; "
                "obj->waitCounter = 7; obj->waitCounter = copy; return 0;"
            ),
            "transformed save write restore": (
                "s16 saved = obj->waitCounter; saved = saved + 1; "
                "saved = saved - 1; obj->waitCounter = saved; return 0;"
            ),
            "branch-laundered save write restore": (
                "s16 saved = obj->waitCounter; "
                "if (obj->activeFlag) { saved = saved + 1; } "
                "else { saved = saved - 1; } "
                "if (obj->activeFlag) { saved = saved - 1; } "
                "else { saved = saved + 1; } "
                "obj->waitCounter = saved; return 0;"
            ),
            "unused initialized local": "s16 padding = 7; return 0;",
            "unused uninitialized local": "s16 padding; return 0;",
            "fake use expression": "s16 padding = 7; padding; return 0;",
            "cancelled local use": "s16 padding = 7; return padding - padding;",
            "constant-only local use": "s16 padding = 7; return padding;",
            "constant-only local comparison": (
                "s16 padding = 7; return padding < 8;"
            ),
            "non-affine constant-only local comparison": (
                "s16 padding = (7 < 8); return padding;"
            ),
            "logical constant-only local": "s16 padding = !0; return padding;",
            "float-cast constant-only local": (
                "s16 padding = (s16)7.0f; return padding;"
            ),
            "same-value local comparison": (
                "s16 padding = 7; return padding == padding;"
            ),
            "local-copy equality laundering": (
                "s16 value = obj->waitCounter; "
                "return value == obj->waitCounter;"
            ),
            "local-copy ordering laundering": (
                "s16 value = obj->waitCounter; "
                "return value < obj->waitCounter;"
            ),
            "constant affine comparison": (
                "return obj->waitCounter < obj->waitCounter + 1;"
            ),
            "offset affine comparison": (
                "return obj->waitCounter + 1 == obj->waitCounter + 2;"
            ),
            "local-copy subtraction laundering": (
                "s16 value = obj->waitCounter; "
                "return value - obj->waitCounter;"
            ),
            "identical conditional arms": (
                "s16 padding = obj->waitCounter; return obj->activeFlag ? "
                "padding : padding;"
            ),
            "alias-identical conditional arms": (
                "s16 value = obj->waitCounter; return obj->activeFlag ? "
                "value : obj->waitCounter;"
            ),
            "constant conditional condition": (
                "return 1 ? obj->waitCounter : 0;"
            ),
            "identical return branches": (
                "if (obj->activeFlag) { return 0; } else { return 0; }"
            ),
            "constant if condition": "if (1) { return obj->waitCounter; } return 0;",
            "redundant local reload": (
                "s16 value = obj->waitCounter; value = obj->waitCounter; "
                "return value;"
            ),
            "duplicate memory write": (
                "obj->waitCounter = 7; obj->waitCounter = 7; return 0;"
            ),
            "dead overwritten memory write": (
                "obj->waitCounter = 1; obj->waitCounter = 2; return 0;"
            ),
            "branch dead writes": (
                "if (obj->activeFlag) { obj->waitCounter = 1; } "
                "else { obj->waitCounter = 2; } "
                "obj->waitCounter = 3; return 0;"
            ),
            "duplicate non-affine memory write": (
                "obj->waitCounter = (obj->activeFlag == 0); "
                "obj->waitCounter = (obj->activeFlag == 0); return 0;"
            ),
            "redundant non-affine local reload": (
                "s16 value = (obj->activeFlag == 0); "
                "value = (obj->activeFlag == 0); return value;"
            ),
            "inverse memory updates": (
                "obj->waitCounter = obj->waitCounter + 1; "
                "obj->waitCounter = obj->waitCounter - 1; return 0;"
            ),
            "inverse pointer updates": (
                "obj->stream = obj->stream + 1; "
                "obj->stream = obj->stream - 1; return 0;"
            ),
            "paired same-condition branch inverse updates": (
                "if (obj->activeFlag) { "
                "obj->waitCounter = obj->waitCounter + 1; } "
                "if (obj->activeFlag) { "
                "obj->waitCounter = obj->waitCounter - 1; } return 0;"
            ),
            "equivalent-condition branch inverse updates": (
                "if (obj->activeFlag) { "
                "obj->waitCounter = obj->waitCounter + 1; } "
                "if (obj->activeFlag != 0) { "
                "obj->waitCounter = obj->waitCounter - 1; } return 0;"
            ),
            "commuted-condition branch inverse updates": (
                "if (obj->activeFlag) { "
                "obj->waitCounter = obj->waitCounter + 1; } "
                "if (0 != obj->activeFlag) { "
                "obj->waitCounter = obj->waitCounter - 1; } return 0;"
            ),
            "alias-condition branch inverse updates": (
                "u8 value = obj->activeFlag; "
                "if (obj->activeFlag) { "
                "obj->waitCounter = obj->waitCounter + 1; } "
                "if (value) { obj->waitCounter = obj->waitCounter - 1; } "
                "return 0;"
            ),
            "by-value parameter write": "obj = obj + 1; return 0;",
            "branch by-value parameter write": (
                "if (obj->activeFlag) { obj = obj + 1; } return 0;"
            ),
            "function decay pointer local": (
                "void (*value)(u32) = menuClose; return value != 0;"
            ),
            "array decay pointer local": (
                "EffectTraceFxEntry* value = lbl_80363B88; return value != 0;"
            ),
        }
        for label, body in bodies.items():
            with self.subTest(label=label):
                self.assert_rejected(
                    f"s32 msgctrlWait(EffectUtilCommandObj* obj) {{ {body} }}"
                )

    def test_context_equality_and_explicit_parser_fail_closed(self) -> None:
        changed_before = CONTEXT.replace("return 7", "return 8")
        with self.assertRaisesRegex(owner_source.OwnerSourceError, "before the target"):
            owner_source.validate_candidate_translation_unit(
                CONTEXT, changed_before, "msgctrlWait", parser=self.clang
            )
        changed_after = CONTEXT.replace("return 9", "return 10")
        with self.assertRaisesRegex(owner_source.OwnerSourceError, "after the target"):
            owner_source.validate_candidate_translation_unit(
                CONTEXT, changed_after, "msgctrlWait", parser=self.clang
            )
        with self.assertRaisesRegex(owner_source.OwnerSourceError, "explicitly attested"):
            owner_source.validate_owner_target(NATURAL, "msgctrlWait")

    def test_cli_enforces_the_same_policy(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            base = root / "base.c"
            candidate = root / "candidate.c"
            base.write_text(CONTEXT, encoding="utf-8")
            candidate.write_text(
                self.translation_unit(
                    "s32 msgctrlWait(EffectUtilCommandObj* obj) { "
                    "s16 x; return x; }"
                ),
                encoding="utf-8",
            )
            result = subprocess.run(
                [
                    sys.executable,
                    "-I",
                    str(MODULE_PATH),
                    "--base-tu",
                    str(base),
                    "--candidate-tu",
                    str(candidate),
                    "--function",
                    "msgctrlWait",
                    "--parser",
                    str(self.clang),
                ],
                capture_output=True,
                text=True,
                env={"LANG": "C", "LC_ALL": "C", "PATH": "/usr/bin:/bin"},
            )
            self.assertEqual(result.returncode, 2)
            self.assertIn("normal form", result.stderr)


if __name__ == "__main__":
    unittest.main()
