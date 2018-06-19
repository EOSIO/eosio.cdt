cmake_minimum_required(VERSION 3.5)
project(eosio_wasm_sdk VERSION 0.0.0)

set(INSTALL_BASE ${CMAKE_INSTALL_PREFIX})
#if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
#endif()

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

configure_file(${CMAKE_SOURCE_DIR}/eosio.imports.in ${CMAKE_INSTALL_BINDIR}/eosio.imports)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
include(EosioWasmToolchain)

add_subdirectory(libraries)

add_subdirectory(examples)

# install the cmake modules
install(DIRECTORY modules DESTINATION "${CMAKE_INSTALL_BINDIR}")

# install cppeos wrapper
configure_file(${CMAKE_SOURCE_DIR}/cppeos.in ${CMAKE_BINARY_DIR}/cppeos)
install(FILES ${CMAKE_BINARY_DIR}/cppeos DESTINATION ${INSTALL_BASE}/bin PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# install eosio.lld wrapper
configure_file(${CMAKE_SOURCE_DIR}/eosio.lld.in ${CMAKE_BINARY_DIR}/eosio.lld)
install(FILES ${CMAKE_BINARY_DIR}/eosio.lld DESTINATION ${INSTALL_BASE}/bin PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

# install pfr
install(DIRECTORY libraries/pfr/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# install examples
install(DIRECTORY examples DESTINATION "${CMAKE_INSTALL_BINDIR}")

install(EXPORT EosioLib DESTINATION ${CMAKE_INSTALL_BINDIR}/modules)

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
include(CMakePackageConfigHelpers)
write_basic_package_version_file(EosioLibConfigVersion.cmake 
   COMPATIBILITY SameMajorVersion)

configure_package_config_file(${CMAKE_SOURCE_DIR}/modules/EosioLibConfig.cmake.in EosioLibConfig.cmake
   INSTALL_DESTINATION "${CMAKE_INSTALL_BINDIR}/modules/"
   PATH_VARS INCLUDE_INSTALL_DIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EosioLibConfigVersion.cmake
   DESTINATION ${CMAKE_INSTALL_BINDIR}/modules)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/EosioLibConfig.cmake
   DESTINATION ${CMAKE_INSTALL_BINDIR}/modules)
