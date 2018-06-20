message(STATUS "Setting up Eosio Wasm Toolchain")
set(CMAKE_SYSTEM_NAME WebAssembly)

if (WASM_SDK_BUILD)
   set(WASM_INSTALL_ROOT "${CMAKE_INSTALL_PREFIX}")
else(WASM_SDK_BUILD)
   if (WASM_ROOT STREQUAL "")
      set(WASM_INSTALL_ROOT "${CMAKE_INSTALL_PREFIX}")
   else(WASM_ROOT STREQUAL "")
      set(WASM_INSTALL_ROOT "${WASM_ROOT}")
   endif(WASM_ROOT STREQUAL "")
endif(WASM_SDK_BUILD)

set(EOSIO_APPLY_LIB "${WASM_INSTALL_ROOT}/eosiosdk/eosiollvm/lib/LLVMEosioApply${CMAKE_SHARED_LIBRARY_SUFFIX}")

set(CMAKE_C_COMPILER "${WASM_INSTALL_ROOT}/eosiosdk/eosiollvm/bin/clang")
set(CMAKE_CXX_COMPILER "${WASM_INSTALL_ROOT}/eosiosdk/eosiollvm/bin/clang++")
set(TRIPLE "wasm32-unknown-unknown-elf")

set(STD_FLAGS " -mllvm -use-cfl-aa-in-codegen=both --target=wasm32 -DBOOST_DISABLE_ASSERTS -DBOOST_EXCEPTION_DISABLE -nostdlib -ffreestanding -fno-builtin -Xclang -load -Xclang ${EOSIO_APPLY_LIB} -DBOOST_DISABLE_ASSERTS -DBOOST_EXCEPTION_DISABLE ")

set(CMAKE_C_COMPILER_TARGET  ${TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${TRIPLE})
set(CMAKE_C_FLAGS " ${STD_FLAGS} -O3")
set(CMAKE_CXX_FLAGS " -std=c++14 -O3 -fno-rtti -fno-exceptions ${STD_FLAGS}")

#set(WASM_LINKER "${WASM_INSTALL_ROOT}/eosiowasm/bin/eosio.lld")
set(WASM_LINKER "${WASM_INSTALL_ROOT}/eosiosdk/eosiollvm/bin/wasm-ld")
if(WASM_SDK_BUILD)
   set(WASM_IMPORTS "${CMAKE_SOURCE_DIR}/eosio.imports.in")
else(WASM_SDK_BUILD)
   set(WASM_IMPORTS "${WASM_INSTALL_ROOT}/eosiosdk/eosio.imports")
endif()

set(CMAKE_C_LINK_EXECUTABLE "${WASM_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_LINK_EXECUTABLE "${WASM_LINKER} <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_AR "${WASM_INSTALL_ROOT}/eosiosdk/eosiollvm/bin/llvm-ar" CACHE PATH "ar" FORCE)
set(CMAKE_RANLIB "${WASM_INSTALL_ROOT}/eosiosdk/eosiollvm/bin/llvm-ranlib" CACHE PATH "ranlib" FORCE)

set(CMAKE_EXE_LINKER_FLAGS "--allow-undefined-file=${WASM_IMPORTS} -e apply --lto-O3 --gc-sections --merge-data-segments --strip-all -stack-first -zstack-size=8192 -mllvm -use-cfl-aa-in-codegen=both")

find_package(Boost 1.67 REQUIRED)

if(WASM_SDK_BUILD)
   set(STANDARD_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/libraries/libc++/libcxx/include"
                         "${CMAKE_CURRENT_SOURCE_DIR}/libraries/libc/musl/include"
                         "${CMAKE_CURRENT_SOURCE_DIR}/libraries/eosiolib"
                         "${CMAKE_CURRENT_SOURCE_DIR}/libraries/pfr/include"
                         "${Boost_INCLUDE_DIRS}")
else(WASM_SDK_BUILD)
   set(STANDARD_INCLUDES "${WASM_INSTALL_ROOT}/eosiosdk/include/eosiolib"
                         "${WASM_INSTALL_ROOT}/eosiosdk/include/libcxx"
                         "${WASM_INSTALL_ROOT}/eosiosdk/include/musl"
                         "${Boost_INCLUDE_DIRS}")
   set(EosioLib_DIR ${WASM_INSTALL_ROOT}/lib/cmake)
   
   find_package(EosioLib CONFIG REQUIRED)
   # hack for CMake on Linux
   set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
   set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

endif(WASM_SDK_BUILD)
set(STANDARD_LIBS c++ c eosio)
