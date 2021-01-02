find_package(Boost REQUIRED)

add_llvm_library(eosio_plugin MODULE eosio_autogen.cpp PLUGIN_TOOL clang)

target_include_directories(eosio_plugin PUBLIC ${Boost_INCLUDE_DIRS})
