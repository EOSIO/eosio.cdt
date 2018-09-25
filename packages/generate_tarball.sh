#! /bin/bash

mkdir -p usr/bin
mkdir -p usr/lib/cmake
mkdir -p usr/opt/eosio.cdt/bin
mkdir -p usr/opt/eosio.cdt/lib
mkdir -p usr/opt/eosio.cdt/include/eosiolib
mkdir -p usr/opt/eosio.cdt/scripts

# install clang based tools and plugins
clang_tools=(clang-7 
             eosio-abigen 
             eosio-cc 
             eosio-cpp 
             eosio-ld 
             llc 
             lld 
             llvm-ar 
             llvm-nm 
             llvm-objcopy 
             llvm-objdump 
             llvm-readobj 
             llvm-readelf 
             llvm-ranlib 
             llvm-strip 
             opt 
             wasm-ld)

clang_plugins=(LLVMEosioApply.*)

for f in ${clang_tools[@]}; do
   cp ../build/EosioClang-prefix/src/EosioClang-build/bin/$f usr/opt/eosio.cdt/bin
done
for f in ${clang_plugins[@]}; do
   cp ../build/EosioClang-prefix/src/EosioClang-build/lib/$f usr/opt/eosio.cdt/bin
done

# install wabt based tools
wabt_tools=(eosio-pp
            wasm2wat
            wat2wasm)

for f in ${wabt_tools[@]}; do
   cp ../build/external/wabt/$f usr/opt/eosio.cdt/bin
done

# install cmake modules
cp ../build/modules/EosioWasmToolchainPackage.cmake usr/lib/cmake/EosioWasmToolchain.cmake

# install scripts
cp -R ../build/scripts usr/opt/eosio.cdt/

# install misc.
cp ../build/eosio.imports usr/opt/eosio.cdt

# install wasm includes
cp -R ../libraries/boost/include usr/opt/eosio.cdt
cp ../libraries/eosiolib/*.h ../libraries/eosiolib/*.hpp usr/opt/eosio.cdt/include/eosiolib
cp -R ../libraries/libc/musl/include usr/opt/eosio.cdt/include/libc
cp -R ../libraries/libc/musl/src/internal usr/opt/eosio.cdt/include/libc
cp -R ../libraries/libc/musl/arch/eos usr/opt/eosio.cdt/include/libc
cp -R ../libraries/libc++/libcxx/include usr/opt/eosio.cdt/include/libcxx

# install wasm libs
cp ../build/libraries/eosiolib/libeosio.a usr/opt/eosio.cdt/lib
cp ../build/libraries/libc/libc.a usr/opt/eosio.cdt/lib
cp ../build/libraries/libc++/libc++.a usr/opt/eosio.cdt/lib

# make symlinks
create_symlink() {
   pushd usr/bin &> /dev/null
   ln -sf ../opt/eosio.cdt/bin/$1 $2
   popd &> /dev/null
}
create_symlink "llvm-ranlib eosio-ranlib"
create_symlink "llvm-ar eosio-ar"
create_symlink "llvm-objdump eosio-objdump"
create_symlink "llvm-readelf eosio-readelf"
create_symlink "eosio-cc eosio-cc"
create_symlink "eosio-cpp eosio-cpp"
create_symlink "eosio-ld eosio-ld"
create_symlink "eosio-pp eosio-pp"
create_symlink "eosio-abigen eosio-abigen"
create_symlink "wasm2wat eosio-wasm2wast"
create_symlink "wat2wasm eosio-wast2wasm"

tar -cvzf eosio.cdt.tar.gz usr/*
rm -r usr
