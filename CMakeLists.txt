cmake_minimum_required(VERSION 3.5)
project(eosio_wasm_sdk)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 2)
set(VERSION_PATCH 1)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   message(WARNING "CMAKE_INSTALL_PREFIX is set to default path of ${CMAKE_INSTALL_PREFIX}, resetting to ${CMAKE_INSTALL_PREFIX}/eosio.cdt")
   set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/eosio.cdt")
elseif ("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
   message(WARNING "CMAKE_INSTALL_PREFIX is explicitly set to /usr/local. This is not recommended.")
endif()

include(GNUInstallDirs)

include(ClangExternalProject.txt)

add_subdirectory(external)

set(WASM_SDK_BUILD true)

### Configure the EosioWasmToolchain.cmakes
set(SYSROOT_DIR ${CMAKE_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake @ONLY)

set(SYSROOT_DIR ${CMAKE_INSTALL_PREFIX})
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchain.cmake @ONLY)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt")
include(EosioWasmToolchain)

if ("${CORE_SYMBOL_NAME}" STREQUAL "")
  set( CORE_SYMBOL_NAME "SYS" )
endif()
string(TOUPPER ${CORE_SYMBOL_NAME} CORE_SYMBOL_NAME)

string(LENGTH ${CORE_SYMBOL_NAME} CORE_SYMBOL_NAME_LENGTH)
if (CORE_SYMBOL_NAME_LENGTH GREATER 7)
  message(FATAL_ERROR "CORE_SYMBOL_NAME length must be between 1 and 7 characters")
endif()


add_subdirectory(libraries)

include(InstallClang.txt)

message( STATUS "Using '${CORE_SYMBOL_NAME}' as CORE symbol name" )

# install examples
#install(DIRECTORY examples DESTINATION "${CMAKE_INSTALL_PREFIX}")

install(FILES ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchain.cmake DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/eosio.cdt)

configure_file(${CMAKE_SOURCE_DIR}/modules/eosio.cdt-config.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/eosio.cdt-config.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/eosio.cdt-config.cmake DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/eosio.cdt)

configure_file(${CMAKE_SOURCE_DIR}/eosio.imports.in ${CMAKE_BINARY_DIR}/eosio.imports COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/eosio.imports DESTINATION ${CMAKE_INSTALL_PREFIX})

configure_file(${CMAKE_SOURCE_DIR}/scripts/ricardeos/ricardeos.py ${CMAKE_BINARY_DIR}/scripts/ricardeos.py COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/scripts/ricardeos.py DESTINATION ${CMAKE_INSTALL_PREFIX}/scripts)
