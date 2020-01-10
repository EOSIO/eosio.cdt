---
content_title: How to iterate and retrieve a multi index table based on secondary index
---

## Preconditions
- It is assumed you already have a multi index table defined with a primary index and a secondary index, if not you can find an example [here](./how-to-define-a-secondary-index.md).

You'll start with this example below which shows the definition of a `multi_index_example` contract class which has defined a multi index table with two indexes, a mandatory primary one and a secondary one:

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
        name secondary;
        // additional data stored in table row
        uint64_t datum;
        // mandatory definition for primary key getter
        uint64_t primary_key( ) const { return test_primary.value; }
        uint64_t by_secondary( ) const { return secondary.value; }
      };

      // the multi index type definition, for ease of use define a type alias `test_tables`, 
      // based on the multi_index template type, parametarized with a random name, the 
      // test_table data structure, and the secondary index
      typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_tables;

      // the multi index table instance declared as a data member of type test_tables
      test_tables testtab;

      [[eosio::action]] void set( name user );
      [[eosio::action]] void print( name user );

      using set_action = action_wrapper<"set"_n, &multi_index_example::set>;
      using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
};
```

To iterate and retreive the multi index table `testtab` defined in `multi_index_example` contract based on secondary index `by_secondary`, define a third action `bysec` which will do exactly that.

1. In the contract definition, add the new action definition, using the `[[eosio::action]] void` and the `eosio::action_wrapper` template like this:

```cpp
  [[eosio::action]] void bysec( name secid );

  using bysec_action = action_wrapper<"bysec"_n, &multi_index_example::bysec>;
```

2. In the contract implementation add the new action implementation like this:

```cpp
// iterates the multi index table rows using the secondary index and prints the row's values
[[eosio::action]] void multi_index_example::bysec( name secid ) {
  // access the secondary index
  auto idx = testtab.get_index<"secid"_n>();
  // iterate through secondary index
  for ( auto itr = idx.begin(); itr != idx.end(); itr++ ) {
    // print each row's values
    eosio::print_f("Test Table : {%, %, %}\n", itr->test_primary, itr->secondary, itr->datum);
  }
}
```

3. The full code for both the contract definition and contract implementation follow:

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
        name secondary;
        // additional data stored in table row
        uint64_t datum;
        // mandatory definition for primary key getter
        uint64_t primary_key( ) const { return test_primary.value; }
        uint64_t by_secondary( ) const { return secondary.value; }
      };

      // the multi index type definition, for ease of use define a type alias `test_tables`, 
      // based on the multi_index template type, parametarized with a random name, the 
      // test_table data structure, and the secondary index
      typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_tables;

      // the multi index table instance declared as a data member of type test_tables
      test_tables testtab;

      [[eosio::action]] void set( name user );
      [[eosio::action]] void print( name user );
      [[eosio::action]] void bysec( name secid );

      using set_action = action_wrapper<"set"_n, &multi_index_example::set>;
      using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
      using bysec_action = action_wrapper<"bysec"_n, &multi_index_example::bysec>;
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

// iterates the multi index table rows using the secondary index and prints the row's values
[[eosio::action]] void multi_index_example::bysec( name secid ) {
  // access the secondary index
  auto idx = testtab.get_index<"secid"_n>();

  // iterate through secondary index
  for ( auto itr = idx.begin(); itr != idx.end(); itr++ ) {
    // print each row's values
    eosio::print_f("Test Table : {%, %, %}\n", itr->test_primary, itr->secondary, itr->datum);
  }
}
```

[[info | Full example location]]
| A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).