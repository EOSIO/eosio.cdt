#!/bin/bash
set -euo pipefail

. "${0%/*}/libfunctions.sh"

echo '--- :docker: Pretest Setup'

DOCKER_PULL="docker pull $DOCKER_REPO/$IMAGE"
echo "$ $DOCKER_PULL"
perform $DOCKER_PULL
echo "Done with pull"
perform "docker run --rm -v \"\$(pwd):/eosio.cdt\" -w '/eosio.cdt' -it $DOCKER_REPO/$IMAGE ./.cicd/test-package.run.sh"