cmake_minimum_required( VERSION 3.5 )

set(EOSIO_VERSION_MIN "2.0")
set(EOSIO_VERSION_SOFT_MAX "3.0")
#set(EOSIO_VERSION_HARD_MAX "")

set(EOSIO_WASM_OLD_BEHAVIOR "off")
find_package(eosio.cdt)

configure_file(${CMAKE_SOURCE_DIR}/contracts.hpp.in ${CMAKE_BINARY_DIR}/contracts.hpp)
include_directories(${CMAKE_BINARY_DIR})

file(GLOB INT_TESTS "*.cpp" "*.hpp")
add_executable(integration_tests ${INT_TESTS})
target_link_libraries(integration_tests -ftester -stack-size=65536)
target_compile_options(integration_tests PUBLIC -ftester -Os)
set_target_properties(integration_tests
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
