cmake_minimum_required(VERSION 3.5)
project(hello_example VERSION 1.0.0)

find_package(eosio.cdt)

### Generate the wasm and abi
add_contract( send_inline send_inline send_inline.cpp )

target_include_directories( send_inline.wasm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../hello )
