configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-ld.cpp.in ${CMAKE_BINARY_DIR}/eosio-ld.cpp)

add_tool(${BLANC_LINKER} ${CMAKE_BINARY_DIR}/eosio-ld.cpp)
