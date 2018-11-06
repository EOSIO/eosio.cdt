file(GLOB CRYPT_SOURCES  "musl/src/crypt/*.c")
file(GLOB CTYPE_SOURCES  "musl/src/ctype/*.c")
file(GLOB ENV_SOURCES  "musl/src/env/*.c")
file(GLOB ERRNO_SOURCES  "musl/src/errno/*.c")
file(GLOB EXIT_SOURCES  "musl/src/exit/*.c")
file(GLOB LOCALE_SOURCES "musl/src/locale/*.c")
file(GLOB MATH_SOURCES  "musl/src/math/*.c")
file(GLOB MBYTE_SOURCES  "musl/src/multibyte/*.c")
file(GLOB MISC_SOURCES "musl/src/misc/*.c")
file(GLOB SEARCH_SOURCES "musl/src/search/*.c")
file(GLOB STDIO_SOURCES "musl/src/stdio/*.c")
file(GLOB STDLIB_SOURCES "musl/src/stdlib/*.c")
file(GLOB STRING_SOURCES "musl/src/string/*.c")
#file(GLOB TIME_SOURCES "musl/src/time/*.c")
file(GLOB THREAD_SOURCES "musl/src/thread/*.c") #only for __lock __unlock

set(INTERNAL_SOURCES musl/src/internal/floatscan.c musl/src/internal/intscan.c musl/src/internal/shgetc.c musl/src/internal/libc.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-everything")

file(GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/musl/include/*.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/musl/src/internal/*.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/musl/arch/eos/*.h"
)

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
            "musl/src/malloc/posix_memalign.c"
            "musl/src/malloc/memalign.c"
            ${HEADERS})

add_dependencies(c EosioTools)
target_include_directories(c 
                           PUBLIC 
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/musl/include;${CMAKE_CURRENT_SOURCE_DIR}/musl/src/internal;${CMAKE_CURRENT_SOURCE_DIR}/musl/src/crypt;${CMAKE_CURRENT_SOURCE_DIR}/musl/arch/eos;${CMAKE_CURRENT_SOURCE_DIR}/../>"
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_FULL_INCLUDEDIR}>)

install(TARGETS c EXPORT EosioLib
   LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR} 
   ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})

add_custom_command( TARGET c POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c> ${CMAKE_BINARY_DIR}/lib )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/musl/include/ DESTINATION ${CMAKE_BINARY_DIR}/include/libc/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/internal/ DESTINATION ${CMAKE_BINARY_DIR}/include/libc/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/musl/arch/eos/ DESTINATION ${CMAKE_BINARY_DIR}/include/libc/)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/musl/include/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/libc)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/internal/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/libc)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/musl/arch/eos/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/libc)
