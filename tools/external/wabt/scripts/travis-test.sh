#!/bin/bash
#
# Copyright 2016 WebAssembly Community Group participants
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set -o nounset
set -o errexit

RTN=0
SCRIPT_DIR="$(cd "$(dirname "$0")"; pwd -P)"
source "${SCRIPT_DIR}/travis-common.sh"

cd ${ROOT_DIR}

log_and_run() {
  echo $*
  if ! $*; then
    echo "travis-test.sh: sub-command failed: $*"
    RTN=1;
  fi
}

run_tests() {
  log_and_run test/run-tests.py -v --bindir ${BINDIR} $* --timeout=10
  log_and_run ${BINDIR}/wabt-unittests
}

set_run_test_args() {
  local COMPILER=$1
  local BUILD_TYPE=$2
  local CONFIG=${3:-}

  BINDIR="out/${COMPILER}/${BUILD_TYPE}/${CONFIG}"
}

for BUILD_TYPE in ${BUILD_TYPES_UPPER}; do
  if [[ -n "${SANITIZER:-}" ]]; then
    if set_run_test_args ${COMPILER} ${BUILD_TYPE} ${SANITIZER}; then
      run_tests
    fi
  else
    if set_run_test_args ${COMPILER} ${BUILD_TYPE}; then
      run_tests
    fi
  fi
done

echo "travis-test.sh done: $RTN"
exit $RTN
