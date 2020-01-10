---
content_title: How to instantiate a multi index table
---

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
4. Add definition of the primary index for the multi index table. The primary index type must be uint64_t, it must be unique and and it must be named `primary_key()`, if you don't have this the compiler (eosio-cpp) will generate an error saying it can't find the field to use as the primary key:
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

[[info | Additional indexes information]]
| Other, secondary, indexes if they will be defined can have duplicates. You can have up to 16 additional indexes and the field types can be uint64_t, uint128_t, uint256_t, double or long double.

5. For ease of use, define a type alias `test_tables` based on the multi_index template type, parametarized with a random name `"testtaba"` and the `test_table` data structure defined above
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

6. Define the multi index table data member of type `test_tables` defined in the privious step
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

7. Instantiate the data member `testtab` by passing to its constructor the `scope` (in this case `receiver`) and the `code` parameters, these two combined with table name `"testtaba"` provide access to the partition of the RAM cache used by this multi index table, in this example you will initialize the `testtab` data member in the smart contract constructor

```diff
// contract class constructor
multi_index_example( name receiver, name code, datastream<const char*> ds ) :
   // contract base class contructor
   contract(receiver, code, ds),
   // instantiate multi index instance as data member (find it defined below)
+   testtab(receiver, receiver.value)
   { }
```
Now you have instantiated the `testtab` variable as a multi index table which has a primary index defined for its `test_primary` data member.

Here is how the definition of a `multi_index_example` contract containing a multi index table could look like after following all the steps above.

__multi_index_example.hpp__
```cpp
#include <eosio/eosio.hpp>
using namespace eosio;

// multi index example contract class
class [[eosio::contract]] multi_index_example : public contract {
   public:
      using contract::contract;

      // contract class constructor
      multi_index_example( name receiver, name code, datastream<const char*> ds ) :
         // contract base class contructor
         contract(receiver, code, ds),
         // instantiate multi index instance as data member (find it defined below)
         testtab(receiver, receiver.value)
         { }

      // the row structure of the multi index table, that is, each row of the table
      // will contain an instance of this type of structure
      struct [[eosio::table]] test_table {
        // this field stores a name for each row of the multi index table
        name test_primary;
        // additional data stored in table row
        uint64_t datum;
        // mandatory definition for primary key getter
        uint64_t primary_key( ) const { return test_primary.value; }
      };

      // the multi index type definition, for ease of use define a type alias `test_tables`, 
      // based on the multi_index template type, parametarized with a random name and 
      // the test_table data structure
      typedef eosio::multi_index<"testtaba"_n, test_table> test_tables;

      // the multi index table instance declared as a data member of type test_tables
      test_tables testtab;

      [[eosio::action]] void set( name user );
      [[eosio::action]] void print( name user );

      using set_action = action_wrapper<"set"_n, &multi_index_example::set>;
      using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
};
```

[[info | Full example location]]
| A full example project demonstrating the instantiation and usage of multi index table can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).