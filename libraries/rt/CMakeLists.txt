# Defines builtins library

set(builtins_sources
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

add_library(eosio_rt STATIC ${builtins_sources})
target_include_directories(eosio_rt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${softfloat_SOURCE_DIR}/source/include )
target_compile_options(eosio_rt PRIVATE -Wall --no-missing-ricardian-clause)

install(TARGETS eosio_rt)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()
