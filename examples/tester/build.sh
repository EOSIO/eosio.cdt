#!/bin/bash
CMAKE_BINARY_DIR="../../build"
CMAKE_ARGS="-DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake"

rm -rf build
mkdir build
cd build
cmake .. "$CMAKE_ARGS"
make
