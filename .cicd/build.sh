#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

export DOCKERIZATION=false
[[ $ENABLE_INSTALL == true ]] && . ./.cicd/helpers/populate-template-and-hash.sh '<!-- DAC BUILD' '<!-- DAC INSTALL' || . ./.cicd/helpers/populate-template-and-hash.sh '<!-- DAC BUILD'

if [[ $(uname) == 'Darwin' ]]; then
    # You can't use chained commands in execute
    if [[ $TRAVIS == true ]]; then
        ccache -s
        brew reinstall openssl@1.1 # Fixes issue where builds in Travis cannot find libcrypto.
        sed -i -e 's/^cmake /cmake -DCMAKE_CXX_COMPILER_LAUNCHER=ccache /g' /tmp/$POPULATED_FILE_NAME
    fi
    . $HELPERS_DIR/populate-template-and-hash.sh -h # obtain $FULL_TAG (and don't overwrite existing file)
    cat /tmp/$POPULATED_FILE_NAME
    . /tmp/$POPULATED_FILE_NAME # This file is populated from the platform's build documentation code block
else # Linux
    if [[ $TRAVIS == true ]]; then
        ARGS=${ARGS:-"-v /usr/lib/ccache -v $HOME/.ccache:/opt/.ccache -e JOBS -e TRAVIS -e CCACHE_DIR=/opt/.ccache"}
        export CONTAINER_NAME=$TRAVIS_JOB_ID
        [[ ! $IMAGE_TAG =~ 'unpinned' ]] && sed -i -e 's/^cmake /cmake -DCMAKE_CXX_COMPILER_LAUNCHER=ccache /g' /tmp/$POPULATED_FILE_NAME
        if [[ $IMAGE_TAG == 'amazon_linux-2-unpinned' ]]; then
            PRE_COMMANDS="export PATH=/usr/lib64/ccache:\\\$PATH"
        elif [[ $IMAGE_TAG == 'centos-7.7-unpinned' ]]; then
            PRE_COMMANDS="export PATH=/usr/lib64/ccache:\\\$PATH"
        elif [[ $IMAGE_TAG == 'ubuntu-16.04-unpinned' ]]; then
            PRE_COMMANDS="export PATH=/usr/lib/ccache:\\\$PATH"
        elif [[ $IMAGE_TAG == 'ubuntu-18.04-unpinned' ]]; then
            PRE_COMMANDS="export PATH=/usr/lib/ccache:\\\$PATH"
        fi
        BUILD_COMMANDS="ccache -s && $PRE_COMMANDS && "
    else
        export CONTAINER_NAME=$BUILDKITE_JOB_ID
    fi
    BUILD_COMMANDS="$BUILD_COMMANDS./$POPULATED_FILE_NAME"
    . $HELPERS_DIR/populate-template-and-hash.sh -h # obtain $FULL_TAG (and don't overwrite existing file)
    ARGS="$ARGS --rm --init --name $CONTAINER_NAME -v $(pwd):$(pwd) $(buildkite-intrinsics) -e JOBS" # We must mount $(pwd) in as itself to avoid https://stackoverflow.com/questions/31381322/docker-in-docker-cannot-mount-volume
    # sed -i '1s;^;#!/bin/bash\nexport PATH=$EOSIO_INSTALL_LOCATION/bin:$PATH\n;' /tmp/$POPULATED_FILE_NAME # /build-script: line 3: cmake: command not found
    # PRE_COMMANDS: Executed pre-cmake
    BUILD_COMMANDS="cd $(pwd) && $BUILD_COMMANDS./$POPULATED_FILE_NAME"
    . $HELPERS_DIR/populate-template-and-hash.sh -h # obtain $FULL_TAG (and don't overwrite existing file)
    cat /tmp/$POPULATED_FILE_NAME
    echo "mv \$EOSIO_CDT_BUILD_LOCATION $(pwd)/build" >> /tmp/$POPULATED_FILE_NAME
    mv /tmp/$POPULATED_FILE_NAME ./$POPULATED_FILE_NAME
    echo "$ docker run $ARGS $FULL_TAG bash -c \"$BUILD_COMMANDS\""
    eval docker run $ARGS $FULL_TAG bash -c \"$BUILD_COMMANDS\"
fi

tar -pczf build.tar.gz build && buildkite-agent artifact upload build.tar.gz

