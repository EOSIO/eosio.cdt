---
content_title: How to compile a smart contract with CMake
---

## Preconditions
- You have the source of your contract saved in one of your local folders, e.g. `./examples/hello`
For details on how to create your first contract follow [this tutorial here](https://developers.eos.io/eosio-home/docs/your-first-contract)

Follow these steps to compile your contract:

1. Navigate to the hello folder in examples (./examples/hello), you should then see the ./src/hello.cpp file
2. Run following commands:
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```
3. This will generate two files:
- The compiled binary wasm, hello.wasm
- The generated ABI file, hello.abi
