---
content_title: How-To Use Key-Value Map
link_text: "How-To Use Key-Value Map"
---

## Overview

This how-to demonstrates how to define and use a `Key-Value Map` (`kv map`) in a smart contract.

To accomplish this task do the following:

1. Instantiate an object of type `eosio::kv::map`.
2. Specify the name for the instantiated `eosio::kv::map` object.
3. Specify the type for the map's key.
4. Specify the type for the values stored for each key.
5. The key and the values types can be of any standard type or a user defined type.

## Reference

See the following code reference:

* The [`kv::map`](https://developers.eos.io/manuals/eosio.cdt/v1.8/classeosio_1_1kv_1_1map) class.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index)
* A smart contract named `smrtcontract`
* A user defined type named `person`, which defines the data stored in the map

Refer to the following reference implementation for your starting point:

`smartcontract.hpp file`

```cpp
struct person {
  eosio::name account_name;
  std::string first_name;
  std::string last_name;
};

class [[eosio::contract]] smartcontract : public eosio::contract {
   public:
      using contract::contract;
      smartcontract(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds)
         : contract(receiver, code, ds) {}
};
```

## Procedure

Complete the following steps to define the `my_map_t` type, based on the `eosio::kv::map`, which stores objects of type `person` with unique keys of type `int` and instantiate a map object of type `my_map_t`:

* Define the `my_map_t` type based on `eosio::kv::map`.
* Specify `"kvmap"_n`, which is an `eosio::name`, as the first parameter, to name the map object.
* Specify `int` as the second parameter to give the type of the unique keys.
* Specify `person` as the third parameter to give the type of the values stored in the map with each key.
* Declare and instantiate, as a private data member, an instance of the type `my_map_t`, and name it `my_map`.

Refer below for a possible implementation of the above described steps:

`smartcontract.hpp file`

```cpp
class [[eosio::contract]] smartcontract : public eosio::contract {

   using my_map_t = eosio::kv::map<"kvmap"_n, int, person>;

   public:
      using contract::contract;
      smartcontract(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds)
         : contract(receiver, code, ds) {}

   private:
      my_map_t my_map{};
};
```

## Summary

In conclusion, the above instructions show how to define and use a `Key-Value Map` (`kv map`) in a smart contract.

## Next Steps

The following option is available when you complete the procedure:

* You [can add values](30_how-to-upsert-into-kv-map.md) in the map object created.
