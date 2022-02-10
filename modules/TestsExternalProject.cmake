include(ExternalProject)
include(GNUInstallDirs)

ExternalProject_Add(
  EosioWasmTests
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/tests/unit"
  BINARY_DIR "${CMAKE_BINARY_DIR}/tests/unit"
  CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/EosioWasmToolchain.cmake 
             -DCMAKE_BUILD_TYPE=Debug 
             -DEOSIO_CDT_BIN=${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/ 
             -DBASE_BINARY_DIR=${CMAKE_BINARY_DIR} 
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
        CMAKE_ARGS -DCMAKE_BUILD_TYPE=${TEST_BUILD_TYPE} 
                   -DCMAKE_FRAMEWORK_PATH=${TEST_FRAMEWORK_PATH} 
                   -DCMAKE_MODULE_PATH=${TEST_MODULE_PATH} 
                   -DEOSIO_ROOT=${EOSIO_ROOT} 
                   -DLLVM_DIR=${LLVM_DIR} 
                   -DBOOST_ROOT=${BOOST_ROOT}
        UPDATE_COMMAND ""
        PATCH_COMMAND  ""
        TEST_COMMAND   ""
        INSTALL_COMMAND ""
        BUILD_ALWAYS 1
        DEPENDS EosioWasmLibraries EosioTools
      )
endif()
