#!/usr/bin/env bash
#set -eo pipefail
. ./.cicd/helpers/general.sh
. $HELPERS_DIR/docker-hash.sh
# look for Docker image
echo "+++ :mag_right: Looking for $FULL_TAG"
ORG_REPO=$(echo $FULL_TAG | cut -d: -f1)
TAG=$(echo $FULL_TAG | cut -d: -f2)
DOCKER_REPO="blockone-b1fs-b1x-docker-dev-local.jfrog.io"
DOCKER_LOGIN_REPO="https://${DOCKER_REPO}"
DOCKER_REPO_GOLDEN="https://blockone-golden-docker-release-local.jfrog.io"

RESOLVE_DNS=`cat /etc/resolv.conf | grep search | xargs -n1 | grep "int.b1fs.net"`
PROXY_ADDR=$(dig +short proxy.service.${RESOLVE_DNS} | head -n1)
PROXY_URL="http://${PROXY_ADDR}:3128/"
echo PROXY_URL: $PROXY_URL

echo "login to artifactory"
echo $ARTIFACTORY_PASSWORD | docker login $DOCKER_LOGIN_REPO -u $ARTIFACTORY_USERNAME --password-stdin
echo $ARTIFACTORY_PASSWORD | docker login $DOCKER_REPO_GOLDEN -u $ARTIFACTORY_USERNAME --password-stdin

DOCKER_PULL="docker pull $FULL_TAG"
echo "$ $DOCKER_PULL"
out=$(eval $DOCKER_PULL 2>&1)
echo "Done pull for $FULL_TAG"
echo "out: $out"

# build, if neccessary
if [[ $out != *"up to date"* ]]; then
    echo "Building container..."
    echo "Build and tag docker ${IMAGE_TAG}.dockerfile"
    DOCKER_BUILD="docker build --network=host --build-arg http_proxy=$PROXY_URL --build-arg https_proxy=$PROXY_URL --build-arg no_proxy=$no_proxy -t $FULL_TAG -f $CICD_DIR/platforms/${IMAGE_TAG}.dockerfile ."
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
    DOCKER_RMI="docker rmi 'DOCKER_REPO/$FULL_TAG' || :"
    echo "$ $DOCKER_RMI"
    eval $DOCKER_RMI
else
    echo "$FULL_TAG already exists."
fi