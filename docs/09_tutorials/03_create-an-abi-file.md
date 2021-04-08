---
content_title: Create an ABI File
---

## Overview

This tutorial provides instructions on how to create an ABI file.

[[warning]]
| As of v1.2.0, the eosio.wasmsdk was decoupled from the core repository. This change has introduced an eosio-cpp regression where the legacy eosio-abigen is no longer bundled with eosio-cpp. Until a new ABI generator is introduced, you will need to hand-write your ABI files.

## Introduction

The Application Binary Interface (ABI) is a JSON-based description on how to convert user actions between their JSON and Binary representations. The ABI also describes how to convert the database state to/from JSON. Once you have described your contract via an ABI then developers and users will be able to interact with your contract seamlessly via JSON.

This tutorial will use the [eosio.token](https://github.com/EOSIO/eosio.contracts/tree/master/eosio.token) contract as an example. *eosio.token contract does not cover every possible permutation of an ABI definition.  

To make things easy, we will start with an empty ABI.

```text

{
   "version": "eosio::abi/1.0",
   "types": [],
   "structs": [],
   "actions": [],
   "tables": [],
   "ricardian_clauses": [],
   "abi_extensions": [],
   "___comment" : ""
}
```

## Types

An ABI enables any client or interface to interpret and even generate an GUI for you contract. For this to work in a consistent manner, we'll need to describe the custom types that are used as a parameter in any public action or struct that we would like to describe in the ABI.

[[info]]
|Built-in Types

EOSIO implements a number of custom built-ins. Built-in types don't need to be described in an ABI file. If you would like to familiarize yourself with EOSIO's built-ins, they are defined [here](https://github.com/EOSIO/eos/blob/master/libraries/chain/abi_serializer.cpp#L65-L103).

Using **eosio.token** as an example, the only type that requires a description in the ABI file is `account_name`. The ABI uses "new_type_name" to describe explicit types, in this case `account_name`, and `account_name` is an alias of `name` type.

So in the ABI file we'll add the following object

```json
{
   "new_type_name": "account_name",
   "type": "name"
 }
```

Our ABI now looks like this:

```json
{
   "version": "eosio::abi/1.0",
   "types": [{
     "new_type_name": "account_name",
     "type": "name"
   }],
   "structs": [],
   "actions": [],
   "tables": [],
   "ricardian_clauses": [],
   "abi_extensions": []
}
```

## Structs

We now need to describe the structs of the token contract. By looking at eosio.token.hpp, we can quickly determine which structs are utilized by public actions. This is particularly important for when we describe our actions in the the ABI file in the next step.

A struct's object definition in JSON looks like the following:

```json
{
     "name": "issue", //The name
     "base": "", 			//Inheritance, parent struct
     "fields": []			//Array of field objects describing the struct's fields.
  }
```

```json
{
  "name":"", // The field's name
  "type":""   // The field's type
}
```

Looking through the `eosio.token` contract, we see a number of structs that require definition. Please note, not all of the structs are explicitly defined, some correspond to an actions' parameters. Here's a list of structs that require an ABI description for the `eosio.token` contract:

## Implicit Structs

The following structs are implicit in that a struct was never explicitly defined in the contract. Looking at the [create](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L24) action, you'll find two parameters, `issuer` of type `account_name ` and `maximum_supply` of type `asset`. For brevity this tutorial won't break down every struct, but applying the same logic, you will end up with the following:

### [create](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L24)

```json
{
  "name": "create",
  "base": "",
  "fields": [
    {
      "name":"issuer",
      "type":"account_name"
    },
    {
      "name":"maximum_supply",
      "type":"asset"
    }
  ]
}
```

### [issue](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L27)

```json
{
  "name": "issue",
  "base": "",
  "fields": [
    {
      "name":"to",
      "type":"account_name"
    },
    {
      "name":"quantity",
      "type":"asset"
    },
    {
      "name":"memo",
      "type":"string"
    }
  ]
}
```

### [retire](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L29)

```json
{
  "name": "retire",
  "base": "",
  "fields": [
    {
      "name":"quantity",
      "type":"asset"
    },
    {
      "name":"memo",
      "type":"string"
    }
  ]
}
```

### [transfer](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L31-L34)

```json
{
  "name": "transfer",
  "base": "",
  "fields": [
    {
      "name":"from",
      "type":"account_name"
    },
    {
      "name":"to",
      "type":"account_name"
    },
    {
      "name":"quantity",
      "type":"asset"
    },
    {
      "name":"memo",
      "type":"string"
    }
  ]
}
```

### [close](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L36)

```json
{
  "name": "close",
  "base": "",
  "fields": [
    {
      "name":"owner",
      "type":"account_name"
    },
    {
      "name":"symbol",
      "type":"symbol"
    }
  ]
 }
```

## Explicit Structs
These structs are explicitly defined, as they are a requirement to instantiate a multi-index table. Describing them is no different than defining the implicit structs as demonstrated above.

### [account](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L43-L47)

```json
{
  "name": "account",
  "base": "",
  "fields": [
    {
      "name":"balance",
      "type":"asset"
    }
  ]
}
```

### [currency_stats](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L49-L55)

```json
{
  "name": "currency_stats",
  "base": "",
  "fields": [
    {
      "name":"supply",
      "type":"asset"
    },
    {
      "name":"max_supply",
      "type":"asset"
    },
    {
      "name":"issuer",
      "type":"account_name"
    }
  ]
}
```

## Actions

An action's JSON object definition looks like the following:

```json
{
  "name": "transfer", 			//The name of the action as defined in the contract
  "type": "transfer", 			//The name of the implicit struct as described in the ABI
  "ricardian_contract": "" 	//An optional ricardian clause to associate to this action describing its intended functionality.
}
```

Next, we'll describe the actions of the `eosio.token` contract by aggregating all the public functions describe in the `eosio.token` contract's [header file](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L24-L36). We'll then describe each action's *type* to their previously described struct. In most situations, the function name and the struct name will be equal, but are not required to be equal.

Below is a list of actions that link to their source code with example JSON provided for how each action would be described.

## [create](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L24-L25)

```json
{
  "name": "create",
  "type": "create",
  "ricardian_contract": ""
}
```

## [issue](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L27)

```json
{
  "name": "issue",
  "type": "issue",
  "ricardian_contract": ""
}
```

## [retire](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L31-L34)

```json
{
  "name": "retire",
  "type": "retire",
  "ricardian_contract": ""
}
```

## [transfer](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L31-L34)

```json
{
  "name": "transfer",
  "type": "transfer",
  "ricardian_contract": ""
}
```

## [close](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L36)

```json
{
  "name": "close",
  "type": "close",
  "ricardian_contract": ""
}
```

You will notice we previously described all of these in the "structs" array of the ABI definition.

## Tables

Finally, we need to describe our tables. Here's a table's JSON object definition:

```json
{
  "name": "",       // The name of the table, determined during instantiation.
  "type": "",       // The table's corresponding struct
  "index_type": "", // The type of primary index of this table
  "key_names" : [], // An array of key names, length must equal length of key_types member
  "key_types" : []  // An array of key types that correspond to key names array member, length of array must equal length of key names array.
}
```

The eosio.token contract instantiates two tables, [accounts](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L57) and [stat](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L58).

The accounts table is an i64 index, based on the [`account` struct](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L43-L47), has a [`uint64` as it's primary key](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L46) and it's key been arbitrarily named "currency".

Here's how the accounts table would be described in the ABI

```text
{
  "name": "accounts",
  "type": "account", // Corresponds to previously defined struct
  "index_type": "i64",
  "key_names" : ["currency"],
  "key_types" : ["uint64"]
}
```

The stat table is an i64 index, based on the [`currenct_stats` struct](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L49-L55), has a [`uint64` as it's primary key](https://github.com/EOSIO/eosio.contracts/blob/master/eosio.token/include/eosio.token/eosio.token.hpp#L54) and it's key been arbitrarily named "currency"

Here's how the stat table would be described in the ABI

```text
{
  "name": "stat",
  "type": "currency_stats",
  "index_type": "i64",
  "key_names" : ["currency"],
  "key_types" : ["uint64"]
}
```

You'll notice the above tables have the same "key name." Naming your keys similar names is symbolic in that it can potentially suggest a subjective relationship. As with this implementation, implying that any given value can be used to query different tables.

## Putting it all Together

Finally, once all the pieces are strewn together, we have ourselves a ABI file that accurately describes the `eosio.token` contract.

```json
{
  "version": "eosio::abi/1.0",
  "types": [
    {
      "new_type_name": "account_name",
      "type": "name"
    }
  ],
  "structs": [
    {
      "name": "create",
      "base": "",
      "fields": [
        {
          "name":"issuer",
          "type":"account_name"
        },
        {
          "name":"maximum_supply",
          "type":"asset"
        }
      ]
    },
    {
       "name": "issue",
       "base": "",
       "fields": [
          {
            "name":"to",
            "type":"account_name"
          },
          {
            "name":"quantity",
            "type":"asset"
          },
          {
            "name":"memo",
            "type":"string"
          }
       ]
    },
    {
       "name": "retire",
       "base": "",
       "fields": [
          {
            "name":"quantity",
            "type":"asset"
          },
          {
            "name":"memo",
            "type":"string"
          }
       ]
    },
    {
       "name": "close",
       "base": "",
       "fields": [
          {
            "name":"owner",
            "type":"account_name"
          },
          {
            "name":"symbol",
            "type":"symbol"
          }
       ]
    },
    {
      "name": "transfer",
      "base": "",
      "fields": [
        {
          "name":"from",
          "type":"account_name"
        },
        {
          "name":"to",
          "type":"account_name"
        },
        {
          "name":"quantity",
          "type":"asset"
        },
        {
          "name":"memo",
          "type":"string"
        }
      ]
    },
    {
      "name": "account",
      "base": "",
      "fields": [
        {
          "name":"balance",
          "type":"asset"
        }
      ]
    },
    {
      "name": "currency_stats",
      "base": "",
      "fields": [
        {
          "name":"supply",
          "type":"asset"
        },
        {
          "name":"max_supply",
          "type":"asset"
        },
        {
          "name":"issuer",
          "type":"account_name"
        }
      ]
    }
  ],
  "actions": [
    {
      "name": "transfer",
      "type": "transfer",
      "ricardian_contract": ""
    },
    {
      "name": "issue",
      "type": "issue",
      "ricardian_contract": ""
    },
    {
      "name": "retire",
      "type": "retire",
      "ricardian_contract": ""
    },
    {
      "name": "create",
      "type": "create",
      "ricardian_contract": ""
    },
    {
      "name": "close",
      "type": "close",
      "ricardian_contract": ""
    }
  ],
  "tables": [
    {
      "name": "accounts",
      "type": "account",
      "index_type": "i64",
      "key_names" : ["currency"],
      "key_types" : ["uint64"]
    },
    {
      "name": "stat",
      "type": "currency_stats",
      "index_type": "i64",
      "key_names" : ["currency"],
      "key_types" : ["uint64"]
    }
  ],
  "ricardian_clauses": [],
  "abi_extensions": []
}
```

## Cases not Covered by Token Contract

### Vectors

When describing a vector in your ABI file, simply append the type with `[]`, so if you need to describe a vector of permission levels, you would describe it like so: `permission_level[]`

### Struct Base

It's a rarely used property worth mentioning. You can use **base** ABI struct property to reference another struct for inheritance, as long as that struct is also described in the same ABI file. Base will do nothing or potentially throw an error if your smart contract logic does not support inheritance.

You can see an example of base in use in the system contract [source code](https://github.com/EOSIO/eosio.contracts/blob/4e4a3ca86d5d3482dfac85182e69f33c49e62fa9/eosio.system/include/eosio.system/eosio.system.hpp#L46) and [ABI](https://github.com/EOSIO/eosio.contracts/blob/4e4a3ca86d5d3482dfac85182e69f33c49e62fa9/eosio.system/abi/eosio.system.abi#L262)

### Key-Value Table

The `EOSIO` version 2.1 introduced the `Key-Value API` which provides a set of classes to facilitate the on-chain storage in form of `Key-Value Tables`.

A new “kv_tables” object is defined which contains objects corresponding to each `Key-Value Table` ('KV-Table'). The format of that object is below where “name” is an `eosio::name`, and `"type"` corresponds to a `"struct"` definition similar to `multi_index`. The `"primary_index"` is an object representing the primary index and secondary_indices is an object that represents 0 or more secondary indices.

```json
"kv_tables": {
  "tablea": {
    "type": "",
    "primary_index": {},
    "secondary_indices": {}
  }
}
```

The primary index object stores both the name and the type of the index. The secondary indices store objects whose key is the `name`, and whose `value` currently contains just the type. This format is chosen for ease of extensibility in the future.

```json
...
  "primary_index": {
    "name": "bcd",
    "type": ""
  },
  "secondary_indices": {
    "a1": {
      "type": ""
    },
    "b2": {
      ...  
    }
...
}
```

A full sample ABI is depicted below:

```json
"kv_tables": {
    "testtable": {
        "type": "my_struct",
        "primary_index": {
          "name": "myindex",
          "type": "name"
        },
        "secondary_indices": {
            "foo": {
              "type": "string"
            },
            "bar": {
              "type": "uint64"
            },
            "nonuniqname": {
              "type": "tuple_string_bool"
            },
            "age": {
              "type": "uint32"
            },
            "tst": {
              "type": "int32[]"
            }
        }
    }
}
```

Refer below for the full `KV Table ABI`.

```json
{
    "____comment": "This file was generated with eosio-abigen. DO NOT EDIT ",
    "version": "eosio::abi/1.2",
    "types": [],
    "structs": [
        {
            "name": "get",
            "base": "",
            "fields": []
        },
        {
            "name": "iteration",
            "base": "",
            "fields": []
        },
        {
            "name": "my_struct",
            "base": "",
            "fields": [
                {
                    "name": "primary_key",
                    "type": "name"
                },
                {
                    "name": "foo",
                    "type": "string"
                },
                {
                    "name": "bar",
                    "type": "uint64"
                },
                {
                    "name": "fullname",
                    "type": "string"
                },
                {
                    "name": "age",
                    "type": "uint32"
                },
                {
                    "name": "non_unique_name",
                    "type": "tuple_string_uint32"
                }
            ]
        },
        {
            "name": "nonunique",
            "base": "",
            "fields": []
        },
        {
            "name": "setup",
            "base": "",
            "fields": []
        },
        {
            "name": "tuple_string_uint32",
            "base": "",
            "fields": [
                {
                    "name": "field_0",
                    "type": "string"
                },
                {
                    "name": "field_1",
                    "type": "uint32"
                }
            ]
        },
        {
            "name": "update",
            "base": "",
            "fields": []
        },
        {
            "name": "updateerr1",
            "base": "",
            "fields": []
        },
        {
            "name": "updateerr2",
            "base": "",
            "fields": []
        }
    ],
    "actions": [
        {
            "name": "get",
            "type": "get",
            "ricardian_contract": ""
        },
        {
            "name": "iteration",
            "type": "iteration",
            "ricardian_contract": ""
        },
        {
            "name": "nonunique",
            "type": "nonunique",
            "ricardian_contract": ""
        },
        {
            "name": "setup",
            "type": "setup",
            "ricardian_contract": ""
        },
        {
            "name": "update",
            "type": "update",
            "ricardian_contract": ""
        },
        {
            "name": "updateerr1",
            "type": "updateerr1",
            "ricardian_contract": ""
        },
        {
            "name": "updateerr2",
            "type": "updateerr2",
            "ricardian_contract": ""
        }
    ],
    "tables": [],
    "kv_tables": {
        "testtable": {
            "type": "my_struct",
            "primary_index": {
                "name": "primarykey",
                "type": "name"
            },
            "secondary_indices": {
                "foo": {
                    "type": "string"
                },
                "bar": {
                    "type": "uint64"
                },
                "nonuniqname": {
                    "type": "tuple_string_uint32"
                },
                "age": {
                    "type": "uint32"
                }
            }
        }
    },
    "ricardian_clauses": [],
    "variants": [],
    "action_results": []
}
```

## ABI Properties Not Covered Here

A few properties of the ABI specification were skipped here for brevity, however, there is a pending ABI specification that will outline every property of the ABI in its entirety.

## Ricardian Clauses

Ricardian clauses describe the intended outcome of a particular actions. It may also be utilized to establish terms between the sender and the contract.

## ABI Extensions

A generic "future proofing" layer that allows old clients to skip the parsing of "chunks" of extension data. For now, this property is unused. In the future each extension would have its own "chunk" in that vector so that older clients skip it and newer clients that understand how to interpret it.

## Maintenance

Every time you change a struct, add a table, add an action or add parameters to an action, use a new type, you will need to remember to update your ABI file. In many cases failure to update your ABI file will not produce any error.

## Troubleshooting

### Table returns no rows

Check that your table is accurately described in the <<glossary:ABI>> file. For example, If you use `cleos` to add a table on a contract with a malformed <<glossary:ABI>> definition and then get rows from that table, you will recieve an empty result. `cleos` will not produce an error when adding a row nor reading a row when a contract has failed to properly describe its tables in its <<glossary:ABI>> File.
