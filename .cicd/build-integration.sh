#!/usr/bin/env bash
set -exo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR

ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}

EOSIO_PATH="/root/eosio/build"

# PRE_COMMANDS: Executed pre-cmake
PRE_COMMANDS="cd $MOUNTED_DIR/build"
BUILD_COMMANDS="cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=${EOSIO_PATH} -DEOSIO_RUN_INTEGRATION_TESTS=1 .. && make -j$JOBS"

# Docker Commands

COMMANDS="$PRE_COMMANDS && $BUILD_COMMANDS"

# Load BUILDKITE Environment Variables for use in docker run
if [[ -f $BUILDKITE_ENV_FILE ]]; then
    evars=""
    while read -r var; do
        evars="$evars --env ${var%%=*}"
    done < "$BUILDKITE_ENV_FILE"
fi

eval docker run $ARGS $evars $IMAGE_TAG bash -c \"$COMMANDS\"
