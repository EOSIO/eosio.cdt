#!/bin/bash
set -eo pipefail
echo '--- :arrow_down: Downloading Package :package:'
buildkite-agent artifact download '*.deb' . --step ':ubuntu: Ubuntu 18.04 - Package Builder'
echo '--- :key: Authenticating Google Service Account :gcloud:'
gcloud --quiet auth activate-service-account b1-automation-svc@b1-automation-dev.iam.gserviceaccount.com --key-file=/etc/gcp-service-account.json
docker-credential-gcr configure-docker
echo '+++ :docker: Building Image'
DOCKER_BUILD_COMMAND="docker build -f '.cicd/legacy.dockerfile' -t 'eosio/cdt:latest' -t 'eosio/cdt:$BUILDKITE_COMMIT' -t 'eosio/cdt:$BUILDKITE_BRANCH'$([[ -z "$BUILDKITE_TAG" || "$BUILDKITE_BRANCH" == "$BUILDKITE_TAG" ]] || echo " -t 'eosio/cdt:$BUILDKITE_TAG'") ."
echo "$ $DOCKER_BUILD_COMMAND"
eval $DOCKER_BUILD_COMMAND
docker tag eosio/cdt:$BUILDKITE_COMMIT gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_COMMIT
docker tag eosio/cdt:$BUILDKITE_BRANCH gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_BRANCH
[[ -z "$BUILDKITE_TAG" || "$BUILDKITE_BRANCH" == "$BUILDKITE_TAG" ]] || docker tag eosio/cdt:$BUILDKITE_TAG gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_TAG
docker tag eosio/cdt:latest gcr.io/b1-automation-dev/eosio/cdt:latest
echo '+++ :arrow_up: Pushing Docker Images'
docker push gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_COMMIT
docker push gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_BRANCH
[[ -z "$BUILDKITE_TAG" || "$BUILDKITE_BRANCH" == "$BUILDKITE_TAG" ]] || docker push gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_TAG
docker push gcr.io/b1-automation-dev/eosio/cdt:latest
echo '+++ :put_litter_in_its_place: Cleaning Up'
docker rmi eosio/cdt:$BUILDKITE_COMMIT
docker rmi eosio/cdt:$BUILDKITE_BRANCH
[[ -z "$BUILDKITE_TAG" || "$BUILDKITE_BRANCH" == "$BUILDKITE_TAG" ]] || docker rmi eosio/cdt:$BUILDKITE_TAG
docker rmi eosio/cdt:latest
docker rmi gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_COMMIT
docker rmi gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_BRANCH
[[ -z "$BUILDKITE_TAG" || "$BUILDKITE_BRANCH" == "$BUILDKITE_TAG" ]] || docker rmi gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_TAG
docker rmi gcr.io/b1-automation-dev/eosio/cdt:latest
