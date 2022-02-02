cmake_minimum_required(VERSION 3.16)

project(eosio_libraries)
include(FetchContent)

find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
  message(STATUS "Using ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

list(APPEND CMAKE_MODULE_PATH ${EOSIO_CDT_BIN})
include(EosioCDTMacros)

set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -O3 -Wall ")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -O3 -Wall ")

macro(fetch_content name)
  FetchContent_Declare(
    ${name}
    ${ARGN}
  )
  FetchContent_GetProperties(${name})
  if(NOT ${name}_POPULATED)
    FetchContent_Populate(${name})
  endif()
endmacro()

fetch_content(
  abieos
  GIT_REPOSITORY https://github.com/EOSIO/abieos
  GIT_TAG        ad5612565b7eec17641c23a0651d9709efcd6b98
)

fetch_content(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        d10b9bd02e098476670f5eb0527d2c7281476e8a
)

fetch_content(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        7512a55aa3ae309587ca89668ef9ec4074a51a1f
)

fetch_content(
  musl
  GIT_REPOSITORY https://github.com/EOSIO/musl
  GIT_TAG        e3ce8f969f2139b0d171c15a1026fad91010c430
)

fetch_content(
  softfloat
  GIT_REPOSITORY https://github.com/EOSIO/berkeley-softfloat-3.git
  GIT_TAG        203b6df7dedc5bae1b2a7b1b23562335a6344578
)

fetch_content(
  boost
  URL            https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.gz
  URL_HASH       SHA256=94ced8b72956591c4775ae2207a9763d3600b30d9d7446562c552f0a14a63be7
)

add_subdirectory(libc)
add_subdirectory(libc++)
add_subdirectory(softfloat)
add_subdirectory(rt)
add_subdirectory(eosiolib)

install(DIRECTORY ${boost_SOURCE_DIR}/boost DESTINATION include/boost)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/meta_refl/include DESTINATION include)

install(TARGETS eosio eosio_malloc eosio_dsm eosio_cmem eosio_embed eosio_tester eosio_rt softfloat
        EXPORT eosio)

install(EXPORT eosio 
        DESTINATION lib/cmake/blanc
        FILE EosioTargets.cmake)

export(TARGETS eosio eosio_malloc eosio_dsm eosio_cmem eosio_embed eosio_tester eosio_rt softfloat 
       FILE ${EOSIO_CDT_BIN}/EosioTargets.cmake)
