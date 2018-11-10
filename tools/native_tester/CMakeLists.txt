cmake_minimum_required(VERSION 3.5)

add_subdirectory(Catch2)

add_library(native_tester STATIC native_tester.cpp)
target_link_libraries(native_tester Catch2)

#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/compiler_options.hpp.in ${CMAKE_BINARY_DIR}/compiler_options.hpp)
