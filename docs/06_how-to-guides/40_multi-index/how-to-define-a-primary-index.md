---
content_title: How to define a primary index
---

## Overview

This guide provides instructions on how to define a primary index in a multi-index table.

## Reference

See the following code reference:

* The [`multi-index`](../../classeosio_1_1multi__index) class.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index),
* A multi-index table named `test_table`,
* A primary key is required when defining a multi-index table structure, therefore you need to know which is the property that is the primary key for your multi-index table structure. That property must store unique values. In this case it is the `test_primary` data member of type `eosio::name`.

## Procedure

Complete the following steps to define a primary index for the multi-index table named `test_table` based on its `test_primary` data member.

### 1. Preparation And Initialization

Include the `eosio.hpp` header and declare the `eosio` namespace usage

  ```cpp
  #include <eosio/eosio.hpp>
  using namespace eosio;
  ```

### 2. Defines The Table Data Structure

Define the data structure for the multi-index table

  ```cpp
    struct [[eosio::table]] test_table {
    };
  ```

Add to the data structure the fields which define the multi-index table

  ```diff
    // the data structure which defines each row of the table
    struct [[eosio::table]] test_table {
  +    // this field stores a name for each row of the multi-index table
  +    name test_primary;
  +    // additional data stored in table row, e.g. an uint64_t type data
  +    uint64_t datum;
    };
  ```

### 3. Define The Primary Index

Add the definition of the primary index for the multi-index table. The primary index type must be uint64_t and must be unique

  ```diff
    // the data structure which defines each row of the table
    struct [[eosio::table]] test_table {
      // this field stores a name for each row of the multi-index table
      name test_primary;
      // additional data stored in table row
      uint64_t datum;
  +    // mandatory definition for primary key getter
  +    uint64_t primary_key( ) const { return test_primary.value; }
    };
  ```

[[info | Secondary indexes information]]
| Other, secondary, indexes if they will be defined can have duplicates. You can have up to 16 additional indexes and the field types can be uint64_t, uint128_t, uint256_t, double or long double.

### 4. Define A Multi-Index Type Alias

For ease of use, define a type alias `test_tables` based on the `eosio::multi_index` template type, parametarized with a random name `"testtaba"` and the `test_table` data structure defined above

  ```diff
    // the data structure which defines each row of the table
    struct [[eosio::table]] test_table {
      // this field stores a name for each row of the multi-index table
      name test_primary;
      // additional data stored in table row
      uint64_t datum;
      // mandatory definition for primary key getter
      uint64_t primary_key( ) const { return test_primary.value; }
    };
    
  +  typedef eosio::multi_index<"testtaba"_n, test_table> test_tables;
  ```

### 5. Instantiate The Multi-Index Table

Declare the multi-index table as a data member of type `test_tables`, as defined above.

  ```diff
    // the data structure which defines each row of the table
    struct [[eosio::table]] test_table {
      // this field stores a name for each row of the multi-index table
      name test_primary;
      // additional data stored in table row
      uint64_t datum;
      // mandatory definition for primary key getter
      uint64_t primary_key( ) const { return test_primary.value; }
    };
    
    typedef eosio::multi_index<"testtaba"_n, test_table> test_tables;
  +  test_tables testtab;
  ```

Now you have instantiated the `testtab` as a multi-index table which has a primary index defined for its `test_primary` data member.

[[info | Full example location]]
| A full example project demonstrating the instantiation and usage of multi-index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).

## Summary

In conclusion, the above instructions show how to define a primary index in a multi-index table.

## Next Steps

The following option is available when you complete the procedure:

* You can [iterate and retrieve data](./how-to-iterate-and-retrieve-a-multi_index-table) from the multi-index table.
