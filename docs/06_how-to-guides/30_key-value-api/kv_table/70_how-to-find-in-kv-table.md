---
content_title: How-To Find in Key-Value Table
link_text: "How-To Find in Key-Value Table"
---

## Overview

This how-to provides instructions to find a specific object in a `Key-Value Table` (`kv table`).

[[caution | Alpha version]]
| `Key-Value Table` is designated as `alpha` and should not be used in production code.

Use the method  `find()` defined by the `eosio::kv::table::index` class to accomplish this task.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index).
* A smart contract named `smrtcontract`.
* A user defined type which defines the data stored in the table, named `person`.
* A `kv table` type which stores objects of type `person`, named `address_table`.
* Each `person` object has the following properties:
  * `account_name`,
  * `first_name`,
  * `last_name`,
  * `personal_id`.
* A unique index, named `account_name_uidx`, defined on the `account_name` property..

Refer to the following reference implementation for your starting point:

`smartcontract.hpp file`

```cpp
struct person {
  eosio::name account_name;
  std::string first_name;
  std::string last_name;
  std::string personal_id;
};

class [[eosio::contract]] smrtcontract : public contract {
    struct [[eosio::table]] address_table : eosio::kv::table<person, "kvaddrbook"_n> {

     index<name> account_name_uidx {
        name{"accname"_n},
        &person::account_name };

     address_table(name contract_name) {
        init(contract_name, account_name_uidx)
     }
  };
  public:
     using contract::contract;
};
```

## Procedure

Complete the following steps to implement an action to find a particular `person` identified by the `account_name` in the `address_table` and returns the person back to the caller:

1. Create a new action `find`, which takes as input parameters the account name of the person to be verified.
2. In the `find` action access the instance of `address_table` by declaring a local variable of `address_table` type.
3. Call the `find` method of the `account_name` index defined in the table and pass the account name of the person to be verified.
4. Check if the `person` was found and return it back to the caller; if person was not found return an empty `person` object.

Refer to the following reference implementation to implement an action to find a particular `person` identified by the `account_name` in the `address_table` and returns the person back to the caller:

`smartcontract.hpp`

```cpp
class [[eosio::contract]] smrtcontract : public contract {
    struct [[eosio::table]] address_table : eosio::kv::table<person, "kvaddrbook"_n> {

     index<name> account_name_uidx {
        name{"accname"_n},
        &person::account_name };

     address_table(name contract_name) {
        init(contract_name, account_name_uidx)
     }
  };
  public:
     using contract::contract;

     // Finds a person based on the primary key account_name.
     [[eosio::action]]
     bool find(eosio::name account_name);

     using find_action = action_wrapper<"find"_n, &smrtcontract::find>;
};
```

`smartcontract.cpp file`

```cpp
// Finds a person based on the primary key account_name.
// Makes use of index find function, which returns an iterator, and then uses iterator value.
[[eosio::action]]
person smrtcontract::find(name account_name) {
  address_table addresses{"kvaddrbook"_n};

  // search for person by primary key account_name
  auto itr = addresses.account_name_uidx.find(account_name);

  // check if person was found
  if (itr != addresses.account_name_uidx.end()) {
     // extract person from iterator, print it and return it to the action sender
     const auto& person_found = itr.value();

     // return found person from action
     return person_found;
  }
  else {
     eosio::print_f("Person not found in addressbook.");
     // return empty person from action
     return person{};
  }
}
```

## Summary

In conclusion, the above instructions show how to find a specific object in a `Key-Value Table` (`kv table`).

## Next Steps

The following options are available when you complete the procedure:

* [Update](30_how-to-upsert-into-kv-table.md) the `person` found and returned.
* [Delete](40_how-to-delete-from-kv-table.md) the `person` found and returned.
