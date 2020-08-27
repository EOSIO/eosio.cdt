project(kv_addr_book)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( kv_addr_book kv_addr_book kv_addr_book.cpp )
target_include_directories( kv_addr_book PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( kv_addr_book ${CMAKE_SOURCE_DIR}/../ricardian )