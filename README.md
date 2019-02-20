# EOSIO.CDT (Contract Development Toolkit)
## Version : 1.5.0

EOSIO.CDT is a toolchain for WebAssembly (WASM) and set of tools to facilitate contract writing for the EOSIO platform.  In addition to being a general purpose WebAssembly toolchain, [EOSIO](https://github.com/eosio/eos) specific optimizations are available to support building EOSIO smart contracts.  This new toolchain is built around [Clang 7](https://github.com/eosio/llvm), which means that EOSIO.CDT has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.

## Important!
EOSIO.CDT Version 1.3.x introduced quite a few breaking changes.  To have binary releases we needed to remove the concept of a core symbol from EOSIO.CDT. This meant drastic changes to symbol, asset and other types/functions that were connected to them. Since these changes would be disruptive, we decided to add as many disruptive changes needed for future contract writing, so that disruption should only occur once. Please read the **_Differences between Version 1.2.x and Version 1.3.x_** section of this readme.

### Binary Releases
EOSIO.CDT currently supports Mac OS X brew, Linux x86_64 Debian packages, and Linux x86_64 RPM packages.

**If you have previously installed EOSIO.CDT, please run the `uninstall` script (it is in the directory where you cloned EOSIO.CDT) before downloading and using the binary releases.**

#### Mac OS X Brew Install
```sh
$ brew tap eosio/eosio.cdt
$ brew install eosio.cdt
```
#### Mac OS X Brew Uninstall
```sh
$ brew remove eosio.cdt
```
#### Debian Package Install
```sh
$ wget https://github.com/EOSIO/eosio.cdt/releases/download/v1.5.0/eosio.cdt_1.5.0-1_amd64.deb
$ sudo apt install ./eosio.cdt-1.5.0-1_amd64.deb
```
#### Debian Package Uninstall
```sh
$ sudo apt remove eosio.cdt
```

#### Fedora RPM Package Install
```sh
$ wget https://github.com/eosio/eosio.cdt/releases/download/v1.5.0/eosio.cdt-1.5.0-1.fc27.x86_64.rpm
$ sudo yum install ./eosio.cdt-1.5.0-1.fc27.x86_64.rpm
```

#### Fedora RPM Package Uninstall
```sh
$ sudo yum remove eosio.cdt
```

#### Centos RPM Package Install
```sh
$ wget https://github.com/eosio/eosio.cdt/releases/download/v1.5.0/eosio.cdt-1.5.0-1.el7.x86_64.rpm
$ sudo yum install ./eosio.cdt-1.5.0-1.el7.x86_64.rpm
```

#### Centos RPM Package Uninstall
```sh
$ sudo yum remove eosio.cdt
```

### Guided Installation (Building from Scratch)
```sh
$ git clone --recursive https://github.com/eosio/eosio.cdt
$ cd eosio.cdt
$ ./build.sh
$ sudo ./install.sh
```

### Installed Tools
---
* eosio-cpp
* eosio-cc
* eosio-ld
* eosio-init
* eosio-abigen
* eosio-abidiff
* eosio-pp (post processing pass for WASM, automatically runs with eosio-cpp and eosio-ld)
* eosio-wasm2wast
* eosio-wast2wasm
* eosio-ranlib
* eosio-ar
* eosio-objdump
* eosio-readelf
