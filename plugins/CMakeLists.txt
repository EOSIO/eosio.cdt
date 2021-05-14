cmake_minimum_required(VERSION 3.15)

project(blanc_plugins)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../modules)

find_package(LLVM 12 REQUIRED CONFIG)
find_package(Clang REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR})
include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_subdirectory(eosio)
add_subdirectory(cosmwasm)
