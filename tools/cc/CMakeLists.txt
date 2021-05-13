configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-cc.cpp.in ${CMAKE_BINARY_DIR}/eosio-cc.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-cpp.cpp.in ${CMAKE_BINARY_DIR}/eosio-cpp.cpp)

add_tool(${BLANC_C_COMPILER} ${CMAKE_BINARY_DIR}/eosio-cc.cpp)
add_tool(${BLANC_CXX_COMPILER} ${CMAKE_BINARY_DIR}/eosio-cpp.cpp)
