#!/usr/bin/env bash
set -exo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR

ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}

EOSIO_PATH="/root/eosio/build"

# . $HELPERS_DIR/docker-hash.sh

# PRE_COMMANDS: Executed pre-cmake
PRE_COMMANDS="cd $MOUNTED_DIR/build"
BUILD_COMMANDS="cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=${EOSIO_PATH} -DEOSIO_RUN_INTEGRATION_TESTS=1 .. && make -j$JOBS"

# Docker Commands
if [[ $BUILDKITE == true ]]; then
    # Generate Base Images
    $CICD_DIR/generate-base-images.sh
fi

COMMANDS="$PRE_COMMANDS && $BUILD_COMMANDS"

# Load BUILDKITE Environment Variables for use in docker run
if [[ -f $BUILDKITE_ENV_FILE ]]; then
    evars=""
    while read -r var; do
        evars="$evars --env ${var%%=*}"
    done < "$BUILDKITE_ENV_FILE"
fi

eval docker run $ARGS $evars $IMAGE_TAG bash -c \"$COMMANDS\"
