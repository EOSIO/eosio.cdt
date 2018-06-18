cmake_minimum_required(VERSION 3.5)
project(eosio_wasm_sdk)

set(INSTALL_BASE ${CMAKE_INSTALL_PREFIX})
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/opt/eosio-wasm")
endif()

include(GNUInstallDirs)

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

set(WASM_SDK_BUILD true)

configure_file(${CMAKE_SOURCE_DIR}/eosio.imports.in ${CMAKE_INSTALL_PREFIX}/eosio.imports)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
include(EosioWasmToolchain)

add_subdirectory(libraries)

add_subdirectory(examples)

# install the cmake modules
install(DIRECTORY modules DESTINATION "${CMAKE_INSTALL_PREFIX}")

# install cppeos wrapper
configure_file(${CMAKE_SOURCE_DIR}/cppeos.in ${CMAKE_BINARY_DIR}/cppeos)
install(FILES ${CMAKE_BINARY_DIR}/cppeos DESTINATION ${INSTALL_BASE}/bin PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# install eosio.lld wrapper
configure_file(${CMAKE_SOURCE_DIR}/eosio.lld.in ${CMAKE_BINARY_DIR}/eosio.lld)
install(FILES ${CMAKE_BINARY_DIR}/eosio.lld DESTINATION ${INSTALL_BASE}/bin PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# install examples
install(DIRECTORY examples DESTINATION "${CMAKE_INSTALL_PREFIX}")
