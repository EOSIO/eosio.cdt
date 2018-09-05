cmake_minimum_required(VERSION 3.5)
project(template_example VERSION 1.0.0)

# if no wasm root is given use default path
if(EOSIO_CDT_ROOT STREQUAL "" OR NOT EOSIO_CDT_ROOT)
   set(EOSIO_CDT_ROOT ${CMAKE_INSTALL_PREFIX}/eosio.cdt)
endif()

# append the path to the module to include
list(APPEND CMAKE_MODULE_PATH ${EOSIO_CDT_ROOT}/lib/cmake)

#include the toolchain cmake
include(EosioWasmToolchain)
