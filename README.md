# Blanc

**Toolchain for WebAssembly-based Blockchain Contracts**

## Overview

Welcome to Blanc!  Blanc is the toolchain for WebAssembly-based blockchain contracts, and the name of "BLANC" comes from BLockcAiN Contract toolchain.  Currently, Blanc supports [EOSIO](https://github.com/EOSIO) only, but we have a plan to support other WebAssembly-based contracts like [Cosmwasm](https://github.com/CosmWasm) which is widely used in blockchains based on Cosmos SDK (It is well known for its consensus algorithm, Tendermint).  In EOSIO ecosystem, there is the official version of contract toolchain provided by block.one already, but Blanc takes a different approach from it.  eosio.cdt uses the modified version of Clang/LLVM, so you have to wait for block.one's updates to utilize the latest version of Clang/LLVM.  Blanc was adapted from eosio.cdt, but uses the vanilla Clang/LLVM for easier maintenance and faster adoption of state-of-the-art technologies.  At this point in time (Jan, 2021), eosio.cdt uses Clang-7, and its upcoming release uses Clang-9, but Blanc uses Clang-11 (and Clang-12 in development can be used also).  The only thing you need to care about is that your Clang/LLVM installed in your system supports Clang plugins and WASM binary generation.  You should have Clang-11 or higher, and in MacOS, you need to install `llvm` by brew.  Clang/LLVM installed by Xcode were not built for supporting plugins.

By using the latest Clang/LLVM, generated WASM binaries will have smaller size by about 10%. (eg. `eosio.token` 18KB &rightarrow; 16KB, `eosio.system` 276KB &rightarrow; 252KB)  All unit tests provided by eosio.cdt and [eosio.contracts](https://github.com/EOSIO/eosio.contracts) are passed and all generated ABIs are identical.


### Binary Releases

The prebuilt binares are provided for Ubuntu 20.04 and MacOS Big Sur.

#### Ubuntu 20.04

Clang-11 is not in the default Ubuntu repository, so it needs to be installed from llvm.org.  Launchpad PPA will be supported in the future. (The offical repository needs to provide Clang-11 or higher, but not yet in focal)

```sh
bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
```

Download debian package and install with apt.

```sh
export BLANC_DOWNLOAD_URL=$(curl https://api.github.com/repos/turnpike/blanc/releases/latest | awk '/browser_download_url.*deb/{ print $2 }' | tr -d '"')
wget $BLANC_DOWNLOAD_URL
sudo apt install ./$(echo $BLANC_DOWNLOAD_URL | awk -F "/" '{ print $NF }')
rm -f $(echo $BLANC_DOWNLOAD_URL | awk -F "/" '{ print $NF }')
unset BLANC_DOWNLOAD_URL
```

#### MacOS Big Sur

You can install Clang/LLVM compatible with Blanc by package manager [brew](https://brew.sh/).

```sh
brew install llvm
```

Install Blanc by brew.

```sh
brew tap turnpike/blanc
brew install blanc
```

### Installed Tools
---
* blanc++
* blanc
* blanc-ld
* blanc-pp
* blanc-ar
* blanc-ranlib

## Contributing

[Contributing Guide](./CONTRIBUTING.md)

[Code of Conduct](./CONTRIBUTING.md#conduct)

## License

[MIT](./LICENSE)

## Important

See [LICENSE](./LICENSE) for copyright and license terms.

All repositories and other materials are provided subject to the terms of this [IMPORTANT](./IMPORTANT.md) notice and you must familiarize yourself with its terms.  The notice contains important information, limitations and restrictions relating to our software, publications, trademarks, third-party resources, and forward-looking statements.  By accessing any of our repositories and other materials, you accept and agree to the terms of the notice.
