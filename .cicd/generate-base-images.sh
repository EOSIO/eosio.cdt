#!/usr/bin/env bash
set -eo pipefail
. ./.cicd/helpers/general.sh
. $HELPERS_DIR/docker-hash.sh
# look for Docker image
echo "+++ :mag_right: Looking for $FULL_TAG"
ORG_REPO=$(echo $FULL_TAG | cut -d: -f1)
TAG=$(echo $FULL_TAG | cut -d: -f2)
DOCKER_REPO="blockone-b1fs-b1x-docker-dev-local.jfrog.io"
DOCKER_LOGIN_REPO="https://${DOCKER_REPO}"
DOCKER_REPO_GOLDEN="https://blockone-golden-docker-release-local.jfrog.io"

echo "login to artifactory"
echo $ARTIFACTORY_PASSWORD | docker login $DOCKER_LOGIN_REPO -u $ARTIFACTORY_USERNAME --password-stdin
echo $ARTIFACTORY_PASSWORD | docker login $DOCKER_REPO_GOLDEN -u $ARTIFACTORY_USERNAME --password-stdin

out=$(docker pull $FULL_TAG)

# build, if neccessary
if [[ $out != *"up to date"* ]]; then
    echo "Build and tag docker ${IMAGE_TAG}.dockerfile"
    DOCKER_BUILD="docker build -t $FULL_TAG -f $CICD_DIR/platforms/${IMAGE_TAG}.dockerfile ."
    echo "$ $DOCKER_BUILD"
    eval $DOCKER_BUILD
    echo "docker push $FULL_TAG"
    DOCKER_PUSH="docker push $FULL_TAG"
    echo "$ $DOCKER_PUSH"
    eval $DOCKER_PUSH
    echo "done pushing $FULL_TAG"
    DOCKER_RMI="docker rmi '$FULL_TAG' || :"
    echo "$ $DOCKER_RMI"
    eval $DOCKER_RMI
else
    echo "$FULL_TAG already exists."
fi