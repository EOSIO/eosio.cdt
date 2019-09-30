from enum import Enum


class Config:
    cdt_path = ""
    verbose = False


class TestType(Enum):
    COMPILE_FAIL = 1
    COMPILE_PASS = 2
    BUILD_FAIL = 3
    BUILD_PASS = 4
    ABIGEN_PASS = 5
    ABIGEN_FAIL = 6

    @staticmethod
    def from_str(s):
        s = s.upper()
        s = s.replace("-", "_")

        return TestType[s]


class TestFailure(Exception):
    def __init__(self, error_msg, failing_test=None):
        super().__init__(error_msg)
        self.failing_test = failing_test


class MissingCppError(Exception):
    pass


class MissingJsonError(Exception):
    pass
