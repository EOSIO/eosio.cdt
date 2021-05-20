---
content_title: How-To Upsert Into Key-Value Table
link_text: "How-To Upsert Into Key-Value Table"
---

## Overview

This how-to provides instructions to upsert into `Key-Value Table` (`kv table`).

[[caution | Alpha version]]
| `Key-Value Table` is designated as `alpha` and should not be used in production code.

Use the method `put` defined by the `eosio::kv::table` type to accomplish this task.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index)
* A smart contract named `smrtcontract`
* A user defined type named `person`, which defines the data stored in the table
* A `kv table` type which stores objects of type `person`, named `address_table`. The primary index of the `kv table` is defined based on the `person::account_name` property.

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

Complete the following steps to insert a new `person` object, and then update it, in the `kv table`:

1. Create a new action `upsert` in your smart contact class, which takes as input parameters an account name, a first name, a last name and a personal id.
2. In the `upsert` action access the instance of `address_table` by declaring a local variable of `address_table` type.
3. And then call the `put` method of the `address_table` and pass to it a newly created `person` object based on the action’s input parameters.

Refer to the following reference implementation to insert a new `person` object, and then update it, in the `kv table`:

`smartcontract.hpp file`

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

     // creates if not exists, or updates if already exists, a person
     [[eosio::action]]
     void upsert(eosio::name account_name,
        string first_name,
        string last_name,
        string country,
        string personal_id);

     using upsert_action = action_wrapper<"upsert"_n, &smrtcontract::upsert>;
};
```

`smartcontract.cpp file`

```cpp
// creates if not exists, or updates if already exists, a person
[[eosio::action]]
void smrtcontract::upsert(
     eosio::name account_name,
     string first_name,
     string last_name,
     string personal_id) {
  address_table addresses{"kvaddrbook"_n};

  // call put which upserts into kv table
  addresses.put({account_name, first_name, last_name, personal_id}, get_self());
}
```

## Summary

In conclusion, the above instructions show how to upsert into `Key-Value Table` (`kv table`).

## Next Steps

The following options are available when you complete the procedure:

* [Check](60_how-to-check-a-record-kv-table.md) if the newly inserted `person` actually exists in the table. To accomplish this task, use the `exists()` function of any index defined for the table.
* [Retrieve](70_how-to-find-in-kv-table.md) the newly inserted or updated `person` from the table. To accomplish this task, use the `find()` function of any index defined for the table.
* [Delete](40_how-to-delete-from-kv-table.md) the newly created or updated `person` from the table. To accomplish this task, use the `erase()` function of the `kv table`.
