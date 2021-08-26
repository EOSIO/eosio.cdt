macro(add_tool name source)
  get_filename_component(source_fn ${source} NAME_WLE)
  configure_file(${source} ${CMAKE_CURRENT_BINARY_DIR}/${source_fn})

  add_executable(${name} ${CMAKE_CURRENT_BINARY_DIR}/${source_fn})
  set_property(TARGET ${name} PROPERTY CXX_STANDARD 20)
  target_include_directories(${name}
    PUBLIC
      ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/internal/include
  )
  target_link_libraries(${name} PUBLIC clangTooling)

  add_custom_command(TARGET ${name} POST_BUILD COMMAND mkdir -p ${CMAKE_BINARY_DIR}/bin)
  add_custom_command(TARGET ${name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${name}> ${CMAKE_BINARY_DIR}/bin/)
  install(TARGETS ${name} DESTINATION ${BLANC_INSTALL_PREFIX}/bin)
endmacro()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/compiler_options.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/compiler_options.hpp)

add_tool(${BLANC_C_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/src/blanc-cc.cpp.in)
add_tool(${BLANC_CXX_COMPILER} ${CMAKE_CURRENT_SOURCE_DIR}/src/blanc-cpp.cpp.in)
add_tool(${BLANC_LINKER} ${CMAKE_CURRENT_SOURCE_DIR}/src/blanc-ld.cpp.in)
add_tool(${BLANC_AR} ${CMAKE_CURRENT_SOURCE_DIR}/src/blanc-ar.cpp.in)
add_tool(${BLANC_RANLIB} ${CMAKE_CURRENT_SOURCE_DIR}/src/blanc-ranlib.cpp.in)

# blanc-pp (for post-processing)
add_subdirectory(external)

if(NOT TARGET BlancTools)
  add_custom_target(BlancTools)
  add_dependencies(BlancTools ${BLANC_C_COMPILER} ${BLANC_CXX_COMPILER} ${BLANC_LINKER} ${BLANC_AR} ${BLANC_RANLIB} ${BLANC_POSTPASS})
endif()

