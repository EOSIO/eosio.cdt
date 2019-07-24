#!/bin/bash
set -eo pipefail
cd $( dirname "${BASH_SOURCE[0]}" )/.. # Ensure we're in the repo root and not inside of scripts
. ./.cicd/.helpers

if [[ "$(uname)" == Darwin ]]; then
    echo 'Detected Darwin, building natively.'
    [[ -d eosio.cdt ]] && cd eosio.cdt
    [[ ! -d build ]] && mkdir build
    cd build
    echo '$ cmake ..'
    cmake ..
    echo "$ make -j$MAKE_PROC_LIMIT"
    travis_wait 150 make -j$MAKE_PROC_LIMIT
    ctest -j$MAKE_PROC_LIMIT -L unit_tests -V -T Test
else # linux
    echo 'Detected Linux, building in Docker.'
    # Testing new core counts.
    if [[ $IMAGE_TAG == "ubuntu-18.04" ]]; then
        echo $($MAKE_PROC_LIMIT)
        travis_wait 40 execute docker run --rm -v $(pwd):/workdir -v /usr/lib/ccache -v $HOME/.ccache:/opt/.ccache -e CCACHE_DIR=/opt/.ccache -e MAKE_PROC_LIMIT eosio/producer:ci-ubuntu-18.04
    else
        travis_wait 40 execute docker run --rm -v $(pwd):/workdir -v /usr/lib/ccache -v $HOME/.ccache:/opt/.ccache -e CCACHE_DIR=/opt/.ccache ${FULL_TAG}
    fi
fi
