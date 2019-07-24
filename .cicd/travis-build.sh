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
    docker run --rm -v $(pwd):/workdir -e MAKE_PROC_LIMIT eosio/producer:ci-ubuntu-18.04-cdt
fi
