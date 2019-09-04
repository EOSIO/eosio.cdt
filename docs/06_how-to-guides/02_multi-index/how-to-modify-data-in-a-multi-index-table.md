## How to modify data in a multi index table

Prerequisites: it is assumed you already have a multi index table instance defined along with its mandatory primary index, otherwise take a look at the section [How to instantiate a multi index table](./how-to-instantiate-a-multi-index-table.md).

To modify data from a multi index table you make use of the multi index table iterator to find out if the data exists, and then use the `modidfy` method to make the update, see below:

```cpp
ACTION multi_index_example::mod( name user, uint32_t n ) {
  // check if the user already exists
  auto itr = testtab.find(user.value);
  check( itr != testtab.end(), "user does not exist in table" );

  // if we got so far it means user exist so we can update its datum field
  testtab.modify( itr, _self, [&]( auto& row ) {
    row.secondary = user;
    row.datum = n;
  });
}
```

__Note__
A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).