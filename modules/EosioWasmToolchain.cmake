message(STATUS " -- setting up Eosio Wasm Toolchain")
set(CMAKE_SYSTEM_NAME WebAssembly)
set(WASM_ROOT "${CMAKE_BINARY_DIR}/EosioClang-prefix/src/EosioClang-build")
set(EOSIO_APPLY_LIB "${WASM_ROOT}/lib/LLVMEosioApply.dylib")

set(CMAKE_C_COMPILER "${WASM_ROOT}/bin/clang")
set(CMAKE_CXX_COMPILER "${WASM_ROOT}/bin/clang++")
set(TRIPLE "wasm32-unknown-unknown-elf")

set(STD_FLAGS " -mllvm -use-cfl-aa-in-codegen=both --target=wasm32 -nostdlib -ffreestanding -fno-builtin -Xclang -load -Xclang ${EOSIO_APPLY_LIB}")

set(CMAKE_C_COMPILER_TARGET  ${TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${TRIPLE})
set(CMAKE_C_FLAGS " ${STD_FLAGS} -O3")
set(CMAKE_CXX_FLAGS " -std=c++14 -O3 -fno-rtti -fno-exceptions ${STD_FLAGS}")

set(WASM_LINKER "${WASM_ROOT}/bin/wasm-ld")
set(WASM_IMPORTS "${CMAKE_SOURCE_DIR}/eosio.imports")

set(CMAKE_C_LINK_EXECUTABLE "${WASM_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE "${WASM_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_AR "${WASM_ROOT}/bin/llvm-ar" CACHE PATH "ar" FORCE)
set(CMAKE_RANLIB "${WASM_ROOT}/bin/llvm-ranlib" CACHE PATH "ranlib" FORCE)

set(CMAKE_EXE_LINKER_FLAGS "--allow-undefined-file=${WASM_IMPORTS} -e apply --lto-O3 --gc-sections --merge-data-segments --strip-all -stack-first -zstack-size=8192 -mllvm -use-cfl-aa-in-codegen=both")

set(BOOST_INSTALL_DIR "/usr/local/include" CACHE STRING "Boost include directory")

set(STANDARD_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/libraries/libc++/libcxx/include"
                      "${CMAKE_CURRENT_SOURCE_DIR}/libraries/libc/musl/include"
                      "${CMAKE_CURRENT_SOURCE_DIR}/libraries/eosiolib"
                      "${CMAKE_CURRENT_SOURCE_DIR}/libraries/pfr/include"
                      "${BOOST_INSTALL_DIR}")

set(STANDARD_LIBS c++ c eosio)

