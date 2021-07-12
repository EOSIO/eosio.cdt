from enum import Enum
import json
import os
from typing import List

from errors import MissingCppError, MissingJsonError
import tests


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


class TestSuite:
    """
    This class represents a collection of test files. The collection is determined
    by the directory structure.
    """

    def __init__(self, directory: str, cdt_path: str):
        self.directory = directory
        self.cdt_path = cdt_path
        self.tests: List[tests.Test] = []
        self.name = self._get_name()
        self.test_type = self._get_test_type()

        test_files = []

        for f in os.listdir(self.directory):
            abs_f = os.path.join(self.directory, f)

            file_name = abs_f.split("/")[-1]
            name = file_name.split(".")[0]

            if ".cpp" in file_name:
                if not os.path.isfile(os.path.join(self.directory, f"{name}.json")):
                    raise MissingJsonError(f"{file_name} is missing the test json file")

            if ".json" in file_name:
                if not os.path.isfile(os.path.join(self.directory, f"{name}.cpp")):
                    raise MissingCppError(f"{file_name} is missing the test cpp file")
                test_files.append(abs_f)

        for tf in test_files:
            with open(tf) as jf:
                test_json = json.load(jf)

            name = tf.split("/")[-1].split(".")[0]
            for i, t in enumerate(test_json["tests"]):
                cpp_file = os.path.join(self.directory, f"{name}.cpp")

                args = [cpp_file, t, i, self]

                if self.test_type == TestType.BUILD_PASS:
                    self.tests.append(tests.BuildPassTest(*args))
                elif self.test_type == TestType.BUILD_FAIL:
                    self.tests.append(tests.BuildPassTest(*args))
                elif self.test_type == TestType.COMPILE_PASS:
                    self.tests.append(tests.CompilePassTest(*args))
                elif self.test_type == TestType.COMPILE_FAIL:
                    self.tests.append(tests.CompileFailTest(*args))
                elif self.test_type == TestType.ABIGEN_PASS:
                    self.tests.append(tests.AbigenPassTest(*args))
                elif self.test_type == TestType.ABIGEN_FAIL:
                    self.tests.append(tests.AbigenFailTest(*args))

    def _get_test_type(self) -> TestType:
        return TestType.from_str(self.directory.split("/")[-1])

    def _get_name(self) -> str:
        return self.directory.split("/")[-1]

    def __repr__(self) -> str:
        return self.__str__()

    def __str__(self) -> str:
        s = self.name
        s += ": "
        first = True

        for t in self.tests:
            if first:
                first = False
            else:
                s += ","

            s += str(t)

        return s
