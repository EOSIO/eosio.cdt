---
content_title: How to iterate and retrieve a multi index table
---

## Preconditions
- It is assumed you already have a multi index table instance defined along with its mandatory primary index, otherwise take a look at the section [How to instantiate a multi index table](./how-to-instantiate-a-multi-index-table.md).

For exemplification define the multi index contract definition like below:

__multi_index_example.hpp__
```cpp
#include <eosio/eosio.hpp>
using namespace eosio;

// multi index example contract class
class [[eosio::contract]] multi_index_example : public contract {
   public:
      using contract::contract;

      // contract class constructor
      multi_index_example( name receiver, name code, datastream<const char*> ds ) :
         // contract base class contructor
         contract(receiver, code, ds),
         // instantiate multi index instance as data member (find it defined below)
         testtab(receiver, receiver.value)
         { }

      // the row structure of the multi index table, that is, each row of the table
      // will contain an instance of this type of structure
      struct [[eosio::table]] test_table {
        // this field stores a name for each row of the multi index table
        name test_primary;
        // additional data stored in table row
        uint64_t datum;
        // mandatory definition for primary key getter
        uint64_t primary_key( ) const { return test_primary.value; }
      };

      // the multi index type definition, for ease of use define a type alias `test_tables`, 
      // based on the multi_index template type, parametarized with a random name and 
      // the test_table data structure
      typedef eosio::multi_index<"testtaba"_n, test_table> test_tables;

      // the multi index table instance declared as a data member of type test_tables
      test_tables testtab;

      [[eosio::action]] void set( name user );

      using set_action = action_wrapper<"set"_n, &multi_index_example::set>;
};
```

The steps below show how to iterate and retrieve a multi index table.

1. Add to the above multi index example contract an action `print` which gets as parameter an acount name

```cpp
[[eosio::action]] void print( name user );
```
2. For ease of use add the action wrapper defition as well
```diff
[[eosio::action]] void print( name user );

+using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
```
3. Implement the action code, by searching for the `user` name in the multi index table using the primary index. If found, print out the value stored in that row for field `datum`. Otherwise asserts with a custom message. In the contract definition add the following implementation for `print` action:
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
4. Finally the whole definition and implementation files for the contract should look like this:

__multi_index_example.hpp__
```cpp
#include <eosio/eosio.hpp>
using namespace eosio;

// multi index example contract class
class [[eosio::contract]] multi_index_example : public contract {
   public:
      using contract::contract;

      // contract class constructor
      multi_index_example( name receiver, name code, datastream<const char*> ds ) :
         // contract base class contructor
         contract(receiver, code, ds),
         // instantiate multi index instance as data member (find it defined below)
         testtab(receiver, receiver.value)
         { }

      // the row structure of the multi index table, that is, each row of the table
      // will contain an instance of this type of structure
      struct [[eosio::table]] test_table {
        // this field stores a name for each row of the multi index table
        name test_primary;
        // additional data stored in table row
        uint64_t datum;
        // mandatory definition for primary key getter
        uint64_t primary_key( ) const { return test_primary.value; }
      };

      // the multi index type definition, for ease of use define a type alias `test_tables`, 
      // based on the multi_index template type, parametarized with a random name and 
      // the test_table data structure
      typedef eosio::multi_index<"testtaba"_n, test_table> test_tables;

      // the multi index table instance declared as a data member of type test_tables
      test_tables testtab;

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
| A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).