---
content_title: How to define a secondary index
---

## Preconditions
- It is assumed you already have a multi index table instance defined along with its mandatory primary index, otherwise take a look at the section [How to instantiate a multi index table](./how-to-instantiate-a-multi-index-table.md).

The steps below show how to add a secondary index to the existing multi index table.

1. Add a second field, `secondary`, to the data structure that defines the row of the table, in your case `test_table`
```diff
  struct [[eosio::table]] test_table {
    // this field stores a name for each row of the multi index table
    name test_primary;
+    name secondary;
    // additional data stored in table row
    uint64_t datum;
    // mandatory definition for primary key getter
    uint64_t primary_key( ) const { return test_primary.value; }
  };
```

2. Add `by_secondary( )` method, which is the index accessor method to the new field value added. The secondary index, that will be added in step 3, will index this new data structure field.
```diff
  struct [[eosio::table]] test_table {
    // this field stores a name for each row of the multi index table
    name test_primary;
    name secondary;
    // additional data stored in table row
    uint64_t datum;
    // mandatory definition for primary key getter
    uint64_t primary_key( ) const { return test_primary.value; }
+    uint64_t by_secondary( ) const { return secondary.value; }
  };
```

3. In the `test_table` alias definition (typedef), add the definition of the secondary index by making use of the `eosio::indexed_by` template. `eosio::index_by` needs two parameters: the name of the index, `"secid"_n`, and a function call operator which extracts the value from the secondary data member as an index key. The function call operator is achieved by employing the `eosio::const_mem_fun` template which receives two parameters: the data structure `test_table` and the reference to the getter function member `by_secondary`.

```diff
-  typedef eosio::multi_index<"testtaba"_n, test_table> test_tables;
+  typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_tables;
```

The full contract definition code with all the changes described above could look like this:

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

      struct [[eosio::table]] test_table {
        // this field stores a name for each row of the multi index table
        name test_primary;
        name secondary;
        // additional data stored in table row
        uint64_t datum;
        // mandatory definition for primary key getter
        uint64_t primary_key( ) const { return test_primary.value; }
        uint64_t by_secondary( ) const { return secondary.value; }
      };

      // the multi index type definition, for ease of use a type alias `test_tables` is defined, 
      // based on the multi_index template type, parametarized with a random name, the 
      // test_table data structure, and the secondary index
      typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_tables;

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