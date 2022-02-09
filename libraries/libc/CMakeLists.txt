


file(GLOB CRYPT_SOURCES  "${musl_SOURCE_DIR}/src/crypt/*.c")
file(GLOB CTYPE_SOURCES  "${musl_SOURCE_DIR}/src/ctype/*.c")
set(ENV_SOURCES
   ${musl_SOURCE_DIR}/src/env/clearenv.c
   ${musl_SOURCE_DIR}/src/env/__environ.c
   ${musl_SOURCE_DIR}/src/env/getenv.c
   ${musl_SOURCE_DIR}/src/env/putenv.c
   ${musl_SOURCE_DIR}/src/env/setenv.c
   ${musl_SOURCE_DIR}/src/env/unsetenv.c
)
file(GLOB ERRNO_SOURCES  "${musl_SOURCE_DIR}/src/errno/*.c")
set(EXIT_SOURCES
   ${musl_SOURCE_DIR}/src/exit/assert.c
   ${musl_SOURCE_DIR}/src/exit/atexit.c
   ${musl_SOURCE_DIR}/src/exit/at_quick_exit.c
   ${musl_SOURCE_DIR}/src/exit/_Exit.c
   ${musl_SOURCE_DIR}/src/exit/quick_exit.c
)
file(GLOB LOCALE_SOURCES  "${musl_SOURCE_DIR}/src/locale/*.c")
list(REMOVE_ITEM LOCALE_SOURCES
   ${musl_SOURCE_DIR}/src/locale/iconv.c
   ${musl_SOURCE_DIR}/src/locale/iconv_close.c
)
file(GLOB MATH_SOURCES  "${musl_SOURCE_DIR}/src/math/*.c")
list(REMOVE_ITEM MATH_SOURCES
   ${musl_SOURCE_DIR}/src/math/fmaf.c
   ${musl_SOURCE_DIR}/src/math/fmal.c
)
file(GLOB MBYTE_SOURCES  "${musl_SOURCE_DIR}/src/multibyte/*.c")
set(MISC_SOURCES
   ${musl_SOURCE_DIR}/src/misc/ffs.c
   ${musl_SOURCE_DIR}/src/misc/ffsl.c
   ${musl_SOURCE_DIR}/src/misc/ffsll.c
)
set(PRNG_SOURCES ${musl_SOURCE_DIR}/src/prng/rand.c)
file(GLOB SEARCH_SOURCES "${musl_SOURCE_DIR}/src/search/*.c")
file(GLOB STDIO_SOURCES "${musl_SOURCE_DIR}/src/stdio/*.c")
list(REMOVE_ITEM STDIO_SOURCES
   ${musl_SOURCE_DIR}/src/stdio/__fopen_rb_ca.c
   ${musl_SOURCE_DIR}/src/stdio/flockfile.c
   ${musl_SOURCE_DIR}/src/stdio/freopen.c
   ${musl_SOURCE_DIR}/src/stdio/ftrylockfile.c
   ${musl_SOURCE_DIR}/src/stdio/funlockfile.c
   ${musl_SOURCE_DIR}/src/stdio/pclose.c
   ${musl_SOURCE_DIR}/src/stdio/popen.c
   ${musl_SOURCE_DIR}/src/stdio/remove.c
   ${musl_SOURCE_DIR}/src/stdio/rename.c
   ${musl_SOURCE_DIR}/src/stdio/tempnam.c
   ${musl_SOURCE_DIR}/src/stdio/tmpfile.c
   ${musl_SOURCE_DIR}/src/stdio/tmpnam.c
   ${musl_SOURCE_DIR}/src/stdio/snprintf.c
   ${musl_SOURCE_DIR}/src/stdio/sprintf.c
   ${musl_SOURCE_DIR}/src/stdio/vsnprintf.c
   ${musl_SOURCE_DIR}/src/stdio/vsprintf.c
)
file(GLOB STDLIB_SOURCES "${musl_SOURCE_DIR}/src/stdlib/*.c")
file(GLOB STRING_SOURCES "${musl_SOURCE_DIR}/src/string/*.c")
list(REMOVE_ITEM STRING_SOURCES
   ${musl_SOURCE_DIR}/src/string/strerror_r.c
   ${musl_SOURCE_DIR}/src/string/strsignal.c
   ${musl_SOURCE_DIR}/src/string/memcpy.c
   ${musl_SOURCE_DIR}/src/string/memmove.c
   ${musl_SOURCE_DIR}/src/string/memset.c
   ${musl_SOURCE_DIR}/src/string/memcmp.c
)
set(TIME_SOURCES
    ${musl_SOURCE_DIR}/src/time/asctime.c
    ${musl_SOURCE_DIR}/src/time/asctime_r.c
    ${musl_SOURCE_DIR}/src/time/clock.c
    ${musl_SOURCE_DIR}/src/time/difftime.c
    ${musl_SOURCE_DIR}/src/time/gmtime.c
    ${musl_SOURCE_DIR}/src/time/gmtime_r.c
    ${musl_SOURCE_DIR}/src/time/__month_to_secs.c
    ${musl_SOURCE_DIR}/src/time/__secs_to_tm.c
    ${musl_SOURCE_DIR}/src/time/strftime.c
    ${musl_SOURCE_DIR}/src/time/strptime.c
    ${musl_SOURCE_DIR}/src/time/time.c
    ${musl_SOURCE_DIR}/src/time/timegm.c
    ${musl_SOURCE_DIR}/src/time/__tm_to_secs.c
    ${musl_SOURCE_DIR}/src/time/__tz.c
    ${musl_SOURCE_DIR}/src/time/wcsftime.c
    ${musl_SOURCE_DIR}/src/time/__year_to_secs.c
)
set(THREAD_SOURCES 
   ${musl_SOURCE_DIR}/src/thread/eos/__pthread_self.c
   ${musl_SOURCE_DIR}/src/thread/__lock.c) #only for __lock __unlock

set(INTERNAL_SOURCES ${musl_SOURCE_DIR}/src/internal/floatscan.c 
                     ${musl_SOURCE_DIR}/src/internal/intscan.c 
                     ${musl_SOURCE_DIR}/src/internal/shgetc.c 
                     ${musl_SOURCE_DIR}/src/internal/libc.c)

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
   ${PRNG_SOURCES}
   ${SEARCH_SOURCES}
   ${STDLIB_SOURCES}
   ${STRING_SOURCES}
   ${TIME_SOURCES}
   ${THREAD_SOURCES}
   "${musl_SOURCE_DIR}/src/malloc/memalign.c"
   "${musl_SOURCE_DIR}/src/unistd/lseek.c"
)

list(APPEND C_PRIVATE_INCLUDES
   ${musl_SOURCE_DIR}/arch/eos
   ${musl_SOURCE_DIR}/src/include
   ${musl_SOURCE_DIR}/src/internal
)

add_library(cstdio STATIC ${STDIO_SOURCES})
target_include_directories(cstdio PRIVATE ${C_PRIVATE_INCLUDES} )
target_compile_definitions(cstdio PUBLIC PRINTF_SUPPORT_FLOAT)

add_library(cstdio-no-float STATIC ${STDIO_SOURCES})
target_include_directories(cstdio-no-float PRIVATE ${C_PRIVATE_INCLUDES})

add_library(c ${C_SOURCES})
target_include_directories(c
   PRIVATE ${C_PRIVATE_INCLUDES}
)

add_custom_command(TARGET c POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:c> ${BASE_BINARY_DIR}/lib)
add_custom_command(TARGET cstdio POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cstdio> ${BASE_BINARY_DIR}/lib)
add_custom_command(TARGET cstdio-no-float POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cstdio-no-float> ${BASE_BINARY_DIR}/lib)

# file(COPY ${musl_SOURCE_DIR}/include/ DESTINATION ${BASE_BINARY_DIR}/include/)
file(GLOB_RECURSE libc_headers RELATIVE ${musl_SOURCE_DIR}/include "${musl_SOURCE_DIR}/include/*.h")
foreach (f ${libc_headers})
   configure_file(${musl_SOURCE_DIR}/include/${f} ${BASE_BINARY_DIR}/include/${f})
endforeach()