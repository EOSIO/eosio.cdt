---
content_title: ABI variants
---

ABI variants give the flexibility of using more than one type for a defined variable or data member. 
In EOSIO, the variants make use of the standard template library `variant` which was introduced in C++ 17. An instance of `std::variant` at any given time either holds a value of one of its alternative types, or in the case of error - no value. Because of this trait, variants can be used to build the multi index table structure with flexibility. Used in conjunction with ABI extensions, it allows for modification of the structure of an exiting multi index table, a.k.a. table.

## Use variant when building the multi index table the first time

To define a `variant` for your table structure one example is shown below

```cpp
  std::variant<int8_t, uint16_t, uint32_t> variant_field;
```

This defines `variant` which can hold three different types, one at a time though. 
So the contract interface could look like this:

```diff
#include <eosio/eosio.hpp>
using namespace eosio;

class [[eosio::contract]] multi_index_example : public contract {
   public:
      using contract::contract;
      multi_index_example( name receiver, name code, datastream<const char*> ds )
         : contract(receiver, code, ds), testtab(receiver, receiver.value)
         { }

      struct [[eosio::table]] test_table {
         name test_primary;
         name secondary;
         uint64_t datum;
+         std::variant<int8_t, uint16_t, uint32_t> variant_field;

         uint64_t primary_key()const { return test_primary.value; }
         uint64_t by_secondary()const { return secondary.value; }
+         std::variant<int8_t, uint16_t, uint32_t> get_variant_field()const {
+            return std::visit(
+               [](auto&& arg) -> std::variant<int8_t, uint16_t, uint32_t> {
+                  return arg;
+               },
+            variant_field);
         }
      };

      typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_tables;

      test_tables testtab;

      [[eosio::action]] void set(name user);
      [[eosio::action]] void print( name user );

      using set_action = action_wrapper<"set"_n, &multi_index_example::set>;
      using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
};
```

Notice above the declaration of the `variant_field` data memember and also the declaration and inline implementation for the `get_variant_field()` accessor for this data member.

In the future, this allows you the flexibility to store in the `variant_field` three different types of data `int8_t`, `int16_t`, and `int32_t`, and also allows you to add more types in the list of supported types for this field. One important thing to keep in mind is that you can only append at the end of the supported types, you can not modify the existing supported types order nor drop one of them. That means if you want in the next version of your contract to add also type `int32_t` to the supported list types for this field, your contract implementation could look like this:

```diff
#include <eosio/eosio.hpp>
using namespace eosio;

class [[eosio::contract]] multi_index_example : public contract {
   public:
      using contract::contract;
      multi_index_example( name receiver, name code, datastream<const char*> ds )
         : contract(receiver, code, ds), testtab(receiver, receiver.value)
         { }

      struct [[eosio::table]] test_table {
         name test_primary;
         name secondary;
         uint64_t datum;
+         std::variant<int8_t, uint16_t, uint32_t, int32_t> variant_field;

         uint64_t primary_key()const { return test_primary.value; }
         uint64_t by_secondary()const { return secondary.value; }
+         std::variant<int8_t, uint16_t, uint32_t, int32_t> get_variant_field()const {
+            return std::visit(
+               [](auto&& arg) -> std::variant<int8_t, uint16_t, uint32_t, int32_t> {
+                  return arg;
+               },
+            variant_field);
         }
      };

      typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_tables;

      test_tables testtab;

      [[eosio::action]] void set(name user);
      [[eosio::action]] void print( name user );

      using set_action = action_wrapper<"set"_n, &multi_index_example::set>;
      using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
};
```

Now you can deploy the contract and it will be backwards compatible with the previous existing multi index table.

## Use variant when changing an already deployed multi index table

### Preconditions
- It is assumed you deployed the contract defined in [this section](../06_how-to-guides/02_multi-index/how-to-instantiate-a-multi-index-table.md) and now you are going to change its table structure. 

To change the existing table structure, you will use the `std::variant` in conjunction with ABI extensions; you can read a tutorial on abi extensions [here](./01_binary-extension.md). You will add another field to the table called `variant_field` which can store either of the following data `int8_t`, `int16_t`, and `int32_t`. You can do it by adding below data member to the table structure:

```cpp
  eosio::binary_extension<std::variant<int8_t, uint16_t, uint32_t>> binary_extension_variant_key;
```

Notice, the use of the `eosio::binary_extension` template which wraps the `std::variant` template parameterized with the types you want to support for the new data field. The full contract implementation can look like this:

```diff
#include <eosio/eosio.hpp>
#include <eosio/binary_extension.hpp>
using namespace eosio;

class [[eosio::contract]] multi_index_example : public contract {
   public:
      using contract::contract;
      multi_index_example( name receiver, name code, datastream<const char*> ds )
         : contract(receiver, code, ds), testtab(receiver, receiver.value)
         { }

      struct [[eosio::table]] test_table {
         name test_primary;
         name secondary;
         uint64_t datum;
+         eosio::binary_extension<std::variant<int8_t, uint16_t, uint32_t>> binary_extension_variant_key;

         uint64_t primary_key()const { return test_primary.value; }
         uint64_t by_secondary()const { return secondary.value; }
+         eosio::binary_extension<std::variant<int8_t, uint16_t, uint32_t>>  get_binary_extension_variant_field()const {
+            return binary_extension_variant_key;
+         }
      };

      typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_tables;

      test_tables testtab;

      [[eosio::action]] void set(name user);
      [[eosio::action]] void print( name user );

      using set_action = action_wrapper<"set"_n, &multi_index_example::set>;
      using print_action = action_wrapper<"print"_n, &multi_index_example::print>;
};
```

[[warning | Not recommended warning]]
| Be aware, it is not recommend to use `eosio::binary_extension` inside variant definition, this can lead to data corruption unless one is very careful in understanding how these two templates work and how to ABI gets generated!

[[info | Implemenatation location]]
| The implementation for ABI `variants' section can be found [here](https://github.com/EOSIO/eos/pull/5652).