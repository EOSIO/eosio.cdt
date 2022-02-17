macro(add_tool name source backend)
 
  add_executable(${name} ${source})
  set_property(TARGET ${name} PROPERTY CXX_STANDARD 20)
  target_compile_definitions(${name} PRIVATE
    TOOL_BACKEND=\"${backend}\" 
    COMPILER_NAME=\"eosio-cpp\"
    LINKER_NAME=\"eosio-ld\"
    SHARED_LIB_SUFFIX=\"${CMAKE_SHARED_LIBRARY_SUFFIX}\"
    POSTPASS_NAME=\"eosio-pp\"
  ) 
  target_include_directories(${name}
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/src
      ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      ${jsoncons_SOURCE_DIR}/include
      ${LLVM_INCLUDE_DIRS}
      ${CLANG_INCLUDE_DIRS}
  )
  target_link_libraries(${name} PUBLIC clangTooling clangDriver)

  set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

  install(TARGETS ${name})
endmacro()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
function(copy_to_bin target)
  add_custom_command(OUTPUT
          ${CMAKE_BINARY_DIR}/bin/${target}
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${target}>
          ${CMAKE_BINARY_DIR}/bin
      DEPENDS ${target}
  )
  # Create target which consume the command via DEPENDS.
  add_custom_target(copy_${target} ALL
      DEPENDS ${CMAKE_BINARY_DIR}/bin/${target}
  )
  install(TARGETS ${target})
endfunction()


if (NOT llvm_SOURCE_DIR)
  set(LLVM_RAW_URL_BASE https://raw.githubusercontent.com/llvm/llvm-project/llvmorg-${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.0)

  file(DOWNLOAD ${LLVM_RAW_URL_BASE}/llvm/include/llvm/Option/OptParser.td
                ${CMAKE_CURRENT_BINARY_DIR}/include/llvm/Option/OptParser.td)

  file(DOWNLOAD ${LLVM_RAW_URL_BASE}/clang/include/clang/Driver/Options.td
                ${CMAKE_CURRENT_BINARY_DIR}/include/clang/Driver/Options.td)

  file(DOWNLOAD ${LLVM_RAW_URL_BASE}/lld/wasm/Options.td
                ${CMAKE_CURRENT_BINARY_DIR}/include/lld/wasm/Options.td)
  set(tblgen_include_flags -I ${CMAKE_CURRENT_BINARY_DIR}/include)
else()
  set(tblgen_include_flags -I ${llvm_SOURCE_DIR} -I ${llvm_SOURCE_DIR}/llvm/include -I ${llvm_SOURCE_DIR}/clang/include)
endif()

function(tblgen tool ofn)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${ofn}.inc
    COMMAND $<TARGET_FILE:llvm-tblgen> --gen-opt-parser-defs ${tblgen_include_flags} 
                            -o ${CMAKE_CURRENT_BINARY_DIR}/${ofn}.inc ${CMAKE_CURRENT_SOURCE_DIR}/src/${ofn}.td
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/${ofn}.td
    COMMENT "Building ${ofn}.inc..."
  )

  add_custom_target(${ofn} DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${ofn}.inc)
endfunction() 

tblgen(clang cc_options)
tblgen(lld ld_options)

add_tool(eosio-cc src/cc.cpp clang)
add_dependencies(eosio-cc cc_options)

add_tool(eosio-cpp src/cc.cpp clang++)
target_compile_definitions(eosio-cpp PRIVATE CPP_COMP=1)
add_dependencies(eosio-cpp cc_options)

add_tool(eosio-ld src/ld.cpp wasm-ld)
add_dependencies(eosio-ld ld_options)

add_executable(eosio-pp src/postpass.cc)
target_link_libraries(eosio-pp wabt)
target_include_directories(eosio-pp PRIVATE ${wabt_SOURCE_DIR} ${wabt_BINARY_DIR})
set_target_properties(eosio-pp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
install(TARGETS eosio-pp)

copy_to_bin(wasm2wat)
copy_to_bin(wasm-objdump)


if(BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(NOT TARGET EosioTools)
  add_custom_target(EosioTools)
  add_dependencies(EosioTools eosio-cc eosio-cpp eosio-ld eosio-pp)
endif()

