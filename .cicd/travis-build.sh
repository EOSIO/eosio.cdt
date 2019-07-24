#!/bin/bash
set -eo pipefail
cd $( dirname "${BASH_SOURCE[0]}" )/.. # Ensure we're in the repo root and not inside of scripts
. ./.cicd/.helpers

CPU_CORES=$(getconf _NPROCESSORS_ONLN)
echo $CPU_CORES

if [[ "$(uname)" == Darwin ]]; then
    echo 'Detected Darwin, building natively.'
    [[ -d eosio.cdt ]] && cd eosio.cdt
    [[ ! -d build ]] && mkdir build
    cd build
    echo '$ cmake ..'
    cmake ..
    echo "$ make -j $CPU_CORES"
    make -j $CPU_CORES
    ctest -j $CPU_CORES -L unit_tests -V -T Test
else # linux
    echo 'Detected Linux, building in Docker.'
    execute docker run --rm -v $(pwd):/workdir -v /usr/lib/ccache -v $HOME/.ccache:/opt/.ccache -e CCACHE_DIR=/opt/.ccache ${FULL_TAG}
fi
