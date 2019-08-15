#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR

PRE_COMMANDS="rm -f $MOUNTED_DIR/modules/ClangExternalProject.txt && ln -s $MOUNTED_DIR/.cicd/helpers/ClangExternalProject.txt $MOUNTED_DIR/modules/ClangExternalProject.txt && ln -s /opt/eosio.cdt/build/eosio_llvm/ $MOUNTED_DIR/build/eosio_llvm && cd $MOUNTED_DIR/build"
TEST="ctest -j$JOBS -L unit_tests -V -T Test"
COMMANDS="$PRE_COMMANDS && $TEST"

if [[ $(uname) == 'Darwin' ]]; then

    # You can't use chained commands in execute
    bash -c "$COMMANDS"
    
else # Linux

    ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}

    . $HELPERS_DIR/docker-hash.sh

    [[ $TRAVIS == true ]] && ARGS="$ARGS -e JOBS -e CCACHE_DIR=/opt/.ccache"

    # Load BUILDKITE Environment Variables for use in docker run
    if [[ -f $BUILDKITE_ENV_FILE ]]; then
        evars=""
        while read -r var; do
            evars="$evars --env ${var%%=*}"
        done < "$BUILDKITE_ENV_FILE"
    fi

    eval docker run $ARGS $evars $FULL_TAG bash -c \"$COMMANDS\"

fi