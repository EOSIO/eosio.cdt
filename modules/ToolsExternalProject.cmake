include(ExternalProject)
include(GNUInstallDirs)

ExternalProject_Add(
  toolchain-tester
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/tools/toolchain-tester"
  BINARY_DIR "${CMAKE_BINARY_DIR}/tools/toolchain-tester"
  UPDATE_COMMAND ""
  PATCH_COMMAND  ""
  TEST_COMMAND   ""
  INSTALL_COMMAND ""
  BUILD_ALWAYS 1
)
