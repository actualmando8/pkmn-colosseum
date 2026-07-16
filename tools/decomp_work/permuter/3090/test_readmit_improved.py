#!/usr/bin/env python3
import json
import os
import subprocess
import tempfile
import unittest
from pathlib import Path


SCRIPT = Path(__file__).with_name("readmit_improved.py")


class ReadmitImprovedTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.base = Path(self.temp.name)
        self.farm = self.base / "farm"
        self.state = self.farm / "state"
        self.state.mkdir(parents=True)
        (self.farm / "queue.tsv").write_text(
            "# tier\tpct\tsize\tname\taddr\tunit\n"
            "1\t99\t4\talpha\t0x1\tmain/a\n"
            "1\t99\t4\tbeta\t0x2\tmain/b\n"
        )

    def tearDown(self):
        self.temp.cleanup()

    def run_readmit(self):
        env = dict(os.environ, FARM_BASE=str(self.base))
        return subprocess.run(
            ["python3", str(SCRIPT)], env=env, check=True,
            text=True, capture_output=True,
        )

    def test_requires_new_record_each_pass(self):
        (self.state / "alpha.status").write_text("NOWIN w1 1 best=50 base=100\n")
        (self.state / "beta.status").write_text("NOWIN w2 1 best=100 base=100\n")
        self.assertEqual(self.run_readmit().stdout.strip(), "1")
        self.assertFalse((self.state / "alpha.status").exists())
        self.assertTrue((self.state / "beta.status").exists())
        self.assertEqual(json.loads((self.state / "retry_best.json").read_text()),
                         {"alpha": 50})

        (self.state / "alpha.status").write_text("NOWIN w1 2 best=60 base=100\n")
        self.assertEqual(self.run_readmit().stdout.strip(), "0")
        self.assertTrue((self.state / "alpha.status").exists())

        (self.state / "alpha.status").write_text("NOWIN w1 3 best=40 base=100\n")
        self.assertEqual(self.run_readmit().stdout.strip(), "1")
        self.assertFalse((self.state / "alpha.status").exists())
        self.assertEqual(json.loads((self.state / "retry_best.json").read_text()),
                         {"alpha": 40})

    def test_ignores_non_numeric_and_nonqueued_states(self):
        (self.state / "alpha.status").write_text("NOWIN w1 1 best=None base=100\n")
        (self.state / "retired.status").write_text("NOWIN w2 1 best=10 base=100\n")
        self.assertEqual(self.run_readmit().stdout.strip(), "0")
        self.assertTrue((self.state / "alpha.status").exists())
        self.assertTrue((self.state / "retired.status").exists())


if __name__ == "__main__":
    unittest.main()
