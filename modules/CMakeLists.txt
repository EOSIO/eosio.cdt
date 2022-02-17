### Configure the EosioWasmToolchain.cmakes
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio.cdt-config.cmake ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_NAME}-config.cmake @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/EosioCDTMacros.cmake ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/EosioCDTMacros.cmake COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/EosioWasmToolchain.cmake.in ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/EosioWasmToolchain.cmake @ONLY)

install(
  FILES 
    ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/${CMAKE_PROJECT_NAME}-config.cmake
    ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/EosioCDTMacros.cmake
    ${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/EosioWasmToolchain.cmake
  DESTINATION
    ${CDT_INSTALL_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}
)

