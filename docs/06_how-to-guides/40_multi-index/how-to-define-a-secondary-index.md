---
content_title: How to define a secondary index
---

## Overview

This guide provides instructions to define a secondary index for a multi-index table.

## Reference

See the following code reference:

* The [`multi-index`](../../classeosio_1_1multi__index) class.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index),
* A multi-index table `testtab` along with its `test_table` data structure, its mandatory primary index, and the type alias definition `test_table_t`. Please see [How To Define A Primary Index](./how-to-define-a-primary-index) to set up these prerequisites.

## Procedure

Complete the following steps to define a secondary index for the multi-index table `testtab`.

### 1. Extend The Multi-Index Data Structure

Add a second data member `secondary`, of type `eosio::name`, to the `test_table` data structure that defines the `testtab` data.

  ```diff
    struct [[eosio::table]] test_table {
      // this data member stores a name for each row of the multi-index table
      name test_primary;
  +    name secondary;
      // additional data stored in table row
      uint64_t datum;
      // mandatory definition for primary key getter
      uint64_t primary_key( ) const { return test_primary.value; }
    };
  ```

### 2. Add The Secondary Index Accessor Method

Add `by_secondary()` method, which is the index accessor method to the new data member added. The secondary index, that will be added in the next step, will index this new data structure data member.

  ```diff
    struct [[eosio::table]] test_table {
      // this data member stores a name for each row of the multi-index table
      name test_primary;
      name secondary;
      // additional data stored in table row
      uint64_t datum;
      // mandatory definition for primary key getter
      uint64_t primary_key( ) const { return test_primary.value; }
  +    uint64_t by_secondary( ) const { return secondary.value; }
    };
  ```

### 3. Define The Secondary Index

In the `test_table_t` type definition, add the definition of the secondary index with the use of the `eosio::indexed_by` template. `eosio::index_by` needs two parameters: the name of the index, `"secid"_n`, and a function call operator which extracts the value from the secondary data member as an index key. The function call operator is defined with the use of `eosio::const_mem_fun` template which receives two parameters: the data structure `test_table` and the reference to the getter function member `by_secondary`.

  ```diff
  -  typedef eosio::multi_index<"testtaba"_n, test_table> test_table_t;
  +  typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_table_t;
  ```

For reference see below the full contract definition code with all the changes described above.

__multi_index_example.hpp__

  ```cpp
  #include <eosio/eosio.hpp>
  using namespace eosio;

  // multi-index example contract class
  class [[eosio::contract]] multi_index_example : public contract {
    public:
        using contract::contract;

        // contract class constructor
        multi_index_example( name receiver, name code, datastream<const char*> ds ) :
          // contract base class contructor
          contract(receiver, code, ds),
          // instantiate multi-index instance as data member (find it defined below)
          testtab(receiver, receiver.value) 
          { }

        struct [[eosio::table]] test_table {
          // this data member stores a name for each row of the multi-index table
          name test_primary;
          name secondary;
          // additional data stored in table row
          uint64_t datum;
          // mandatory definition for primary key getter
          uint64_t primary_key( ) const { return test_primary.value; }
          uint64_t by_secondary( ) const { return secondary.value; }
        };

        // the multi-index type definition, for ease of use a type alias `test_table_t` is defined, 
        // based on the multi_index template type, parametarized with a random name, the 
        // test_table data structure, and the secondary index
        typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_table_t;

        // the multi-index table instance declared as a data member of type test_table_t
        test_table_t testtab;

        [[eosio::action]] void set( name user );
        [[eosio::action]] void print( name user );

        using set_action = action_wrapper<"set"_n, &multi_index_example::set>;
        using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
  };
  ```

Now you have instantiated the `testtab` as a multi-index table which has a primary index defined for its `test_primary` data member and a secondary index for its `secondary` data member.

[[info | Full example location]]
| A full example project demonstrating the instantiation and usage of multi-index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).

[[warning | Do not add a secondary index to an existing table]]
| Adding a secondary index to an existing multi-index table it will have unpredictable outcome. Consult the [Data design and migration](../best-practices/data-design-and-migration) documentation for more details.


## Summary

In conclusion, the above instructions show how to define a secondary index for a multi-index table.

## Next Steps

* You can [iterate and retrieve data using the secondary index](./how-to-iterate-and-retrieve-a-multi_index-table-based-on-secondary-index) from the multi-index table.
