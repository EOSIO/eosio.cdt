cmake_minimum_required(VERSION 3.5)
project(eosio_wasm_sdk)

### include eosio clang 7 as external project
include(ClangExternalProject.txt)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
include(EosioWasmToolchain)

add_subdirectory(libraries)

#add_subdirectory(hello)
#add_subdirectory(exchange)
#add_subdirectory(test)
