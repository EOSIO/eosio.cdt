---
content_title: How-To Query Range in Key-Value Table
link_text: "How-To Query Range in Key-Value Table"
---

## Overview

This how-to provides instructions to retrieve a list of values, from a `Key-Value Table` (`kv table`) index, which share a particular commonality.

[[caution | Alpha version]]
| `Key-Value Table` is designated as `alpha` and should not be used in production code.

Use the method `range` defined by the `eosio::kv::table::index` class to accomplish this task.

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
* A non-unique index defined on the `last_name` property, named `last_name_idx`.

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

     index<std::tuple<name, string>> last_name_idx {
        name{"lastname"_n},
        &person::last_name};

     address_table(name contract_name) {
        init(contract_name, account_name_uidx)
     }
  };
  public:
     using contract::contract;
};
```

## Procedure

Complete the following steps to implement an action to retrieve a list of persons with the same name from `address_table` and return it back to the caller:

1. Create a new action `getbylastname`, which takes as an input parameter a string which represents the last name to filter by the persons in the `address_table`.
2. Use the `range()` function of the `last_name_idx` index and pass as parameters the beginning and the end of the range to retrieve:
    1. The first parameter is a person object with its account name equal to `the minimum possible value for an account name` and its last name the same value to filter by the persons in the `address_table`.
    2. The second parameter is a person object with its account name equal to `the maximum possible value for an account name` and its last name the same value to filter by the persons in the `address_table`.
3. Return back to the caller the list of persons the `range()` function returns.

Refer to the following reference implementation to implement an action to retrieve a list of persons with the same name from `address_table` and return it back to the caller:

`smartcontract.hpp`

```cpp
class [[eosio::contract]] smrtcontract : public contract {
    struct [[eosio::table]] address_table : eosio::kv::table<person, "kvaddrbook"_n> {

     index<name> account_name_uidx {
        name{"accname"_n},
        &person::account_name };

     index<std::tuple<name, string>> last_name_idx {
        name{"lastname"_n},
        &person::last_name};

     address_table(name contract_name) {
        init(contract_name, account_name_uidx)
     }
  };
  public:
     using contract::contract;

     // retrieves list of persons with the same last name
     [[eosio::action]]
     std::vector<person> getbylastname(string last_name);
);
     using get_by_last_name_action = action_wrapper<"getbylastname"_n, &smrtcontract::getbylastname>;
};
```

`smartcontract.cpp file`

```cpp
// retrieves list of persons with the same last name
[[eosio::action]]
std::vector<person> smrtcontract::getbylastname(string last_name) {
  address_table addresses{"kvaddrbook"_n};

  eosio::name min_account_name{0};
  eosio::name max_account_name{UINT_MAX};
  auto list_of_persons = addresses.last_name_idx.range(
     {min_account_name, last_name},
     {max_account_name, last_name});
  // return found list of person from action
  return list_of_persons;
}
```

## Summary

In conclusion, the above instructions show how to retrieve a list of values, from a `Key-Value Table` (`kv table`) index, which share a particular commonality.

## Next Steps

The following options are available when you complete the procedure:

* Access the list of objects returned by the `getbylastname` action.
