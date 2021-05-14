file(GLOB HEADERS "*.hpp" "*.h")

add_library(cosmwasm
   cosmwasm.cpp
)

target_include_directories(cosmwasm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cosmwasm c++ cnf)

add_custom_command(TARGET cosmwasm POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cosmwasm> ${BASE_BINARY_DIR}/lib)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../cosmwasm DESTINATION ${BASE_BINARY_DIR}/include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
