---
content_title: How-To Allow Users to Pay for Key-Value Map Resources
link_text: "How-To Allow Users to Pay for Key-Value Map Resources"
---

## Summary

This how-to procedure provides instructions to allow users to pay for the resources needed to store data in a `Key-Value Map` (`kv map`).

Before you begin, complete the following prerequisites:

* An EOSIO development environment, for details consult the [Get Started](https://developers.eos.io/welcome/latest/getting-started/development-environment/introduction) Guide
* A smart contract named `smrtcontract`
* A user defined type named `person`, which defines the data stored in the map
* A `kv map` object, name `my_map`, which stores objects of type `person`, with unique keys of type `int`.

Refer to the following possible implementation of your starting point.

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

Complete the following steps to allow the payer for the resources needed to store the `person` object in the `my_map` object, to be the person account which is stored.

1. Create a new action in your contract, named `upsert`, which takes as input parameters the person `id`, an `account_name`, a `first_name` and a `last_name`.
2. Create an instance of the `person` class, named `person_upsert`, based on the input parameters: `account_name`, `first_name` and `last_name`.
3. Use the `[]` operator defined for the `kv::map` type. Pass as the input parameter for the `[]` operator an `std::pair<int, person>` instance with its first parameter the unique `id` and the second parameter the account which is paying for the resources `account_name`.

Refer to the following possible implementation to insert a new `person` object, and then update it, in the `kv map`:

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
      // the payer is the account_name, specified as input parameter.
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

   // upsert into kv::map and set the payer to be the account_name
   my_map[std::pair<int, eosio::name>(id, account_name)] = person_upsert;
}
```

## Next Steps

The following options are available when you complete the procedure:

* [Verify](70_how-to-find-in-kv-map.md) if the newly inserted `person` actually exists in the map. To accomplish this task use the `find()` function of the `kv_map`.
* [Delete](40_how-to-delete-from-kv-map.md) the newly created or updated `person` from the map. To accomplish this task, use the `erase()` function of the `kv map`.
