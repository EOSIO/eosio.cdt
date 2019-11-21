---
content_title: The eosio::binary_extension type
---

Let's fully explain what the `eosio::binary_extension` type is, what it does, and why we need it for contract upgrades in certain situations.

You can find the implementation of `eosio::binary_extension` in the `eosio.cdt` repository in file: `eosio.cdt/libraries/eosiolib/core/eosio/binary_extension.hpp`.

Our primary concern when using this type is when we are adding a new field to a smart contract's data structure that is currently utilized in an `eosio::multi_index` type (AKA a _table_), or when adding a new parameter to an action declaration.

By wrapping the new field in an `eosio::binary_extension`, you are enabling your contract to be backwards compatible for future use. Note that this new field/parameter **MUST** be appended at the end of a data structure (this is due to implementation details in `eosio::multi_index`, which relies on the `boost::multi_index` type), or at the end of the parameter list in an action declaration.

If you don't wrap the new field in an `eosio::binary_extension`, the `eosio::multi_index` table will be reformatted in such a way that disallows reads to the former datum; or in an action's case, the function will be uncallable.

<hr>

But let's see how the `eosio::binary_extension` type works with a good example.

Take a moment to study this smart contract and its corresponding `.abi`.

This contract not only serves as a good example to the `eosio::binary_extension` type, but can also be used as a gateway for developing smart contracts on the eosio protocol.

**binary_extension_contract.hpp**

```c++
#include <eosio/contract.hpp>         // eosio::contract
#include <eosio/binary_extension.hpp> // eosio::binary_extension
#include <eosio/datastream.hpp>       // eosio::datastream
#include <eosio/name.hpp>             // eosio::name
#include <eosio/multi_index.hpp>      // eosio::indexed_by, eosio::multi_index
#include <eosio/print.hpp>            // eosio::print_f

class [[eosio::contract]] binary_extension_contract : public eosio::contract {
public:
   using contract::contract;
   binary_extension_contract(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds)
      : contract{receiver, code, ds}, _table{receiver, receiver.value}
   { }

   [[eosio::action]] void regpkey (eosio::name primary_key);                ///< Register primary key.
   [[eosio::action]] void printbyp(eosio::name primary_key);                ///< Print by primary key.
   [[eosio::action]] void printbys(eosio::name secondary_key);              ///< Print by secondary key.
   [[eosio::action]] void modifyp (eosio::name primary_key, eosio::name n); ///< Modify primary key by primary key.
   [[eosio::action]] void modifys (eosio::name primary_key, eosio::name n); ///< Modify secondary key by primary key.

   struct [[eosio::table]] structure {
      eosio::name _primary_key;
      eosio::name _secondary_key;
         
      uint64_t primary_key()   const { return _primary_key.value;   }
      uint64_t secondary_key() const { return _secondary_key.value; }
   };

   using index1 = eosio::indexed_by<"index1"_n, eosio::const_mem_fun<structure, uint64_t, &structure::primary_key>>;
   using index2 = eosio::indexed_by<"index2"_n, eosio::const_mem_fun<structure, uint64_t, &structure::secondary_key>>;
   using table  = eosio::multi_index<"table"_n, structure, index1, index2>;

private:
   table _table;
};

```

**binary_extension_contract.cpp**

```c++
#include "binary_extension_contract.hpp"

using eosio::name;

[[eosio::action]] void binary_extension_contract::regpkey(name primary_key) {
   eosio::print_f("`regpkey` executing.\n");
   
   auto index{_table.get_index<"index1"_n>()}; ///< `index` represents `_table` organized by `index1`.
   auto iter {index.find(primary_key.value) }; ///< Note: the type returned by `index.find` is different than the type returned by `_table.find`.
   
   if (iter == _table.get_index<"index1"_n>().end()) {
      eosio::print_f("`_primary_key`: % not found; registering.\n", primary_key.to_string());
      _table.emplace(_self, [&](auto& row) {
         row._primary_key   = primary_key;
         row._secondary_key = "nothin"_n;
      });
   }
   else {
      eosio::print_f("`_primary_key`: % found; not registering.\n", primary_key.to_string());
   }

   eosio::print_f("`regpkey` finished executing.\n");
}

[[eosio::action]] void binary_extension_contract::printbyp(eosio::name primary_key) {
   eosio::print_f("`printbyp` executing.\n");
   
   auto index{_table.get_index<"index1"_n>()};
   auto iter {index.find(primary_key.value) };
   
   if (iter != _table.get_index<"index1"_n>().end()) {
      eosio::print_f("`_primary_key`: % found; printing.\n", primary_key.to_string());
      eosio::print_f("{%, %}\n", iter->_primary_key, iter->_secondary_key);
   }
   else {
      eosio::print_f("`_primary_key`: % not found; not printing.\n", primary_key.to_string());
   }

   eosio::print_f("`printbyp` finished executing.\n");
}

[[eosio::action]] void binary_extension_contract::printbys(eosio::name secondary_key) {
   eosio::print_f("`printbys` executing.\n");
   
   auto index{_table.get_index<"index2"_n>()};
   auto iter {index.find(secondary_key.value)};
   
   if (iter != _table.get_index<"index2"_n>().end()) {
      eosio::print_f("`_secondary_key`: % found; printing.\n", secondary_key.to_string());
      printbyp(iter->_primary_key);
   }
   else {
      eosio::print_f("`_secondary_key`: % not found; not printing.\n", secondary_key.to_string());
   }

   eosio::print_f("`printbys` finished executing.\n");
}

[[eosio::action]] void binary_extension_contract::modifyp(eosio::name primary_key, name n) {
   eosio::print_f("`modifyp` executing.\n");
   
   auto index{_table.get_index<"index1"_n>()};
   auto iter {index.find(primary_key.value)};
   
   if (iter != _table.get_index<"index1"_n>().end()) {
      eosio::print_f("`_primary_key`: % found; modifying `_primary_key`.\n", primary_key.to_string());
      index.modify(iter, _self, [&](auto& row) {
         row._primary_key = n;
      });
   }
   else {
      eosio::print_f("`_primary_key`: % not found; not modifying `_primary_key`.\n", primary_key.to_string());
   }

   eosio::print_f("`modifyp` finished executing.\n");
}

[[eosio::action]] void binary_extension_contract::modifys(eosio::name primary_key, name n) {
   eosio::print_f("`modifys` executing.\n");
   
   auto index{_table.get_index<"index1"_n>()};
   auto iter {index.find(primary_key.value)};
   
   if (iter != _table.get_index<"index1"_n>().end()) {
      eosio::print_f("`_primary_key`: % found; modifying `_secondary_key`.\n", primary_key.to_string());
      index.modify(iter, _self, [&](auto& row) {
         row._secondary_key = n;
      });
   }
   else {
      eosio::print_f("`_primary_key`: % not found; not modifying `_secondary_key`.\n", primary_key.to_string());
   }

   eosio::print_f("`modifys` finished executing.\n");
}
```

**binary_extension_contract.abi**

```javascript
{
    "____comment": "This file was generated with eosio-abigen. DO NOT EDIT ",
    "version": "eosio::abi/1.1",
    "types": [],
    "structs": [
        {
            "name": "modifyp",
            "base": "",
            "fields": [
                {
                    "name": "primary_key",
                    "type": "name"
                },
                {
                    "name": "n",
                    "type": "name"
                }
            ]
        },
        {
            "name": "modifys",
            "base": "",
            "fields": [
                {
                    "name": "primary_key",
                    "type": "name"
                },
                {
                    "name": "n",
                    "type": "name"
                }
            ]
        },
        {
            "name": "printbyp",
            "base": "",
            "fields": [
                {
                    "name": "primary_key",
                    "type": "name"
                }
            ]
        },
        {
            "name": "printbys",
            "base": "",
            "fields": [
                {
                    "name": "secondary_key",
                    "type": "name"
                }
            ]
        },
        {
            "name": "regpkey",
            "base": "",
            "fields": [
                {
                    "name": "primary_key",
                    "type": "name"
                }
            ]
        },
        {
            "name": "structure",
            "base": "",
            "fields": [
                {
                    "name": "_primary_key",
                    "type": "name"
                },
                {
                    "name": "_secondary_key",
                    "type": "name"
                }
            ]
        }
    ],
    "actions": [
        {
            "name": "modifyp",
            "type": "modifyp",
            "ricardian_contract": ""
        },
        {
            "name": "modifys",
            "type": "modifys",
            "ricardian_contract": ""
        },
        {
            "name": "printbyp",
            "type": "printbyp",
            "ricardian_contract": ""
        },
        {
            "name": "printbys",
            "type": "printbys",
            "ricardian_contract": ""
        },
        {
            "name": "regpkey",
            "type": "regpkey",
            "ricardian_contract": ""
        }
    ],
    "tables": [
        {
            "name": "table",
            "type": "structure",
            "index_type": "i64",
            "key_names": [],
            "key_types": []
        }
    ],
    "ricardian_clauses": [],
    "variants": []
}
```

<hr>

Take note of the action `regpkey`, and the struct `structure` in `con.hpp` and `con.cpp`; the parts of the contract we will be upgrading.

**binary_extension_contract.hpp**

```c++
[[eosio::action]] void regpkey (eosio::name primary_key);
```

```c++
struct [[eosio::table]] structure {
    eosio::name _primary_key;
    eosio::name _secondary_key;

    uint64_t primary_key()   const { return _primary_key.value;   }
    uint64_t secondary_key() const { return _secondary_key.value; }
};
```

**binary_extension_contract.cpp**

```c++
[[eosio::action]] void binary_extension_contract::regpkey(name primary_key) {
   eosio::print_f("`regpkey` executing.\n");
   
   auto index{_table.get_index<"index1"_n>()}; ///< `index` represents `_table` organized by `index1`.
   auto iter {index.find(primary_key.value) }; ///< Note: the type returned by `index.find` is different than the type returned by `_table.find`.
   
   if (iter == _table.get_index<"index1"_n>().end()) {
      eosio::print_f("`_primary_key`: % not found; registering.\n", primary_key.to_string());
      _table.emplace(_self, [&](auto& row) {
         row._primary_key   = primary_key;
         row._secondary_key = "nothin"_n;
      });
   }
   else {
      eosio::print_f("`_primary_key`: % found; not registering.\n", primary_key.to_string());
   }

   eosio::print_f("`regpkey` finished executing.\n");
}
```

And their corresponding sections in the `.abi` files:

**binary_extension_contract.abi**

```javascript
{
    "name": "regpkey",
    "base": "",
    "fields": [
        {
            "name": "primary_key",
            "type": "name"
        }
    ]
}
```

```javascript
{
    "name": "structure",
    "base": "",
    "fields": [
        {
            "name": "_primary_key",
            "type": "name"
        },
        {
            "name": "_secondary_key",
            "type": "name"
        }
    ]
}
```

<hr>

Now, let's start up a blockchain instance, compile this smart contract, and test it out.

```
~/binary_extension_contract $ eosio-cpp binary_extension_contract.cpp -o binary_extension_contract.wasm
```

```
~/binary_extension_contract $ cleos set contract eosio ./
```

```
Reading WASM from /Users/john.debord/binary_extension_contract/binary_extension_contract.wasm...
Publishing contract...
executed transaction: 6c5c7d869a5be67611869b5f300bc452bc57d258d11755f12ced99c7d7fe154c  4160 bytes  729 us
#         eosio <= eosio::setcode               "0000000000ea30550000d7600061736d01000000018f011760000060017f0060027f7f0060037f7f7f017f6000017e60067...
#         eosio <= eosio::setabi                "0000000000ea3055d1020e656f73696f3a3a6162692f312e310006076d6f646966797000020b7072696d6172795f6b65790...
warning: transaction executed locally, but may not be confirmed by the network yet
```

Next, let's push some data to our contract.

```
~/binary_extension_contract $ cleos push action eosio regpkey '{"primary_key":"eosio.name"}' -p eosio
```

```
executed transaction: 3c708f10dcbf4412801d901eb82687e82287c2249a29a2f4e746d0116d6795f0  104 bytes  248 us
#         eosio <= eosio::regpkey               {"primary_key":"eosio.name"}
[(eosio,regpkey)->eosio]: CONSOLE OUTPUT BEGIN =====================
`regpkey` executing.
`_primary_key`: eosio.name not found; registering.
`regpkey` finished executing.
[(eosio,regpkey)->eosio]: CONSOLE OUTPUT END   =====================
warning: transaction executed locally, but may not be confirmed by the network yet
```

Finally, let's read back the data we have just written.

```
~/binary_extension_contract $ cleos push action eosio printbyp '{"primary_key":"eosio.name"}' -p eosio
```

```
executed transaction: e9b77d3cfba322a7a3a93970c0c883cb8b67e2072a26d714d46eef9d79b2f55e  104 bytes  227 us
#         eosio <= eosio::printbyp              {"primary_key":"eosio.name"}
[(eosio,printbyp)->eosio]: CONSOLE OUTPUT BEGIN =====================
`printbyp` executing.
`_primary_key`: eosio.name found; printing.
{eosio.name, nothin}
`printbyp` finished executing.
[(eosio,printbyp)->eosio]: CONSOLE OUTPUT END   =====================
warning: transaction executed locally, but may not be confirmed by the network yet
```

<hr>

Now, let's upgrade the smart contract by adding a new field to the table and a new parameter to an action while **NOT** wrapping the new field/parameter in an `eosio::binary_extension` type and see what happens:

**binary_extension_contract.hpp**

```diff
+[[eosio::action]] void regpkey (eosio::name primary_key, eosio::name secondary_key);
-[[eosio::action]] void regpkey (eosio::name primary_key);
```

```diff
struct [[eosio::table]] structure {
    eosio::name _primary_key;
    eosio::name _secondary_key;
+   eosio::name _non_binary_extension_key;

    uint64_t primary_key()   const { return _primary_key.value;   }
    uint64_t secondary_key() const { return _secondary_key.value; }
};
```

**binary_extension_contract.cpp**

```diff
+[[eosio::action]] void binary_extension_contract::regpkey(name primary_key, name secondary_key) {
-[[eosio::action]] void binary_extension_contract::regpkey(name primary_key) {
   eosio::print_f("`regpkey` executing.\n");
   
   auto index{_table.get_index<"index1"_n>()}; ///< `index` represents `_table` organized by `index1`.
   auto iter {index.find(primary_key.value) }; ///< Note: the type returned by `index.find` is different than the type returned by `_table.find`.
   
   if (iter == _table.get_index<"index1"_n>().end()) {
      eosio::print_f("`_primary_key`: % not found; registering.\n", primary_key.to_string());
      _table.emplace(_self, [&](auto& row) {
         row._primary_key   = primary_key;
+        if (secondary_key) {
+           row._secondary_key = secondary_key;
+         }
+         else {
            row._secondary_key = "nothin"_n;
+         }
      });
   }
   else {
      eosio::print_f("`_primary_key`: % found; not registering.\n", primary_key.to_string());
   }

   eosio::print_f("`regpkey` finished executing.\n");
}
```

**binary_extension_contract.abi**
```diff
{
    "name": "regpkey",
    "base": "",
    "fields": [
        {
            "name": "primary_key",
            "type": "name"
+       },
+       {
+           "name": "secondary_key",
+           "type": "name"
        }
    ]
}
```

```diff
{
    "name": "structure",
    "base": "",
    "fields": [
        {
            "name": "_primary_key",
            "type": "name"
        },
        {
            "name": "_secondary_key",
            "type": "name"
+       },
+	{
+           "name": "_non_binary_extension_key",
+           "type": "name"
        }
    ]
}
```

Next, let's upgrade the contract and try to read from our table and write to our table the original way:

```
~/binary_extension_contract $ eosio-cpp binary_extension_contract.cpp -o binary_extension_contract.wasm
```

```
~/binary_extension_contract $ cleos set contract eosio ./
```

```
Reading WASM from /Users/john.debord/binary_extension_contract/binary_extension_contract.wasm...
Publishing contract...
executed transaction: b8ea485842fa5645e61d35edd97e78858e062409efcd0a4099d69385d9bc6b3e  4408 bytes  664 us
#         eosio <= eosio::setcode               "0000000000ea30550000a2660061736d01000000018f011760000060017f0060027f7f0060037f7f7f017f6000017e60067...
#         eosio <= eosio::setabi                "0000000000ea305583030e656f73696f3a3a6162692f312e310006076d6f646966797000020b7072696d6172795f6b65790...
warning: transaction executed locally, but may not be confirmed by the network yet
```

```
~/binary_extension_contract $ cleos push action eosio printbyp '{"primary_key":"eosio.name"}' -p eosio
```

```
Error 3050003: eosio_assert_message assertion failure
Error Details:
assertion failure with message: read
```

Whoops! We aren't able to read the data we've previously written to our table!

```
~/binary_extension_contract $ cleos push action eosio regpkey '{"primary_key":"eosio.name2"}' -p eosio
```

```
Error 3015014: Pack data exception
Error Details:
Missing field 'secondary_key' in input object while processing struct 'regpkey'
```

Whoops! We aren't able to write to our table the original way with the upgraded action either!

<hr>

Ok, let's back up and wrap the new field and the new action parameter in an `eosio::binary_extension` type:

**binary_extension_contract.hpp**

```diff
+[[eosio::action]] void regpkey (eosio::name primary_key. eosio::binary_extension<eosio::name> secondary_key);
-[[eosio::action]] void regpkey (eosio::name primary_key, eosio::name secondary_key);
```

```diff
struct [[eosio::table]] structure {
    eosio::name                          _primary_key;
    eosio::name                          _secondary_key;
+   eosio::binary_extension<eosio::name> _binary_extension_key;
-   eosio::name                          _non_binary_extension_key;

    uint64_t primary_key()   const { return _primary_key.value;   }
    uint64_t secondary_key() const { return _secondary_key.value; }
};
```

**binary_extension_contract.cpp**

```diff
+[[eosio::action]] void binary_extension_contract::regpkey(name primary_key, binary_extension<name> secondary_key) {
-[[eosio::action]] void binary_extension_contract::regpkey(name primary_key, name secondary_key) {
   eosio::print_f("`regpkey` executing.\n");
   
   auto index{_table.get_index<"index1"_n>()}; ///< `index` represents `_table` organized by `index1`.
   auto iter {index.find(primary_key.value) }; ///< Note: the type returned by `index.find` is different than the type returned by `_table.find`.
   
   if (iter == _table.get_index<"index1"_n>().end()) {
      eosio::print_f("`_primary_key`: % not found; registering.\n", primary_key.to_string());
      _table.emplace(_self, [&](auto& row) {
         row._primary_key   = primary_key;
         if (secondary_key) {
+           row._secondary_key = secondary_key.value();
-           row._secondary_key = secondary_key;
          }
          else {
            row._secondary_key = "nothin"_n;
          }
      });
   }
   else {
      eosio::print_f("`_primary_key`: % found; not registering.\n", primary_key.to_string());
   }

   eosio::print_f("`regpkey` finished executing.\n");
}
```

**binary_extension_contract.abi**
```diff
{
    "name": "regpkey",
    "base": "",
    "fields": [
        {
            "name": "primary_key",
            "type": "name"
        },
        {
            "name": "secondary_key",
+           "type": "name$"
-           "type": "name"
        }
    ]
}
```

```diff
{
    "name": "structure",
    "base": "",
    "fields": [
        {
            "name": "_primary_key",
            "type": "name"
        },
        {
            "name": "_secondary_key",
            "type": "name"
        },
	{
+           "name": "_binary_extension_key",
+           "type": "name$"
-           "name": "_non_binary_extension_key",
-           "type": "name"
        }
    ]
}
```

Note the `$` after the types now; this indicates that this type is an `eosio::binary_extension` type field.
```diff
{
    "name": "secondary_key",
+   "type": "name$"
-   "type": "name"
}
```

```diff
{
    "name": "_binary_extension_key",
+   "type": "name$"
-   "type": "name"
}
```

Now, let's upgrade the contract again and try to read/write from/to our table:

```
~/binary_extension_contract $ cleos set contract eosio ./
```

```
Reading WASM from /Users/john.debord/binary_extension_contract/binary_extension_contract.wasm...
Publishing contract...
executed transaction: 497584d4e43ec114dbef83c134570492893f49eacb555d0cd47d08ea4a3a72f7  4696 bytes  648 us
#         eosio <= eosio::setcode               "0000000000ea30550000cb6a0061736d01000000018f011760000060017f0060027f7f0060037f7f7f017f6000017e60017...
#         eosio <= eosio::setabi                "0000000000ea305581030e656f73696f3a3a6162692f312e310006076d6f646966797000020b7072696d6172795f6b65790...
warning: transaction executed locally, but may not be confirmed by the network yet
```

```
~/binary_extension_contract $ cleos push action eosio printbyp '{"primary_key":"eosio.name"}' -p eosio
```

```
executed transaction: 6108f3206e1824fe3a1fdcbc2fe733f38dc07ae3d411a1ccf777ecef56ddec97  104 bytes  224 us
#         eosio <= eosio::printbyp              {"primary_key":"eosio.name"}
[(eosio,printbyp)->eosio]: CONSOLE OUTPUT BEGIN =====================
`printbyp` executing.
`_primary_key`: eosio.name found; printing.
{eosio.name, nothin}
`printbyp` finished executing.
[(eosio,printbyp)->eosio]: CONSOLE OUTPUT END   =====================
warning: transaction executed locally, but may not be confirmed by the network yet
```

```
~/binary_extension_contract $ cleos push action eosio regpkey '{"primary_key":"eosio.name2"}' -p eosio
```

```
executed transaction: 75a135d1279a9c967078b0ebe337dc0cd58e1ccd07e370a899d9769391509afc  104 bytes  227 us
#         eosio <= eosio::regpkey               {"primary_key":"eosio.name2"}
[(eosio,regpkey)->eosio]: CONSOLE OUTPUT BEGIN =====================
`regpkey` executing.
`_primary_key`: eosio.name2 not found; registering.
`regpkey` finished executing.
[(eosio,regpkey)->eosio]: CONSOLE OUTPUT END   =====================
warning: transaction executed locally, but may not be confirmed by the network yet
```

Nice! The smart contract is now backwards compatible for the future use of its tables and/or actions.

<hr>

Just keep these simple rules in mind when upgrading a smart contract.
If you are adding a new field to a struct currently in use by a `eosio::multi_index` be **SURE** to:
- add the field at the end of the struct.
- wrap the type using an `eosio::binary_extension` type.
