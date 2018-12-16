project(eosio_libraries)

list(APPEND CMAKE_MODULE_PATH ${EOSIO_CDT_BIN})
include(EosioCDTMacros)

set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -O3 -Wall ")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -O3 -Wall ")

add_subdirectory(libc)
add_subdirectory(libc++)
add_subdirectory(eosiolib)
add_subdirectory(boost)
add_subdirectory(native)
add_subdirectory(rt)
