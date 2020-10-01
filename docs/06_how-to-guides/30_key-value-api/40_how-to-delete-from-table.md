---
content_title: How-To Delete from Key-Value Table
link_text: "How-To Delete from Key-Value Table"
---

## Summary

This how-to procedure provides instructions to delete an object from a `Key-Value Table` (`kv table’).

Use the method `erase` defined by the `eosio::kv::table` type to accomplish this task.

## Prerequisites

Before you begin, complete the following prerequisites:

* An EOSIO development environment, for details consult the [Get Started](https://developers.eos.io/welcome/latest/getting-started/development-environment/introduction) Guide
* A smart contract, let’s call it `smrtcontract`
* A user defined type, let’s call it `person`, which defines the data which is stored in the table
* A `kv table` type which stores objects of type `person`, let’s call it `address_table`. The primary index of the `kv table` is defined based on the `person::account_name` property.

Refer to the following possible implementation of your starting point.

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

1. Create a new action in your contact, let’s call it `delete`, which takes as input parameters an account name, a first name, a last name and a personal id.
2. In the `delete` action access the instance of `address_table` by declaring a local variable of `address_table` type.
3. Call the `erase` method of the `address_table` and pass to it the primary key for the object which is deleted. If you try to erase an object which is not present in the `kv table` no error will be raised.

Refer to the following possible implementation to delete a `person` object from `address_table`:

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

## Next Steps

The following options are available when you complete the procedure:

* Verify if the newly inserted `person` was actually deleted from the table. To accomplish this task, use the `exists()` function of any index defined for the table.
