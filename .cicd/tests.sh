#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

PRE_COMMANDS="cd $(pwd)/build"
TEST="ctest -j$JOBS -L unit_tests -V -T Test"
TESTS_COMMANDS="$PRE_COMMANDS && $TEST"

[[ $TRAVIS != true ]] && buildkite-agent artifact download build.tar.gz . --step "$PLATFORM_FULL_NAME - Build"

if [[ $(uname) == 'Darwin' ]]; then

    [[ $TRAVIS != true ]] && tar -xzf build.tar.gz
    # You can't use chained commands in execute
    cd $BUILD_DIR
    set +e
    bash -c "$TEST"
    EXIT_STATUS=$?
    cd $ROOT_DIR

else # Linux

    ARGS=${ARGS:-"--rm --init $(buildkite-intrinsics) -v $(pwd):$(pwd)"}
    [[ $TRAVIS == true ]] && ARGS="$ARGS -e JOBS -e CCACHE_DIR=/opt/.ccache" || TESTS_COMMANDS="cd $(pwd) && tar -xzf build.tar.gz && $TESTS_COMMANDS"
    . $HELPERS_DIR/populate-template-and-hash.sh -h # obtain $FULL_TAG (and don't overwrite existing file)
    echo "$ docker run $ARGS $FULL_TAG bash -c \"$TESTS_COMMANDS\""
    set +e
    eval docker run $ARGS $FULL_TAG bash -c \"$TESTS_COMMANDS\"
    EXIT_STATUS=$?

fi
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