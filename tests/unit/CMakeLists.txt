list(APPEND CMAKE_MODULE_PATH ${EOSIO_CDT_BIN})
include(EosioCDTMacros)

add_native_executable(name_tests name_tests.cpp)
add_dependencies(name_tests EosioTools)
