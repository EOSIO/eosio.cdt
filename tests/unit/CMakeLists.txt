list(APPEND CMAKE_MODULE_PATH ${EOSIO_CDT_BIN})
include(EosioCDTMacros)

add_dependencies(asset_tests EosioTools)
add_dependencies(crypto_tests EosioTools)
add_dependencies(name_tests EosioTools)
add_dependencies(rope_tests EosioTools)
add_dependencies(symbol_tests EosioTools)
add_dependencies(system_tests EosioTools)

add_native_executable(asset_tests asset_tests.cpp)
add_native_executable(crypto_tests crypto_tests.cpp)
add_native_executable(name_tests name_tests.cpp)
add_native_executable(rope_tests rope_tests.cpp)
add_native_executable(symbol_tests symbol_tests.cpp)
add_native_executable(system_tests system_tests.cpp)

add_subdirectory(test_contracts)
