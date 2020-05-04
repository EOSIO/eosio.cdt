cmake_minimum_required(VERSION 3.5)

project(eosio_libraries)

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
    message(STATUS "Using ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

list(APPEND CMAKE_MODULE_PATH ${EOSIO_CDT_BIN})
include(EosioCDTMacros)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -Wall -no-missing-ricardian-clause")

add_subdirectory(libc)
add_subdirectory(libc++)
add_subdirectory(eosiolib)
add_subdirectory(boost)
# TODO re-enable after more CMake fixes
#add_subdirectory(native)
add_subdirectory(rt)

set(ABIEOS_PIC OFF CACHE BOOL "no pic abieos" FORCE)
set(ABIEOS_ASSERT_ERRORS ON CACHE BOOL "assert on errors" FORCE)
set(ABIEOS_ONLY_LIBRARY ON CACHE BOOL "only use the library" FORCE)
add_subdirectory(abieos)
