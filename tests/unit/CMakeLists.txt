list(APPEND CMAKE_MODULE_PATH ${EOSIO_CDT_BIN})
include(EosioCDTMacros)

add_native_executable(name_tests name_tests.cpp)
add_native_executable(system_tests system_tests.cpp)
add_native_executable(rope_tests rope_tests.cpp)
add_native_executable(tree_tests tree_tests.cpp)

add_subdirectory(test_contracts)
