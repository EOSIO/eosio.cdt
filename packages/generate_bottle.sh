#! /bin/bash

VERSION=$1
PREFIX="eosio.cdt/$VERSION"

mkdir -p ${PREFIX}/bin
mkdir -p ${PREFIX}/lib/cmake
mkdir -p ${PREFIX}/opt/eosio_cdt/bin
mkdir -p ${PREFIX}/opt/eosio_cdt/lib
mkdir -p ${PREFIX}/opt/eosio_cdt/include/eosiolib
mkdir -p ${PREFIX}/opt/eosio_cdt/scripts

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
   cp ../build/EosioClang-prefix/src/EosioClang-build/bin/$f ${PREFIX}/opt/eosio_cdt/bin
done
for f in ${clang_plugins[@]}; do
   cp ../build/EosioClang-prefix/src/EosioClang-build/lib/$f ${PREFIX}/opt/eosio_cdt/bin
done
pushd ${PREFIX}/opt/eosio_cdt/bin &> /dev/null
ln -sf clang-7 clang
ln -sf clang-7 clang++
popd &> /dev/null

# install wabt based tools
wabt_tools=(eosio-pp
            wasm2wat
            wat2wasm)

for f in ${wabt_tools[@]}; do
   cp ../build/external/wabt/$f ${PREFIX}/opt/eosio_cdt/bin
done

# install cmake modules
sed 's/_PREFIX_/\/usr\/local/g' ../build/modules/EosioWasmToolchainPackage.cmake &> ${PREFIX}/lib/cmake/EosioWasmToolchain.cmake

# install scripts
cp -R ../build/scripts ${PREFIX}/opt/eosio_cdt/

# install misc.
cp ../build/eosio.imports ${PREFIX}/opt/eosio_cdt

# install wasm includes
cp -R ../libraries/boost/include ${PREFIX}/opt/eosio_cdt
cp ../libraries/eosiolib/*.h ../libraries/eosiolib/*.hpp ${PREFIX}/opt/eosio_cdt/include/eosiolib
cp -R ../libraries/libc/musl/include ${PREFIX}/opt/eosio_cdt/include/libc
cp -R ../libraries/libc/musl/src/internal ${PREFIX}/opt/eosio_cdt/include/libc
cp -R ../libraries/libc/musl/arch/eos ${PREFIX}/opt/eosio_cdt/include/libc
cp -R ../libraries/libc++/libcxx/include ${PREFIX}/opt/eosio_cdt/include/libcxx

# install wasm libs
cp ../build/libraries/eosiolib/libeosio.a ${PREFIX}/opt/eosio_cdt/lib
cp ../build/libraries/libc/libc.a ${PREFIX}/opt/eosio_cdt/lib
cp ../build/libraries/libc++/libc++.a ${PREFIX}/opt/eosio_cdt/lib

# make symlinks
create_symlink() {
   pushd ${PREFIX}/bin &> /dev/null
   ln -sf ../opt/eosio_cdt/bin/$1 $2
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

#cp brew/INSTALL_RECEIPT.json ${PREFIX}
tar -cvzf eosio.cdt-${VERSION}.high_sierra.bottle.tar.gz ${PREFIX}

rm -r eosio.cdt

hash=`openssl dgst -sha256 eosio.cdt-${VERSION}.high_sierra.bottle.tar.gz | awk 'NF>1{print $NF}'`

echo "class EosioCdt < Formula

   homepage \"https://github.com/eosio/eosio.cdt\"
   revision 0
   url \"https://github.com/larryk85/eosio.cdt/archive/v${VERSION}.tar.gz\"
   version \"${VERSION}\"
   #sha1 yada yada
   
   option :universal

   depends_on \"cmake\" => :build
   depends_on :xcode => :build
   depends_on :macos => :leopard
   depends_on :arch =>  :intel
   
   bottle do
      root_url \"https://github.com/larryk85/test/releases/download/1.0.5\"
      #cellar :any_skip_relocation
      sha256 \"${hash}\" => :high_sierra
   end
   def install
      system \"./build.sh\"
      system \"./install.sh\"
   end
end
__END__" &> eosio.cdt.rb
