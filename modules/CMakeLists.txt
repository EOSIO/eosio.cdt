### Configure the EosioWasmToolchain.cmakes
set(BLANC_ROOT_DIR ${CMAKE_BINARY_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio.cdt-config.cmake ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_NAME}-config.cmake @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/EosioCDTMacros.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/EosioCDTMacros.cmake @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/EosioWasmToolchain.cmake @ONLY)

set(BLANC_ROOT_DIR ${BLANC_INSTALL_PREFIX})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio.cdt-config.cmake ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config.cmake @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/EosioCDTMacros.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/EosioCDTMacros.cmake @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/EosioWasmToolchain.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/EosioWasmToolchain.cmake @ONLY)

set(BLANC_ROOT_DIR "_PREFIX_")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio.cdt-config.cmake ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config.cmake.package @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/EosioCDTMacros.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/EosioCDTMacrosPackage.cmake @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/EosioWasmToolchain.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/EosioWasmToolchainPackage.cmake @ONLY)

install(
  FILES 
    ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/EosioCDTMacros.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/EosioWasmToolchain.cmake
  DESTINATION
    ${BLANC_INSTALL_PREFIX}/lib/cmake/${CMAKE_PROJECT_NAME}
)

