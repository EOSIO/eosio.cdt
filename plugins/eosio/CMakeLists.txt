add_llvm_library(eosio_plugin MODULE eosio_plugin.cpp PLUGIN_TOOL clang)

if(APPLE)
  target_link_libraries(eosio_plugin PUBLIC clang-cpp)
endif()

add_custom_command(TARGET eosio_plugin POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/bin)
add_custom_command(TARGET eosio_plugin POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_plugin> ${CMAKE_BINARY_DIR}/bin/)
install(TARGETS eosio_plugin DESTINATION ${BLANC_INSTALL_PREFIX}/bin)
