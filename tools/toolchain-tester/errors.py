class TestFailure(Exception):
    def __init__(self, error_msg, failing_test=None):
        super().__init__(error_msg)
        self.failing_test = failing_test


class MissingCppError(Exception):
    pass


class MissingJsonError(Exception):
    pass
