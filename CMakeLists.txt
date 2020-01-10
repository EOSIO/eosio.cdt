cmake_minimum_required(VERSION 3.5)

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

project(eosio_cdt)

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
set(VERSION_MINOR 7)
set(VERSION_PATCH 0)
#set(VERSION_SUFFIX rc1)

if (VERSION_SUFFIX)
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SUFFIX}")
else()
   set(VERSION_FULL "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
endif()

set(CDT_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/eosio.cdt)

include(GNUInstallDirs)

include(modules/ClangExternalProject.txt)
include(modules/ToolsExternalProject.txt)

set(WASM_SDK_BUILD true)

### Configure the EosioWasmToolchain.cmakes
set(CDT_ROOT_DIR ${CMAKE_BINARY_DIR})

configure_file(${CMAKE_SOURCE_DIR}/modules/eosio.cdt-config.cmake ${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/eosio.cdt-config.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/EosioCDTMacros.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake @ONLY)

set(CDT_ROOT_DIR ${CDT_INSTALL_PREFIX})

configure_file(${CMAKE_SOURCE_DIR}/modules/eosio.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/eosio.cdt-config.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioCDTMacros.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchain.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/EosioCDTMacros.cmake DESTINATION ${CDT_INSTALL_PREFIX}/lib/cmake/eosio.cdt)

set(CDT_ROOT_DIR "_PREFIX_")
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioCDTMacrosPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchainPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/eosio.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/eosio.cdt-config.cmake.package @ONLY)

include(modules/LibrariesExternalProject.txt)

include(modules/InstallCDT.cmake)

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
configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_tarball.sh ${CMAKE_BINARY_DIR}/packages/generate_tarball.sh COPYONLY)

# add licenses
configure_file(${CMAKE_SOURCE_DIR}/eosio_llvm/LICENSE.TXT ${CMAKE_BINARY_DIR}/licenses/llvm.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/libraries/boost/boost.license ${CMAKE_BINARY_DIR}/licenses/boost.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/tools/external/wabt/LICENSE ${CMAKE_BINARY_DIR}/licenses/wabt.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/tools/jsoncons/LICENSE ${CMAKE_BINARY_DIR}/licenses/jsoncons.license COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/LICENSE ${CMAKE_BINARY_DIR}/licenses/eosio.cdt.license COPYONLY)

include(modules/TestsExternalProject.txt)

include(CTest)
enable_testing()
add_subdirectory(tests)
