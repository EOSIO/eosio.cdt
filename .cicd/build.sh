#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR

if [[ $(uname) == 'Darwin' ]]; then

    # You can't use chained commands in execute
    cd $BUILD_DIR
    cmake ..
    make -j$JOBS

else # Linux

    ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}

    . $HELPERS_DIR/docker-hash.sh

    PRE_COMMANDS="rm -f $MOUNTED_DIR/modules/ClangExternalProject.txt && ln -s $MOUNTED_DIR/.cicd/helpers/ClangExternalProject.txt /workdir/modules/ClangExternalProject.txt && cd $MOUNTED_DIR/build"
    # PRE_COMMANDS: Executed pre-cmake

    BUILD_COMMANDS="cmake .. && make -j$JOBS"

    # Docker Commands
    if [[ $BUILDKITE == true ]]; then
        # Generate Base Images
        $CICD_DIR/generate-base-images.sh
        COMMANDS="$BUILD_COMMANDS"
    elif [[ $TRAVIS == true ]]; then
        ARGS="$ARGS -e JOBS -e CCACHE_DIR=/opt/.ccache"
        COMMANDS="$BUILD_COMMANDS"
    fi

    COMMANDS="$PRE_COMMANDS && $COMMANDS"

    # Load BUILDKITE Environment Variables for use in docker run
    if [[ -f $BUILDKITE_ENV_FILE ]]; then
        evars=""
        while read -r var; do
            evars="$evars --env ${var%%=*}"
        done < "$BUILDKITE_ENV_FILE"
    fi

    eval docker run $ARGS $evars $FULL_TAG bash -c \"$COMMANDS\"

fi