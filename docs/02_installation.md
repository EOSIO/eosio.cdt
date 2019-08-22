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

## Guided Installation (Building from Scratch)
```sh
$ git clone --recursive https://github.com/eosio/eosio.cdt
$ cd eosio.cdt
$ ./build.sh
$ sudo ./install.sh
```

## Installed Tools
---
TO DO: is below list correct? is any of them deprecated? eosio-abimerge should it be listed here?
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

TO DO: 
these tools are not installed after brew install, how can I get them?
eosio-abidiff
eosio-ranlib
eosio-ar
eosio-objdump
eosio-readelf
eosio-merge


License
----
[MIT](../LICENCE)
