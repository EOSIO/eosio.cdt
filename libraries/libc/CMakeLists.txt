file(GLOB CRYPT_SOURCES  "cdt-musl/src/crypt/*.c")
file(GLOB CTYPE_SOURCES  "cdt-musl/src/ctype/*.c")
file(GLOB ENV_SOURCES  "cdt-musl/src/env/*.c")
file(GLOB ERRNO_SOURCES  "cdt-musl/src/errno/*.c")
file(GLOB EXIT_SOURCES  "cdt-musl/src/exit/*.c")
file(GLOB LOCALE_SOURCES "cdt-musl/src/locale/*.c")
file(GLOB MATH_SOURCES  "cdt-musl/src/math/*.c")
file(GLOB MBYTE_SOURCES  "cdt-musl/src/multibyte/*.c")
file(GLOB MISC_SOURCES "cdt-musl/src/misc/*.c")
file(GLOB SEARCH_SOURCES "cdt-musl/src/search/*.c")
file(GLOB STDIO_SOURCES "cdt-musl/src/stdio/*.c")
file(GLOB STDLIB_SOURCES "cdt-musl/src/stdlib/*.c")
file(GLOB STRING_SOURCES "cdt-musl/src/string/*.c")
file(GLOB TIME_SOURCES "cdt-musl/src/time/*.c")
file(GLOB THREAD_SOURCES "cdt-musl/src/thread/*.c") #only for __lock __unlock

set(INTERNAL_SOURCES cdt-musl/src/internal/floatscan.c cdt-musl/src/internal/intscan.c cdt-musl/src/internal/shgetc.c cdt-musl/src/internal/libc.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-everything -allow-sse")
set(CMAKE_BUILD_TYPE "Release")

if(__APPLE)
  add_definitions(-D__APPLE__)
endif()

file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/include/*.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/src/internal/*.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/arch/eos/*.h")

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
            ${SEARCH_SOURCES}
            ${STDIO_SOURCES}
            ${STDLIB_SOURCES}
            ${STRING_SOURCES}
            ${TIME_SOURCES}
            ${THREAD_SOURCES}
            "cdt-musl/src/malloc/posix_memalign.c"
            "cdt-musl/src/malloc/memalign.c"
            ${HEADERS})


target_include_directories(c
                           PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/include
                           ${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/src/internal
                           ${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/src/crypt
                           ${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/arch/eos
                           ${CMAKE_SOURCE_DIR})

add_custom_command( TARGET c POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c> ${BASE_BINARY_DIR}/lib )

if (ENABLE_NATIVE_COMPILER)
   add_native_library(native_c
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
               ${SEARCH_SOURCES}
               ${STDIO_SOURCES}
               ${STDLIB_SOURCES}
               ${STRING_SOURCES}
               ${TIME_SOURCES}
               ${THREAD_SOURCES}
               "cdt-musl/src/malloc/posix_memalign.c"
               "cdt-musl/src/malloc/memalign.c"
               ${HEADERS})

   target_include_directories(native_c
                              PUBLIC
                              ${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/include
                              ${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/src/internal
                              ${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/src/crypt
                              ${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/arch/eos
                              ${CMAKE_SOURCE_DIR})


   add_custom_command( TARGET native_c POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_c> ${BASE_BINARY_DIR}/lib )
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/include/ DESTINATION ${BASE_BINARY_DIR}/include/libc/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/src/internal/ DESTINATION ${BASE_BINARY_DIR}/include/libc/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cdt-musl/arch/eos/ DESTINATION ${BASE_BINARY_DIR}/include/libc/)
