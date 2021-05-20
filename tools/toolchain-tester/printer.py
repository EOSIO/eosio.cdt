# Prevents an import at runtime so there's no cyclical dependency
from __future__ import annotations
from typing import TYPE_CHECKING
from typing import List, Optional, Tuple
from errors import TestFailure

if TYPE_CHECKING:
    from tests import Test

COLORS = {
    "black": 0,
    "red": 1,
    "green": 2,
    "yellow": 3,
    "blue": 4,
    "magenta": 5,
    "cyan": 6,
    "white": 7,
}

STYLES = {"reset": 0, "bold": 1, "italic": 3, "underline": 4}


def print_test_results(
    results: List[Tuple[Test, Optional[TestFailure]]], run_time: float
) -> List[TestFailure]:
    Printer.print("\n========= Results =========")

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
            Printer.red("Failure: ", newline=False)
            Printer.print(f"{f.failing_test.fullname} failed with message: ")
            Printer.red(f"\t{f}")
        Printer.print()

        for s in successes:
            Printer.green("Success: ", newline=False)
            Printer.print(f"{s.fullname}")

        num_failures = len(failures)
        pct = 100 - (100 * num_failures / total_tests)
        Printer.yellow(
            f"\n{pct:.0f}% of tests passed, {num_failures} tests failed out of {total_tests}"
        )
    else:
        Printer.green(f"\n100% of tests passed, 0 tests failed out of {total_tests}")

    Printer.print(f"\nTotal Test discovery and run time = {run_time:.2f} sec")

    return failures


def print_test_results_machine(
    _results: List[Tuple[Test, Optional[TestFailure]]], _run_time: float
) -> None:
    Printer.red("TODO")
    return []


class Printer:
    verbose = False

    @staticmethod
    def print(*text, verbose=False, newline=True):
        end = "\n" if newline else ""

        if not verbose:
            print(*text, end=end)

        if verbose and Printer.verbose:
            print(*text, end=end)

    @staticmethod
    def red(*text, verbose=False, newline=True):
        if not verbose:
            print_decorated(*text, fcolor="red", newline=newline)

        if verbose and Printer.verbose:
            print_decorated(*text, fcolor="red", newline=newline)

    @staticmethod
    def green(*text, verbose=False, newline=True):
        if not verbose:
            print_decorated(*text, fcolor="green", newline=newline)

        if verbose and Printer.verbose:
            print_decorated(*text, fcolor="green", newline=newline)

    @staticmethod
    def yellow(*text, verbose=False, newline=True):
        if not verbose:
            print_decorated(*text, fcolor="yellow", newline=newline)

        if verbose and Printer.verbose:
            print_decorated(*text, fcolor="yellow", newline=newline)


def print_decorated(*text, style=None, fcolor=None, bcolor=None, newline=True):
    text_format = parse(style, fcolor, bcolor)
    end = "\n" if newline else ""
    print(f"\033[{text_format}m", end="")
    print(*text, end="")
    print("\033[0m", end=end)


def parse(style="", fcolor="", bcolor=""):
    attr = [] if not style else [STYLES[style]]

    if fcolor:
        attr.append(30 + COLORS[fcolor])

    if bcolor:
        attr.append(40 + COLORS[bcolor])

    return ";".join([str(x) for x in attr])
