#add_subdirectory(eosio_ld_tool)

set(CLANG_BINDIR ${CMAKE_BINARY_DIR}/EosioClang-prefix/src/EosioClang-build)
set(LLVM_DIR ${CLANG_BINDIR}/lib/cmake/llvm)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_BINDIR}/tools/clang/include)
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_subdirectory(eosio_abigen_tool)
add_subdirectory(eosio_c_tool)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/compiler_options.hpp.in ${CMAKE_BINARY_DIR}/compiler_options.hpp)
