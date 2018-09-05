cmake_minimum_required(VERSION 3.5)
project(hello_example VERSION 1.0.0)

# if no wasm root is given use default path
if(EOSIO_WASMSDK_ROOT STREQUAL "" OR NOT EOSIO_WASMSDK_ROOT)
   set(EOSIO_WASMSDK_ROOT "/usr/local/eosio.wasmsdk")
endif()

# append the path to the module to include
list(APPEND CMAKE_MODULE_PATH ${EOSIO_WASMSDK_ROOT}/lib/cmake)

#include the toolchain cmake
include(EosioWasmToolchain)

set(CMAKE_CXX_FLAGS " -abigen ${CMAKE_CXX_FLAGS} ")
add_executable( test.wasm test.cpp )
