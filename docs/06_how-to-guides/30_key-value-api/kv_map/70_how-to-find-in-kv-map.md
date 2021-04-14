---
content_title: How-To Find in Key-Value Map
link_text: "How-To Find in Key-Value Map"
---

## Overview

This how-to provides instructions to find an object in `Key-Value Map` (`kv map`) based on the unique key.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index)
* A smart contract named `smrtcontract`
* A user defined type named `person`, which defines the data stored in the map
* A `kv map` object, name `my_map`, which stores objects of type `person`, with unique keys of type `int`.

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

Complete the following steps to find a `person` object with a given ID:

1. Create a new action in your contract, named `delete`, which takes as input parameters the person ID.
2. Use the `find()` function defined for the `kv::map` type, with the give ID as parameter, to find the `person` with the given ID as unique key.

Refer to the following reference implementation to find `person` object with a given ID as unique key:

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

      // finds a person based on unique id
      [[eosio::action]]
      void find(int id);

   private:
      my_map_t my_map{};
};
```

`smartcontract.cpp file`

```cpp
// finds a person based on unique id
[[eosio::action]]
void kv_map::find(int id) {

   auto itr = my_map.find(id);

   // check if person was found
   if (itr != my_map.end()) {
      // extract person from iterator and delete it
      const auto person_found = itr->second();

      eosio::print_f("Person with unique ID=% was found: %, %, %.",
         id, person_found.first_name, person_found.last_name, person_found.personal_id);
   }
   else {
      eosio::print_f("Person with ID % not found.", id);
   }
}
```

## Summary

In conclusion, the above instructions show how to find an object in `Key-Value Map` (`kv map`) based on the unique key.

## Next Steps

The following options are available when you complete the procedure:

* [Update](30_how-to-upsert-into-kv-map.md) the `person` found.
* [Delete](40_how-to-delete-from-kv-map.md) the `person` found.
