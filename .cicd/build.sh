#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR

if [[ $(uname) == 'Darwin' ]]; then

    # You can't use chained commands in execute
    cd $BUILD_DIR
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make -j$JOBS

else # Linux

    ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}

    . $HELPERS_DIR/docker-hash.sh

    # PRE_COMMANDS: Executed pre-cmake
    PRE_COMMANDS="cd $MOUNTED_DIR/build"
    BUILD_COMMANDS="cmake .. -DCMAKE_BUILD_TYPE=Release && make -j$JOBS"

    BUILD_COMMANDS_1604="cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_FLAGS=\"-stdlib=libc++\" && make -j$JOBS"
    [[ $IMAGE_TAG == 'ubuntu-16.04' ]] && BUILD_COMMANDS="$BUILD_COMMANDS_1604"

    [[ $IMAGE_TAG == 'centos-7.7' ]] && PRE_COMMANDS="$PRE_COMMANDS && source /opt/rh/devtoolset-7/enable"
    # Docker Commands
    if [[ $BUILDKITE == true ]]; then
        # Generate Base Images
        $CICD_DIR/generate-base-images.sh
        if [[ "$IMAGE_TAG" == 'ubuntu-18.04' ]]; then
          FULL_TAG='eosio/ci-contracts-builder:base-ubuntu-18.04-develop'
        fi
    fi

    COMMANDS="$PRE_COMMANDS && $BUILD_COMMANDS"

    # Load BUILDKITE Environment Variables for use in docker run
    if [[ -f $BUILDKITE_ENV_FILE ]]; then
        evars=""
        while read -r var; do
            evars="$evars --env ${var%%=*}"
        done < "$BUILDKITE_ENV_FILE"
    fi

    eval docker run $ARGS $evars $FULL_TAG bash -c \"$COMMANDS\"

fi
