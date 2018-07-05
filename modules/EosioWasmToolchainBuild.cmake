message(STATUS "Setting up Eosio Wasm Toolchain")
set(CMAKE_SYSTEM_NAME WebAssembly)

set(WASM_INSTALL_ROOT "${CMAKE_INSTALL_PREFIX}")
set(CLANG_DIR ${CMAKE_BINARY_DIR}/EosioClang-prefix/src/EosioClang-build)

set(CMAKE_C_COMPILER "${CLANG_DIR}/bin/eosio.cc")
set(CMAKE_CXX_COMPILER "${CLANG_DIR}/bin/eosio.cpp")

set(CMAKE_C_FLAGS " -O3 -eosio-imports=${CMAKE_BINARY_DIR}/eosio.imports ")
set(CMAKE_CXX_FLAGS " -O3 ")

set(WASM_LINKER "${CLANG_DIR}/bin/eosio.ld")

set(CMAKE_C_LINK_EXECUTABLE "${WASM_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE "${WASM_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_AR "${CLANG_DIR}/bin/llvm-ar" CACHE PATH "ar" FORCE)
set(CMAKE_RANLIB "${CLANG_DIR}/bin/llvm-ranlib" CACHE PATH "ranlib" FORCE)

find_package(Boost 1.67 REQUIRED)

set(STANDARD_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/libraries/libc++/libcxx/include"
                      "${CMAKE_CURRENT_SOURCE_DIR}/libraries/libc/musl/include"
                      "${CMAKE_CURRENT_SOURCE_DIR}/libraries/eosiolib"
                      "${CMAKE_CURRENT_SOURCE_DIR}/libraries/boost/include"
                      "${Boost_INCLUDE_DIRS}")
set(STANDARD_LIBS c++ c eosio)
