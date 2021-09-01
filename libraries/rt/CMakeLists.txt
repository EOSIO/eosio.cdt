# Defines builtins library
set(C_DEFINES, "-D__wasm__ -DQUAD_PRECISION")
set(CMAKE_C_FLAGS "-Wall ${CMAKE_C_FLAGS} ${C_DEFINES}")
set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS} ${C_DEFINES}")
set(builtins_sources
   compiler_builtins.cpp
   #fixtfti.c
   #fixunstfti.c
   #fixsfti.c
   #fixdfti.c
   #fixunssfti.c
   #fixunsdfti.c
   #floattidf.c
   #floatuntidf.c
   multi3.c
   ashlti3.c
   ashrti3.c
   lshrti3.c
   divti3.c
   udivti3.c
   modti3.c
   umodti3.c
)

file(GLOB builtins_headers "${CMAKE_CURRENT_SOURCE_DIR}*.h")
list(APPEND builtins_sources ${builtins_headers})

add_library(rt STATIC ${builtins_sources})
target_include_directories(rt PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_custom_command(TARGET rt POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rt> ${BASE_BINARY_DIR}/lib)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
