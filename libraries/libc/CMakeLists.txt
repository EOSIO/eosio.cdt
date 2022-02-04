


file(GLOB CRYPT_SOURCES  "${musl_SOURCE_DIR}/src/crypt/*.c")
file(GLOB CTYPE_SOURCES  "${musl_SOURCE_DIR}/src/ctype/*.c")
file(GLOB ENV_SOURCES  "${musl_SOURCE_DIR}/src/env/*.c")
file(GLOB ERRNO_SOURCES  "${musl_SOURCE_DIR}/src/errno/*.c")
file(GLOB EXIT_SOURCES  "${musl_SOURCE_DIR}/src/exit/*.c")
file(GLOB LOCALE_SOURCES "${musl_SOURCE_DIR}/src/locale/*.c")
file(GLOB MATH_SOURCES  "${musl_SOURCE_DIR}/src/math/*.c")
file(GLOB MBYTE_SOURCES  "${musl_SOURCE_DIR}/src/multibyte/*.c")
file(GLOB MISC_SOURCES "${musl_SOURCE_DIR}/src/misc/*.c")
file(GLOB PRNG_SOURCES "${musl_SOURCE_DIR}/src/prng/*.c")
file(GLOB SEARCH_SOURCES "${musl_SOURCE_DIR}/src/search/*.c")
file(GLOB STDIO_SOURCES "${musl_SOURCE_DIR}/src/stdio/*.c")
file(GLOB STDLIB_SOURCES "${musl_SOURCE_DIR}/src/stdlib/*.c")
file(GLOB STRING_SOURCES "${musl_SOURCE_DIR}/src/string/*.c")
list(REMOVE_ITEM STRING_SOURCES
   ${musl_SOURCE_DIR}/src/string/memcmp.c
)
file(GLOB TIME_SOURCES "${musl_SOURCE_DIR}/src/time/*.c")
file(GLOB_RECURSE THREAD_SOURCES "${musl_SOURCE_DIR}/src/thread/*.c") #only for __lock __unlock

set(INTERNAL_SOURCES ${musl_SOURCE_DIR}/src/internal/floatscan.c ${musl_SOURCE_DIR}/src/internal/intscan.c ${musl_SOURCE_DIR}/src/internal/shgetc.c ${musl_SOURCE_DIR}/src/internal/libc.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-everything -allow-sse")


file(GLOB HEADERS "${musl_SOURCE_DIR}/include/*.h"
                  "${musl_SOURCE_DIR}/src/internal/*.h"
                  "${musl_SOURCE_DIR}/arch/eos/*.h")

add_library(c 
            ${CRYPT_SOURCES} 
            ${CTYPE_SOURCES}
            ${ENV_SOURCES}
            ${ERRNO_SOURCES}
            ${EXIT_SOURCES}
            ${INTERNAL_SOURCES}
            ${LOCALE_SOURCES}
            ${MATH_SOURCES}
            ${MBYTE_SOURCES}
            ${MISC_SOURCES}
            ${PRNG_SOURCES}
            ${SEARCH_SOURCES}
            ${STDIO_SOURCES}
            ${STDLIB_SOURCES}
            ${STRING_SOURCES}
            ${TIME_SOURCES}
            ${THREAD_SOURCES}
            "${musl_SOURCE_DIR}/src/malloc/posix_memalign.c"
            "${musl_SOURCE_DIR}/src/malloc/memalign.c")



target_include_directories(c 
PRIVATE
                           ${musl_SOURCE_DIR}/include
                           ${musl_SOURCE_DIR}/src/internal
                           ${musl_SOURCE_DIR}/src/crypt
                           ${musl_SOURCE_DIR}/arch/eos)


add_custom_command( TARGET c POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c> ${BASE_BINARY_DIR}/lib )

# file(COPY ${musl_SOURCE_DIR}/include/ DESTINATION ${BASE_BINARY_DIR}/include/libc/)
# file(COPY ${musl_SOURCE_DIR}/src/internal/ DESTINATION ${BASE_BINARY_DIR}/include/libc/)
# file(COPY ${musl_SOURCE_DIR}/arch/eos/ DESTINATION ${BASE_BINARY_DIR}/include/libc/)


# file(COPY ${musl_SOURCE_DIR}/include/ DESTINATION ${BASE_BINARY_DIR}/include/)
file(GLOB_RECURSE libc_headers RELATIVE ${musl_SOURCE_DIR}/include "${musl_SOURCE_DIR}/include/*.h")
foreach (f ${libc_headers})
   configure_file(${musl_SOURCE_DIR}/include/${f} ${BASE_BINARY_DIR}/include/${f})
endforeach()