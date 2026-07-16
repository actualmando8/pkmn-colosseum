#!/usr/bin/env python3
import importlib.util
import unittest
from pathlib import Path


SCRIPT = Path(__file__).with_name("harvest.py")
SPEC = importlib.util.spec_from_file_location("harvest_3090", SCRIPT)
HARVEST = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(HARVEST)


class ExtractFunctionTest(unittest.TestCase):
    def test_extracts_only_complete_target(self):
        source = '''
int before(void) { return 1; }
int target(int x)
{
    const char* brace = "}";
    /* { ignored } */
    if (x) { // } ignored
        x++;
    }
    return x;
}
int after(void) { return 2; }
'''
        got = HARVEST.extract_function(source, "target")
        self.assertTrue(got.startswith("int target(int x)"))
        self.assertIn("return x;", got)
        self.assertNotIn("before", got)
        self.assertNotIn("after", got)

    def test_ignores_declaration_and_call(self):
        source = '''
int target(int x);
int caller(void) { return target(1); }
int target(int x) { return x + 1; }
'''
        self.assertEqual(HARVEST.extract_function(source, "target"),
                         "int target(int x) { return x + 1; }\n")


if __name__ == "__main__":
    unittest.main()
