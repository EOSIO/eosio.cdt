# Defines builtins library
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
)

file ( GLOB builtins_headers "${CMAKE_CURRENT_SOURCE_DIR}*.h" )
list( APPEND builtins_sources ${builtins_headers} )

add_library ( rt STATIC ${builtins_sources} )
add_native_library ( native_rt STATIC ${builtins_sources} )

target_include_directories( rt PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
                                      "${CMAKE_CURRENT_SOURCE_DIR}/../native/softfloat/source/include" )

target_include_directories( native_rt PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
                                             "${CMAKE_CURRENT_SOURCE_DIR}/../native/softfloat/source/include" )

add_custom_command( TARGET rt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rt> ${BASE_BINARY_DIR}/lib )
add_custom_command( TARGET native_rt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:native_rt> ${BASE_BINARY_DIR}/lib )
