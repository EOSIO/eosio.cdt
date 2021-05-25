cmake_minimum_required(VERSION 3.5)
find_package(eosio.cdt)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..
   ${CMAKE_CURRENT_SOURCE_DIR}/../../native/softfloat/source/include
)

add_native_executable( test_ashlti3 ashlti3_test.cpp )
add_native_executable( test_ashrti3 ashrti3_test.cpp )
add_native_executable( test_lshrti3 lshrti3_test.cpp )
add_native_executable( test_divti3 divti3_test.cpp )
add_native_executable( test_udivti3 udivti3_test.cpp )
add_native_executable( test_multi3 multi3_test.cpp )
add_native_executable( test_modti3 modti3_test.cpp )
add_native_executable( test_umodti3 umodti3_test.cpp )

add_test(NAME rt_tests COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/rt_tests.cmake)
