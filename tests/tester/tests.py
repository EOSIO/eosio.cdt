import os
import subprocess
from abc import ABC, abstractmethod
from typing import List

from config import Config, TestFailure
from printer import Printer as P


class Test(ABC):
    """
    This class represents a singular test file.
    """

    def __init__(self, cpp_file, test_json, name, test_suite):
        self.cpp_file = cpp_file
        self.test_json = test_json
        self.test_suite = test_suite
        self.name = name

        self._name = self.cpp_file.split("/")[-1].split(".")[0]
        self.out_wasm = f"{self._name}.wasm"
        self.success = False

    @abstractmethod
    def _run(self, eosio_cpp: str, args: List[str]):
        pass

    def run(self):
        cf = self.test_json.get("compile_flags")
        args = cf if cf else []

        eosio_cpp = os.path.join(Config.cdt_path, "eosio-cpp")
        self._run(eosio_cpp, args)

    def handle_test_result(self, res: subprocess.CompletedProcess, expected_pass=True):
        P.print(res.stdout.decode("utf-8").strip(), verbose=True)
        P.print(res.stderr.decode("utf-8").strip(), verbose=True)

        if expected_pass and res.returncode > 0:
            self.success = False
            raise TestFailure(
                f"{self.name} failed with the following stderr {res.stderr}",
                failing_test=self,
            )

        if not expected_pass and res.returncode == 0:
            self.success = False
            raise TestFailure(
                "expected to fail compilationg/linking but didn't.", failing_test=self
            )

        if not self.test_json.get("expected"):
            self.success = True
        else:
            self.handle_expecteds(res)

    def handle_expecteds(self, res: subprocess.CompletedProcess):
        expected = self.test_json["expected"]

        if expected.get("exit-code"):
            exit_code = expected["exit-code"]

            if res.returncode != exit_code:
                self.success = False
                raise TestFailure(
                    f"expected {exit_code} exit code but got {res.returncode}",
                    failing_test=self,
                )

        if expected.get("stderr"):
            e_stderr = expected["stderr"]
            a_stderr = res.stderr.decode("utf-8")

            if e_stderr not in a_stderr:
                self.success = False
                raise TestFailure(
                    f"expected {e_stderr} stderr but got {a_stderr}", failing_test=self
                )

        if expected.get("abi"):
            e_abi = expected["abi"]
            a_abi_file = f"{self._name}.abi"
            with open(a_abi_file) as f:
                a_abi = f.read()

            if e_abi != a_abi:
                self.success = False
                raise TestFailure(
                    "actual abi did not match expected abi", failing_test=self
                )

        if expected.get("wasm"):
            e_wasm = expected["wasm"]

            xxd = subprocess.Popen(("xxd", "-p", self.out_wasm), stdout=subprocess.PIPE)
            tr = subprocess.check_output(("tr", "-d", "\n"), stdin=xxd.stdout)
            xxd.wait()

            a_wasm = tr.decode("utf-8")

            if e_wasm != a_wasm:
                self.success = False
                raise TestFailure(
                    "actual wasm did not match expected wasm", failing_test=self
                )

        self.success = True

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return self.name


class BuildPassTest(Test):
    def _run(self, eosio_cpp, args):
        command = [eosio_cpp, self.cpp_file, "-o", self.out_wasm]
        command.extend(args)
        res = subprocess.run(command, capture_output=True)
        self.handle_test_result(res)

        return res


class CompilePassTest(Test):
    def _run(self, eosio_cpp, args):
        command = [eosio_cpp, "-c", self.cpp_file, "-o", self.out_wasm]
        command.extend(args)
        res = subprocess.run(command, capture_output=True)
        self.handle_test_result(res)

        return res


class AbigenPassTest(Test):
    def _run(self, eosio_cpp, args):
        command = [eosio_cpp, "-abigen", self.cpp_file, "-o", self.out_wasm]
        command.extend(args)
        res = subprocess.run(command, capture_output=True)
        self.handle_test_result(res)

        return res


class BuildFailTest(Test):
    def _run(self, eosio_cpp, args):
        command = [eosio_cpp, self.cpp_file, "-o", self.out_wasm]
        command.extend(args)
        res = subprocess.run(command, capture_output=True)
        self.handle_test_result(res, expected_pass=False)

        return res


class CompileFailTest(Test):
    def _run(self, eosio_cpp, args):
        command = [eosio_cpp, "-c", self.cpp_file, "-o", self.out_wasm]
        command.extend(args)
        res = subprocess.run(command, capture_output=True)
        self.handle_test_result(res, expected_pass=False)

        return res
