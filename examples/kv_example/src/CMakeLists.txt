project(kv_example)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( kv_example kv_example kv_example.cpp )
target_include_directories( kv_example PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( kv_example ${CMAKE_SOURCE_DIR}/../ricardian )