#!/usr/bin/env bash
set -eo pipefail

. ./.cicd/helpers/general.sh
. $HELPERS_DIR/docker-hash.sh
# look for Docker image
echo "+++ :mag_right: Looking for $FULL_TAG"
ORG_REPO=$(echo $FULL_TAG | cut -d: -f1)
TAG=$(echo $FULL_TAG | cut -d: -f2)

DOCKER_PULL="docker pull $DOCKER_REPO/$FULL_TAG"
echo "$ $DOCKER_PULL"
#We want to catch the error on the DOCKER_PULL
set +eo pipefail
out=$(eval $DOCKER_PULL 2>&1)
echo "Done pull for $FULL_TAG"
echo "out: $out"
#Set it back
set -eo pipefail

# build, if neccessary
if [[ $out != *"up to date"* ]]; then
    echo "Building container..."
    echo "Build and tag docker ${IMAGE_TAG}.dockerfile"
    DOCKER_BUILD="docker build $PROXY_DOCKER_BUILD_ARGS -t $FULL_TAG -f $CICD_DIR/platforms/${IMAGE_TAG}.dockerfile ."
    echo "$ $DOCKER_BUILD"
    eval $DOCKER_BUILD

    DOCKER_TAG="docker tag '$FULL_TAG' '$DOCKER_REPO/$FULL_TAG'"
    echo "$ $DOCKER_TAG"
    eval $DOCKER_TAG
    echo "docker push $DOCKER_REPO/$FULL_TAG"
    DOCKER_PUSH="docker push $DOCKER_REPO/$FULL_TAG"
    echo "$ $DOCKER_PUSH"
    eval $DOCKER_PUSH
    echo "done pushing $FULL_TAG"
    DOCKER_RMI="docker rmi '$DOCKER_REPO/$FULL_TAG' || :"
    echo "$ $DOCKER_RMI"
    eval $DOCKER_RMI
    echo "done removing $FULL_TAG"
else
    echo "$FULL_TAG already exists."
fi