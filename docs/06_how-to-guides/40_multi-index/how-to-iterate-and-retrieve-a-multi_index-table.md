---
content_title: How to iterate and retrieve from multi-index table
---

## Overview

This guide provides instructions to iterate and retrieve data from a multi-index table.

## Reference

See the following code reference:

* The [`multi-index`](../../classeosio_1_1multi__index) class.
* The [`multi-index::find(...)`](../../group__multiindex#function-find) method.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index),
* A multi-index `testab` table instance which stores `user` objects indexed by the primary key which is of type `eosio::name`. Consult the section [How to instantiate a multi-index table](./how-to-instantiate-a-multi-index-table) to learn how to set it up.

## Procedure

Complete the following steps to iterate, retrieve and print data from the `testtab` multi-index table.

### 1. Define The print(...) Action

Add a `print` action to the `testtab` multi-index table. The `print` action takes an account name as a parameter.

```cpp
[[eosio::action]] void print( name user );
```

Optionally, for ease of use add the action wrapper definition as well.

```diff
[[eosio::action]] void print( name user );

+using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
```

### 2. Implement The `print(...)` Action

Search for the `user` name in the multi-index table using the primary index. If found, print out the value of field `datum`. Otherwise raise an error with a custom message. In the contract definition add the following implementation for `print` action:

```cpp
  [[eosio::action]] void multi_index_example::print( name user ) {
    // searches for the row that corresponds to the user parameter
    auto itr = testtab.find(user.value);
    
    // asserts if the row was found for user parameter, if fails use the given message
    check( itr != testtab.end(), "user does not exist in table" );

    // prints the test_primary and datum fields stored for user parameter
    eosio::print_f("Test Table : {%, %}\n", itr->test_primary, itr->datum);
}
```

### 3. Put It All Together

The full definition and implementation files for the contract should look like this:

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

      // the row structure of the multi-index table, that is, each row of the table
      // will contain an instance of this type of structure
      struct [[eosio::table]] test_table {
        // this data member stores a name for each row of the multi-index table
        name test_primary;
        // additional data stored in table row
        uint64_t datum;
        // mandatory definition for primary key getter
        uint64_t primary_key( ) const { return test_primary.value; }
      };

      // the multi-index type definition, for ease of use define a type alias `test_table_t`, 
      // based on the multi_index template type, parametarized with a random name and 
      // the test_table data structure
      typedef eosio::multi_index<"testtaba"_n, test_table> test_table_t;

      // the multi-index table instance declared as a data member of type test_table_t
      test_table_t testtab;

      [[eosio::action]] void set( name user );
      [[eosio::action]] void print( name user );

      using set_action = action_wrapper<"set"_n, &multi_index_example::set>;
      using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
};
```

__multi_index_example.cpp__

```cpp
#include <multi_index_example.hpp>

[[eosio::action]] void multi_index_example::set( name user ) {
  // check if the user already exists
  auto itr = testtab.find(user.value);

  if ( itr == testtab.end() ) {
    // user is not found in table, use emplace to insert a new row data structure in table
    testtab.emplace( _self, [&]( auto& u ) {
      u.test_primary = user;
      u.secondary = "second"_n;
      u.datum = 0;
    });
  }
}

[[eosio::action]] void multi_index_example::print( name user ) {
  // searches for the row that corresponds to the user parameter
  auto itr = testtab.find(user.value);
  
  // asserts if the row was found for user parameter, if fails use the given message
  check( itr != testtab.end(), "user does not exist in table" );

  // prints the test_primary and datum fields stored for user parameter
  eosio::print_f("Test Table : {%, %}\n", itr->test_primary, itr->datum);
}
```

[[info | Full example location]]
| A full example project demonstrating the instantiation and usage of multi-index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).

## Summary

In conclusion, the above instructions show how to iterate and retrieve a multi-index table.

## Next Steps

* You can [insert data](./how-to-insert-data-into-a-multi-index-table) into the multi-index table.
* You can [delete data](./how-to-delete-data-from-a-multi-index-table) from the multi-index table.
