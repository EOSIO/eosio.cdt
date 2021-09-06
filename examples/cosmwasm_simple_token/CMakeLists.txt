cmake_minimum_required(VERSION 3.16)

find_package(blanc)
# XXX: cmake 3.20 adds unsupported depfiles forcibly
# To avoid nonexistent compiler_depend.ts error,  include `EosioWasmToolchain` before project()
include(EosioWasmToolchain)

project(cosmwasm_simple_token)

add_executable(token ${CMAKE_CURRENT_SOURCE_DIR}/src/token.cpp)
target_include_directories(token PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# TODO: Add a convenient way to set build profile
target_compile_options(token PUBLIC --profile=cosmwasm)
target_link_options(token PUBLIC --profile=cosmwasm)
