---
content_title: How-To Check a Record in a Key-Value Table
link_text: "How-To Check a Record in a Key-Value Table"
---

## Overview

This how-to provides instructions to check if a specific object exists in a `Key-Value Table` (`kv table`).

[[caution | Alpha version]]
| `Key-Value Table` is designated as `alpha` and should not be used in production code.

Use the  method `exists` defined by the `eosio::kv::table::index` class to accomplish this task.

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

Complete the following steps to implement an action that is verifying whether a particular `person` identified by its `account_name` exists in the `address_table`:

1. Create a new action `verify`, which takes as an input parameter the account name of the person to be verified.
2. In the `verify` action access the instance of `address_table` by declaring a local variable of `address_table` type.
3. Call the `exists()` method of the `account_name` index defined in the `kv table` class and pass the account name of the person to be verified.

Refer to the following reference implementation to implement an action that is verifying whether a particular `person` identified by its `account_name` exists in the `address_table`:

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

     // checks if a person with a specific account name exists in addressbook
     [[eosio::action]]
     bool verify(eosio::name account_name);

     using verify_action = action_wrapper<"verify"_n, &smrtcontract::verify>;
};
```

`smartcontract.cpp file`

```cpp
// checks if a person with a specific account name exists in addressbook
[[eosio::action]]
bool smrtcontract::verify(string personal_id, string country) {
  address_table addresses{"kvaddrbook"_n};

  return addresses.account_name_uidx.exists({personal_id, country});
}
```

## Summary

In conclusion, the above instructions show how to check if a specific object exists in a `Key-Value Table` (`kv table`).

## Next Steps

The following options are available when you complete the procedure:

* Implement business logic and rely on the information that the `person` object exists in the table.
