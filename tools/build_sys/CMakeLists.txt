configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-build.cpp.in ${CMAKE_BINARY_DIR}/eosio-build.cpp)

add_tool(eosio-build)
