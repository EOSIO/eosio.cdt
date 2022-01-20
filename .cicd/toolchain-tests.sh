#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh

mkdir -p $BUILD_DIR

DOCKER_REPO="blockone-b1fs-b1x-docker-dev-local.jfrog.io"
DOCKER_LOGIN_REPO="https://${DOCKER_REPO}"

if [[ $(uname) == 'Darwin' ]]; then

    # You can't use chained commands in execute
    cd $BUILD_DIR
    TEST="./tools/toolchain-tester/toolchain-tester ../tests/toolchain/"
    bash -c "$TEST"
    
else # Linux
    echo "login to artifactory"
    echo $ARTIFACTORY_PASSWORD | docker login $DOCKER_LOGIN_REPO -u $ARTIFACTORY_USERNAME --password-stdin

    ARGS=${ARGS:-"--rm --init -v $(pwd):$MOUNTED_DIR"}

    PRE_COMMANDS="cd $MOUNTED_DIR/build"
    SET_PYTHON_PATH="export CICD_PYTHON_PATH=/usr/local/bin/python3"
    TEST="./tools/toolchain-tester/toolchain-tester ../tests/toolchain/"

    COMMANDS="$PRE_COMMANDS && $SET_PYTHON_PATH && $TEST"

    . $HELPERS_DIR/docker-hash.sh

    # Load BUILDKITE Environment Variables for use in docker run
    if [[ -f $BUILDKITE_ENV_FILE ]]; then
        evars=""
        while read -r var; do
            evars="$evars --env ${var%%=*}"
        done < "$BUILDKITE_ENV_FILE"
    fi

    DOCKER_PULL="docker pull $DOCKER_REPO/$FULL_TAG"
    echo "$ $DOCKER_PULL"
    eval $DOCKER_PULL
    echo "Done with pull"

    eval docker run $ARGS $evars $DOCKER_REPO/$FULL_TAG bash -c \"$COMMANDS\"

fi
