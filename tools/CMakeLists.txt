macro(add_tool name source backend fallback)
 
  add_executable(${name} ${source})
  set_property(TARGET ${name} PROPERTY CXX_STANDARD 20)
  target_compile_definitions(${name} PRIVATE
    TOOL_BACKEND=\"${backend}\" 
    TOOL_BACKEND_FALLBACK=\"${fallback}\"
    COMPILER_NAME=\"${BLANC_CXX_COMPILER}\"
    LINKER_NAME=\"${BLANC_LINKER}\"
    SHARED_LIB_SUFFIX=\"${CMAKE_SHARED_LIBRARY_SUFFIX}\"
    LLD_BACKEND=\"ld.lld-${LLVM_VERSION_MAJOR}\"
    POSTPASS_NAME=\"${BLANC_POSTPASS}\"
  ) 
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

find_program(TABLEGEN_EXE NAMES llvm-tblgen PATHS ${LLVM_TOOLS_BINARY_DIR} NO_DEFAULT_PATH)

message("LLVM_TOOLS_BINARY_DIR=${LLVM_TOOLS_BINARY_DIR}")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(CREATE_LINK ${LLVM_TOOLS_BINARY_DIR}/clang ${CMAKE_BINARY_DIR}/bin/clang-${LLVM_VERSION_MAJOR} SYMBOLIC)
file(CREATE_LINK ${LLVM_TOOLS_BINARY_DIR}/clang++ ${CMAKE_BINARY_DIR}/bin/clang++-${LLVM_VERSION_MAJOR} SYMBOLIC)
file(CREATE_LINK ${LLVM_TOOLS_BINARY_DIR}/wasm-ld ${CMAKE_BINARY_DIR}/bin/wasm-ld-${LLVM_VERSION_MAJOR} SYMBOLIC)

function(tblgen tool ofn)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${ofn}.inc
    COMMAND ${TABLEGEN_EXE} --gen-opt-parser-defs -I ${llvm_SOURCE_DIR} -I ${llvm_SOURCE_DIR}/llvm/include -I ${llvm_SOURCE_DIR}/${tool}/include 
                            -o ${CMAKE_CURRENT_BINARY_DIR}/${ofn}.inc ${CMAKE_CURRENT_SOURCE_DIR}/src/${ofn}.td
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/${ofn}.td
    COMMENT "Building ${ofn}.inc..."
  )

  add_custom_target(${ofn} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${ofn}.inc)
endfunction() 

tblgen(clang cc_options)
tblgen(lld ld_options)

add_tool(blanc-cc src/cc.cpp ${BLANC_C_COMPILER_BACKEND} ${BLANC_C_COMPILER_BACKEND_FALLBACK})
set_target_properties(blanc-cc PROPERTIES OUTPUT_NAME ${BLANC_C_COMPILER_BACKEND_FALLBACK})
add_dependencies(blanc-cc cc_options)

add_tool(blanc-cpp src/cc.cpp ${BLANC_CXX_COMPILER_BACKEND} ${BLANC_CXX_COMPILER_BACKEND_FALLBACK})
set_target_properties(blanc-cpp PROPERTIES OUTPUT_NAME ${BLANC_CXX_COMPILER_BACKEND_FALLBACK})
target_compile_definitions(blanc-cpp PRIVATE CPP_COMP=1)
add_dependencies(blanc-cpp cc_options)

add_tool(${BLANC_LINKER} src/ld.cpp ${BLANC_LINKER_BACKEND} ${BLANC_LINKER_BACKEND_FALLBACK})
add_dependencies(${BLANC_LINKER} ld_options)
add_tool(${BLANC_AR} src/base.cpp ${BLANC_AR_BACKEND} ${BLANC_AR_BACKEND_FALLBACK})
add_tool(${BLANC_RANLIB} src/base.cpp ${BLANC_RANLIB_BACKEND} ${BLANC_RANLIB_BACKEND_FALLBACK})

add_tool(${BLANC_C_COMPILER} src/dummy.cpp ${BLANC_C_COMPILER_BACKEND_FALLBACK} ${BLANC_C_COMPILER_BACKEND_FALLBACK})
add_tool(${BLANC_CXX_COMPILER} src/dummy.cpp ${BLANC_CXX_COMPILER_BACKEND_FALLBACK} ${BLANC_CXX_COMPILER_BACKEND_FALLBACK})

# blanc-pp (for post-processing)
add_subdirectory(external)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(NOT TARGET BlancTools)
  add_custom_target(BlancTools)
  add_dependencies(BlancTools blanc-cc blanc-cpp ${BLANC_LINKER} ${BLANC_AR} ${BLANC_RANLIB} ${BLANC_POSTPASS})
endif()

