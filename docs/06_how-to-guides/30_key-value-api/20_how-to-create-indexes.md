---
content_title: How-To Create Indexes on a Key-Value Table
link_text: "How-To Create Indexes on a Key-Value Table"
---

## Summary

This how-to procedure provides instructions to create the following indexes on a `Key-Value Table` (`kv table`):

* A unique index using the macro `KV_NAMED_INDEX`
* A unique index using the `eosio::kv::table::index` template class
* A non-unique index using the macro `KV_NAMED_INDEX`
* A non-unique index using the `eosio::kv::table::index` template class

The  `KV_NAMED_INDEX` macro and the `eosio::kv::table::index` template class are provided by the KV API.

## Prerequisites

Before you begin, complete the following prerequisites:

* An EOSIO development environment, for details consult the [Get Started](https://developers.eos.io/welcome/latest/getting-started/development-environment/introduction) Guide
* A smart contract, let’s call it `smrtcontract`
* A user defined type which defines the data stored in the table, let’s call it `person`
* A `kv table` type which stores objects of type `person`, let’s call it `address_table`
* Each `person` object has the following properties:
  * `account_name`,
  * `first_name`,
  * `last_name`,
  * `personal_id`.

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
    }
  };
  public:
     using contract::contract;
};
```

## Procedures

### Define a unique index on property account_name using the macro KV_NAMED_INDEX

1. Use the `KV_NAMED_INDEX` macro with two parameters.
2. Pass the name of the index as the first parameter. The parameter must be a qualified `eosio::name`. See documentation for the [eosio name restrictions](https://developers.eos.io/welcome/latest/glossary/index#account-name).
3. Pass the name of the property for which the index is defined as the second parameter.
4. Call `init()` of the base class in the constructor of `address_table` type and pass the contract name as the first parameter and the primary index defined previously as the second parameter.

Refer to the following possible implementation of a unique index on property `account_name` using macro `KV_NAMED_INDEX`:

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

### Define a unique index on property personal_id using the eosio::kv::table::index template class

1. Use the `eosio::kv::table::index` template class with two parameters.
2. Pass the name of the index as the first parameter. The parameter must be a qualified `eosio:name`. See documentation for the [eosio name restrictions](https://developers.eos.io/welcome/latest/glossary/index#account-name).
3. Pass the reference to the property for which the index is defined, `&person::personal_id`, as the second parameter.

Refer to the following possible implementation of a unique index on property `personal_id` using `eosio::kv::table::index` template class:

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

### Define a non-unique index on property first_name using the macro KV_NAMED_INDEX

1. Use the `KV_NAMED_INDEX` with two parameters.
2. Pass the name of the index as the first parameter. The parameter must be a qualified `eosio:name`. See documentation for the [eosio name restrictions](https://developers.eos.io/welcome/latest/glossary/index#account-name).
3. Pass the name of the property for which the index is defined as the second parameter.

The property used for the second parameter must be of template type `eosio::non_unique`. The first parameter of the template type must be the type of a property name which is unique. In our case the type `eosio::name` is used because property`account_name` is unique. And the second parameter must be the type of the property indexed non-uniquely, in our case the type `std::string` is used because `first_name` is the property indexed non-uniquely. It is important to mention that multiple properties can be indexed non-uniquely, thus after the first parameter type is specified multiple parameter types can follow, corresponding to more than one properties being indexed, not just one, like in your case.

Refer to the following possible implementation of a non-unique index on property `account_name` using macro `KV_NAMED_INDEX`:

`smartcontract.hpp file`

```cpp
struct person {
  eosio::name account_name;
  eosio::non_unique<eosio::name, std::string> first_name;
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

### Define a non-unique index on property last_name using the eosio::kv::table::index template class

1. Use the `eosio::kv::table::index` template class.
2. Pass as the first parameter the name of the index. It must be a qualified `eosio:name`, see documentation for the [eosio name restrictions](https://developers.eos.io/welcome/latest/glossary/index#account-name).
3. Pass the reference to the property for which the index is defined, `&person::last_name`, as the second parameter.

The property used for the second parameter must be of template type `eosio::non_unique`. The first parameter of the template type must be the type of a property name which is unique. In our case the type `eosio::name` is used because property`account_name` is unique. And the second parameter must be the type of the property indexed non-uniquely, in our case the type `std::string` is used because `last_name` is the property indexed non-uniquely. It is important to mention that multiple properties can be indexed non-uniquely, thus after the first parameter type is specified multiple parameter types can follow, corresponding to more than one properties being indexed, not just one, like in your case.

Refer to the following possible implementation of a non-unique index on property `last_name` using `eosio::kv::table::index` template class:

`smartcontract.hpp file`

```cpp
struct person {
  eosio::name account_name;
  std::string first_name;
  eosio::non_unique<eosio::name, std::string> last_name;
  std::string personal_id;
};

class [[eosio::contract]] smrtcontract : public contract {
    struct [[eosio::table]] address_table : eosio::kv::table<person, "kvaddrbook"_n> {

     index<non_unique<name, string>> last_name_idx {
        name{"persid"_n},
        &person::last_name};

    address_table(eosio::name contract_name) {
      init(contract_name, last_name)
    }
  };
  public:
     using contract::contract;
};
```

## Next Steps

The following options are available when you complete the procedure:

* Access the defined indexes by their names.
* Search by index key for values or range of values in the defined `kv table`.
* Check if a particular key exists in an index.
