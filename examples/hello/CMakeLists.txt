cmake_minimum_required(VERSION 3.5)
project(hello_example VERSION 1.0.0)

find_package(eosio.cdt)

### Only generate the wasm
#add_executable( hello.wasm hello.cpp )

### Generate the wasm and abi
add_eosio_cdt_executable( hello hello.cpp )
