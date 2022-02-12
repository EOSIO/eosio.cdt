include(ExternalProject)

ExternalProject_Add(
  EosioWasmLibraries
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/libraries"
  BINARY_DIR "${CMAKE_BINARY_DIR}/libraries"
  CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/EosioWasmToolchain.cmake 
             -DEOSIO_CDT_BIN=${CMAKE_BINARY_DIR}/lib/cmake/${CMAKE_PROJECT_NAME}/ 
             -DBASE_BINARY_DIR=${CMAKE_BINARY_DIR}
             -Dllvm_SOURCE_DIR=${llvm_SOURCE_DIR}
             -DCMAKE_BUILD_TYPE=${LIBS_BUILD_TYPE}
             -DCMAKE_INSTALL_PREFIX=${CDT_INSTALL_PREFIX}
  UPDATE_COMMAND ""
  PATCH_COMMAND  ""
  TEST_COMMAND   ""
  INSTALL_COMMAND ""
  BUILD_ALWAYS 1
  DEPENDS EosioTools EosioPlugins
)

install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} --install .
                              WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/libraries)")


