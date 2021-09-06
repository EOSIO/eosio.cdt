file(GLOB HEADERS "*.hpp" "*.h")

add_library(cosmwasm
   cosmwasm.cpp
   base64.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/../internal/uint128_t/uint128_t.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/../internal/rt/compiler_builtins.cpp
)

target_compile_definitions(cosmwasm PUBLIC NO_BUILTIN_UINT128)
target_include_directories(cosmwasm
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../internal/include
)
target_link_libraries(cosmwasm c++ c cstdio-no-float)

add_custom_command(TARGET cosmwasm POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cosmwasm> ${BASE_BINARY_DIR}/lib)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cosmwasm DESTINATION ${BASE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
