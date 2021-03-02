configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-abidiff.cpp.in ${CMAKE_BINARY_DIR}/eosio-abidiff.cpp)

add_tool(eosio-abidiff)

set_target_properties(eosio-abidiff PROPERTIES LINK_FLAGS "-Wl,-rpath,\"\\$ORIGIN/../lib\"")
