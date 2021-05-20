---
content_title: How-To Upsert Into Key-Value Map
link_text: "How-To Upsert Into Key-Value Map"
---

## Overview

This how-to provides instructions to upsert into `Key-Value Map` (`kv map`). Upsert means insert when the item doesn't already exist, and update the item if it already exists in the map.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index)
* A smart contract named `smrtcontract`
* A user defined type named `person`, which defines the data stored in the map
* A `kv map` object, name `my_map`, which stores objects of type `person`, with unique keys of type `int`

Refer to the following reference implementation for your starting point:

`smartcontract.hpp file`

```cpp
struct person {
  eosio::name account_name;
  std::string first_name;
  std::string last_name;
};

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

## Procedure

Complete the following steps to insert a new `person` object with a given ID, if it doesn't exist already, or update it in the `kv map` if the `person` with the given ID already exists:

1. Create a new action in your contract, named `upsert`, which takes as input parameters the person `id`, an `account_name`, a `first_name` and a `last_name`.
2. Create an instance of the `person` class, named `person_upsert`, based on the input parameters: `account_name`, `first_name` and `last_name`.
3. Use the `[]` operator defined for the `kv::map` type, and set the newly created `person_upsert` object as the value for the `id` key.

Refer to the following reference implementation to insert a new `person` object, and then update it, in the `kv map`:

`smartcontract.hpp file`

```cpp
struct person {
  eosio::name account_name;
  std::string first_name;
  std::string last_name;
};

class [[eosio::contract]] smartcontract : public eosio::contract {

   using my_map_t = eosio::kv::map<"kvmap"_n, int, person>;

   public:
      using contract::contract;
      smartcontract(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds)
         : contract(receiver, code, ds) {}

      // inserts a person if not exists, or updates it if already exists.
      // the payer for the resources consumed is the account that created the kv::map
      // object in the first place, the account that owns this smart contract.
      [[eosio::action]]
      void upsert(int id,
         eosio::name account_name,
         std::string first_name,
         std::string last_name);

   private:
      my_map_t my_map{};
};
```

`smartcontract.cpp file`

```cpp
// inserts if not exists, or updates if already exists, a person
[[eosio::action]]
void smartcontract::upsert(
      int id,
      eosio::name account_name,
      std::string first_name,
      std::string last_name) {

   // create the person object which will be stored in kv::map
   const person& person_upsert = person{
      account_name = account_name,
      first_name = first_name,
      last_name = last_name};

   // upsert into kv::map
   my_map[id] = person_upsert;
}
```

## Summary

In conclusion, the above instructions show how to upsert into `Key-Value Map` (`kv map`).

## Next Steps

The following options are available when you complete the procedure:

* [Verify](70_how-to-find-in-kv-map.md) if the newly inserted `person` actually exists in the map. To accomplish this task use the `find()` function of the `kv_map`.
* [Delete](40_how-to-delete-from-kv-map.md) the newly created or updated `person` from the map. To accomplish this task, use the `erase()` function of the `kv map`.
