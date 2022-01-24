
function(add_clang_plugin name src)
  add_library(${name} SHARED ${src})
  target_include_directories(${name} SYSTEM PRIVATE "${LLVM_INCLUDE_DIRS};${CLANG_INCLUDE_DIRS}")
  if (NOT LLVM_ENABLE_RTTI)
    target_compile_options(${name} PRIVATE -fno-rtti)
  endif()

  add_custom_command(TARGET ${name} POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/lib)
  add_custom_command(TARGET ${name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${name}> ${CMAKE_BINARY_DIR}/lib/)
  install(TARGETS ${name} DESTINATION ${BLANC_INSTALL_PREFIX}/lib)

  # On Darwin (unlike on Linux), undefined symbols in shared objects are not
  # allowed at the end of the link-edit. The plugins defined here:
  #  - _are_ shared objects
  #  - reference symbols from LLVM shared libraries, i.e. symbols which are
  #    undefined until those shared objects are loaded in memory (and hence
  #    _undefined_ during static linking)
  # The build will fail with errors like this:
  #    "Undefined symbols for architecture x86_64"
  # with various LLVM symbols being undefined. Since those symbols are later
  # loaded and resolved at runtime, these errors are false positives.
  # This behaviour can be modified via the '-undefined' OS X linker flag as
  # follows.
  target_link_libraries(
    ${name}
    "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>"
    )
endfunction()

add_clang_plugin(eosio_attrs attrs.cpp)
add_clang_plugin(eosio_codegen codegen.cpp)

target_include_directories(eosio_codegen
  PRIVATE 
    ${CMAKE_SOURCE_DIR}/tools/include
    ${CMAKE_SOURCE_DIR}/libraries/internal/include)

if(NOT TARGET EosioPlugins)
  add_custom_target(EosioPlugins)
  add_dependencies(EosioPlugins eosio_attrs eosio_codegen)
endif()
