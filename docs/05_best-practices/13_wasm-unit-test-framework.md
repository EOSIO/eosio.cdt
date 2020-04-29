---
content_title: WASM Unit Test Framework
link_text: WASM Unit Test Framework
---

## Introduction

The WASM Unit Test Framework makes testing smart contracts easier for the developers because they are able to utilize the same `eosio.cdt` libraries they used to write the contract, to test the contract.

## Basics of WASM Unit Test Framework

By providing a set of constructs dedicated for smart contract testing in the same EOSIO smart contract development toolkit (the `eosio.cdt`) and making use of the [`Catch2` C++ test framework](https://github.com/catchorg/Catch2) developers will save time and focus more on feature development.

`Catch2` is a modern, C++-native, header-only, test framework for unit-tests, TDD and BDD which makes it lean, powerful and flexible in the same time and a very good fit fot the purpose of testing smart contracts.

Mainly the WASM Unit Test Framework implements the class `test_chain` which is simulating a blockchain for the smart contract tests to run on and provides useful utility functionality like `push_transaction`, `set_code`, `create_token` and much more.

For more details about using it check the [`WASM Unit Test Framework`](../09_tutorials/03_wasm-unit-test-framework.md) tutorial.

For fast reference on how to accomplish specific things with `WASM Unit Test Framework` follow any of the dedicated [How Tos](../06_how-to-guides/10_wasm-unit-test-framework/index.md).

And last but not least consult the [Reference documentation for WASM Unit Test Framework](../classeosio_1_1test__chain) to learn about each individual function or construct implemented and made available through this framework.
