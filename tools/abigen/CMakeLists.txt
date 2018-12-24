configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-abigen.cpp.in ${CMAKE_BINARY_DIR}/eosio-abigen.cpp)

add_tool(eosio-abigen)
