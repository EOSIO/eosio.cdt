cmake_minimum_required(VERSION 3.5)
project(exchange_example VERSION 1.0.0)

# if no wasm root is given use default path
if(EOSIO_WASMSDK_ROOT STREQUAL "" OR NOT EOSIO_WASMSDK_ROOT)
   set(EOSIO_WASMSDK_ROOT ${CMAKE_INSTALL_PREFIX})
endif()

# append the path to the module to include
list(APPEND CMAKE_MODULE_PATH ${EOSIO_WASMSDK_ROOT}/lib/cmake)

#include the toolchain cmake
include(EosioWasmToolchain)

add_library(exchange_lib 
   ${CMAKE_SOURCE_DIR}/exchange_accounts.cpp
   ${CMAKE_SOURCE_DIR}/exchange_state.cpp
   ${CMAKE_SOURCE_DIR}/market_state.cpp)
target_include_directories( exchange_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ )

add_executable(exchange exchange.cpp )
target_include_directories( exchange PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ )
target_link_libraries( exchange exchange_lib)
