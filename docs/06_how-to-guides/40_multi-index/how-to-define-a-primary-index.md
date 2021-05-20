---
content_title: How to define a primary index
---

A primary key is required when defining a multi index table structure. See the following example:

1. Include the `eosio.hpp` header and declare the `eosio` namespace usage
```
#include <eosio/eosio.hpp>
using namespace eosio;
```
2. Define the data structure for the multi index table
```cpp
  struct [[eosio::table]] test_table {
  };
```
3. Add to the data structure the fields which define the multi index table
```diff
  // the data structure which defines each row of the table
  struct [[eosio::table]] test_table {
+    // this field stores a name for each row of the multi index table
+    name test_primary;
+    // additional data stored in table row, e.g. an uint64_t type data
+    uint64_t datum;
  };
```
4. Add the definition of the primary index for the multi index table. The primary index type must be uint64_t and must be unique
```diff
  // the data structure which defines each row of the table
  struct [[eosio::table]] test_table {
    // this field stores a name for each row of the multi index table
    name test_primary;
    // additional data stored in table row
    uint64_t datum;
+    // mandatory definition for primary key getter
+    uint64_t primary_key( ) const { return test_primary.value; }
  };
```

[[info | Secondary indexes information]]
| Other, secondary, indexes if they will be defined can have duplicates. You can have up to 16 additional indexes and the field types can be uint64_t, uint128_t, uint256_t, double or long double.

5. For ease of use, define a type alias `test_tables` based on the `eosio::multi_index` template type, parametarized with a random name `"testtaba"` and the `test_table` data structure defined above
```diff
  // the data structure which defines each row of the table
  struct [[eosio::table]] test_table {
    // this field stores a name for each row of the multi index table
    name test_primary;
    // additional data stored in table row
    uint64_t datum;
    // mandatory definition for primary key getter
    uint64_t primary_key( ) const { return test_primary.value; }
  };
  
+  typedef eosio::multi_index<"testtaba"_n, test_table> test_tables;
```

Declare the multi index table as a data member of type `test_tables`, as defined above.
```diff
  // the data structure which defines each row of the table
  struct [[eosio::table]] test_table {
    // this field stores a name for each row of the multi index table
    name test_primary;
    // additional data stored in table row
    uint64_t datum;
    // mandatory definition for primary key getter
    uint64_t primary_key( ) const { return test_primary.value; }
  };
  
  typedef eosio::multi_index<"testtaba"_n, test_table> test_tables;
+  test_tables testtab;
```

Now you have instantiated the `testtab` as a multi index table which has a primary index defined for its `test_primary` data member.

[[info | Full example location]]
| A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).