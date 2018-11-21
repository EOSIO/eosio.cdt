project(crt LANGUAGES CXX ASM)
file(GLOB SOURCES macho_crt.s crt.cpp)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-everything")

add_native_library(crt 
            ${SOURCES})

target_include_directories(crt PUBLIC ${CMAKE_SOURCE_DIR})
add_dependencies(crt native_eosio)

add_custom_command( TARGET crt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:crt> ${BASE_BINARY_DIR}/lib )
