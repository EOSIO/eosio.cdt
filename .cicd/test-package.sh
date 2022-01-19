#!/bin/bash
set -eu

echo '--- :docker: Selecting Container'

RESOLVE_DNS=`cat /etc/resolv.conf | grep search | xargs -n1 | grep "int.b1fs.net"`
PROXY_ADDR=$(dig +short proxy.service.${RESOLVE_DNS} | head -n1)
PROXY_URL="http://${PROXY_ADDR}:3128/"
echo PROXY_URL: $PROXY_URL

DOCKER_REPO="blockone-dockerhub.jfrog.io"
DOCKER_LOGIN_REPO="https://${DOCKER_REPO}"

echo "login to artifactory"
echo $ARTIFACTORY_PASSWORD | docker login $DOCKER_LOGIN_REPO -u $ARTIFACTORY_USERNAME --password-stdin

DOCKER_PULL="docker pull $DOCKER_REPO/$IMAGE"
echo "$ $DOCKER_PULL"
eval $DOCKER_PULL
echo "Done with pull"
echo "docker run --rm -v "$(pwd):/eos" -w '/eos' -it $DOCKER_REPO/$IMAGE ./.cicd/test-package.run.sh"
docker run --network=host -e http_proxy=$PROXY_URL -e https_proxy=$PROXY_URL -e no_proxy=$no_proxy --rm -v "$(pwd):/eos" -w '/eos' -it $DOCKER_REPO/$IMAGE ./.cicd/test-package.run.sh
echo "done with docker run..."
