cmake_minimum_required(VERSION 3.5)

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
set(VERSION_MINOR 8)
set(VERSION_PATCH 1)

if (VERSION_SUFFIX)
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SUFFIX}")
else()
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
endif()

set(CDT_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_PROJECT_NAME})

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/modules)

include(BlancExecutableNames)
include(BlancPluginsExternalProject)
include(ToolsExternalProject)

if(APPLE)
   set(CMAKE_C_COMPILER "${BREW_LLVM_PATH}/bin/clang")
   set(CMAKE_CXX_COMPILER "${BREW_LLVM_PATH}/bin/clang++")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DARWIN_C_SOURCE")
endif()

### Configure the EosioWasmToolchain.cmakes
set(CDT_ROOT_DIR ${CMAKE_BINARY_DIR})

configure_file(${CMAKE_SOURCE_DIR}/modules/blanc-config.cmake ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_NAME}-config.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/EosioCDTMacros.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/EosioWasmToolchain.cmake @ONLY)

set(CDT_ROOT_DIR ${CDT_INSTALL_PREFIX})

configure_file(${CMAKE_SOURCE_DIR}/modules/blanc-config.cmake ${CMAKE_BINARY_DIR}/modules/${CMAKE_PROJECT_NAME}-config.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioCDTMacros.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchain.cmake @ONLY)

set(CDT_ROOT_DIR "_PREFIX_")
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioCDTMacrosPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchainPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/blanc-config.cmake ${CMAKE_BINARY_DIR}/modules/${CMAKE_PROJECT_NAME}-config.cmake.package @ONLY)

include(LibrariesExternalProject)

include(InstallBlanc)

configure_file(${CMAKE_SOURCE_DIR}/imports/eosio.imports.in ${CMAKE_BINARY_DIR}/eosio.imports COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/eosio.imports DESTINATION ${CDT_INSTALL_PREFIX})

configure_file(${CMAKE_SOURCE_DIR}/scripts/ricardeos/ricardeos.py ${CMAKE_BINARY_DIR}/scripts/ricardeos.py COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/scripts/ricardeos.py DESTINATION ${CDT_INSTALL_PREFIX}/scripts)

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
configure_file(${CMAKE_SOURCE_DIR}/tools/jsoncons/LICENSE ${CMAKE_BINARY_DIR}/licenses/jsoncons.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/LICENSE ${CMAKE_BINARY_DIR}/licenses/${CMAKE_PROJECT_NAME}.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/cosmwasm/cosmwasm/picojson/LICENSE ${CMAKE_BINARY_DIR}/licenses/picojson.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/cosmwasm/cosmwasm/uint128_t/LICENSE ${CMAKE_BINARY_DIR}/licenses/uint128_t.license COPYONLY)

set(BUILD_TESTS FALSE CACHE BOOL "Build unit tests")

if(BUILD_TESTS)
  include(TestsExternalProject)

  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()
