import argparse
import os
import tempfile

from multiprocessing import Pool
from pathlib import Path
from timeit import default_timer as timer

from typing import Dict, List, Optional, Tuple

from config import Config, TestFailure
from printer import Printer as P
from testsuite import TestSuite
from tests import Test


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "test_directory", help="The directory where the integration tests are located."
    )

    parser.add_argument(
        "-v", "--verbose", action="store_true", help="Print verbose output"
    )
    parser.add_argument(
        "-j",
        "--jobs",
        type=int,
        default=int(os.cpu_count() / 2),
        help="Number of threads to use for parallel execution",
    )
    parser.add_argument("-t", "--tests", default="all", help="Test/Testsuite to run")
    parser.add_argument(
        "--format",
        choices=("human", "xunit"),
        default="human",
        help="Format of the test output",
    )
    parser.add_argument(
        "--cdt",
        default=_get_cdt_path(),
        help="Path to CDT (defaults to built CDT in this repo)",
    )

    args = parser.parse_args()

    Config.cdt_path = args.cdt
    Config.verbose = args.verbose
    P.verbose = args.verbose

    abs_test_directory = os.path.abspath(args.test_directory)

    temp_dir = tempfile.mkdtemp()

    P.print(f"Temp files will be written to {temp_dir}...", verbose=True)

    os.chdir(temp_dir)

    test_directories: List[str] = []

    start = timer()

    for f in os.listdir(abs_test_directory):
        abs_f = os.path.join(abs_test_directory, f)

        if os.path.isdir(abs_f):
            test_directories.append(abs_f)

    test_suites = list(map(lambda d: TestSuite(d), test_directories))

    tests_to_run: List[Test] = []

    if args.tests == "all":
        tests_to_run = [t for ts in test_suites for t in ts.tests]
    elif "/" in args.tests:
        # We're running a test
        tm = build_test_map(test_suites)
        tests_to_run.append(tm[args.tests])
    else:
        # We're running a test suite
        tsm = build_test_suite_map(test_suites)
        ts = tsm[args.tests]
        tests_to_run = [t for t in ts.tests]

    with Pool(args.jobs) as p:
        results_list = p.map(run_test, tests_to_run)

    end = timer()
    if args.format == "human":
        print_test_results(results_list, end - start)
    else:
        print_test_results_machine(results_list, end - start)


def run_test(t: Test) -> Tuple[Test, Optional[TestFailure]]:
    try:
        P.print(f"Running {t.fullname}")
        t.run()
    except TestFailure as f:
        return (t, f)

    return (t, None)


def print_test_results(
    results: List[Tuple[Test, Optional[TestFailure]]], run_time: float
) -> None:
    P.print("\n========= Results =========")

    failures = []
    successes = []

    total_tests = len(results)

    for t, r in results:
        if r is not None:
            failures.append(r)
        else:
            successes.append(t)

    if failures:
        for f in failures:
            P.red("Failure: ", newline=False)
            P.print(f"{f.failing_test.fullname} failed with message: ", newline=False)
            P.red(f"{f}")
        P.print()

        for s in successes:
            P.green("Success: ", newline=False)
            P.print(f"{s.fullname}")

        num_failures = len(failures)
        pct = 100 - (100 * num_failures / total_tests)
        P.yellow(
            f"\n{pct:.0f}% of tests passed, {num_failures} tests failed out of {total_tests}"
        )
    else:
        P.green("\n100% of tests passed, 0 tests failed out of {total_tests}")

    P.print(f"\nTotal Test discovery and run time = {run_time:.2f} sec")

def print_test_results_machine(
    results: List[Tuple[Test, Optional[TestFailure]]], run_time: float
) -> None:
    P.red("TODO")


def build_test_suite_map(test_suites: List[TestSuite]) -> Dict[str, TestSuite]:
    test_suites_map = {}

    for ts in test_suites:
        test_suites_map[ts.name] = ts

    return test_suites_map


def build_test_map(test_suites: List[TestSuite]) -> Dict[str, Test]:
    test_map = {}

    for ts in test_suites:
        for t in ts.tests:
            test_map[t.name] = t

    return test_map


def _get_cdt_path() -> str:
    return os.path.join(
        Path(os.path.realpath(__file__)).parent.parent.parent, "build", "bin"
    )


if __name__ == "__main__":
    main()
