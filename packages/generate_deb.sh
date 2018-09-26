#! /bin/bash

VERSION=$1

mkdir -p eosio.cdt/DEBIAN
echo "Package: eosio.cdt
Version: ${VERSION}
Section: devel
Priority: optional
Architecture: amd64
Homepage: https://github.com/EOSIO/eosio.cdt
Maintainer: support@block.one
Description: Toolchain and supporting tools for the EOS.IO platform" &> eosio.cdt/DEBIAN/control

mkdir -p usr/opt/eosio.cdt/${VERSION}/lib/cmake 

sed 's/_PREFIX_/\/usr/g' ../build/modules/EosioWasmToolchainPackage.cmake &> usr/opt/eosio.cdt/${VERSION}/lib/cmake/EosioWasmToolchain.cmake

./generate_tarball.sh ${VERSION} usr usr/opt/eosio.cdt/${VERSION} eosio.cdt.tar.gz

tar -xvzf eosio.cdt.tar.gz -C eosio.cdt
dpkg-deb --build eosio.cdt

rm -r eosio.cdt
