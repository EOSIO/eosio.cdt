cmake_minimum_required( VERSION 3.4 )

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

include( eosio.depman )

check_dependencies("${CMAKE_SOURCE_DIR}/eosio.deps")
add_external_dependency( "${CMAKE_SOURCE_DIR}/eosio.deps" "llvm4")
