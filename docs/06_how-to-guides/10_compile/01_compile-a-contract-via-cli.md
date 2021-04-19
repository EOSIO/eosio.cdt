---
content_title: How to compile a smart contract via CLI
---

## Overview

This guide provides instructions how to compile a smart contract using the command line interface (CLI).

## Reference

See the following code reference:

* The [`eosio-cpp`](https://developers.eos.io/manuals/eosio.cdt/latest/command-reference/eosio-cpp) tool.

## Before you begin

* You have the source of the contract saved in a local folder, e.g. `./examples/hello/`
For details on how to create your first contract follow the [Hello World Contract](https://developers.eos.io/welcome/latest/smart-contract-guides/hello-world) guide.

## Procedure

Follow the following steps to compile your contract.

1. Navigate to the hello folder in examples `./examples/hello`. You should see the `./src/hello.cpp` file.

2. Run the following commands:

    ```sh
    mkdir build
    cd build
    eosio-cpp -abigen ../src/hello.cpp -o hello.wasm -I ../include/
    ```

    Where:
    - `eosio-cpp` = Is the [`eosio-cpp`](https://developers.eos.io/manuals/eosio.cdt/latest/command-reference/eosio-cpp) tool.
    - `-abigen` = It instructs the `eosio-cpp` tool to generate ABI file.
    - `../src/hello.cpp` = Is the input cpp source file to be compiled.
    - `-o hello.wasm` = It instructs the `eosio-cpp` tool who to name the output wasm file.
    - `-I ../include/` = It tells `eosio-cpp` tool what the include folder path is, in this particular case it is relative path.

3. Verify the following two files were generated:

* the compiled binary wasm: `hello.wasm`,
* and the generated ABI file: `hello.abi`.

## Summary

In conclusion, the above instructions show how to compile a smart contract using the command line interface (CLI).
