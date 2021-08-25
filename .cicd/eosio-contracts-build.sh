#!/usr/bin/env bash/
set -eo pipefail
. ./.cicd/helpers/general.sh
CDT_DIR_HOST=$(pwd)
mkdir -p build_eosio_contracts 


[[ ! -z "$CONTRACTS_VERSION" ]] || export CONTRACTS_VERSION="$(cat "$PIPELINE_CONFIG" | jq -r '.dependencies["eosio.contracts"]')"
git clone -b "$CONTRACTS_VERSION" https://github.com/EOSIO/eosio.contracts.git 

if [[ $(uname) == 'Darwin' ]]; then
    export PATH=$CDT_DIR_HOST/build/bin:$PATH
    cd build_eosio_contracts
    cmake $CDT_DIR_HOST/eosio.contracts
    make -j$JOBS
else #Linux
    ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}
    . $HELPERS_DIR/docker-hash.sh

    PRE_CONTRACTS_COMMAND="export PATH=$MOUNTED_DIR/build/bin:$PATH && cd $MOUNTED_DIR/build_eosio_contracts"
    BUILD_CONTRACTS_COMMAND="cmake $MOUNTED_DIR/eosio.contracts && make -j$JOBS"

    # Docker Commands
    if [[ $BUILDKITE == true ]]; then
        # Generate Base Images
        $CICD_DIR/generate-base-images.sh
        if [[ "$IMAGE_TAG" == 'ubuntu-18.04' ]]; then
            FULL_TAG='eosio/ci-contracts-builder:base-ubuntu-18.04-develop'
            export CMAKE_FRAMEWORK_PATH="$MOUNTED_DIR/build:${CMAKE_FRAMEWORK_PATH}"
            BUILD_CONTRACTS_COMMAND="cmake -DBUILD_TESTS=true $MOUNTED_DIR/eosio.contracts && make -j$JOBS"
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
    cd $CDT_DIR_HOST/build/tests/unit/test_contracts
    touch wasm_abi_size.json
    PATH_WASM=$(pwd)
    JSON=$(echo '{}' | jq -r '.')
    echo '--- :arrow_up: Generating wasm_abi_size.log file'
    for FILENAME in *.{abi,wasm}; do
        FILESIZE=$(wc -c <"$FILENAME")
        export value=$FILESIZE
        export key="$FILENAME"
        JSON=$(echo "$JSON" | jq -r "(.\"$key\") += (env.value | tonumber)")
    done

    cd $CDT_DIR_HOST/build_eosio_contracts/contracts
    for dir in */; do
        cd $dir
        for FILENAME in *.{abi,wasm}; do
            if [[ -f $FILENAME ]]; then
                FILESIZE=$(wc -c <"$FILENAME")
                export value=$FILESIZE
                export key="$FILENAME"
                JSON=$(echo "$JSON" | jq -r "(.\"$key\") += (env.value | tonumber)")
            fi
        done
        cd ..
    done

    echo '--- :arrow_up: Uploading wasm_abi_size.json'
    cd $PATH_WASM
    echo $JSON >> wasm_abi_size.json
    buildkite-agent artifact upload wasm_abi_size.json
    echo 'Done uploading wasm_abi_size.json'
    echo '--- :arrow_up: Uploading eosio.contract build'
    echo 'Compressing eosio.contract build directory.'
    cd $CDT_DIR_HOST
    tar -pczf 'build_eosio_contracts.tar.gz' build_eosio_contracts
    echo 'Uploading eosio.contract build directory.'
    buildkite-agent artifact upload 'build_eosio_contracts.tar.gz'
    echo 'Done uploading artifacts.'

fi