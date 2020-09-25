project(addressbook)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( addressbook addressbook addressbook.cpp )
target_include_directories( addressbook PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( addressbook ${CMAKE_SOURCE_DIR}/../ricardian )