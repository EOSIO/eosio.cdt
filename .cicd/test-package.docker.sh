#!/bin/bash
set -euo pipefail

. "${0%/*}/libfunctions.sh"

echo '--- :docker: Pretest Setup'

DOCKER_REPO="blockone-b1fs-b1x-docker-dev-local.jfrog.io"
DOCKER_LOGIN_REPO="https://${DOCKER_REPO}"

echo "login to artifactory"
echo $ARTIFACTORY_PASSWORD | docker login $DOCKER_LOGIN_REPO -u $ARTIFACTORY_USERNAME --password-stdin

DOCKER_PULL="docker pull $DOCKER_REPO/$IMAGE"
echo "$ $DOCKER_PULL"
perform $DOCKER_PULL
echo "Done with pull"
perform "docker run --rm -v \"\$(pwd):/eosio.cdt\" -w '/eosio.cdt' -it $DOCKER_REPO/$IMAGE ./.cicd/test-package.run.sh"