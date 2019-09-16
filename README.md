# EOSIO.CDT (Contract Development Toolkit)
## Version : 1.7.0

EOSIO.CDT is a toolchain for WebAssembly (WASM) and set of tools to facilitate contract writing for the EOSIO platform.  In addition to being a general purpose WebAssembly toolchain, [EOSIO](https://github.com/eosio/eos) specific optimizations are available to support building EOSIO smart contracts.  This new toolchain is built around [Clang 7](https://github.com/eosio/llvm), which means that EOSIO.CDT has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.

### New Introductions
As of this release two new repositories are under the suite of tools provided by **EOSIO.CDT**.  These are the [Ricardian Template Toolkit](https://github.com/eosio/ricardian-template-toolkit) and the [Ricardian Specification](https://github.com/eosio/ricardian-spec).  The **Ricardian Template Toolkit** is a set of libraries to facilitate smart contract writers in crafting their Ricardian contracts.  The Ricardian specification is the working specification for the above mentioned toolkit.  Please note that both projects are **alpha** releases and are subject to change.

### Attention
- Please see the [Upgrading Guide](https://eosio.github.io/eosio.cdt/1.6.0/upgrading/) between new versions of EOSIO.CDT to be aware of any breaking changes.
- There is currently a known issue that a minimum of 2 CPU cores is required for using EOSIO.CDT

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
#### Ubuntu Package Install
```sh
$ wget https://github.com/EOSIO/eosio.cdt/releases/download/v1.6.2/eosio.cdt_1.6.2-1-ubuntu-18.04_amd64.deb
$ sudo apt install ./eosio.cdt_1.6.2-1-ubuntu-18.04_amd64.deb
```
#### Ubuntu Package Uninstall
```sh
$ sudo apt remove eosio.cdt
```

#### Centos RPM Package Install
```sh
$ wget https://github.com/EOSIO/eosio.cdt/releases/download/v1.6.2/eosio.cdt-1.6.2-1.el7.x86_64.rpm
$ sudo yum install ./eosio.cdt-1.6.2-1.el7.x86_64.rpm
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
* eosio-abidiff
* eosio-wasm2wast
* eosio-wast2wasm
* eosio-ranlib
* eosio-ar
* eosio-objdump
* eosio-readelf

## Contributing

[Contributing Guide](./CONTRIBUTING.md)

[Code of Conduct](./CONTRIBUTING.md#conduct)

## License

[MIT](./LICENSE)

## Important

See [LICENSE](./LICENSE) for copyright and license terms.

All repositories and other materials are provided subject to the terms of this [IMPORTANT](./IMPORTANT.md) notice and you must familiarize yourself with its terms.  The notice contains important information, limitations and restrictions relating to our software, publications, trademarks, third-party resources, and forward-looking statements.  By accessing any of our repositories and other materials, you accept and agree to the terms of the notice.
