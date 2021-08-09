add_contract(eosio.wrap eosio.wrap ${CMAKE_CURRENT_SOURCE_DIR}/src/eosio.wrap.cpp)

target_include_directories(eosio.wrap
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(eosio.wrap
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ricardian/eosio.wrap.contracts.md.in ${CMAKE_CURRENT_BINARY_DIR}/ricardian/eosio.wrap.contracts.md @ONLY )

target_compile_options( eosio.wrap PUBLIC -R${CMAKE_CURRENT_SOURCE_DIR}/ricardian -R${CMAKE_CURRENT_BINARY_DIR}/ricardian )
