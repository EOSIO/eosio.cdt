file(GLOB CRYPT_SOURCES  "musl/src/crypt/*.c")
file(GLOB CTYPE_SOURCES  "musl/src/ctype/*.c")
set(ENV_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/env/clearenv.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/env/__environ.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/env/getenv.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/env/putenv.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/env/setenv.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/env/unsetenv.c
)
file(GLOB ERRNO_SOURCES  "musl/src/errno/*.c")
set(EXIT_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/exit/assert.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/exit/atexit.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/exit/at_quick_exit.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/exit/_Exit.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/exit/quick_exit.c
)
file(GLOB LOCALE_SOURCES  "musl/src/locale/*.c")
list(REMOVE_ITEM LOCALE_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/locale/iconv.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/locale/iconv_close.c
)
file(GLOB MATH_SOURCES  "musl/src/math/*.c")
list(REMOVE_ITEM MATH_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/math/fmaf.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/math/fmal.c
)
file(GLOB MBYTE_SOURCES  "musl/src/multibyte/*.c")
set(MISC_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/misc/ffs.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/misc/ffsl.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/misc/ffsll.c
)
file(GLOB SEARCH_SOURCES "musl/src/search/*.c")
file(GLOB STDIO_SOURCES "musl/src/stdio/*.c")
list(REMOVE_ITEM STDIO_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/__fdopen.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/__fmodeflags.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/__fopen_rb_ca.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/flockfile.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/fopen.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/freopen.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/ftrylockfile.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/funlockfile.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/pclose.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/popen.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/remove.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/rename.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/tempnam.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/tmpfile.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/tmpnam.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/snprintf.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/sprintf.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/vsnprintf.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/stdio/vsprintf.c
)
file(GLOB STDLIB_SOURCES "musl/src/stdlib/*.c")
file(GLOB STRING_SOURCES "musl/src/string/*.c")
list(REMOVE_ITEM STRING_SOURCES
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/string/strerror_r.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/string/strsignal.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/string/memcpy.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/string/memmove.c
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/string/memset.c
)
set(TIME_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/asctime.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/asctime_r.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/difftime.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/gmtime.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/gmtime_r.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/__month_to_secs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/__secs_to_tm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/strftime.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/strptime.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/timegm.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/__tm_to_secs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/__tz.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/wcsftime.c
    ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/time/__year_to_secs.c
)
set(THREAD_SOURCES musl/src/thread/__lock.c) #only for __lock __unlock

set(INTERNAL_SOURCES musl/src/internal/floatscan.c musl/src/internal/intscan.c musl/src/internal/shgetc.c musl/src/internal/libc.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-everything -D_XOPEN_SOURCE=700")
set(CMAKE_BUILD_TYPE "Release")

list(APPEND C_SOURCES
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
   ${STDLIB_SOURCES}
   ${STRING_SOURCES}
   ${TIME_SOURCES}
   ${THREAD_SOURCES}
   "musl/src/malloc/memalign.c"
   "musl/src/unistd/lseek.c"
)

list(APPEND C_PRIVATE_INCLUDES
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/include
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/src/internal
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/arch/eos
)

list(APPEND C_PUBLIC_INCLUDES
   ${CMAKE_CURRENT_SOURCE_DIR}/musl/include
)

add_library(cstdio STATIC ${STDIO_SOURCES})
target_include_directories(cstdio PRIVATE ${C_PRIVATE_INCLUDES})
target_compile_definitions(cstdio PUBLIC PRINTF_SUPPORT_FLOAT)

add_library(cstdio-no-float STATIC ${STDIO_SOURCES})
target_include_directories(cstdio-no-float PRIVATE ${C_PRIVATE_INCLUDES})

add_library(c ${C_SOURCES})
target_include_directories(c
   PRIVATE ${C_PRIVATE_INCLUDES}
   PUBLIC ${C_PUBLIC_INCLUDES}
)

add_custom_command(TARGET c POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c> ${BASE_BINARY_DIR}/lib)
add_custom_command(TARGET cstdio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cstdio> ${BASE_BINARY_DIR}/lib)
add_custom_command(TARGET cstdio-no-float POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cstdio-no-float> ${BASE_BINARY_DIR}/lib)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/musl/include/ DESTINATION ${BASE_BINARY_DIR}/include/)
