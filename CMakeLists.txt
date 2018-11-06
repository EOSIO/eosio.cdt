cmake_minimum_required(VERSION 3.5)
project(eosio_cdt)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 4)
set(VERSION_PATCH 0)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   message(WARNING "CMAKE_INSTALL_PREFIX is set to default path of ${CMAKE_INSTALL_PREFIX}, resetting to ${CMAKE_INSTALL_PREFIX}/eosio.cdt")
   set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/eosio.cdt")
elseif ("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
   message(WARNING "CMAKE_INSTALL_PREFIX is explicitly set to /usr/local. This is not recommended.")
endif()

include(GNUInstallDirs)

include(ClangExternalProject.txt)
include(ToolsExternalProject.txt)

add_subdirectory(external)

set(WASM_SDK_BUILD true)

### Configure the EosioWasmToolchain.cmakes
set(CDT_ROOT_DIR ${CMAKE_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/modules/eosio.cdt-config.cmake ${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/eosio.cdt-config.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake @ONLY)

set(CDT_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
configure_file(${CMAKE_SOURCE_DIR}/modules/eosio.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/eosio.cdt-config.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/eosio.cdt-config.cmake DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/eosio.cdt)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchain.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchain.cmake DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/eosio.cdt)

set(CDT_ROOT_DIR "_PREFIX_")
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchainPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/eosio.cdt-config.cmake ${CMAKE_BINARY_DIR}/modules/eosio.cdt-config.cmake.package @ONLY)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt")
include(EosioWasmToolchain)

add_subdirectory(libraries)

include(InstallClang.txt)

configure_file(${CMAKE_SOURCE_DIR}/eosio.imports.in ${CMAKE_BINARY_DIR}/eosio.imports COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/eosio.imports DESTINATION ${CMAKE_INSTALL_PREFIX})

configure_file(${CMAKE_SOURCE_DIR}/scripts/ricardeos/ricardeos.py ${CMAKE_BINARY_DIR}/scripts/ricardeos.py COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/scripts/ricardeos.py DESTINATION ${CMAKE_INSTALL_PREFIX}/scripts)

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
