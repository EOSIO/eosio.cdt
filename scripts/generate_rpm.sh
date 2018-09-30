#! /bin/bash

VERSION=$1

mkdir -p eosio.cdt/
echo "Summary: Toolchain and supporting tools for the EOS.IO platform
Name: eosio.cdt
Version: ${VERSION}
License: MIT
Vendor: block.one
Source: https://github.com/EOSIO/eosio.cdt
URL: https://github.com/EOSIO/eosio.cdt
Packager: block.one <support@block.one>" &> eosio.cdt.spec

mkdir -p usr/opt/eosio.cdt/${VERSION}/lib/cmake 

sed 's/_PREFIX_/\/usr/g' ../build/modules/EosioWasmToolchainPackage.cmake &> usr/opt/eosio.cdt/${VERSION}/lib/cmake/EosioWasmToolchain.cmake

./generate_tarball.sh ${VERSION} usr usr/opt/eosio.cdt/${VERSION} eosio.cdt.tar.gz

tar -xvzf eosio.cdt.tar.gz -C eosio.cdt
dpkg-deb --build eosio.cdt

rm -r eosio.cdt
