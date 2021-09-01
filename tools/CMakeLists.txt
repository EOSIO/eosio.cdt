macro(add_tool name source)
  if(${ARGC} GREATER_EQUAL 3)
    set(source_fn ${ARGV2})
    set(BLANC_TOOL_BACKEND ${ARGV3})
    set(BLANC_TOOL_BACKEND_FALLBACK ${ARGV4})
  else()
    get_filename_component(source_fn ${source} NAME_WLE)
  endif()
  configure_file(${source} ${CMAKE_CURRENT_BINARY_DIR}/${source_fn})

  add_executable(${name} ${CMAKE_CURRENT_BINARY_DIR}/${source_fn})
  set_property(TARGET ${name} PROPERTY CXX_STANDARD 20)
  target_include_directories(${name}
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/src
      ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/internal/include
  )
  target_link_libraries(${name} PUBLIC clangTooling clangDriver)

  add_custom_command(TARGET ${name} POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/bin)
  add_custom_command(TARGET ${name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${name}> ${CMAKE_BINARY_DIR}/bin/)
  install(TARGETS ${name} DESTINATION ${BLANC_INSTALL_PREFIX}/bin)
endmacro()

add_tool(${BLANC_C_COMPILER} src/cc.cpp.in blanc-cc.cpp ${BLANC_C_COMPILER_BACKEND} ${BLANC_C_COMPILER_BACKEND_FALLBACK})
add_tool(${BLANC_CXX_COMPILER} src/cc.cpp.in blanc-cpp.cpp ${BLANC_CXX_COMPILER_BACKEND} ${BLANC_CXX_COMPILER_BACKEND_FALLBACK})
add_tool(${BLANC_LINKER} src/ld.cpp.in blanc-ld.cpp)
add_tool(${BLANC_AR} src/base.cpp.in blanc-ar.cpp ${BLANC_AR_BACKEND} ${BLANC_AR_BACKEND_FALLBACK})
add_tool(${BLANC_RANLIB} src/base.cpp.in blanc-ranlib.cpp ${BLANC_RANLIB_BACKEND} ${BLANC_RANLIB_BACKEND_FALLBACK})

target_compile_definitions(${BLANC_CXX_COMPILER} PUBLIC CPP_COMP=1)

# blanc-pp (for post-processing)
add_subdirectory(external)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(NOT TARGET BlancTools)
  add_custom_target(BlancTools)
  add_dependencies(BlancTools ${BLANC_C_COMPILER} ${BLANC_CXX_COMPILER} ${BLANC_LINKER} ${BLANC_AR} ${BLANC_RANLIB} ${BLANC_POSTPASS})
endif()

