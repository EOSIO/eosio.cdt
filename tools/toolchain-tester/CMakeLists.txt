project(toolchain-tester)
cmake_minimum_required(VERSION 3.5)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/toolchain-tester ${CMAKE_BINARY_DIR}/ COPYONLY)
