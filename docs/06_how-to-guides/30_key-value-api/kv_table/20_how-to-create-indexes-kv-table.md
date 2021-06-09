---
content_title: How-To Create Indexes on a Key-Value Table
link_text: "How-To Create Indexes on a Key-Value Table"
---

## Overview

This how-to provides instructions to create the following indexes on a `Key-Value Table` (`kv table`):

* A unique index using the macro `KV_NAMED_INDEX`
* A unique index using the `eosio::kv::table::index` template class
* A non-unique index using the macro `KV_NAMED_INDEX`
* A non-unique index using the `eosio::kv::table::index` template class

The  `KV_NAMED_INDEX` macro and the `eosio::kv::table::index` template class are provided by the KV API.

[[caution | Alpha version]]
| `Key-Value Table` is designated as `alpha` and should not be used in production code.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index)
* A smart contract named `smrtcontract`
* A user defined type which defines the data stored in the table, named `person`
* A `kv table` type which stores objects of type `person`, named `address_table`
* Each `person` object has the following properties:
  * `account_name`,
  * `first_name`,
  * `last_name`,
  * `personal_id`.

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
    }
  };
  public:
     using contract::contract;
};
```

## Procedure

### Define a unique index on data member account_name using the macro KV_NAMED_INDEX

1. Use the `KV_NAMED_INDEX` macro with two parameters.
2. The first parameter is the name of the index and it must be a qualified `eosio::name`. See documentation for the [eosio name restrictions](https://developers.eos.io/welcome/latest/glossary/index#account-name).
3. The second parameter is the name of the data member the index is defined for.
4. Call the base class method `init()` in the constructor of `address_table` type and pass the contract name as the first parameter and `account_name` index, by the KV_NAMED_INDEX macro, as the second parameter.

Refer to the following reference implementation of a unique index on data member `account_name` using macro `KV_NAMED_INDEX`:

`smartcontract.hpp file`

```cpp
class [[eosio::contract]] smrtcontract : public contract {
    struct [[eosio::table]] address_table : eosio::kv::table<person, "kvaddrbook"_n> {

    KV_NAMED_INDEX("accname"_n, account_name)

    address_table(eosio::name contract_name) {
      init(contract_name, account_name);
    }
  };
  public:
     using contract::contract;
};
```

### Define a unique index on data member personal_id using the eosio::kv::table::index template class

1. Use the `eosio::kv::table::index` template class with two parameters.
2. The first parameter is the name of the index and it must be a qualified `eosio:name`. See documentation for the [eosio name restrictions](https://developers.eos.io/welcome/latest/glossary/index#account-name).
3. The second parameter is the reference to the data member for which the index is defined, `&person::personal_id`.
4. Call the base class method `init()` in the constructor of `address_table` type and pass the contract name as the first parameter and the `personal_id_idx` index as the second parameter.

Refer to the following reference implementation of a unique index on data member `personal_id` using `eosio::kv::table::index` template class:

`smartcontract.hpp file`

```cpp
class [[eosio::contract]] smrtcontract : public contract {
    struct [[eosio::table]] address_table : eosio::kv::table<person, "kvaddrbook"_n> {

  index<std::string> personal_id_idx {
    name{"persid"_n},
    &person::personal_id };

    address_table(eosio::name contract_name) {
       init(contract_name, personal_id_idx)
    }
  };
  public:
     using contract::contract;
};
```

### Define a non-unique index on data member first_name using the macro KV_NAMED_INDEX

1. Use the `KV_NAMED_INDEX` with two parameters.
2. The first parameter is the name of the index and it must be a qualified `eosio::name`. See documentation for the [eosio name restrictions](https://developers.eos.io/welcome/latest/glossary/index#account-name).
3. The second parameter is the name of the data member the index is defined for.
4. Call the base class method `init()` in the constructor of `address_table` type and pass the contract name as the first parameter and `first_name` index, by the KV_NAMED_INDEX macro, as the second parameter.

The second parameter is the name of the data member the index is defined for and the type of this data member must be `std::tuple<>`. The first parameter of the tuple must be the type of the data member indexed non-uniquely, in our case the type `std::string` is used because `first_name` is the data member indexed non-uniquely. The last parameter of the tuple type must be the type of a data member name which is unique. In our case the type `eosio::name` is used because data member `account_name` is unique. Multiple properties can be indexed non-uniquely as well. In this case the first parameter types correspond to the properties being indexed. As previously already mentioned, the last parameter correspond to the type of a data member name which is unique.

Refer to the following reference implementation of a non-unique index on data member `account_name` using macro `KV_NAMED_INDEX`:

`smartcontract.hpp file`

```cpp
struct person {
  eosio::name account_name;
  std::tuple<std::string, eosio::name> first_name;
  std::string last_name;
  std::string personal_id;
};

class [[eosio::contract]] smrtcontract : public contract {
    struct [[eosio::table]] address_table : eosio::kv::table<person, "kvaddrbook"_n> {

    KV_NAMED_INDEX("accname"_n, first_name)

    address_table(eosio::name contract_name) {
      init(contract_name, first_name);
    }
  };
  public:
     using contract::contract;
};
```

### Define a non-unique index on data member last_name using the eosio::kv::table::index template class

1. Use the `eosio::kv::table::index` template class.
2. The first parameter is the name of the index and it must be a qualified `eosio:name`, see documentation for the [eosio name restrictions](https://developers.eos.io/welcome/latest/glossary/index#account-name).
3. The second parameter is the name of the data member the index is defined for, `&person::last_name`.
4. Call the base class method `init()` in the constructor of `address_table` type and pass the contract name as the first parameter and the `last_name_idx` index as the second parameter.

The data member used for the second parameter must be of template type `std::tuple`. The first parameter must be the type of the data member indexed non-uniquely, in our case the type `std::string` is used because `first_name` is the data member indexed non-uniquely. The last parameter of the tuple type must be the type of a data member name which is unique. In our case the type `eosio::name` is used because data member `account_name` is unique. Multiple properties can be indexed non-uniquely as well. In this case the first parameter types correspond to the properties being indexed. As previously already mentioned, the last parameter correspond to the type of a data member name which is unique.

Refer to the following reference implementation of a non-unique index on data member `last_name` using `eosio::kv::table::index` template class:

`smartcontract.hpp file`

```cpp
struct person {
  eosio::name account_name;
  std::string first_name;
  std::tuple<std::string, eosio::name> last_name;
  std::string personal_id;
};

class [[eosio::contract]] smrtcontract : public contract {
    struct [[eosio::table]] address_table : eosio::kv::table<person, "kvaddrbook"_n> {

     index<std::tuple<name, string>> last_name_idx {
        name{"persid"_n},
        &person::last_name};

    address_table(eosio::name contract_name) {
      init(contract_name, last_name_idx)
    }
  };
  public:
     using contract::contract;
};
```

## Summary

In conclusion, the above instructions show how to create indexes on a `Key-Value Table` (`kv table`).

## Next Steps

* [Search](70_how-to-find-in-kv-table.md) by index key for values or range of values in the defined `kv table`.
* [Check](60_how-to-check-a-record-kv-table.md) if a particular key exists in an index.
