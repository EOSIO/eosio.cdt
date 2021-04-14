---
content_title: How-To Use Key-Value Table
link_text: "How-To Use Key-Value Table"
---

## Overview

This how-to demonstrates how to define and use a `Key-Value Table` (`kv table`) in a smart contract.

To accomplish this task, define the user type which will be stored in the `kv table`, and extend the `eosio::kv::table` template class with a specialized definition based on the user defined type.

[[caution | Alpha version]]
| `Key-Value Table` is designated as `alpha` and should not be used in production code.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index).
* A smart contract named `smrtcontract`.
* A user defined type named `person`, which defines the data stored in the table.

Refer to the following reference implementation for your starting point:

`smartcontract.hpp file`

```cpp
struct person {
  eosio::name account_name;
  std::string first_name;
  std::string last_name;
  std::string street;
  std::string city;
  std::string state;
  std::string country;
  std::string personal_id;
};
class [[eosio::contract]] smrtcontract : public contract {
  public:
     using contract::contract;
};
```

## Procedure

Complete the following steps to define the `address_table` type, based on the `eosio::kv::table`, which stores objects of type `person`:

1. Define the structure or class `address_table` in the scope of your smart contract class, for the abi generation to find it and place it into the abi file.
2. Derive `address_table` from `eosio::`kv::table` class template. Pass the `person` user defined type as the type parameter for `eosio::`kv::table` base class and the name of the `kv table`, let’s say `kvaddrbook`.
3. Annotate `address_table` type with `[[eosio::table]]`, and make sure it is placed after the `struct` keyword but before the name of the type.
4. Define a primary index `first_name_idx` based on the property `person::first_name`. Every `kv table` requires a primary index to be defined based on a property that stores unique values.
5. In the `address_table` constructor, call the `init(...)` base class method with the two parameters:
    1. The first parameter, of type `eosio::name`, is the contract that owns the table.
    2. The second parameter is the `account_name_uidx` primary index.

Refer below for a possible implementation of the above described steps:

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
};
```

## Summary

In conclusion, the above instructions show how to define and use a `Key-Value Table` (`kv table`) in a smart contract.

## Next Steps

The following options are available when you complete the procedure:

* You can [create one or more unique indexes](20_how-to-create-indexes-kv-table.md) using the `KV_NAMED_INDEX` macro or the `eosio::kv::table::index` template class.
* You can [create one or more non-unique indexes](20_how-to-create-indexes-kv-table.md) using the `KV_NAMED_INDEX` macro or the `eosio::kv::table::index` template class.
* You can access the defined `kv table` and perform operations on it and its defined indexes:
  * [How To Upsert Into KV Table](./30_how-to-upsert-into-kv-table.md)
  * [How To Delete From KV Table](./40_how-to-delete-from-kv-table.md)
  * [How To Iterate A KV Table](./50_how-to-iterate-kv-table.md)
  * [How To Check A Record In KV Table](./60_how-to-check-a-record-kv-table.md)
  * [How To Find In KV Table](./70_how-to-find-in-kv-table.md)
  * [How To Query Range In KV Table](./80_how-to-query-range-in-kv-table.md)
  * [How To Allow Users To Pay](./90_how-to-allow-users-to-pay-kv-table.md)
