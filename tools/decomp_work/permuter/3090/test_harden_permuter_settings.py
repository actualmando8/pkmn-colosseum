#!/usr/bin/env python3
import subprocess
import tempfile
import unittest
from pathlib import Path


SCRIPT = Path(__file__).with_name("harden_permuter_settings.py")
UNSAFE = {
    "perm_dummy_comma_expr",
    "perm_pad_var_decl",
    "perm_duplicate_assignment",
    "perm_add_self_assignment",
    "perm_refer_to_var",
}


class HardenSettingsTest(unittest.TestCase):
    def test_zeros_only_unsafe_weights_and_is_idempotent(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            unit = root / "fn"
            unit.mkdir()
            settings = unit / "settings.toml"
            settings.write_text(
                "[weight_overrides]\n"
                "perm_reorder_decls = 40\n"
                + "".join(f"{key} = 15\n" for key in sorted(UNSAFE))
            )

            first = subprocess.run(
                ["python3", str(SCRIPT), str(root)], check=True,
                text=True, capture_output=True,
            )
            text = settings.read_text()
            self.assertIn("changed=1", first.stdout)
            self.assertIn("perm_reorder_decls = 40", text)
            for key in UNSAFE:
                self.assertIn(f"{key} = 0", text)

            second = subprocess.run(
                ["python3", str(SCRIPT), str(root)], check=True,
                text=True, capture_output=True,
            )
            self.assertIn("changed=0", second.stdout)


if __name__ == "__main__":
    unittest.main()
