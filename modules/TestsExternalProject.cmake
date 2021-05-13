include(ExternalProject)
include(GNUInstallDirs)

ExternalProject_Add(
  EosioWasmTests
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/tests/unit"
  BINARY_DIR "${CMAKE_BINARY_DIR}/tests/unit"
  CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake -DCMAKE_BUILD_TYPE=Release -DEOSIO_CDT_BIN=${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/ -DBASE_BINARY_DIR=${CMAKE_BINARY_DIR} -D__APPLE=${APPLE} -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH} -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH} -Deosio_DIR=${eosio_DIR}
  UPDATE_COMMAND ""
  PATCH_COMMAND  ""
  TEST_COMMAND   ""
  INSTALL_COMMAND ""
  BUILD_ALWAYS 1
  DEPENDS EosioWasmLibraries EosioTools
)


if (EOSIO_RUN_INTEGRATION_TESTS)
      ExternalProject_Add(
        EosioIntegrationTests
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/tests/integration"
        BINARY_DIR "${CMAKE_BINARY_DIR}/tests/integration"
        CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake -DCMAKE_BUILD_TYPE=Debug -DEOSIO_CDT_BIN=${CMAKE_BINARY_DIR}/lib/cmake/eosio.cdt/ -DBASE_BINARY_DIR=${CMAKE_BINARY_DIR} -D__APPLE=${APPLE}
        UPDATE_COMMAND ""
        PATCH_COMMAND  ""
        TEST_COMMAND   ""
        INSTALL_COMMAND ""
        BUILD_ALWAYS 1
        DEPENDS EosioWasmLibraries EosioTools
      )
endif()
