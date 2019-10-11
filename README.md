# EOSIO.CDT (Contract Development Toolkit)
## Version : 1.7.0

EOSIO.CDT is a toolchain for WebAssembly (WASM) and set of tools to facilitate smart contract development for the EOSIO platform. In addition to being a general purpose WebAssembly toolchain, [EOSIO](https://github.com/eosio/eos) specific optimizations are available to support building EOSIO smart contracts.  This new toolchain is built around [Clang 7](https://github.com/eosio/llvm), which means that EOSIO.CDT has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are incomplete or not available.

## New Introductions
As of this release two new repositories are under the suite of tools provided by **EOSIO.CDT**.  These are the [Ricardian Template Toolkit](https://github.com/eosio/ricardian-template-toolkit) and the [Ricardian Specification](https://github.com/eosio/ricardian-spec).  The **Ricardian Template Toolkit** is a set of libraries to facilitate smart contract writers in crafting their Ricardian contracts.  The Ricardian specification is the working specification for the above mentioned toolkit.  Please note that both projects are **alpha** releases and are subject to change.

## Important!
EOSIO.CDT Version 1.3.x introduced quite a few breaking changes. To have binary releases we needed to remove the concept of a core symbol from EOSIO.CDT. This meant drastic changes to symbol, asset and other types/functions that were connected to them. Since these changes would be disruptive, we decided to add as many disruptive changes needed for future contract writing, so that disruption should only occur once. Please read the [Upgrade guide from 1.2 to 1.3](./upgrading/1.2-to-1.3) section of this readme.

## Binary Releases
EOSIO.CDT currently supports Mac OS X brew, Linux x86_64 Debian packages, and Linux x86_64 RPM packages.

**If you have previously installed EOSIO.CDT, run the `uninstall` script (it is in the directory where you cloned EOSIO.CDT) before downloading and using the binary releases.**

### Mac OS X Brew Install
```sh
$ brew tap eosio/eosio.cdt
$ brew install eosio.cdt
```

### Mac OS X Brew Uninstall
```sh
$ brew remove eosio.cdt
```

### Debian Package Install
```sh
$ wget https://github.com/eosio/eosio.cdt/releases/download/v1.6.3/eosio.cdt_1.6.3-1-ubuntu-18.04_amd64.deb
$ sudo apt install ./eosio.cdt_1.6.3-1-ubuntu-18.04_amd64.deb
```

### Debian Package Uninstall
```sh
$ sudo apt remove eosio.cdt
```

### RPM Package Install
```sh
$ wget https://github.com/eosio/eosio.cdt/releases/download/v1.6.3/eosio.cdt-1.6.3-1.el7.x86_64.rpm
$ sudo yum install ./eosio.cdt-1.6.3-1.el7.x86_64.rpm
```

### RPM Package Uninstall
```sh
$ sudo yum remove eosio.cdt
```

## Guided Installation or Building from Scratch
```sh
$ git clone --recursive https://github.com/eosio/eosio.cdt
$ cd eosio.cdt
$ mkdir build
$ cd build
$ cmake ..
$ make -j8
```

From here onward you can build your contracts code by simply exporting the `build` directory to your path, so you don't have to install globally (makes things cleaner).
Or you can install globally by running this command

```sh
sudo make install
```

### Uninstall after manual installation

```sh
$ sudo rm -fr /usr/local/eosio.cdt
$ sudo rm -fr /usr/local/lib/cmake/eosio.cdt
$ sudo rm /usr/local/bin/eosio-*
```

## Installed Tools
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

Below tools are not installed after brew install, you get them only by building the repository and installing from scracth, [see here](#guided_installation_or_building_from_scratch)
eosio-abidiff
eosio-ranlib
eosio-ar
eosio-objdump
eosio-readelf

## Contributing

[Contributing Guide](../CONTRIBUTING.md)

[Code of Conduct](../CONTRIBUTING.md#conduct)

## License

[MIT](../LICENSE)

## Important

See LICENSE for copyright and license terms.  Block.one makes its contribution on a voluntary basis as a member of the EOSIO community and is not responsible for ensuring the overall performance of the software or any related applications.  We make no representation, warranty, guarantee or undertaking in respect of the software or any related documentation, whether expressed or implied, including but not limited to the warranties or merchantability, fitness for a particular purpose and noninfringement. In no event shall we be liable for any claim, damages or other liability, whether in an action of contract, tort or otherwise, arising from, out of or in connection with the software or documentation or the use or other dealings in the software or documentation.  Any test results or performance figures are indicative and will not reflect performance under all conditions.  Any reference to any third party or third-party product, service or other resource is not an endorsement or recommendation by Block.one.  We are not responsible, and disclaim any and all responsibility and liability, for your use of or reliance on any of these resources. Third-party resources may be updated, changed or terminated at any time, so the information here may be out of date or inaccurate.
