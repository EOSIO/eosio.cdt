---
content_title: Centos 7.7
---

<!-- This document is aggregated by our internal documentation tool to generate EOSIO.CDT documentation. The code within the codeblocks below is used in our CI/CD. It will be converted line by line into statements inside of a temporary Dockerfile and used to build our docker tag for this OS. Therefore, COPY and other Dockerfile-isms are not permitted. Code changes will update hashes and regenerate new docker images, so use with caution and do not modify unless necessary. -->

This section contains shell commands to manually download, build, install, test, and uninstall EOSIO.CDT and dependencies on Centos 7.7.

[[info | Building EOSIO.CDT is for Advanced Developers]]
| If you are new to EOSIO.CDT, it is recommended that you install the [EOSIO.CDT Prebuilt Binaries](../../00_install-prebuilt-binaries.md) instead of building from source.

Select a manual task below, then copy/paste the shell commands to a Unix terminal to execute:

* [Download EOSIO.CDT Repository](#download-EOSIO.CDT-repository)
* [Install EOSIO.CDT Dependencies](#install-EOSIO.CDT-dependencies)
* [Build EOSIO.CDT](#build-EOSIO.CDT)
* [Install EOSIO.CDT](#install-EOSIO.CDT)
* [Test EOSIO.CDT](#test-EOSIO.CDT)
* [Uninstall EOSIO.CDT](#uninstall-EOSIO.CDT)

[[info | Building EOSIO.CDT on another OS?]]
| Visit the [Build EOSIO.CDT from Source](../index.md) section.

## Set EOSIO.CDT Environment Variables
<!-- DAC ENV -->
```sh
export EOSIO_LOCATION=$HOME/eosio
export EOSIO_CDT_LOCATION=$EOSIO_LOCATION/cdt
export EOSIO_CDT_INSTALL_LOCATION=$EOSIO_LOCATION/install
export PATH=$EOSIO_CDT_INSTALL_LOCATION/bin:$PATH
export EOSIO_CDT_BUILD_LOCATION=$EOSIO_CDT_LOCATION/build
```
<!-- DAC ENV END -->

## Download EOSIO.CDT Repository
These commands set the EOSIO.CDT directories, install git, and clone the EOSIO.CDT repository.
<!-- DAC CLONE -->
```sh
# create EOSIO.CDT dependency directory
mkdir -p $EOSIO_CDT_INSTALL_LOCATION
# install git
yum update -y && yum install -y git
# clone EOSIO.CDT repository
git clone https://github.com/eosio/eosio.cdt.git $EOSIO_CDT_LOCATION
cd $EOSIO_CDT_LOCATION && git submodule update --init --recursive
```
<!-- DAC CLONE END -->

## Install EOSIO.CDT Dependencies
These commands install the EOSIO.CDT software dependencies. Make sure to [Download the EOSIO.CDT Repository](#download-EOSIO.CDT-repository) first and set the EOSIO.CDT directories.
<!-- DAC DEPS -->
```sh
# install dependencies
yum --enablerepo=extras install -y centos-release-scl && \
    yum install -y devtoolset-7 && \
    yum install -y autoconf automake bzip2 \
    libtool ocaml.x86_64 doxygen graphviz-devel.x86_64 \
    libicu-devel.x86_64 bzip2.x86_64 bzip2-devel.x86_64 openssl-devel.x86_64 \
    gmp-devel.x86_64 gettext-devel.x86_64 gcc-c++.x86_64 perl libffi-devel.x86_64
# build Python 3.7.4
cd $EOSIO_CDT_INSTALL_LOCATION && curl -LO https://www.python.org/ftp/python/3.7.4/Python-3.7.4.tgz && \
    source /opt/rh/devtoolset-7/enable && \
    tar xzf Python-3.7.4.tgz && \
    cd Python-3.7.4 && \
    ./configure --enable-optimizations --prefix=$EOSIO_CDT_INSTALL_LOCATION && \
    make -j$(nproc) altinstall && \
    rm -rf $EOSIO_CDT_INSTALL_LOCATION/Python-3.7.4 $EOSIO_CDT_INSTALL_LOCATION/Python-3.7.4.tgz
# build lcov
cd $EOSIO_CDT_INSTALL_LOCATION && git clone https://github.com/linux-test-project/lcov.git && \
    source /opt/rh/devtoolset-7/enable && \
    cd lcov && \
    make PREFIX=$EOSIO_CDT_INSTALL_LOCATION install && \
    rm -rf $EOSIO_CDT_INSTALL_LOCATION/lcov
# build cmake
cd $EOSIO_CDT_INSTALL_LOCATION && curl -LO https://cmake.org/files/v3.10/cmake-3.10.2.tar.gz && \
    source /opt/rh/devtoolset-7/enable && \
    tar -xzf cmake-3.10.2.tar.gz && \
    cd cmake-3.10.2 && \
    ./bootstrap --prefix=$EOSIO_CDT_INSTALL_LOCATION && \
    make -j$(nproc) && \
    make install && \
    rm -f $EOSIO_CDT_INSTALL_LOCATION/cmake-3.10.2.tar.gz
```
<!-- DAC DEPS END -->

## Build EOSIO.CDT
These commands build the EOSIO.CDT software on the specified OS. Make sure to [Install EOSIO.CDT Dependencies](#install-EOSIO.CDT-dependencies) first.
<!-- DAC BUILD -->
```sh
mkdir -p $EOSIO_CDT_BUILD_LOCATION
cd $EOSIO_CDT_BUILD_LOCATION && source /opt/rh/devtoolset-7/enable && $EOSIO_CDT_INSTALL_LOCATION/bin/cmake -DCMAKE_BUILD_TYPE='Release' -DCMAKE_INSTALL_PREFIX=$EOSIO_CDT_INSTALL_LOCATION ..
cd $EOSIO_CDT_BUILD_LOCATION && source /opt/rh/devtoolset-7/enable && make -j$(nproc)
```
<!-- DAC BUILD END -->

## Install EOSIO.CDT
This command installs the EOSIO.CDT software on the specified OS. Make sure to [Build EOSIO.CDT](#build-EOSIO.CDT) first.
<!-- DAC INSTALL -->
```sh
cd $EOSIO_CDT_BUILD_LOCATION && make install
```
<!-- DAC INSTALL END -->

## Test EOSIO.CDT
These commands validate the EOSIO.CDT software installation on the specified OS. This task is optional but recommended. Make sure to [Install EOSIO.CDT](#install-EOSIO.CDT) first.
<!-- DAC IGNORE -->
```sh
cd $EOSIO_CDT_BUILD_LOCATION && make test
```
<!-- DAC IGNORE END -->

## Uninstall EOSIO.CDT
These commands uninstall the EOSIO.CDT software from the specified OS.
<!-- DAC UNINSTALL -->
```sh
awk '!seen[$0]++' $EOSIO_CDT_LOCATION/build/install_manifest.txt > $EOSIO_CDT_LOCATION/build/install_manifest.txt_deduped && rm -f $EOSIO_CDT_LOCATION/build/install_manifest.txt && mv $EOSIO_CDT_LOCATION/build/install_manifest.txt_deduped $EOSIO_CDT_LOCATION/build/install_manifest.txt
xargs rm < $EOSIO_CDT_LOCATION/build/install_manifest.txt
rm -rf $EOSIO_CDT_LOCATION/build
```
<!-- DAC UNINSTALL END -->