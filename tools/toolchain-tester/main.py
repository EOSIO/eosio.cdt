import argparse
import os
import tempfile

from pathlib import Path
from timeit import default_timer as timer
from typing import List

from printer import Printer as P
from printer import print_test_results, print_test_results_machine

from settings import Config
from testrunner import TestRunner
from testsuite import TestSuite


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "test_directory", help="The directory where the toolchain tests are located."
    )

    parser.add_argument(
        "-v", "--verbose", action="store_true", help="Print verbose output"
    )
    parser.add_argument(
        "-j",
        "--jobs",
        type=int,
        default=int(os.cpu_count() / 2),
        help="Number of threads to use for parallel execution (defaults to half of the system max)",
    )
    parser.add_argument(
        "-t", "--tests", default="all", help="Test/Testsuite to run (defaults to all)"
    )
    parser.add_argument(
        "--format",
        choices=("human", "xunit"),
        default="human",
        help="Format of the test output (defaults to human)",
    )
    parser.add_argument(
        "--cdt",
        default=get_cdt_path(),
        help="Path to CDT (defaults to built CDT in this repo)",
    )

    args = parser.parse_args()

    Config.cdt_path = args.cdt
    P.verbose = args.verbose

    abs_test_directory = os.path.abspath(args.test_directory)

    temp_dir = tempfile.mkdtemp()

    P.print(f"Temp files will be written to {temp_dir}...", verbose=True)

    os.chdir(temp_dir)

    test_directories: List[str] = []

    for f in os.listdir(abs_test_directory):
        abs_f = os.path.join(abs_test_directory, f)

        if os.path.isdir(abs_f):
            test_directories.append(abs_f)

    test_suites = list(map(lambda d: TestSuite(d), test_directories))

    start = timer()
    test_runner = TestRunner(test_suites, args.tests, args.jobs)
    test_results = test_runner.run_tests()
    end = timer()

    if args.format == "human":
        print_test_results(test_results, end - start)
    else:
        print_test_results_machine(test_results, end - start)


def get_cdt_path() -> str:
    return os.path.join(
        Path(os.path.realpath(__file__)).parent.parent.parent, "build", "bin"
    )


if __name__ == "__main__":
    main()
