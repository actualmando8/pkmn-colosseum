#!/usr/bin/env python3
import re
import unittest
from pathlib import Path


HERE = Path(__file__).resolve().parent
GENERATORS = (HERE / "build_dir.py", HERE.parent / "gen_workunits.py")
UNSAFE = {
    "perm_dummy_comma_expr",
    "perm_pad_var_decl",
    "perm_duplicate_assignment",
    "perm_add_self_assignment",
    "perm_refer_to_var",
}


class HonestGeneratorWeightsTest(unittest.TestCase):
    def test_generators_disable_source_invalid_mutations(self):
        for generator in GENERATORS:
            text = generator.read_text()
            with self.subTest(generator=generator.name):
                for key in UNSAFE:
                    match = re.search(rf"{re.escape(key)}\s*=\s*(\d+)", text)
                    self.assertIsNotNone(match, key)
                    self.assertEqual(match.group(1), "0", key)


if __name__ == "__main__":
    unittest.main()
