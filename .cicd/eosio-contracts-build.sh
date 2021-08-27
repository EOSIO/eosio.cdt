#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh
CDT_DIR_HOST=$(pwd)
mkdir -p build_eosio_contracts 


[[ ! -z "$CONTRACTS_VERSION" ]] || export CONTRACTS_VERSION="$(cat "$PIPELINE_CONFIG" | jq -r '.dependencies["eosio.contracts"]')"
git clone -b "$CONTRACTS_VERSION" https://github.com/EOSIO/eosio.contracts.git 

if [[ $(uname) == 'Darwin' ]]; then
    export PATH=$CDT_DIR_HOST/build/bin:$PATH
    cd build_eosio_contracts
    CMAKE="cmake ../eosio.contracts"
    echo "$ $CMAKE"
    eval $CMAKE
    MAKE="make -j $JOBS"
    echo "$ $MAKE"
    eval $MAKE
    cd ..
else #Linux
    ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}
    . $HELPERS_DIR/docker-hash.sh

    PRE_CONTRACTS_COMMAND="export PATH=$MOUNTED_DIR/build/bin:$PATH && cd $MOUNTED_DIR/build_eosio_contracts"
    BUILD_CONTRACTS_COMMAND="cmake $MOUNTED_DIR/eosio.contracts && make -j$JOBS"

    # Docker Commands
    # Generate Base Images
    $CICD_DIR/generate-base-images.sh
    if [[ "$IMAGE_TAG" == 'ubuntu-18.04' ]]; then
        FULL_TAG='eosio/ci-contracts-builder:base-ubuntu-18.04-v2.1.0'
        export CMAKE_FRAMEWORK_PATH="$MOUNTED_DIR/build:${CMAKE_FRAMEWORK_PATH}"
        BUILD_CONTRACTS_COMMAND="cmake -DBUILD_TESTS=true $MOUNTED_DIR/eosio.contracts && make -j$JOBS"
    fi

    COMMANDS_EOSIO_CONTRACTS="$PRE_CONTRACTS_COMMAND && $BUILD_CONTRACTS_COMMAND"

    # Load BUILDKITE Environment Variables for use in docker run
    if [[ -f $BUILDKITE_ENV_FILE ]]; then
        evars=""
        while read -r var; do
            evars="$evars --env ${var%%=*}"
        done < "$BUILDKITE_ENV_FILE"
    fi

    DOCKER_RUN="docker run $ARGS $evars $FULL_TAG bash -c \"$COMMANDS_EOSIO_CONTRACTS\""
    echo "$ $DOCKER_RUN"
    eval $DOCKER_RUN

fi

touch wasm_abi_size.json
cd build/tests/unit/test_contracts
JSON=$(echo '{}' | jq -r '.')
echo '--- :arrow_up: Generating wasm_abi_size.json file'
for FILENAME in *.{wasm,abi}; do
    FILESIZE=$(wc -c <"$FILENAME")
    export value=$FILESIZE
    export key="$FILENAME"
    JSON="$(echo "$JSON" | jq -r '.[env.key] += (env.value | tonumber)')"
done

cd ../../../../build_eosio_contracts/contracts
for dir in */; do
    cd $dir
    for FILENAME in *.{wasm,abi}; do
        if [[ -f $FILENAME ]]; then
            FILESIZE=$(wc -c <"$FILENAME")
            export value=$FILESIZE
            export key="$FILENAME"
            JSON="$(echo "$JSON" | jq -r '.[env.key] += (env.value | tonumber)')"
        fi
    done
    cd ..
done

echo '--- :arrow_up: Uploading wasm_abi_size.json'
cd ../../
echo $JSON >> wasm_abi_size.json
if [[ $BUILDKITE == true ]]; then

    buildkite-agent artifact upload wasm_abi_size.json
    echo 'Done uploading wasm_abi_size.json'
    echo '--- :arrow_up: Uploading eosio.contract build'
    echo 'Compressing eosio.contract build directory.'
    tar -pczf 'build_eosio_contracts.tar.gz' build_eosio_contracts
    echo 'Uploading eosio.contract build directory.'
    buildkite-agent artifact upload 'build_eosio_contracts.tar.gz'
    echo 'Done uploading artifacts.'

fi