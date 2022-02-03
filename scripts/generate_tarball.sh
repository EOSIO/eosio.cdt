#! /bin/bash
set -euo pipefail

NAME=$1
CDT_PREFIX=${PREFIX}/${SUBPREFIX}
mkdir -p ${PREFIX}/bin/
mkdir -p ${PREFIX}/lib/cmake/${PROJECT}
mkdir -p ${CDT_PREFIX}/bin
mkdir -p ${CDT_PREFIX}/include
mkdir -p ${CDT_PREFIX}/lib/cmake/${PROJECT}
mkdir -p ${CDT_PREFIX}/cmake
mkdir -p ${CDT_PREFIX}/scripts
mkdir -p ${CDT_PREFIX}/licenses

# install binaries
cp -R ${BUILD_DIR}/bin/* ${CDT_PREFIX}/bin
cp -R ${BUILD_DIR}/licenses/* ${CDT_PREFIX}/licenses

# install cmake modules
sed "s/_PREFIX_/\/${SPREFIX}/g" ${BUILD_DIR}/modules/EosioCDTMacrosPackage.cmake &> ${CDT_PREFIX}/lib/cmake/${PROJECT}/EosioCDTMacros.cmake
sed "s/_PREFIX_/\/${SPREFIX}/g" ${BUILD_DIR}/modules/EosioWasmToolchainPackage.cmake &> ${CDT_PREFIX}/lib/cmake/${PROJECT}/EosioWasmToolchain.cmake
sed "s/_PREFIX_/\/${SPREFIX}\/${SSUBPREFIX}/g" ${BUILD_DIR}/modules/${PROJECT}-config.cmake.package &> ${CDT_PREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake

# install scripts
cp -R ${BUILD_DIR}/scripts/* ${CDT_PREFIX}/scripts 

# install misc.
cp ${BUILD_DIR}/eosio.imports ${CDT_PREFIX}

# install wasm includes
cp -R ${BUILD_DIR}/include/* ${CDT_PREFIX}/include

# install wasm libs
cp ${BUILD_DIR}/lib/*.a ${CDT_PREFIX}/lib

# make symlinks
pushd ${PREFIX}/lib/cmake/${PROJECT} &> /dev/null
ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake ${PROJECT}-config.cmake
ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/EosioWasmToolchain.cmake EosioWasmToolchain.cmake
ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/EosioCDTMacros.cmake EosioCDTMacros.cmake
popd &> /dev/null

create_symlink() {
   ln -sf ../${SUBPREFIX}/bin/$1 ${PREFIX}/bin/$2
}

create_symlink eosio-cc eosio-cc
create_symlink eosio-cpp eosio-cpp
create_symlink eosio-ld eosio-ld
create_symlink eosio-pp eosio-pp
create_symlink eosio-init eosio-init
create_symlink eosio-wasm2wast eosio-wasm2wast
create_symlink eosio-wast2wasm eosio-wast2wasm
create_symlink eosio-ar eosio-ar
create_symlink eosio-abidiff eosio-abidiff
create_symlink eosio-nm eosio-nm
create_symlink eosio-objcopy eosio-objcopy
create_symlink eosio-objdump eosio-objdump
create_symlink eosio-ranlib eosio-ranlib
create_symlink eosio-readelf eosio-readelf
create_symlink eosio-strip eosio-strip

echo "Generating Tarball $NAME.tar.gz..."
tar -cvzf $NAME.tar.gz ./${PREFIX}/*
rm -r ${PREFIX}
