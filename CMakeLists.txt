cmake_minimum_required(VERSION 3.5)
project(eosio_wasm_sdk)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)


include(GNUInstallDirs)

include(ClangExternalProject.txt)

add_subdirectory(external)

set(WASM_SDK_BUILD true)

configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchainBuild.cmake.in ${CMAKE_BINARY_DIR}/cmake/EosioWasmToolchain.cmake @ONLY)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/cmake")
include(EosioWasmToolchain)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

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

configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchain.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchain.cmake DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake)
install(FILES ${CMAKE_BINARY_DIR}/eosio.imports DESTINATION ${CMAKE_INSTALL_PREFIX})
