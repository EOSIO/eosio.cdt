configure_file(${CMAKE_CURRENT_SOURCE_DIR}/eosio-init.cpp ${CMAKE_BINARY_DIR}/eosio-init.cpp @ONLY)

add_tool(eosio-init)

set_target_properties(eosio-init PROPERTIES LINK_FLAGS "-Wl,-rpath,\"\\$ORIGIN/../lib\"")
