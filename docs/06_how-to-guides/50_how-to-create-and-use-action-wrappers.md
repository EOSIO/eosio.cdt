---
content_title: How to create and use action wrappers
link_text: "How to create and use action wrappers"
---

## Overview

This guide provides instructions on how to create and use an action wrapper in a smart contract.

## Code Reference

See the following code reference guide for action wrapper:

* [eosio::action_wrapper](../structeosio_1_1action__wrapper).

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index).
* A smart contract named `multi_index_example`, defined in file `multi_index_example.hpp`.
* An action `mod` which modifies the integer value `n` stored for row with key `user`.

Refer to the following reference implementation for your starting point:

```cpp
class [[eosio::contract]] multi_index_example : public contract {
  // ...
  [[eosio::action]] void mod( name user, uint32_t n );
  // ...
}
```

## Procedure

Complete the following steps to create and use `mod_action` action wrapper for the existing `mod` action in the smart contract:

1. [Define the action wrapper](#1-define-the-action-wrapper)
2. [Use the action wrapper](#2-use-the-action-wrapper)
    * [2.1. Include Header File](#21-include-header-file)
    * [2.2. Instantiate The Action Wrapper](#22-instantiate-the-action-wrapper)
    * [2.3. Send The Action Using The Action Wrapper](#23-send-the-action-using-the-action-wrapper)

### 1. Define The Action Wrapper

To define an action wrapper for the `mod` action, make use of the `eosio::action_wrapper` template, with  the first parameter the action name as a `eosio::name` and second parameter as the reference to the action method:

```diff
class [[eosio::contract]] multi_index_example : public contract {
  // ...
  [[eosio::action]] void mod(name user);
  // ...
+  using mod_action = action_wrapper<"mod"_n, &multi_index_example::mod>;
  // ...
}
```

### 2. Use The Action Wrapper

#### 2.1. Include Header File

To use the action wrapper, you have to include the header file where the action wrapper is defined:

```cpp
#include <multi_index_example.hpp>
```

#### 2.2. Instantiate The Action Wrapper

Instantiate the `mod_action` defined above, specifying the contract to send the action to as the first argument. In this case, it is assumed the contract is deployed to `multiindexex` account, and a structure which is defined by two parameters: the self account, obtained by `get_self()` call, and the `active` permission (you can modify these two parameters based on your requirements).

```diff
#include <multi_index_example.hpp>

+multi_index_example::mod_action modaction("multiindexex"_n, {get_self(), "active"_n});
```

#### 2.3. Send The Action Using The Action Wrapper

Call the `send` method of the action wrapper and pass in the `mod` action's parameters as positional arguments:

```diff
#include <multi_index_example.hpp>

multi_index_example::mod_action modaction("multiindexex"_n, {get_self(), 1});

+modaction.send("eostutorial"_n, 1);
```

For a full example see the [`multi_index` contract implementation](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).

## Summary

In conclusion, the above instructions show how to create and use action wrapper in a smart contract.
