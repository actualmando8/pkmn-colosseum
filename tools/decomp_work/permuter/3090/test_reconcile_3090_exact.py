#!/usr/bin/env python3
import json
import os
import subprocess
import tempfile
import unittest
from pathlib import Path


SCRIPT = Path(__file__).with_name("reconcile_3090_exact.py")


class ReconcileExactTest(unittest.TestCase):
    def test_uses_farm_base_and_defers_active_targets(self):
        with tempfile.TemporaryDirectory() as tmp:
            base = Path(tmp)
            farm = base / "farm"
            state = farm / "state"
            state.mkdir(parents=True)
            queue = farm / "queue.tsv"
            queue.write_text(
                "# tier\tpct\tsize\tname\taddr\tunit\n"
                "1\t99\t4\talpha\t0x1\tmain/a\n"
                "1\t99\t4\tbeta\t0x2\tmain/b\n"
                "1\t99\t4\tgamma\t0x3\tmain/c\n"
                "1\t99\t4\tdelta\t0x4\tmain/d\n"
            )
            (state / "beta.status").write_text("CLAIMED w1 1\n")
            (state / "gamma.status").write_text("NOWIN w2 1 best=10 base=20\n")
            env = dict(os.environ, FARM_BASE=str(base))

            result = subprocess.run(
                ["python3", str(SCRIPT)], input="alpha\nbeta\ngamma\n",
                env=env, check=True, text=True, capture_output=True,
            )
            summary = json.loads(result.stdout)
            self.assertEqual(summary["removed"], ["alpha", "gamma"])
            self.assertEqual(summary["deferred_active"], ["beta"])
            self.assertEqual(summary["remaining"], 2)
            remaining = queue.read_text()
            self.assertNotIn("\talpha\t", remaining)
            self.assertIn("\tbeta\t", remaining)
            self.assertNotIn("\tgamma\t", remaining)
            self.assertIn("\tdelta\t", remaining)


if __name__ == "__main__":
    unittest.main()
