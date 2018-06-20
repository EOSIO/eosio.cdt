cmake_minimum_required(VERSION 3.5)
project(eosio_wasm_sdk VERSION 0.0.0)

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

add_subdirectory(external)

set(WASM_SDK_BUILD true)

configure_file(${CMAKE_SOURCE_DIR}/eosio.imports.in ${CMAKE_BINARY_DIR}/eosio.imports)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
include(EosioWasmToolchain)

add_subdirectory(libraries)

#add_subdirectory(examples)

# install cppeos wrapper
configure_file(${CMAKE_SOURCE_DIR}/cppeos.in ${CMAKE_BINARY_DIR}/cppeos)
install(FILES ${CMAKE_BINARY_DIR}/cppeos DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# install eosio.lld wrapper
configure_file(${CMAKE_SOURCE_DIR}/eosio.lld.in ${CMAKE_BINARY_DIR}/eosio.lld)
install(FILES ${CMAKE_BINARY_DIR}/eosio.lld DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# install pfr
install(DIRECTORY libraries/pfr/include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/eosiosdk/include/)

# install examples
install(DIRECTORY examples DESTINATION "${CMAKE_INSTALL_PREFIX}/eosiosdk")

install(EXPORT EosioLib DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/eosiosdk/include)
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
install(FILES ${CMAKE_BINARY_DIR}/eosio.imports DESTINATION ${CMAKE_INSTALL_PREFIX}/eosiosdk/)
