#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

[[ ! -z "$CONTRACTS_VERSION" ]] || export CONTRACTS_VERSION="$(cat "$PIPELINE_CONFIG" | jq -r '.dependencies["eosio.contracts"]')"
git clone -b "$CONTRACTS_VERSION" https://github.com/EOSIO/eosio.contracts.git 

# Docker Commands
ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}
. $HELPERS_DIR/docker-hash.sh

FULL_TAG='eosio/ci-contracts-builder:base-ubuntu-18.04-v2.1.0'
TEST_CONTRACTS_COMMAND="cd $MOUNTED_DIR/build_eosio_contracts/tests && ctest -j $JOBS --output-on-failure -T Test"

# Load BUILDKITE Environment Variables for use in docker run
if [[ -f $BUILDKITE_ENV_FILE ]]; then
    evars=""
    while read -r var; do
        evars="$evars --env ${var%%=*}"
    done < "$BUILDKITE_ENV_FILE"
fi

echo '--- :arrow_up: Unit-tests for eosio.contract wasm and abi files'
DOCKER_RUN="docker run $ARGS $evars $FULL_TAG bash -c \"$TEST_CONTRACTS_COMMAND\""
echo "$ $DOCKER_RUN"
eval $DOCKER_RUN
EXIT_STATUS=$?

if [[ "$EXIT_STATUS" != 0 ]]; then
    echo "Failing due to non-zero exit status from ctest: $EXIT_STATUS"
    exit $EXIT_STATUS
fi


