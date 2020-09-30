---
content_title: How-To Use Key-Value Table
link_text: "How-To Use Key-Value Table"
---

## Summary

This how-to procedure demonstrates how to define and use a `Key-Value Table` (`kv table`) in your smart contract.

To accomplish this task, define the user type which will be stored in the `kv table`, and extend the `eosio::kv::table` template class with a specialized definition based on the user defined type.

## Prerequisites

* The EOSIO development environment, for details consult the [Get Started](https://developers.eos.io/welcome/latest/getting-started/development-environment/introduction) Guide.
* A smart contract, let’s call it `smrtcontract`.
* A user defined type, let’s call it `person`, which defines the data which is stored in the table.

Refer to the following possible implementation of your starting point.

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

## Next Steps

The following options are available when you complete the procedure:

* You can define one or more **unique indexes** using the `KV_NAMED_INDEX` macro or the `eosio::kv::table::index` template class.
* You can define one or more **non-unique indexes** using the `KV_NAMED_INDEX` macro or the `eosio::kv::table::index` template class.
* You can access the defined `kv table` and perform operations on it and its defined indexes.
