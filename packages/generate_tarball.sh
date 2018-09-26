#! /bin/bash

VERSION=$1

PREFIX=$2
CDT_PREFIX=$3
SYMLINK_PREFIX=$4

mkdir -p ${PREFIX}/bin
mkdir -p ${PREFIX}/lib/cmake
mkdir -p ${CDT_PREFIX}/bin 
mkdir -p ${CDT_PREFIX}/lib/cmake
mkdir -p ${CDT_PREFIX}/cmake
mkdir -p ${CDT_PREFIX}/include/eosiolib
mkdir -p ${CDT_PREFIX}/scripts

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
   cp ../build/EosioClang-prefix/src/EosioClang-build/bin/$f ${CDT_PREFIX}/bin 
done
for f in ${clang_plugins[@]}; do
   cp ../build/EosioClang-prefix/src/EosioClang-build/lib/$f ${CDT_PREFIX}/bin 
done

pushd ${CDT_PREFIX}/bin &> /dev/null
ln -sf clang-7 clang
ln -sf clang-7 clang++
popd &> /dev/null

# install wabt based tools
wabt_tools=(eosio-pp
            wasm2wat
            wat2wasm)

for f in ${wabt_tools[@]}; do
   cp ../build/external/wabt/$f ${CDT_PREFIX}/bin
done

# install cmake modules
pushd ${PREFIX}/lib/cmake &> /dev/null
ln -sf ../../${SYMLINK_PREFIX}/lib/cmake/EosioWasmToolchain.cmake EosioWasmToolchain.cmake
popd &> /dev/null

# install scripts
cp -R ../build/scripts/* ${CDT_PREFIX}/scripts 

# install misc.
cp ../build/eosio.imports ${CDT_PREFIX}

# install wasm includes
cp -R ../libraries/boost/include/* ${CDT_PREFIX}/include
cp ../libraries/eosiolib/*.h ../libraries/eosiolib/*.hpp ${CDT_PREFIX}/include/eosiolib
cp -R ../libraries/libc/musl/include ${CDT_PREFIX}/include/libc
cp -R ../libraries/libc/musl/src/internal ${CDT_PREFIX}/include/libc/
cp -R ../libraries/libc/musl/arch/eos ${CDT_PREFIX}/include/libc/
cp -R ../libraries/libc++/libcxx/include ${CDT_PREFIX}/include/libcxx

# install wasm libs
cp ../build/libraries/eosiolib/libeosio.a ${CDT_PREFIX}/lib
cp ../build/libraries/libc/libc.a ${CDT_PREFIX}/lib
cp ../build/libraries/libc++/libc++.a ${CDT_PREFIX}/lib

# make symlinks
create_symlink() {
   pushd ${PREFIX}/bin &> /dev/null
   ln -sf ../${SYMLINK_PREFIX}/bin/$1 $2
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

tar -cvzf $5 ./${PREFIX}/*
rm -r ${PREFIX}
