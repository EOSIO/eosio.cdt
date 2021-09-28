#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR

PRE_COMMANDS="cd $MOUNTED_DIR/build"
TEST="ctest -j$JOBS -L integration_tests -V -T Test && ctest -j$JOBS -L tester_tests -V -T Test"
COMMANDS="$PRE_COMMANDS && $TEST"

ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}

# . $HELPERS_DIR/docker-hash.sh

# Load BUILDKITE Environment Variables for use in docker run
if [[ -f $BUILDKITE_ENV_FILE ]]; then
    evars=""
    while read -r var; do
        evars="$evars --env ${var%%=*}"
    done < "$BUILDKITE_ENV_FILE"
fi

if [[ "$BUILDKITE" == 'true' && "$IMAGE_TAG" == 'ubuntu-18.04' ]]; then
  FULL_TAG='eosio/ci-contracts-builder:base-ubuntu-18.04-develop-boxed'
fi

set +e
eval docker run $ARGS $evars $FULL_TAG bash -c \"$COMMANDS\"
EXIT_STATUS=$?

# buildkite
if [[ "$BUILDKITE" == 'true' ]]; then
    cd build
    # upload artifacts
    echo '+++ :arrow_up: Uploading Artifacts'
    echo 'Exporting xUnit XML'
    mv -f ./Testing/$(ls ./Testing/ | grep '2' | tail -n 1)/Test.xml test-results.xml
    echo 'Uploading artifacts'
    buildkite-agent artifact upload test-results.xml
    echo 'Done uploading artifacts.'
fi
# re-throw
if [[ "$EXIT_STATUS" != 0 ]]; then
    echo "Failing due to non-zero exit status from ctest: $EXIT_STATUS"
    exit $EXIT_STATUS
fi
