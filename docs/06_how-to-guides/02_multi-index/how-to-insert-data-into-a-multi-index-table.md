---
content_title: How to insert data into a multi index table
---

## Preconditions
- It is assumed you already have a multi index table instance defined along with its mandatory primary index, otherwise take a look at the section [How to instantiate a multi index table](./how-to-instantiate-a-multi-index-table.md).

To insert data into a multi index table follow the following steps

1. Make use of the multi index table iterator to find out if the data doesn't already exist
```cpp
[[eosio::action]] void multi_index_example::set( name user ) {
  // check if the user already exists
  auto itr = testtab.find(user.value);

}
```

2. Use the `emplace` method to make the insertion if the user is not already in table
```diff
[[eosio::action]] void multi_index_example::set( name user ) {
  // check if the user already exists
  auto itr = testtab.find(user.value);

+  if ( itr == testtab.end() ) {
+    testtab.emplace( _self, [&]( auto& u ) {
+      u.test_primary = user;
+      u.secondary = "second"_n;
+      u.datum = 0;
+    });
+  }
}
```

[[info | Full example location]]
| A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).
