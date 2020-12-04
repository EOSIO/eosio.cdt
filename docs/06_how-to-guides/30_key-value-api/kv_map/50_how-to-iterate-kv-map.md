---
content_title: How-To Iterate Through Key-Value Map
link_text: "How-To Iterate Through Key-Value Map"
---

## Summary

This how-to procedure provides instructions to iterate through a `Key-Value Map` (`kv map`) and read values from it.

## Prerequisites

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

## Procedures

Complete the following steps to implement an action that is iterating through the first N `person` objects in `kv map` and prints their first and last names:

1. Create a new action `iterate`, which takes as an input parameter the number of iterations to be executed.
2. 

Refer to the following possible implementation to implement an action that is iterating through the first `iterations_count` objects in `my_map` and prints their first and last names:

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

      // iterates over the first iterations_count persons in the table
      // and prints their first and last names
      [[eosio::action]]
      void iterate(int iterations_count);

   private:
      my_map_t my_map{};
};
```

`smartcontract.cpp file`

```cpp
// iterates over the first iterations_count persons using
// and prints their first and last names
[[eosio::action]]
void kv_map::iterate(int iterations_count) {

   int current_iteration = 0;
   for ( const auto& person_detail : my_map ) {
      if (current_iteration ++ < iterations_count) {
         eosio::print_f(
            "Person %: {%, %}. ",
            current_iteration,
            person_detail.second().first_name,
            person_detail.second().last_name);
      }
      else {
         break;
      }
   }
}```
