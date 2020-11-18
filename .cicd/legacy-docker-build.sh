#!/bin/bash
set -eo pipefail
echo '+++ :evergreen_tree: Configuring Environment'
[[ ! -z "$DOCKER_REGISTRY" ]] || export DOCKER_REGISTRY="${1:-"gcr.io/b1-automation-dev/eosio/cdt"}"
echo '--- :arrow_down: Downloading Package :package:'
buildkite-agent artifact download '*.deb' . --step ':ubuntu: Ubuntu 18.04 - Package Builder'
echo '--- :key: Authenticating Google Service Account :gcloud:'
gcloud --quiet auth activate-service-account b1-automation-svc@b1-automation-dev.iam.gserviceaccount.com --key-file=/etc/gcp-service-account.json
docker-credential-gcr configure-docker
echo '+++ :docker: Building Image'
DOCKER_BUILD_COMMAND="docker build -f '.cicd/legacy.dockerfile' -t 'eosio/cdt:latest' -t 'eosio/cdt:$BUILDKITE_COMMIT' -t 'eosio/cdt:$BUILDKITE_BRANCH'$([[ -z "$BUILDKITE_TAG" || "$BUILDKITE_BRANCH" == "$BUILDKITE_TAG" ]] || echo " -t 'eosio/cdt:$BUILDKITE_TAG'") ."
echo "$ $DOCKER_BUILD_COMMAND"
eval $DOCKER_BUILD_COMMAND
docker tag eosio/cdt:$BUILDKITE_COMMIT $DOCKER_REGISTRY:$BUILDKITE_COMMIT
docker tag eosio/cdt:$BUILDKITE_BRANCH $DOCKER_REGISTRY:$BUILDKITE_BRANCH
[[ -z "$BUILDKITE_TAG" || "$BUILDKITE_BRANCH" == "$BUILDKITE_TAG" ]] || docker tag eosio/cdt:$BUILDKITE_TAG $DOCKER_REGISTRY:$BUILDKITE_TAG
docker tag eosio/cdt:latest $DOCKER_REGISTRY:latest
echo '+++ :arrow_up: Pushing Docker Images'
docker push $DOCKER_REGISTRY:$BUILDKITE_COMMIT
docker push $DOCKER_REGISTRY:$BUILDKITE_BRANCH
[[ -z "$BUILDKITE_TAG" || "$BUILDKITE_BRANCH" == "$BUILDKITE_TAG" ]] || docker push $DOCKER_REGISTRY:$BUILDKITE_TAG
docker push $DOCKER_REGISTRY:latest
echo '+++ :put_litter_in_its_place: Cleaning Up'
docker rmi eosio/cdt:$BUILDKITE_COMMIT
docker rmi eosio/cdt:$BUILDKITE_BRANCH
[[ -z "$BUILDKITE_TAG" || "$BUILDKITE_BRANCH" == "$BUILDKITE_TAG" ]] || docker rmi eosio/cdt:$BUILDKITE_TAG
docker rmi eosio/cdt:latest
docker rmi $DOCKER_REGISTRY:$BUILDKITE_COMMIT
docker rmi $DOCKER_REGISTRY:$BUILDKITE_BRANCH
[[ -z "$BUILDKITE_TAG" || "$BUILDKITE_BRANCH" == "$BUILDKITE_TAG" ]] || docker rmi $DOCKER_REGISTRY:$BUILDKITE_TAG
docker rmi $DOCKER_REGISTRY:latest
