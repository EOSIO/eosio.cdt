---
content_title: How to compile a smart contract with CMake
---

## Overview

This guide provides instructions on how to compile a smart contract with CMake.

## Before you begin

* You have the source of the contract saved in a local folder, e.g. `./examples/hello/`
For details on how to create your first contract follow the [Hello World Contract](https://developers.eos.io/welcome/latest/smart-contract-guides/hello-world) guide.

## Procedure

Follow the following steps to compile your contract.

1. Navigate to the hello folder in examples (./examples/hello), you should then see the ./src/hello.cpp file
2. Run following commands:

    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

3. Verify the following two files were generated:

* the compiled binary wasm: `hello.wasm`,
* and the generated ABI file: `hello.abi`.

## Summary

In conclusion, the above instructions show how to compile a smart contract with CMake.
