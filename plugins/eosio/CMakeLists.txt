cmake_minimum_required(VERSION 3.15)

project(eosio_plugins)

set(CMAKE_CXX_STANDARD 20)

find_package(LLVM REQUIRED CONFIG)
if(LLVM_VERSION_MAJOR VERSION_LESS 11)
  message(FATAL_ERROR "Blanc requires LLVM 11 or higher")
endif()
find_package(Clang REQUIRED)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_MODULE_PATH ${LLVM_DIR})
include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_subdirectory(EosioApply)
#add_subdirectory(EosioSoftfloat)
add_subdirectory(eosio_plugin)
add_subdirectory(EosioAttrs)
