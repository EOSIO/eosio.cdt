# Blanc

**BLockchAiN Contract toolchain**

Welcome to Blanc!  Blanc is a toolchain for WebAssembly-based blockchain contracts.  Currently, Blanc supports [EOSIO](https://github.com/EOSIO) and [CosmWasm](https://github.com/CosmWasm) (experimental), but we have a plan to support other WebAssembly-based contracts like [Substrate](https://github.com/paritytech/substrate/tree/df501229/frame/contracts).  In EOSIO ecosystem, block.one already provides the official version of contract toolchain [eosio.cdt](https://github.com/EOSIO/eosio.cdt), but Blanc takes a different approach from it.  eosio.cdt uses the modified version of Clang/LLVM, so you have to wait for block.one's updates to utilize the latest version of Clang/LLVM.  Blanc was adapted from eosio.cdt, but uses the vanilla Clang/LLVM for easier maintenance and faster adoption of state-of-the-art technologies.  At this point in time (Jun, 2021), eosio.cdt uses Clang-9, but Blanc uses Clang-12.  The only thing you need to care about is that your Clang/LLVM installed in your system supports Clang plugins and WASM binary generation.  You should have Clang-12 or higher, and in MacOS, you need to install `llvm` by brew.  Clang/LLVM installed by Xcode were not built for supporting plugins.

By using the latest Clang/LLVM, generated WASM binaries will have smaller size by about 10%. (eg. `eosio.token` 18KB &rightarrow; 16KB, `eosio.system` 276KB &rightarrow; 252KB)  All unit tests provided by eosio.cdt and [eosio.contracts](https://github.com/EOSIO/eosio.contracts) are passed and all generated ABIs are identical.


## Nota Bene

We changed branch strategy. The branch for daily development is `master` now, so if you want to build from the source, please use `release` branch for stable version.

## Binary Releases

The prebuilt binares are provided for Ubuntu 20.04 and MacOS Big Sur.

### Ubuntu 20.04

Clang-12 is backported to Ubuntu 20.04, so launchpad PPA support is restored.

```sh
sudo add-apt-repository ppa:conr2d/blanc
sudo apt install blanc
```

### MacOS Big Sur

You can install Clang/LLVM compatible with Blanc by package manager [brew](https://brew.sh/).

```sh
brew install llvm@12
```

Install Blanc by brew.

```sh
brew tap turnpike/blanc
brew install blanc
```

## Installed Tools

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
