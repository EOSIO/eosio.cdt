ARG EOS_CONTAINER_TAG=latest
FROM blockone-b1x-taurus-docker-local.jfrog.io/taurus/taurus-node/pinned/base:${EOS_CONTAINER_TAG}

COPY *.deb /

RUN apt-get update && \
    apt-get install -y /*.deb && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*