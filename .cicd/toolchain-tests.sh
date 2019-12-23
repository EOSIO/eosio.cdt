#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR

PRE_COMMANDS="cd $(pwd)/build"
TEST="./tools/toolchain-tester/toolchain-tester ../tests/toolchain/"
TOOLCHAIN_TESTS_COMMANDS="$PRE_COMMANDS && $TEST"

[[ $TRAVIS != true ]] && buildkite-agent artifact download build.tar.gz . --step "$PLATFORM_FULL_NAME - Build"

if [[ $(uname) == 'Darwin' ]]; then

    # You can't use chained commands in execute
    cd $BUILD_DIR
    bash -c "$TEST"
    
else # Linux

    ARGS=${ARGS:-"--rm --init $(buildkite-intrinsics) -v $(pwd):$(pwd)"}
    [[ $TRAVIS == true ]] && ARGS="$ARGS -e JOBS -e CCACHE_DIR=/opt/.ccache" || TOOLCHAIN_TESTS_COMMANDS="cd $(pwd) && tar -xzf build.tar.gz && $TOOLCHAIN_TESTS_COMMANDS"
    . $HELPERS_DIR/populate-template-and-hash.sh -h # obtain $FULL_TAG (and don't overwrite existing file)
    echo "$ docker run $ARGS $evars $FULL_TAG bash -c \"$TOOLCHAIN_TESTS_COMMANDS\""
    eval docker run $ARGS $evars $FULL_TAG bash -c \"$TOOLCHAIN_TESTS_COMMANDS\"

fi
