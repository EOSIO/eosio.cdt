---
content_title: How to delete data from a multi index table
---

## Preconditions
- It is assumed you already have a multi index table instance defined along with its mandatory primary index, otherwise take a look at the section [How to instantiate a multi index table](./how-to-instantiate-a-multi-index-table.md).

To delete data from a multi index table follow the steps below:

1. Make use of the multi index table iterator to find out if the data exists
```cpp
[[eosio::action]] void multi_index_example::del( name user ) {
  // check if the user already exists
  auto itr = testtab.find(user.value);
}
```

2. If the data exists use the `delete` method to delete the row from table
```diff
[[eosio::action]] void multi_index_example::del( name user ) {
  // check if the user already exists
  auto itr = testtab.find(user.value);
+  if ( itr == testtab.end() ) {
+    printf("user does not exist in table, nothing to delete" );
+    return;
+  }

+  testtab.erase( itr );
}
```

[[info | Full example location]]
| A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).