#!/usr/bin/env bash
set -exo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR

PRE_COMMANDS="cd $MOUNTED_DIR/build"

if [[ $(uname) == 'Darwin' ]]; then

    # You can't use chained commands in execute
    cd $BUILD_DIR
    DEBUG_COMMAND="$BUILD_DIR/bin/eosio-cpp --version"
    echo "$ $DEBUG_COMMAND"
    eval $DEBUG_COMMAND
    TEST="./tools/toolchain-tester/toolchain-tester --cdt ./bin ../tests/toolchain/"
    echo "$ $TEST"
    bash -c "$TEST"
    
else # Linux

    ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}

    . $HELPERS_DIR/docker-hash.sh

    SET_PYTHON_PATH="export CICD_PYTHON_PATH=/usr/local/bin/python3"
    TEST="./tools/toolchain-tester/toolchain-tester ../tests/toolchain/"
    COMMANDS="$PRE_COMMANDS && $SET_PYTHON_PATH && $TEST"

    # Load BUILDKITE Environment Variables for use in docker run
    if [[ -f $BUILDKITE_ENV_FILE ]]; then
        evars=""
        while read -r var; do
            evars="$evars --env ${var%%=*}"
        done < "$BUILDKITE_ENV_FILE"
    fi
    DOCKER_COMMAND="docker run $ARGS $evars $FULL_TAG bash -c \"$COMMANDS\""
    echo "$ $DOCKER_COMMAND"
    eval $DOCKER_COMMAND
fi
