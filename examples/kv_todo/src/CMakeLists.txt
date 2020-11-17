project(kv_todo)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( kv_todo kv_todo kv_todo.cpp )
target_include_directories( kv_todo PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( kv_todo ${CMAKE_SOURCE_DIR}/../ricardian )
