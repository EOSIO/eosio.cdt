# EOSCC
## Version : 1.6.3

The EOSIO Compiler Collection (EOSCC) is a toolchain for WebAssembly (WASM) and set of tools to facilitate contract writing for the EOSIO platform.  In addition to being a general purpose WebAssembly toolchain, [EOSIO](https://github.com/eosio/eos) specific optimizations are available to support building EOSIO smart contracts.  This new toolchain is built around [Clang 7](https://github.com/eosio/llvm), which means that EOSCC has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.

### Binary Releases
EOSCC currently supports Linux x86_64 Debian packages and Mac OS X brew.

**If you have previously installed EOSCC (or EOSIO.CDT), please run the `uninstall` script (it is in the directory where you cloned EOSCC) before downloading and using the binary releases.**

#### Debian Package Install
```sh
$ wget https://github.com/turnpike2/eoscc/releases/download/1.6.3-pre1/eoscc_1.6.3-pre1-ubuntu-18.04_amd64.deb
$ sudo apt install ./eoscc_1.6.3-pre1-ubuntu-18.04_amd64.deb
```
#### Debian Package Uninstall
```sh
$ sudo apt remove eoscc
```

#### Mac OS X Brew Install
```sh
$ brew tap turnpike2/eoscc
$ brew install eoscc
```
#### Mac OS X Brew Uninstall
```sh
$ brew remove eoscc
```

### Guided Installation (Building from Scratch)
```sh
$ git clone --recursive https://github.com/turnpike2/eoscc
$ cd eoscc
$ ./build.sh
$ sudo ./install.sh
```

### Installed Tools
---
* eosio-cpp
* eosio-cc
* eosio-ld
* eosio-init
* eosio-abidiff
* eosio-wasm2wast
* eosio-wast2wasm
* eosio-ranlib
* eosio-ar
* eosio-objdump
* eosio-readelf
