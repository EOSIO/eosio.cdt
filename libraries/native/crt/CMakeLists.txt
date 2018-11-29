project(crt LANGUAGES CXX ASM)

if (NOT __APPLE)
   set(CRT_ASM elf_crt.s)
else()
   set(CRT_ASM macho_crt.s)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-everything")

add_native_library(native_crt 
                   ${CRT_ASM}
                   crt.cpp
                   softfloat.cpp)

target_include_directories(native_crt PUBLIC ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../softfloat/source/include)
add_dependencies(native_crt native_eosio)

add_custom_command( TARGET native_crt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_crt> ${BASE_BINARY_DIR}/lib )
