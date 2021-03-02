configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-ld.cpp.in ${CMAKE_BINARY_DIR}/eosio-ld.cpp)

add_tool(eosio-ld)

set_target_properties(eosio-ld PROPERTIES LINK_FLAGS "-Wl,-rpath,\"\\$ORIGIN/../lib\"")
