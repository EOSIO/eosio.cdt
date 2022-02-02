#!/bin/bash

set -eu

FORCE_REBUILD="${FORCE_CONTRACT_BUILDER_BUILD:-false}"
VERSION_NAME="$CONTRACT_BUILDER_VERSION"
IMAGE_NAME="taurus/taurus-contract-builder/${VERSION_NAME}-contract-builder-alt"
IMAGE="blockone-b1x-taurus-docker-local.jfrog.io/${IMAGE_NAME}"
# Massage branch to be short enough and have no '/' characters
BRANCH=$(echo "$BUILDKITE_BRANCH" | sed 's|/|-|g' | sed 's|_|-|g' | cut -c 1-63 | sed 's/-$//g' | tr '[:upper:]' '[:lower:]')

echo "Using EOS container tag ${EOS_CONTAINER_TAG}"
echo "Using EOS commit ${EOS_COMMITISH}"
echo "Using CDT commit ${CDT_COMMITISH}"
IMAGE_TAG="${EOS_COMMITISH}-${CDT_COMMITISH}"
echo "IMAGE_TAG ${IMAGE_TAG}"
echo "ls for build.tar.gz: $(ls *.gz)"

git clone https://github.com/b1-as/taurus-node.git eos
pushd eos
git checkout develop-boxed
git submodule update --init --recursive
popd

set +e
docker manifest inspect ${IMAGE}:${IMAGE_TAG} > /dev/null
if [[ $? -eq 0 ]]; then
  if [[ "${FORCE_REBUILD}" != 'true' ]]; then
    buildkite-agent annotate ":docker: ${VERSION_NAME} - Image ${IMAGE}:${IMAGE_TAG} already exists, so was not rebuilt" --style info --context "contracts-builder-${VERSION_NAME}"
    exit 0
  fi
fi
set -e

echo "Building ${IMAGE}:${IMAGE_TAG}"
DOCKER_BUILD=" docker build $PROXY_DOCKER_BUILD_ARGS -t \"${IMAGE}:${IMAGE_TAG}\" -f ./docker/contract-builder/Dockerfile2 ."
echo "$ $DOCKER_BUILD"
eval $DOCKER_BUILD

if [[ "${BRANCH}" == 'master' ]]; then
  echo "Pushing ${IMAGE}:${IMAGE_TAG}"
  docker push "${IMAGE}:${IMAGE_TAG}"
  buildkite-agent annotate ":docker: ${VERSION_NAME} - Built and pushed image ${IMAGE}:${IMAGE_TAG}" --style success --context "contract-builder-${VERSION_NAME}"
else
  echo "Not master. Appending b1x-platform-components branch/commit to tag and pushing."
  docker tag "${IMAGE}:${IMAGE_TAG}" "${IMAGE}:${IMAGE_TAG}-${BRANCH}"
  docker tag "${IMAGE}:${IMAGE_TAG}" "${IMAGE}:${IMAGE_TAG}-${BUILDKITE_COMMIT}"
  docker push "${IMAGE}:${IMAGE_TAG}-${BRANCH}"
  docker push "${IMAGE}:${IMAGE_TAG}-${BUILDKITE_COMMIT}"
  docker rmi "${IMAGE}:${IMAGE_TAG}-${BRANCH}" --force
  docker rmi "${IMAGE}:${IMAGE_TAG}-${BUILDKITE_COMMIT}" --force
  buildkite-agent annotate ":docker: ${VERSION_NAME} - Built and pushed image ${IMAGE}:${IMAGE_TAG}-${BUILDKITE_COMMIT}" --style success --context "contract-builder-${VERSION_NAME}"
fi

echo "Cleaning up ${IMAGE}:${IMAGE_TAG}"
docker rmi "${IMAGE}:${IMAGE_TAG}" --force