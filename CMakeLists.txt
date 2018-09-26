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

configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchainBuild.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/EosioWasmToolchain.cmake @ONLY)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/lib/cmake")
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

configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchainPackage.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchainPackage.cmake @ONLY)
configure_file(${CMAKE_SOURCE_DIR}/modules/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchain.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/modules/EosioWasmToolchain.cmake DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/cmake)
configure_file(${CMAKE_SOURCE_DIR}/eosio.imports.in ${CMAKE_BINARY_DIR}/eosio.imports COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/eosio.imports DESTINATION ${CMAKE_INSTALL_PREFIX})

configure_file(${CMAKE_SOURCE_DIR}/scripts/ricardeos/ricardeos.py ${CMAKE_BINARY_DIR}/scripts/ricardeos.py COPYONLY)
install(FILES ${CMAKE_BINARY_DIR}/scripts/ricardeos.py DESTINATION ${CMAKE_INSTALL_PREFIX}/scripts)

configure_file(${CMAKE_SOURCE_DIR}/scripts/generate_package.sh.in ${CMAKE_SOURCE_DIR}/packages/generate_package.sh @ONLY)

include(InstallRequiredSystemLibraries)

#install_directory_permissions( DIRECTORY usr/${CMAKE_INSTALL_INCLUDEDIR}/eosio )

set(CPACK_PACKAGE_CONTACT "support@block.one")
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_BINARY_DIR}/packages)
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
endif()

SET(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/install")
set(CPACK_PACKAGE_NAME "EOS.IO")
set(CPACK_PACKAGE_VENDOR "block.one")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION "Software for the EOS.IO network")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Software for the EOS.IO network")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "EOS.IO ${CPACK_PACKAGE_VERSION}")

set(CPACK_GENERATOR "RPM")
set(CPACK_RPM_PACKAGE_RELEASE 0)
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_URL "https://github.com/EOSIO/eos")

include(CPack)
