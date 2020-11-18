#!/bin/bash
echo "--- :arrow_down: Downloading package" && \
buildkite-agent artifact download "build/packages/*.deb" docker/dev/. --step ":ubuntu: Ubuntu 18.04 Package Builder" && \
echo "--- :key: AUTHENTICATING GOOGLE SERVICE ACCOUNT" && \
gcloud --quiet auth activate-service-account b1-automation-svc@b1-automation-dev.iam.gserviceaccount.com --key-file=/etc/gcp-service-account.json && \
docker-credential-gcr configure-docker && \
echo "--- :hammer_and_wrench: BUILDING BUILD IMAGE" && \
cd docker/dev && \
[[ "$BUILDKITE_TAG" != "" ]] && docker build -t eosio/cdt:latest -t eosio/cdt:$BUILDKITE_COMMIT -t eosio/cdt:$BUILDKITE_BRANCH -t eosio/cdt:$BUILDKITE_TAG . || docker build -t eosio/cdt:latest -t eosio/cdt:$BUILDKITE_COMMIT -t eosio/cdt:$BUILDKITE_BRANCH . && \
docker tag eosio/cdt:$BUILDKITE_COMMIT gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_COMMIT && \
docker tag eosio/cdt:$BUILDKITE_BRANCH gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_BRANCH && \
[[ "$BUILDKITE_TAG" != "" ]] && docker tag eosio/cdt:$BUILDKITE_TAG gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_TAG || : && \
docker tag eosio/cdt:latest gcr.io/b1-automation-dev/eosio/cdt:latest && \
echo "--- :hand: PUSHING DOCKER IMAGES" && \
docker push gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_COMMIT && \
docker push gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_BRANCH && \
[[ "$BUILDKITE_TAG" != "" ]] && docker push gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_TAG || : && \
docker push gcr.io/b1-automation-dev/eosio/cdt:latest && \
echo "--- :thought_balloon: TRASHING OLD IMAGES" && \
docker rmi eosio/cdt:$BUILDKITE_COMMIT && \
docker rmi eosio/cdt:$BUILDKITE_BRANCH && \
[[ "$BUILDKITE_TAG" != "" ]] && docker rmi eosio/cdt:$BUILDKITE_TAG || : && \
docker rmi eosio/cdt:latest && \
docker rmi gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_COMMIT && \
docker rmi gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_BRANCH && \
[[ "$BUILDKITE_TAG" != "" ]] && docker rmi gcr.io/b1-automation-dev/eosio/cdt:$BUILDKITE_TAG || : && \
docker rmi gcr.io/b1-automation-dev/eosio/cdt:latest
