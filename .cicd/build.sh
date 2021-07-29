#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR
CDT_DIR_PATH=$(pwd)
BUILD_DIR_PATH=$CDT_DIR_PATH/$BUILD_DIR
BIN_DIR_PATH=$BUILD_DIR_PATH/bin
git clone https://github.com/EOSIO/eosio.contracts.git
CONTRACTS_DIR_PATH=$(pwd)/eosio.contracts

if [[ $(uname) == 'Darwin' ]]; then

    # You can't use chained commands in execute
    cd $BUILD_DIR_PATH
    cmake .. -DCMAKE_BUILD_TYPE=Release
    make -j$JOBS
    export PATH=$BIN_DIR_PATH:$PATH
    cd $BUILD_DIR_PATH/tests/unit/test_contracts
    mkdir -p eosio.contracts
    cd eosio.contracts
    cmake $CONTRACTS_DIR_PATH 
    make -j$JOBS

else # Linux
    ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}
    cd $CDT_DIR_PATH
    . $HELPERS_DIR/docker-hash.sh

    # PRE_COMMANDS: Executed pre-cmake
    PRE_COMMANDS="cd $MOUNTED_DIR/build"
    BUILD_COMMANDS="cmake .. -DCMAKE_BUILD_TYPE=Release && make -j$JOBS"

    BUILD_COMMANDS_1604="cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_FLAGS=\"-stdlib=libc++\" && make -j$JOBS"
    [[ $IMAGE_TAG == 'ubuntu-16.04' ]] && BUILD_COMMANDS="$BUILD_COMMANDS_1604"

    [[ $IMAGE_TAG == 'centos-7.7' ]] && PRE_COMMANDS="$PRE_COMMANDS && source /opt/rh/devtoolset-7/enable"

    PRE_CONTRACTS_COMMAND="export PATH=$MOUNTED_DIR/build/bin:$PATH && cd $MOUNTED_DIR/build/tests/unit/test_contracts && mkdir -p eosio.contracts && cd eosio.contracts"
    BUILD_CONTRACTS_COMMAND="cmake $MOUNTED_DIR/eosio.contracts && make -j$JOBS"

    # Docker Commands
    if [[ $BUILDKITE == true ]]; then
        # Generate Base Images
        $CICD_DIR/generate-base-images.sh
        if [[ "$IMAGE_TAG" == 'ubuntu-18.04' ]]; then
            FULL_TAG='eosio/ci-contracts-builder:base-ubuntu-18.04-develop'
            export CMAKE_FRAMEWORK_PATH="$MOUNTED_DIR/build:${CMAKE_FRAMEWORK_PATH}"
            export CMAKE_FRAMEWORK_PATH="/root/eosio/build:${CMAKE_FRAMEWORK_PATH}"
            ./build.sh -e /root/eosio/build -c $BUILD_DIR_PATH -t
            #BUILD_CONTRACTS_COMMAND="cmake -DBUILD_TESTS=true $MOUNTED_DIR/eosio.contracts && make -j$JOBS"
        fi

    fi

    COMMANDS="$PRE_COMMANDS && $BUILD_COMMANDS"
    COMMANDS_EOSIO_CONTRACTS="$PRE_CONTRACTS_COMMAND && $BUILD_CONTRACTS_COMMAND"

    # Load BUILDKITE Environment Variables for use in docker run
    if [[ -f $BUILDKITE_ENV_FILE ]]; then
        evars=""
        while read -r var; do
            evars="$evars --env ${var%%=*}"
        done < "$BUILDKITE_ENV_FILE"
    fi

    eval docker run $ARGS $evars $FULL_TAG bash -c \"$COMMANDS\"
    START_TIME=$SECONDS
    eval docker run $ARGS $evars $FULL_TAG bash -c \"$COMMANDS_EOSIO_CONTRACTS\"
    ELAPSED_TIME=$(($SECONDS - $START_TIME))

fi

if [[ $BUILDKITE == true ]]; then
    cd $BUILD_DIR_PATH/tests/unit/test_contracts
    touch wasm_abi_size.log
    PATH_WASM=$(pwd)
    echo '--- :arrow_up: Generating wasm_abi_size.log file'
    echo '####### EOSIO test contracts wasm files sizes #######'>> $PATH_WASM/wasm_abi_size.log
    echo '####### wasm files path: tests/unit/test_contracts #######' >> $PATH_WASM/wasm_abi_size.log
    for FILENAME in ./*.wasm; do
        FILESIZE=$(wc -c "$FILENAME")
        echo $FILESIZE >> $PATH_WASM/wasm_abi_size.log
    done
    echo '####### EOSIO test contracts abi files sizes #######' >> $PATH_WASM/wasm_abi_size.log
    echo '####### abi files path: tests/unit/test_contracts #######' >> $PATH_WASM/wasm_abi_size.log
    for FILENAME in ./*.abi; do
        FILESIZE=$(wc -c "$FILENAME")
        echo $FILESIZE >> $PATH_WASM/wasm_abi_size.log
    done

    echo '####### EOSIO system contracts wasm files sizes #######' >> $PATH_WASM/wasm_abi_size.log
    echo '####### wasm files path: tests/unit/test_contracts/eosio.contracts/contracts #######' >> $PATH_WASM/wasm_abi_size.log
    cd eosio.contracts/contracts
    for dir in */; do
        cd $dir
        for FILENAME in ./*.wasm; do
            if [[ -f $FILENAME ]]; then
                FILESIZE=$(wc -c "$FILENAME")
                echo $FILESIZE >> $PATH_WASM/wasm_abi_size.log
            fi
        done
        cd ..
    done

    echo '####### EOSIO system contracts abi files sizes #######' >> $PATH_WASM/wasm_abi_size.log
    echo '####### abi files path: tests/unit/test_contracts/eosio.contracts/contracts #######' >> $PATH_WASM/wasm_abi_size.log
    for dir in */; do
        cd $dir
        for FILENAME in ./*.abi; do
            if [[ -f $FILENAME ]]; then
                FILESIZE=$(wc -c "$FILENAME")
                echo $FILESIZE >> $PATH_WASM/wasm_abi_size.log
            fi
        done
        cd ..
    done

    echo "Elapsed time to build eosio.contracts: $ELAPSED_TIME" >> $PATH_WASM/wasm_abi_size.log

    echo '--- :arrow_up: Uploading wasm_abi_size.log'
    cd $PATH_WASM
    buildkite-agent artifact upload wasm_abi_size.log
    echo 'Done uploading wasm_abi_size.log'
fi
