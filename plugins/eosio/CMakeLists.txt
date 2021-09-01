set(LLVM_ENABLE_EH 1)

add_llvm_library(eosio_attrs PARTIAL_SOURCES_INTENDED MODULE attrs.cpp PLUGIN_TOOL clang)
add_llvm_library(eosio_codegen PARTIAL_SOURCES_INTENDED MODULE codegen.cpp PLUGIN_TOOL clang)

if(APPLE)
  target_link_libraries(eosio_attrs PUBLIC clang-cpp)
  target_link_libraries(eosio_codegen PUBLIC clang-cpp)
endif()

target_include_directories(eosio_codegen
  PRIVATE 
    ${CMAKE_SOURCE_DIR}/tools/include
    ${CMAKE_SOURCE_DIR}/libraries/internal/include)

add_custom_command(TARGET eosio_attrs POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/lib)
add_custom_command(TARGET eosio_attrs POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_attrs> ${CMAKE_BINARY_DIR}/lib/)

add_custom_command(TARGET eosio_codegen POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/lib)
add_custom_command(TARGET eosio_codegen POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:eosio_codegen> ${CMAKE_BINARY_DIR}/lib/)

install(TARGETS eosio_attrs eosio_codegen DESTINATION ${BLANC_INSTALL_PREFIX}/lib)

if(NOT TARGET EosioPlugins)
  add_custom_target(EosioPlugins)
  add_dependencies(EosioPlugins eosio_attrs eosio_codegen)
endif()
