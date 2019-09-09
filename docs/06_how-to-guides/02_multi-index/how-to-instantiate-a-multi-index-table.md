## How to instantiate a multi index table

Prerequisites: To instantiate a multi index table you need to make use of the `eosio::multi_index` template, create a struct which can be stored in the multi index table, and define getters on the fields you want to index. Remember that one of these getters must be named `primary_key()`, if you don't have this the compiler (eosio-cpp) will generate an error it can't find the field to use as the primary key.

One example to accomplish this is exemplified below.

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
         testtab(receiver, receiver.value) { }

      // the row structure of the multi index table, that is, each row of the table
      // will contain an instance of this type of structure
      struct [[eosio::table]] test_table {
        // this field is used later for definition of the primary index
        name test_primary;
        // additional data stored in table row
        uint64_t datum;
        // mandatory definition for primary key getter
        uint64_t primary_key( ) const { return test_primary.value; }
      };

      // the multi index type definition, for ease of use we define a type alias `test_tables`, 
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

__Note__
A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).