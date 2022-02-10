cmake_minimum_required(VERSION 3.16)

# Sanity check our source directory to make sure that we are not trying to
# generate an in-source build, and to make
# sure that we don't have any stray generated files lying around in the tree
if( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed.
Please create a directory and run cmake from there, passing the path
to this source directory as the last argument.
This process created the file `CMakeCache.txt' and the directory `CMakeFiles'.
Please delete them.")
endif()

project(eosio.cdt)
include(FetchContent)

find_program(SCCACHE_FOUND sccache)
if (SCCACHE_FOUND)
  message(STATUS "Using sccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE sccache)
else()
  find_program(CCACHE_FOUND ccache)
  if (CCACHE_FOUND)
    message(STATUS "Using ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  endif()
endif()

set(VERSION_MAJOR 1)
set(VERSION_MINOR 8)
set(VERSION_PATCH 1)
#set(VERSION_SUFFIX rc2)

if (VERSION_SUFFIX)
  set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SUFFIX}")
else()
  set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
endif()

#

set(LIBS_BUILD_TYPE "Release" CACHE STRING "The build type for libraries")

# compatible eosio.cdt version

set(CDT_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/eosio.cdt)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)

find_package(LLVM 13 REQUIRED CONFIG)
find_package(Clang REQUIRED)

set(BUILD_TESTS FALSE CACHE BOOL "Build unit tests")

list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR} ${CMAKE_SOURCE_DIR}/modules)

include(AddLLVM)
include(fetch_content)
include(BlancExecutableNames)

fetch_content(
  llvm
  URL            https://github.com/llvm/llvm-project/archive/refs/tags/llvmorg-${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.0.tar.gz
  URL_HASH       SHA256=a1131358f1f9f819df73fa6bff505f2c49d176e9eef0a3aedd1fdbce3b4630e8          
)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_content(
  wabt
  GIT_REPOSITORY https://github.com/WebAssembly/wabt
  GIT_TAG        830d32a41449278cacb0bf17530618d47c43340b       
)

fetch_content(
  jsoncons
  GIT_REPOSITORY https://github.com/danielaparker/jsoncons
  GIT_TAG        4438d5c9d2f3a9a72455adcdb3a73c7778e9b48d       
)

fetch_content(
  ut
  GIT_REPOSITORY https://github.com/boost-ext/ut
  GIT_TAG        v1.1.8       
)


if(UNIX AND NOT APPLE)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-Wno-deprecated-enum-enum-conversion)
  endif()
endif()

add_subdirectory(plugins)
add_subdirectory(tools)
add_subdirectory(modules)

include(LibrariesExternalProject)

configure_file(${CMAKE_SOURCE_DIR}/imports/eosio.imports.in ${CMAKE_BINARY_DIR}/eosio.imports COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/eosio.imports DESTINATION ${CDT_INSTALL_PREFIX})

configure_file(${CMAKE_SOURCE_DIR}/scripts/ricardeos/ricardeos.py ${CMAKE_BINARY_DIR}/scripts/ricardeos.py COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/scripts/ricardeos.py DESTINATION ${CDT_INSTALL_PREFIX}/scripts)

# section for package construction
set(VENDOR "block.one")
set(PROJECT_NAME "eosio.cdt")
set(DESC "Toolchain and supporting tools for the EOS.IO platform")
set(URL "https://github.com/eosio/eosio.cdt")
set(EMAIL "support@block.one")
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_package.sh.in ${CMAKE_BINARY_DIR}/packages/generate_package.sh @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_deb.sh ${CMAKE_BINARY_DIR}/packages/generate_deb.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_bottle.sh ${CMAKE_BINARY_DIR}/packages/generate_bottle.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_rpm.sh ${CMAKE_BINARY_DIR}/packages/generate_rpm.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_tarball.sh.in ${CMAKE_BINARY_DIR}/packages/generate_tarball.sh @ONLY)

# add licenses
# configure_file(${CMAKE_SOURCE_DIR}/libraries/boost/boost.license ${CMAKE_BINARY_DIR}/licenses/boost.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/meta_refl/LICENSE ${CMAKE_BINARY_DIR}/licenses/meta_refl.license COPYONLY)
configure_file(${wabt_SOURCE_DIR}/LICENSE ${CMAKE_BINARY_DIR}/licenses/wabt.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/LICENSE ${CMAKE_BINARY_DIR}/licenses/${CMAKE_PROJECT_NAME}.license COPYONLY)
configure_file(${jsoncons_SOURCE_DIR}/LICENSE ${CMAKE_BINARY_DIR}/licenses/jsoncons.license COPYONLY)
install(DIRECTORY ${CMAKE_BINARY_DIR}/licenses DESTINATION ${CDT_INSTALL_PREFIX})

if(BUILD_TESTS)
  include(TestsExternalProject)

  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()

# install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION ${CDT_INSTALL_PREFIX}/include)
# install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION ${CDT_INSTALL_PREFIX}/lib)
