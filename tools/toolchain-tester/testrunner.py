from multiprocessing import Pool
from typing import Dict, List, NamedTuple, Optional

from printer import Printer as P

from errors import TestFailure
from testsuite import TestSuite
from tests import Test


class TestResult(NamedTuple):
    test: Test
    failure: Optional[TestFailure]


class TestRunner:
    def __init__(
        self, test_suites: List[TestSuite], name_of_tests_to_run: str, num_jobs: int
    ):
        self.test_suites: List[TestSuite] = test_suites
        self.name_of_tests_to_run = name_of_tests_to_run
        self.tests_to_run: List[Test] = self.get_tests_to_run()
        self.num_jobs: int = num_jobs

        self.test_map: Dict[str, Test] = {}
        self.test_suites_map: Dict[str, TestSuite] = {}

    def get_tests_to_run(self):
        tests_to_run: List[Test] = []

        if self.name_of_tests_to_run == "all":
            tests_to_run = [t for ts in self.test_suites for t in ts.tests]
        elif "/" in self.name_of_tests_to_run:
            # We're running a test
            self.build_test_map()
            tests_to_run.append(self.test_map[self.name_of_tests_to_run])
        else:
            # We're running a test suite
            self.build_test_suite_map()
            ts = self.test_suites_map[self.name_of_tests_to_run]
            tests_to_run = ts.tests

        return tests_to_run

    def run_tests(self) -> List[TestResult]:
        with Pool(self.num_jobs) as p:
            results_list = p.map(self.run_test, self.tests_to_run)

        return results_list

    def run_test(self, t: Test) -> TestResult:
        try:
            P.print(f"Running {t.fullname}")
            t.run()
        except TestFailure as f:
            return TestResult(t, f)

        return TestResult(t, None)

    def build_test_suite_map(self):
        tst_suites_map = {}

        for ts in self.test_suites:
            tst_suites_map[ts.name] = ts

        self.test_suites_map = tst_suites_map

    def build_test_map(self):
        test_map = {}

        for ts in self.test_suites:
            for t in ts.tests:
                test_map[t.name] = t

        self.test_map = test_map
