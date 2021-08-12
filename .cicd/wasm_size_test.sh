#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh
CDT_home_DIR=$(pwd)

[[ ! -z "$CONTRACTS_VERSION" ]] || export CONTRACTS_VERSION="$(cat "$PIPELINE_CONFIG" | jq -r '.dependencies["eosio.contracts"]')"
git clone -b "$CONTRACTS_VERSION" https://github.com/EOSIO/eosio.contracts.git 

echo fls=$(ls)
echo fpwd=$(pwd)

if [[ $(uname) == 'Darwin' ]]; then
    export PATH=$BIN_DIR:$PATH
    cd $ROOT_DIR/eosio.contracts
    mkdir -p build_eosio_contracts
    cd build_eosio_contracts
    cmake ..
    make -j$JOBS
else #Linux
    ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}
    . $HELPERS_DIR/docker-hash.sh

    PRE_CONTRACTS_COMMAND="echo fls=$(ls) && echo fpwd=$(pwd) && export PATH=$MOUNTED_DIR/build/bin:$PATH && cd $MOUNTED_DIR/eosio.contracts && mkdir -p build_eosio_contracts && cd build_eosio_contracts"
    BUILD_CONTRACTS_COMMAND="cmake .. && make -j$JOBS"

    # Docker Commands
    if [[ $BUILDKITE == true ]]; then
        # Generate Base Images
        $CICD_DIR/generate-base-images.sh
        if [[ "$IMAGE_TAG" == 'ubuntu-18.04' ]]; then
            FULL_TAG='eosio/ci-contracts-builder:base-ubuntu-18.04-develop'
            export CMAKE_FRAMEWORK_PATH="$MOUNTED_DIR/build:${CMAKE_FRAMEWORK_PATH}"
            BUILD_CONTRACTS_COMMAND="cmake -DBUILD_TESTS=true .. && make -j$JOBS"
        fi

    fi

    COMMANDS_EOSIO_CONTRACTS="$PRE_CONTRACTS_COMMAND && $BUILD_CONTRACTS_COMMAND"

    # Load BUILDKITE Environment Variables for use in docker run
    if [[ -f $BUILDKITE_ENV_FILE ]]; then
        evars=""
        while read -r var; do
            evars="$evars --env ${var%%=*}"
        done < "$BUILDKITE_ENV_FILE"
    fi

    eval docker run $ARGS $evars $FULL_TAG bash -c \"$COMMANDS_EOSIO_CONTRACTS\"

fi

if [[ $BUILDKITE == true ]]; then
    cd $BUILD_DIR/tests/unit/test_contracts
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
    echo '####### wasm files path: eosio.contracts/contracts #######' >> $PATH_WASM/wasm_abi_size.log
    cd $CDT_home_DIR/eosio.contracts/contracts
    echo fls=$(ls)
    echo fpwd=$(pwd)
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
    echo '####### abi files path: eosio.contracts/contracts #######' >> $PATH_WASM/wasm_abi_size.log
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

    echo '--- :arrow_up: Uploading wasm_abi_size.log'
    cd $PATH_WASM
    buildkite-agent artifact upload wasm_abi_size.log
    echo 'Done uploading wasm_abi_size.log'
    echo '--- :arrow_up: Uploading eosio.contract build'
    echo 'Compressing eosio.contract build directory.'
    cd $CDT_home_DIR/eosio.contracts
    tar -pczf 'build_eosio_contracts.tar.gz' build_eosio_contracts
    echo 'Uploading eosio.contract build directory.'
    buildkite-agent artifact upload 'build_eosio_contracts.tar.gz'
    echo 'Done uploading artifacts.'

fi