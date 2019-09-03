## How to define a primary index

How to declare and instantiate an instance of a multi index table can be found [here](./how-to-instantiate-a-multi_index-table.md), and as par of the definition of the multi index table, the definition of a primary index is mandatory. You can find in the how to section linked previously the full example of a multi index table including the primary index definition.

For a quick reference of how a primary index is defined see below:

```cpp
    // ...

    TABLE test_table {
      // this field is used later for definition of the primary index
      name test_primary;
      // additional data stored in table row
      uint64_t datum;
      // mandatory definition for primary index
      uint64_t primary_key( ) const { return test_primary.value; }
    };
    
    // ...
```

__Note__
A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).