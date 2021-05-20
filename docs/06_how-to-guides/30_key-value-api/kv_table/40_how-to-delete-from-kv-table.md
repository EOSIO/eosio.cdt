---
content_title: How-To Delete from Key-Value Table
link_text: "How-To Delete from Key-Value Table"
---

## Overview

This how-to provides instructions to delete an object from a `Key-Value Table` (`kv table`).

[[caution | Alpha version]]
| `Key-Value Table` is designated as `alpha` and should not be used in production code.

Use the method `erase` defined by the `eosio::kv::table` type to accomplish this task.

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

Complete the following steps to delete a `person` object from `address_table`:

1. Create a new action `delete`, in a smart contract class, which takes as input parameters an account name, a first name, a last name and a personal id.
2. In the `delete` action access the instance of `address_table` by declaring a local variable of `address_table` type.
3. Call the `erase` method of the `address_table` and pass to it the primary key for the object which is deleted. If you try to erase an object which is not present in the `kv table` no error will be raised.

Refer to the following reference implementation to delete a `person` object from `address_table`:

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

     // deletes a person based on primary key account_name
     [[eosio::action]]
     void del(eosio::name account_name);

     using del_action = eosio::action_wrapper<"del"_n, &smrtcontract::del>;
};
```

`smartcontract.cpp file`

```cpp
[[eosio::action]]
void smrtcontract::delete(eosio::name account_name) {
  address_table addresses{"kvaddrbook"_n};

  // delete from kv table
  addresses.erase(account_name, get_self());
}
```

## Summary

In conclusion, the above instructions show how to delete an object from a `Key-Value Table` (`kv table`).

## Next Steps

The following options are available when you complete the procedure:

* [Check](60_how-to-check-a-record-kv-table.md) if the newly inserted `person` was actually deleted from the table. To accomplish this task, use the `exists()` function of any index defined for the table.
