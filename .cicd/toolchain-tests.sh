#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR

if [[ $(uname) == 'Darwin' ]]; then

    # You can't use chained commands in execute
    cd $BUILD_DIR
    TEST="./tools/toolchain-tester/toolchain-tester ../tests/toolchain/"
    bash -c "$TEST"
    
else # Linux

    ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}

    if [[ "$IMAGE_TAG" == "centos-8" ]]; then
        PRE_COMMANDS="cd $MOUNTED_DIR/build"
        PACKAGE_COMMANDS="yum install -y python38"
        SET_PYTHON_PATH="export CICD_PYTHON_PATH=/usr/bin/python3.8"
        TEST="./tools/toolchain-tester/toolchain-tester ../tests/toolchain/"

        COMMANDS="$PRE_COMMANDS && $PACKAGE_COMMANDS && $SET_PYTHON_PATH && $TEST"
    else
        PRE_COMMANDS="cd $MOUNTED_DIR/build"
        SET_PYTHON_PATH="export CICD_PYTHON_PATH=/usr/local/bin/python3"
        TEST="./tools/toolchain-tester/toolchain-tester ../tests/toolchain/"

        COMMANDS="$PRE_COMMANDS && $SET_PYTHON_PATH && $TEST"
    fi

    . $HELPERS_DIR/docker-hash.sh

    # Load BUILDKITE Environment Variables for use in docker run
    if [[ -f $BUILDKITE_ENV_FILE ]]; then
        evars=""
        while read -r var; do
            evars="$evars --env ${var%%=*}"
        done < "$BUILDKITE_ENV_FILE"
    fi

    eval docker run $ARGS $evars $FULL_TAG bash -c \"$COMMANDS\"

fi
