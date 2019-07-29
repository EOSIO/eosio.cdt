#! /bin/bash

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

#echo "${PREFIX} ** ${SUBPREFIX} ** ${CDT_PREFIX}"

# install binaries 
cp -R ${BUILD_DIR}/bin/* ${CDT_PREFIX}/bin || exit 1
cp -R ${BUILD_DIR}/licenses/* ${CDT_PREFIX}/licenses || exit 1

# install cmake modules
sed "s/_PREFIX_/\/${SPREFIX}/g" ${BUILD_DIR}/modules/EosioCDTMacrosPackage.cmake &> ${CDT_PREFIX}/lib/cmake/${PROJECT}/EosioCDTMacros.cmake || exit 1
sed "s/_PREFIX_/\/${SPREFIX}/g" ${BUILD_DIR}/modules/EosioWasmToolchainPackage.cmake &> ${CDT_PREFIX}/lib/cmake/${PROJECT}/EosioWasmToolchain.cmake || exit 1
sed "s/_PREFIX_/\/${SPREFIX}\/${SSUBPREFIX}/g" ${BUILD_DIR}/modules/${PROJECT}-config.cmake.package &> ${CDT_PREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake || exit 1

# install scripts
cp -R ${BUILD_DIR}/scripts/* ${CDT_PREFIX}/scripts  || exit 1

# install misc.
cp ${BUILD_DIR}/eosio.imports ${CDT_PREFIX} || exit 1

# install wasm includes
cp -R ${BUILD_DIR}/include/* ${CDT_PREFIX}/include || exit 1

# install wasm libs
cp ${BUILD_DIR}/lib/*.a ${CDT_PREFIX}/lib || exit 1

# make symlinks
pushd ${PREFIX}/lib/cmake/${PROJECT} &> /dev/null
ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/${PROJECT}-config.cmake ${PROJECT}-config.cmake || exit 1
ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/EosioWasmToolchain.cmake EosioWasmToolchain.cmake || exit 1
ln -sf ../../../${SUBPREFIX}/lib/cmake/${PROJECT}/EosioCDTMacros.cmake EosioCDTMacros.cmake || exit 1
popd &> /dev/null

create_symlink() {
   ln -sf ../${SUBPREFIX}/bin/$1 ${PREFIX}/bin/$2 || exit 1
}

create_symlink eosio-cc eosio-cc
create_symlink eosio-cpp eosio-cpp
create_symlink eosio-ld eosio-ld
create_symlink eosio-pp eosio-pp
create_symlink eosio-init eosio-init
create_symlink eosio-abigen eosio-abigen
create_symlink eosio-wasm2wast eosio-wasm2wast
create_symlink eosio-wast2wasm eosio-wast2wasm
create_symlink eosio-ar eosio-ar

echo "Generating Tarball $NAME.tar.gz..."
tar -cvzf $NAME.tar.gz ./${PREFIX}/* || exit 1
rm -r ${PREFIX} || exit 1
