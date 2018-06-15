cmake_minimum_required(VERSION 3.5)
project(eosio_wasm_sdk)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/opt/eosio-wasm")
endif()

### include eosio clang 7 as external project
include(ClangExternalProject.txt)
set(WASM_SDK_BUILD true)

install(FILES ${CMAKE_SOURCE_DIR}/eosio.imports DESTINATION ${CMAKE_INSTALL_PREFIX})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
include(EosioWasmToolchain)

add_subdirectory(libraries)

add_subdirectory(examples)
