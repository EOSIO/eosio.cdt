#!/bin/bash
set -eo pipefail
# The purpose of this test is to ensure that the output of the "eosio-cpp --version" command matches the version string defined by our CMake files
echo '##### Eosio-cpp Version Label Test #####'
# orient ourselves
[[ -z "$BUILD_ROOT" ]] && export BUILD_ROOT="$(pwd)"
echo "Using BUILD_ROOT=\"$BUILD_ROOT\"."
# test expectations
if [[ -z "$EXPECTED" ]]; then
    [[ -z "$BUILDKITE_TAG" ]] && export BUILDKITE_TAG="${GIT_TAG:-$1}"
    export EXPECTED="eosio-cpp version $BUILDKITE_TAG"
fi
if [[ -z "$EXPECTED" ]]; then
    echo "Missing version input."
    exit 1
fi
echo "Expecting \"$EXPECTED\"..."
# get eosio-cpp version
ACTUAL=$($BUILD_ROOT/bin/eosio-cpp --version)
EXIT_CODE=$?
# verify 0 exit code explicitly
if [[ $EXIT_CODE -ne 0 ]]; then
    echo "Eosio-cpp produced non-zero exit code \"$EXIT_CODE\"."
    exit $EXIT_CODE
fi
# test version
if [[ "$EXPECTED" == "$ACTUAL" ]]; then
    echo "Passed with \"$ACTUAL\"."
    exit 0
fi
echo 'Failed!'
echo "\"$EXPECTED\" != \"$ACTUAL\""
exit 1
