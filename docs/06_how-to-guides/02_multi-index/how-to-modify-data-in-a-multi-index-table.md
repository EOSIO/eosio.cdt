---
content_title: How to modify data in a multi index table
---

## Preconditions
- It is assumed you already have a multi index table instance defined along with its mandatory primary index, otherwise take a look at the section [How to instantiate a multi index table](./how-to-instantiate-a-multi-index-table.md).

To modify data in the multi index table defined in the above tutorial, you will implement an action `mod` which it will receive as parameter the `user` which is the key of the row you want to modify and the `value` param which is the value to update with the row.

1. Make use of the multi index table iterator to find out if the data exists
```cpp
[[eosio::action]] void multi_index_example::mod( name user, uint32_t value ) {
  auto itr = testtab.find(user.value);
}
```

2. If the row you want to update is not found, then assert by using the `check` method and yield an error message
```diff
[[eosio::action]] void multi_index_example::mod( name user, uint32_t value ) {
  auto itr = testtab.find(user.value);
+  check( itr != testtab.end(), "user does not exist in table" );
}
```

3. If the row you want to update is found, the `check` method will do nothing and the iterator `itr` will be pointing at the row which you want to update, so then use the multi index `modify` method to make the update like below

```diff
[[eosio::action]] void multi_index_example::mod( name user, uint32_t value ) {
  // check if the user already exists
  auto itr = testtab.find(user.value);
  check( itr != testtab.end(), "user does not exist in table" );

+  testtab.modify( itr, _self, [&]( auto& row ) {
+    row.secondary = user;
+    row.datum = value;
+  });
}
```

[[info | Full example location]
| A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).
