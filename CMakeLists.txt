cmake_minimum_required(VERSION 3.5)
project(eosio_wasm_sdk)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/opt/eosio-wasm")
endif()

### include eosio clang 7 as external project
include(ClangExternalProject.txt)
set(WASM_SDK_BUILD true)

configure_file(${CMAKE_SOURCE_DIR}/eosio.imports.in ${CMAKE_INSTALL_PREFIX}/eosio.imports)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
include(EosioWasmToolchain)

add_subdirectory(libraries)

add_subdirectory(examples)

install(DIRECTORY modules DESTINATION "${CMAKE_INSTALL_PREFIX}")

configure_file(${CMAKE_SOURCE_DIR}/cppeos.in ${CMAKE_BINARY_DIR}/cppeos)
install(FILES ${CMAKE_BINARY_DIR}/cppeos DESTINATION ${CMAKE_INSTALL_PREFIX}/bin PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

configure_file(${CMAKE_SOURCE_DIR}/eosio.lld.in ${CMAKE_BINARY_DIR}/eosio.lld)
install(FILES ${CMAKE_BINARY_DIR}/eosio.lld DESTINATION ${CMAKE_INSTALL_PREFIX}/bin PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
