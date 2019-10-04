## Binary Releases
EOSIO.CDT currently supports Mac OS X brew, Linux x86_64 Debian packages, and Linux x86_64 RPM packages.

**If you have previously installed EOSIO.CDT, please run the `uninstall` script (it is in the directory where you cloned EOSIO.CDT) before downloading and using the binary releases.**

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
$ wget https://github.com/eosio/eosio.cdt/releases/download/v1.6.1/eosio.cdt_1.6.1-1_amd64.deb
$ sudo apt install ./eosio.cdt_1.6.1-1_amd64.deb
```
### Debian Package Uninstall
```sh
$ sudo apt remove eosio.cdt
```

### Fedora RPM Package Install
```sh
$ wget https://github.com/eosio/eosio.cdt/releases/download/v1.6.1/eosio.cdt-1.6.1-1.fedora-x86_64.rpm
$ sudo yum install ./eosio.cdt-1.6.1-1.fedora-x86_64.rpm
```

### Fedora RPM Package Uninstall
```sh
$ sudo yum remove eosio.cdt
```

### Centos RPM Package Install
```sh
$ wget https://github.com/eosio/eosio.cdt/releases/download/v1.6.1/eosio.cdt-1.6.1-1.centos-x86_64.rpm
$ sudo yum install ./eosio.cdt-1.6.1-1.centos-x86_64.rpm
```

### Centos RPM Package Uninstall
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


License
----
[MIT](../LICENCE)
