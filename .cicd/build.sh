#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR

if [[ $(uname) == 'Darwin' ]]; then

    # You can't use chained commands in execute
    CDT_DIR_PATH=$(pwd)
    BUILD_DIR_PATH=$CDT_DIR_PATH/$BUILD_DIR
    BIN_DIR_PATH=$BUILD_DIR_PATH/bin
    cd ..
    git clone https://github.com/EOSIO/eosio.contracts.git
    CONTRACTS_DIR_PATH=$(pwd)/eosio.contracts
    cd $BUILD_DIR_PATH
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make -j$JOBS
    export PATH=$BIN_DIR_PATH:$PATH
    cd $CONTRACTS_DIR_PATH
    mkdir -p build
    cd build
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

    PRE_CONTRACTS_COMMAND="cd $MOUNTED_DIR && git clone https://github.com/EOSIO/eosio.contracts.git && echo items=$(ls) && export PATH=$MOUNTED_DIR/build/bin:$PATH && cd $MOUNTED_DIR/eosio.contracts && mkdir -p build && cd build"
    BUILD_CONTRACTS_COMMAND="cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang && make -j$JOBS"

    COMMANDS="$PRE_COMMANDS && $BUILD_COMMANDS && $PRE_CONTRACTS_COMMAND && $BUILD_CONTRACTS_COMMAND"

    # Load BUILDKITE Environment Variables for use in docker run
    if [[ -f $BUILDKITE_ENV_FILE ]]; then
        evars=""
        while read -r var; do
            evars="$evars --env ${var%%=*}"
        done < "$BUILDKITE_ENV_FILE"
    fi

    eval docker run $ARGS $evars $FULL_TAG bash -c \"$COMMANDS\"

fi

if [[ $BUILDKITE == true ]]; then
    cd $BUILD_DIR_PATH
    touch wasm_size.log
    PATH_WASM=$(pwd)
    cd tests/unit/test_contracts
    echo '--- :arrow_up: Generating wasm_size.log file'
    echo '####### EOSIO test contracts wasm files sizes #######' >> $PATH_WASM/wasm_size.log
    for FILENAME in ./*.wasm; do
        FILESIZE=$(wc -c "$FILENAME")
        echo $FILESIZE >> $PATH_WASM/wasm_size.log
    done

    echo '####### EOSIO system contracts wasm files sizes #######' >> $PATH_WASM/wasm_size.log
    cd $CONTRACTS_DIR_PATH/build/contracts
    for dir in */; do
        cd $dir
        for FILENAME in ./*.wasm; do
            if [[ -f $FILENAME ]]; then
                FILESIZE=$(wc -c "$FILENAME")
                echo $FILESIZE >> $PATH_WASM/wasm_size.log
            fi
        done
        cd ..
    done

    echo '--- :arrow_up: Uploading wasm_size.log'
    cd $PATH_WASM
    buildkite-agent artifact upload wasm_size.log
    echo 'Done uploading wasm_size.log'
fi
