cmake_minimum_required(VERSION 3.5)
project(eosio_wasm_sdk VERSION 0.0.0)

include(GNUInstallDirs)

set(SDK_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/eosio.wasmsdk)

if(NOT ${NO_WASM_COMPILER} STREQUAL "")
   string(TOLOWER ${NO_WASM_COMPILER} no_wasm_comp)
   if(NOT ${no_wasm_comp} STREQUAL "true")
      ### include eosio clang 7 as external project
      include(ClangExternalProject.txt)
   else(NOT ${no_wasm_comp} STREQUAL "true")
      add_custom_target(EosioClang)
   endif()
else(NOT ${NO_WASM_COMPILER} STREQUAL "")
   set(no_wasm_comp "false")
   include(ClangExternalProject.txt)
endif()
#include(ToolingExternalProject.txt)

add_subdirectory(external)

set(WASM_SDK_BUILD true)

configure_file(${CMAKE_SOURCE_DIR}/eosio.imports.in ${CMAKE_BINARY_DIR}/eosio.imports)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
include(EosioWasmToolchainBuild)

if ("${CORE_SYMBOL_NAME}" STREQUAL "")
  set( CORE_SYMBOL_NAME "SYS" )
endif()
string(TOUPPER ${CORE_SYMBOL_NAME} CORE_SYMBOL_NAME)

string(LENGTH ${CORE_SYMBOL_NAME} CORE_SYMBOL_NAME_LENGTH)
if (CORE_SYMBOL_NAME_LENGTH GREATER 7)
  message(FATAL_ERROR "CORE_SYMBOL_NAME length must be between 1 and 7 characters")
endif()


add_subdirectory(libraries)

if ( ${no_wasm_comp} STREQUAL "false" )
   include(InstallClang.txt)
endif()

message( STATUS "Using '${CORE_SYMBOL_NAME}' as CORE symbol name" )

# install examples
install(DIRECTORY examples DESTINATION "${SDK_INSTALL_PREFIX}")

install(EXPORT EosioLib DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)

set(INCLUDE_INSTALL_DIR ${SDK_INSTALL_PREFIX}/include)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(EosioLibConfigVersion.cmake 
   COMPATIBILITY SameMajorVersion)

configure_package_config_file(${CMAKE_SOURCE_DIR}/modules/EosioLibConfig.cmake.in EosioLibConfig.cmake
   INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/"
   PATH_VARS INCLUDE_INSTALL_DIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EosioLibConfigVersion.cmake
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EosioLibConfig.cmake
   DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)
install(FILES ${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)
install(FILES ${CMAKE_BINARY_DIR}/eosio.imports DESTINATION ${SDK_INSTALL_PREFIX})
