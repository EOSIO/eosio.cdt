---
content_title: How-To Delete from Key-Value Map
link_text: "How-To Delete from Key-Value Map"
---

## Summary

This how-to procedure provides instructions to delete from `Key-Value Map` (`kv map`) based on the unique key.

## Prerequisites

Before you begin, complete the following prerequisites:

* An EOSIO development environment, for details consult the [Get Started](https://developers.eos.io/welcome/latest/getting-started/development-environment/introduction) Guide
* A smart contract named `smrtcontract`
* A user defined type named `person`, which defines the data stored in the map
* A `kv map` object which stores objects of type `person`, with unique keys of type `int`.
* A `person` object with key value 1, of type integer, is stored in the `kv map` object.

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

Complete the following steps to insert a new `person` object with a given ID, if it doesn't exist already, or update it in the `kv map` if the `person` with the given ID already exists:

1. Create a new action in your contract, named `upsert`, which takes as input parameters the person ID, an account name, a first name and a last name.
2. Create an instance of the `person` class, named `person_upsert`, based on the input parameters: `account_name`, `first_name` and `last_name`.
3. Use the `[]` operator defined for the `kv::map` type, and set the newly created `person_upsert` object as the value for the `ID` key.

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

      // inserts if not exists, or updates if already exists, a person
      [[eosio::action]]
      void delete(int id);

   private:
      my_map_t my_map{};
};
```

`smartcontract.cpp file`

```cpp
// deletes a person based on unique id
[[eosio::action]]
void kv_map::delete(int id) {

   // search for person by primary key account_name
   auto itr = my_map.find(id);

   // check if person was found
   if (itr != my_map.end()) {
      // extract person from iterator and delete it
      const auto person_found = itr->second();

      // delete it from kv::map
      my_map.erase(id);
      eosio::print_f("Person (%, %, %) was successfully deleted.",
         person_found.first_name, person_found.last_name, person_found.personal_id);
   }
   else {
      eosio::print_f("Person with ID % not found.", id);
   }
}
```
