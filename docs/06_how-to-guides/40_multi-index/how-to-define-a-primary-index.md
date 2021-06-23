---
content_title: How to define a primary index
---

## Overview

This guide provides instructions on how to define a primary index for a multi-index table.

## Reference

See the following code reference:

* The [`multi-index`](../../classeosio_1_1multi__index) class.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index),

## Procedure

Complete the following steps to define a primary index for the multi-index table `testtab`.

### 1. Preparation And Initialization

Include the `eosio.hpp` header and declare the `eosio` namespace usage.

  ```cpp
  #include <eosio/eosio.hpp>
  using namespace eosio;
  ```

### 2. Define The Table Data Structure

Define the data structure for the multi-index table.

  ```cpp
    struct [[eosio::table]] test_table {
    };
  ```

Add to the data structure the properties which define it. Each property corresponds to a field of the multi-index table. A primary key is required when defining a multi-index table structure, therefore you need to know which is the multi-index table field that is the primary key for your multi-index table. The corresponding property for the primary key field must store unique values. In this case it is the `test_primary` data member of type `eosio::name`.

  ```diff
    // the data structure which defines each row of the table
    struct [[eosio::table]] test_table {
  +    // this property stores a name for each row of the multi-index table
  +    name test_primary;
  +    // additional data stored in table row, e.g. an uint64_t type data
  +    uint64_t datum;
    };
  ```

### 3. Define The Primary Index

Add the definition of the primary index for the multi-index table. The primary index type must be uint64_t, it must be unique and it must be named `primary_key()`, otherwise the compiler (eosio-cpp) will generate an error saying it can not find the field to use as the primary key:

  ```diff
    // the data structure which defines each row of the table
    struct [[eosio::table]] test_table {
      // this property stores a name for each row of the multi-index table
      name test_primary;
      // additional data stored in table row
      uint64_t datum;
  +    // mandatory definition for primary key getter
  +    uint64_t primary_key( ) const { return test_primary.value; }
    };
  ```

[[info | Secondary indexes information]]
| Other, secondary, indexes if they will be defined can have duplicates. You can have up to 16 additional indexes and the corresponding property types can be uint64_t, uint128_t, uint256_t, double or long double.

### 4. Define A Multi-Index Type Alias

For ease of use, define a type alias `test_table_t` based on the `eosio::multi_index` template type, parametarized with a random name `"testtaba"` and the `test_table` data structure defined above.

  ```diff
    // the data structure which defines each row of the table
    struct [[eosio::table]] test_table {
      // this property stores a name for each row of the multi-index table
      name test_primary;
      // additional data stored in table row
      uint64_t datum;
      // mandatory definition for primary key getter
      uint64_t primary_key( ) const { return test_primary.value; }
    };
    
  +  typedef eosio::multi_index<"testtaba"_n, test_table> test_table_t;
  ```

### 5. Instantiate The Multi-Index Table

Declare the `testtab` multi-index table as a data member of type `test_table_t`.

  ```diff
    // the data structure which defines each row of the table
    struct [[eosio::table]] test_table {
      // this property stores a name for each row of the multi-index table
      name test_primary;
      // additional data stored in table row
      uint64_t datum;
      // mandatory definition for primary key getter
      uint64_t primary_key( ) const { return test_primary.value; }
    };
    
    typedef eosio::multi_index<"testtaba"_n, test_table> test_table_t;
  +  test_table_t testtab;
  ```

Now you have instantiated the `testtab` as a multi-index table which has a primary index defined for its `test_primary` data member.

[[info | Full example location]]
| A full example project demonstrating the instantiation and usage of multi-index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).

## Summary

In conclusion, the above instructions show how to define a primary index for a multi-index table.

## Next Steps

The following option is available when you complete the procedure:

* You can [insert data in the multi-index table](./how-to-insert-data-into-a-multi-index-table).
* You can [iterate and retrieve data](./how-to-iterate-and-retrieve-a-multi_index-table) from the multi-index table.
