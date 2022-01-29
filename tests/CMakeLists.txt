find_program(SCCACHE_FOUND sccache)
if (SCCACHE_FOUND)
   message(STATUS "Using sccache")
   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE sccache)
else()
   find_program(CCACHE_FOUND ccache)
   if (CCACHE_FOUND)
      message(STATUS "Using ccache")
      set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
   endif()
endif()

macro(add_unit_test TEST_NAME)
   add_test( ${TEST_NAME} ${CMAKE_BINARY_DIR}/tests/unit/${TEST_NAME} )
   set_property(TEST ${TEST_NAME} PROPERTY LABELS unit_tests)
endmacro()

add_unit_test( asset_tests )
add_unit_test( binary_extension_tests )
add_unit_test( crypto_tests )
add_unit_test( datastream_tests )
add_unit_test( fixed_bytes_tests )
add_unit_test( name_tests )
add_unit_test( rope_tests )
add_unit_test( print_tests )
add_unit_test( serialize_tests )
add_unit_test( string_tests1 )
add_unit_test( string_tests2 )
add_unit_test( symbol_tests )
add_unit_test( system_tests )
add_unit_test( time_tests )
add_unit_test( varint_tests )

add_test( NAME toolchain_tests COMMAND ${CMAKE_BINARY_DIR}/tools/toolchain-tester/toolchain-tester ${CMAKE_SOURCE_DIR}/tests/toolchain --cdt ${CMAKE_BINARY_DIR}/bin )
set_property(TEST toolchain_tests PROPERTY LABELS toolchain_tests)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/unit/version_tests.sh ${CMAKE_BINARY_DIR}/tests/unit/version_tests.sh COPYONLY)
add_test(NAME version_tests COMMAND ${CMAKE_BINARY_DIR}/tests/unit/version_tests.sh "${VERSION_FULL}" WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
set_property(TEST version_tests PROPERTY LABELS unit_tests)


if (eosio_FOUND)

   if (NOT eosio_FOUND)
      message(WARNING "Could not find eosio")
   endif()

   add_test(integration_tests ${EOSIO_ROOT}/bin/eosio-tester ${CMAKE_BINARY_DIR}/tests/integration/integration_tests.wasm)
   set_property(TEST integration_tests PROPERTY LABELS integration_tests)

   add_test(
      NAME tester_tests
      COMMAND ${EOSIO_ROOT}/bin/eosio-tester ${CMAKE_BINARY_DIR}/tests/tester/tester.wasm
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests/tester
   )
   set_property(TEST tester_tests PROPERTY LABELS tester_tests)
endif()

ExternalProject_Add(
   TesterTests
   SOURCE_DIR "${CMAKE_SOURCE_DIR}/tests/tester"
   BINARY_DIR "${CMAKE_BINARY_DIR}/tests/tester"
   CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/EosioWasmToolchain.cmake -DEOSIO_CDT_BIN=${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/ -DBASE_BINARY_DIR=${CMAKE_BINARY_DIR} -D__APPLE=${APPLE}
   UPDATE_COMMAND ""
   PATCH_COMMAND  ""
   TEST_COMMAND   ""
   INSTALL_COMMAND ""
   BUILD_ALWAYS 1
   DEPENDS EosioTools EosioWasmLibraries
)
