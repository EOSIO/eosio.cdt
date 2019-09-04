## How to define a secondady index

Prerequisites: it is assumed you already have a multi index table instance defined along with its mandatory primary index, otherwise take a look at the section [How to instantiate a multi index table](./how-to-instantiate-a-multi-index-table.md).

The steps below show how to add a secondary index to the existing multi index table.

__First__, you have to add a second field `secondary` to the data structure that defines the row of the table, in our case `test_table`, then add the `by_secondary( )` method which is the index accessor method to the new field value added. The secondary index which we will create in second step will index this new data structure field.

```cpp
  // the row structure of the multi index table, that is, each row of the table
  // will contain an instance of this type of structure
  TABLE test_table {
    // this field is used later for definition of the primary index
    name test_primary;
    name secondary;
    // additional data stored in table row
    uint64_t datum;
    // mandatory definition for primary key getter
    uint64_t primary_key( ) const { return test_primary.value; }
    uint64_t by_secondary( ) const { return secondary.value; }
  };
```

__Second__, in the `test_table` alias definition (typedef), add the definition of the secondary index by making use of the `eosio::indexed_by` template, which needs two parameters, the name of the index `"secid"_n`, and a function call operator which extracts the value from the `secondary` data member as an index key, this is achieved by employing the `eosio::const_mem_fun` template which receives two paras: the data structure `test_table` and the reference to the getter function member `by_secondary`.

```cpp
  typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_tables;
```

The full contract definition code with all the changes described above will look like this:

__multi_index_example.hpp__
```cpp
#include <eosio/eosio.hpp>
using namespace eosio;

// multi index example contract class
CONTRACT multi_index_example : public contract {
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
      TABLE test_table {
        // this field is used later for definition of the primary index
        name test_primary;
        name secondary;
        // additional data stored in table row
        uint64_t datum;
        // mandatory definition for primary key getter
        uint64_t primary_key( ) const { return test_primary.value; }
        uint64_t by_secondary( ) const { return secondary.value; }
      };

      // the multi index type definition, for ease of use we define a type alias `test_tables`, 
      // based on the multi_index template type, parametarized with a random name, the 
      // test_table data structure, and the secondary index
      typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_tables;

      // the multi index table instance declared as a data member of type test_tables
      test_tables testtab;

      ACTION set( name user );
      ACTION print( name user );

      using set_action = action_wrapper<"set"_n, &multi_index_example::set>;
      using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
};
```

__Note__
A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).