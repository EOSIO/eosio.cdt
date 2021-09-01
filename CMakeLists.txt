cmake_minimum_required(VERSION 3.16)

project(blanc)

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


set(VERSION_MAJOR 0)
set(VERSION_MINOR 10)
set(VERSION_PATCH 0)

if (VERSION_SUFFIX)
  set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SUFFIX}")
else()
  set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
endif()

set(BLANC_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/opt/${CMAKE_PROJECT_NAME}/${VERSION_FULL})

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)

find_package(LLVM 12 REQUIRED CONFIG)
find_package(Clang REQUIRED)

list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR})
include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)
include(BlancExecutableNames)

if(UNIX AND NOT APPLE)
  add_compile_options(-Wno-deprecated-enum-enum-conversion)
endif()

add_subdirectory(plugins)
add_subdirectory(tools)
add_subdirectory(modules)

include(LibrariesExternalProject)

configure_file(${CMAKE_SOURCE_DIR}/imports/eosio.imports.in ${CMAKE_BINARY_DIR}/eosio.imports COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/eosio.imports DESTINATION ${BLANC_INSTALL_PREFIX})

configure_file(${CMAKE_SOURCE_DIR}/scripts/ricardeos/ricardeos.py ${CMAKE_BINARY_DIR}/scripts/ricardeos.py COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/scripts/ricardeos.py DESTINATION ${BLANC_INSTALL_PREFIX}/scripts)

# section for package construction
set(VENDOR "Jeeyong Um")
set(PROJECT_NAME "${CMAKE_PROJECT_NAME}")
set(DESC "Toolchain for WebAssembly-based blockchain contracts")
set(URL "https://github.com/turnpike/blanc")
set(EMAIL "conr2d@gmail.com")
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_package.sh.in ${CMAKE_BINARY_DIR}/packages/generate_package.sh @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_deb.sh ${CMAKE_BINARY_DIR}/packages/generate_deb.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_bottle.sh ${CMAKE_BINARY_DIR}/packages/generate_bottle.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_rpm.sh ${CMAKE_BINARY_DIR}/packages/generate_rpm.sh COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_tarball.sh.in ${CMAKE_BINARY_DIR}/packages/generate_tarball.sh @ONLY)

# add licenses
configure_file(${CMAKE_SOURCE_DIR}/libraries/boost/boost.license ${CMAKE_BINARY_DIR}/licenses/boost.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/tools/external/wabt/LICENSE ${CMAKE_BINARY_DIR}/licenses/wabt.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/LICENSE ${CMAKE_BINARY_DIR}/licenses/${CMAKE_PROJECT_NAME}.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/internal/include/picojson/LICENSE ${CMAKE_BINARY_DIR}/licenses/picojson.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/internal/include/tsl/LICENSE ${CMAKE_BINARY_DIR}/licenses/tsl-ordered-map.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/internal/include/uint128_t/LICENSE ${CMAKE_BINARY_DIR}/licenses/uint128_t.license COPYONLY)
install(DIRECTORY ${CMAKE_BINARY_DIR}/licenses DESTINATION ${BLANC_INSTALL_PREFIX})

set(BUILD_TESTS FALSE CACHE BOOL "Build unit tests")

if(BUILD_TESTS)
  include(TestsExternalProject)

  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()

install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION ${BLANC_INSTALL_PREFIX}/include)
install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/ DESTINATION ${BLANC_INSTALL_PREFIX}/lib)
