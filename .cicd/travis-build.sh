#!/bin/bash
set -e

function build() {
    # Per distro additions to docker command
    [[ $IMAGE_TAG == centos-7 ]] && PRECOMMANDS="source /opt/rh/python33/enable && source /opt/rh/devtoolset-7/enable &&"
    [[ $IMAGE_TAG == amazonlinux-2 || $IMAGE_TAG == centos-7 ]] && EXPORTS="export PATH=/usr/lib64/ccache:$PATH &&" || EXPORTS="$EXPORTS export PATH=/usr/lib/ccache:$PATH &&" # ccache needs to come first in the list
    # DOCKER
    docker run --rm -v $(pwd):/eosio.cdt -v /usr/lib/ccache -v $HOME/.ccache:/opt/.ccache -e CCACHE_DIR=/opt/.ccache eosio/producer:ci-${IMAGE_TAG}-cdt bash -c " \
        $PRE_COMMANDS ccache -s && \
        mkdir /eosio.cdt/build && cd /eosio.cdt/build && $EXPORTS cmake -DCMAKE_CXX_COMPILER_LAUNCHER=ccache /eosio.cdt && make -j $(getconf _NPROCESSORS_ONLN)
        ctest -j $CPU_CORES -L unit_tests -V -T Test"
}

CPU_CORES=$(getconf _NPROCESSORS_ONLN)

if [[ "$(uname)" == Darwin ]]; then
    ccache -s
    export PATH="/usr/local/opt/ccache/libexec:$PATH"
    echo 'Detected Darwin, building natively.'
    [[ -d eosio.cdt ]] && cd eosio.cdt
    [[ ! -d build ]] && mkdir build
    cd build
    echo '$ cmake ..'
    cmake /eosio.cdt
    echo "$ make -j $CPU_CORES"
    make -j $CPU_CORES
    ctest -j $CPU_CORES -L unit_tests -V -T Test
else # linux
    echo 'Detected Linux, building in Docker.'
    echo "$ docker pull eosio/producer:ci-${IMAGE_TAG}-cdt"
    build
fi
