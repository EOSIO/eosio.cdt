## How to instantiate a multi index table

__multi_index_example.hpp__
```cpp
#include <eosio/eosio.hpp>
using namespace eosio;

CONTRACT multi_index_example : public contract {
   public:
      using contract::contract;

      // contract class constructor
      multi_index_example( name receiver, name code, datastream<const char*> ds ) :
         // contract base class contructor
         contract(receiver, code, ds),
         // instantiate multi index instance as data member (find it defined below)
         testtab(receiver, receiver.value) { }

      ACTION set( name user );
      ACTION print( name user );

      // the row structure of the multi index table, that is, each row of the table
      // will contain an instance of this type of structure
      TABLE test_table {
         name test_primary;
         uint64_t datum;
         uint64_t primary_key( ) const { return test_primary.value; }
      };

      // for ease of use we define a type alias `test_tables`
      typedef eosio::multi_index<"testtabname"_n, test_table> test_tables;

      // here we declare a data member of type test_tables, it gets instantiated 
      // in the constructor of our contract class
      test_tables testtab;

      using set_action = action_wrapper<"set"_n, &multi_index_example::set>;
      using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
};
```

__Note__
A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).