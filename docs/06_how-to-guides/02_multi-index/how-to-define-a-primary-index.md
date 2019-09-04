## How to define a primary index

How to declare and instantiate an instance of a multi index table can be found [here](./how-to-instantiate-a-multi-index-table.md), and as par of the definition of the multi index table, the definition of a primary index key getter is mandatory. You can find in the section linked previously the full example of a multi index table including the primary index definition.

For a quick reference of how a multi index table and the primary key index getter are defined see below:

```cpp
  // ...

  // the data structure which defines each row of the table
  TABLE test_table {
    // this field is used later for definition of the primary index
    name test_primary;
    // additional data stored in table row
    uint64_t datum;
    // mandatory definition for primary key getter
    uint64_t primary_key( ) const { return test_primary.value; }
  };
  
  // ...

  // the multi index type definition, for ease of use we define a type alias `test_tables`, 
  // based on the multi_index template type, parametarized with a random name and 
  // the test_table data structure
  typedef eosio::multi_index<"testtaba"_n, test_table> test_tables;

  // the multi index table instance declared as a data member of type test_tables
  test_tables testtab;
```

__Note__
A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).