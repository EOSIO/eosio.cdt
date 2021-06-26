cmake_minimum_required(VERSION 3.16)

project(cosmwasm_simple_token)

find_package(blanc)

# TODO: Separate cosmwasm toolchain configuration
include(EosioWasmToolchain)

add_executable(token ${CMAKE_CURRENT_SOURCE_DIR}/src/token.cpp)

target_include_directories(token PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# TODO: Add a convenient way to set build profile
target_compile_options(token PUBLIC --profile=cosmwasm)
target_link_options(token PUBLIC --profile=cosmwasm)

