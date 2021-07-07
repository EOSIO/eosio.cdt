---
content_title: How to modify data in a multi-index table
---

## Overview

This guide provides instructions to modify data in a multi-index table.

## Reference

See the following code reference:

* The [`multi-index`](../../classeosio_1_1multi__index) class.
* The [`multi-index::modify(...)`](../../group__multiindex/#function-modify) method.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index),
* A multi-index `testab` table instance which stores `user` objects indexed by the primary key which is of type `eosio::name`. Consult the section [How to instantiate a multi-index table](./how-to-instantiate-a-multi-index-table) to learn how to set it up.

## Procedure

Complete the following steps to modify data in the `testtab` multi-index table.

### 1. Define The mod(...) Action

Add a `mod` action to the `testtab` multi-index table. The `mod` action takes as input parameters a `user` of type `eosio::name` and a `value` of type `uint32_t`. The `mod` action updates the `user` object `datum` data member with the `uint32_t` value.

```cpp
[[eosio::action]] void mod( name user, uint32_t value );
```

Optionally, for ease of use add the action wrapper definition as well.

```diff
[[eosio::action]] void mod( name user, uint32_t value );

+using mod_action = action_wrapper<"mod"_n, &multi_index_example::mod>;
```

### 2. Find The User You Want To Modify

Use the multi-index [`find(...)`](../../group__multiindex#function-find) method to locate the user object you want to modify. The targeted user is searched based on its account name.

```cpp
[[eosio::action]] void multi_index_example::mod( name user, uint32_t value ) {
  auto itr = testtab.find(user.value);
}
```

### 3. Yield Error If User Not Found

If the `user` object you want to update is not found then raise an error message by using the [`eosio::check`](../../namespaceeosio/#function-check-17) method.

```diff
[[eosio::action]] void multi_index_example::mod( name user, uint32_t value ) {
  auto itr = testtab.find(user.value);
+  check( itr != testtab.end(), "user does not exist in table" );
}
```

### 4. Update The User If Found

If the `user` object you want to update is found, the [`eosio::check`](../../namespaceeosio/#function-check-17) method will do nothing and the iterator `itr` will be pointing at the object which you want to update. Use the [`multi-index::modify(...)`](../../group__multiindex/#function-modify) method to update the user object `datum` data member with the `value` parameter.

```diff
[[eosio::action]] void multi_index_example::mod( name user, uint32_t value ) {
  // check if the user already exists
  auto itr = testtab.find(user.value);
  check( itr != testtab.end(), "user does not exist in table" );

+  testtab.modify( itr, _self, [&]( auto& row ) {
+    row.datum = value;
+  });
}
```

Now you have implemented a new action `mod`. Call `mod` to update the `datum` data member for the user object identified by the `user` name parameter.

[[info | Full example location]]
| A full example project demonstrating the instantiation and usage of multi-index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).

## Summary

In conclusion, the above instructions show how to modify data in a multi-index table.

## Next Steps

* You can [insert data](./how-to-insert-data-into-a-multi-index-table) into the multi-index table.
* You can [delete data](./how-to-delete-data-from-a-multi-index-table) from the multi-index table.
