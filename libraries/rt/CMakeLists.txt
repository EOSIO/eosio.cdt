# Defines builtins library
# cmake_minimum_required (VERSION 3.8)
# project(rt LANGUAGES CXX C)

set(C_DEFINES, "-D__wasm__ -DQUAD_PRECISION")
set( CMAKE_C_FLAGS " -Wall ${CMAKE_C_FLAGS} ${C_DEFINES} -allow-sse" )
set( CMAKE_CXX_FLAGS " -Wall ${CMAKE_CXX_FLAGS} ${C_DEFINES} -allow-sse" )
set ( builtins_sources
   compiler_builtins.cpp
   fixtfti.c
   fixunstfti.c
   fixsfti.c
   fixdfti.c
   fixunssfti.c
   fixunsdfti.c
   floattidf.c
   floatuntidf.c
   multi3.c
   ashlti3.c
   ashrti3.c
   lshrti3.c
   divti3.c
   udivti3.c
   modti3.c
   umodti3.c
)

file ( GLOB builtins_headers "${CMAKE_CURRENT_SOURCE_DIR}*.h" )
list( APPEND builtins_sources ${builtins_headers} )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../native/softfloat/source/include
)

add_library ( rt STATIC ${builtins_sources} )
add_native_library ( native_rt STATIC ${builtins_sources} )

add_custom_command( TARGET rt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rt> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET native_rt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_rt> ${BASE_BINARY_DIR}/lib )

enable_testing()
add_native_executable( test_ashlti3 test/ashlti3_test.cpp )
add_native_executable( test_ashrti3 test/ashrti3_test.cpp )
add_native_executable( test_lshrti3 test/lshrti3_test.cpp )
add_native_executable( test_divti3 test/divti3_test.cpp )
add_native_executable( test_udivti3 test/udivti3_test.cpp )
add_native_executable( test_multi3 test/multi3_test.cpp )
add_native_executable( test_modti3 test/modti3_test.cpp )
add_native_executable( test_umodti3 test/umodti3_test.cpp )
add_test(NAME test_ashlti3 COMMAND test_ashlti3)
add_test(NAME test_ashrti3 COMMAND test_ashrti3)
add_test(NAME test_lshrti3 COMMAND test_lshrti3)
add_test(NAME test_divti3 COMMAND test_divti3)
add_test(NAME test_udivti3 COMMAND test_udivti3)
add_test(NAME test_multi3 COMMAND test_multi3)
add_test(NAME test_modti3 COMMAND test_modti3)
add_test(NAME test_umodti3 COMMAND test_umodti3)
