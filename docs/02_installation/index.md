---
content_title: Binary Releases
---

EOSIO.CDT currently supports Mac OS X brew, Linux x86_64 Debian packages, and Linux x86_64 RPM packages.

**If you have previously installed EOSIO.CDT, run the `uninstall` script (it is in the directory where you cloned EOSIO.CDT) before downloading and using the binary releases.**

## Mac OS X Brew Install

```sh
brew tap eosio/eosio.cdt
brew install eosio.cdt
```

## Mac OS X Brew Uninstall

```sh
brew remove eosio.cdt
```

## Debian Package Install

```sh
wget https://github.com/EOSIO/eosio.cdt/releases/download/v1.8.0/eosio.cdt_1.8.0-1-ubuntu-18.04_amd64.deb
sudo apt install ./eosio.cdt_1.8.0-1-ubuntu-18.04_amd64.deb
```

## Debian Package Uninstall

```sh
sudo apt remove eosio.cdt
```

## RPM Package Install

```sh
wget https://github.com/EOSIO/eosio.cdt/releases/download/v1.8.0/eosio.cdt-1.8.0-1.el7.x86_64.rpm
sudo yum install ./eosio.cdt-1.8.0-1.el7.x86_64.rpm
```

## RPM Package Uninstall

```sh
sudo yum remove eosio.cdt
```

# Guided Installation or Building from Scratch

```sh
git clone --recursive https://github.com/eosio/eosio.cdt
cd eosio.cdt
mkdir build
cd build
cmake ..
make -j8
```

From here onward you can build your contracts code by simply exporting the `build` directory to your path, so you don't have to install globally (makes things cleaner).
Or you can install globally by running this command

```sh
sudo make install
```

## Uninstall after manual installation

```sh
sudo rm -fr /usr/local/eosio.cdt
sudo rm -fr /usr/local/lib/cmake/eosio.cdt
sudo rm /usr/local/bin/eosio-*
```

# Installed Tools

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

The following tools are not installed after brew install, you get them when you build the repository and install it from scratch, [see here](#guided-installation-or-building-from-scratch):

* eosio-abidiff
* eosio-ranlib
* eosio-ar
* eosio-objdump
* eosio-readelf

License
[MIT](../LICENSE)
