ARG EOS_CONTAINER_TAG=latest
FROM blockone-b1x-taurus-docker-local.jfrog.io/taurus/taurus-node/pinned/base:${EOS_CONTAINER_TAG}

COPY build.tar.gz /

RUN mkdir eosio.cdt && \
    mv build.tar.gz eosio.cdt && \
    cd eosio.cdt && \
    tar -xzf build.tar.gz

ENV PATH="/root/eosio.cdt/build/bin/:${PATH}"