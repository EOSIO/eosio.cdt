---
content_title: "Understanding ABI Files"
link_text: "Understanding ABI Files"
---
## Introduction

ABI files can be generated using the `eosio-cpp` utility provided by `eosio.cdt`. However, there are several situations that may cause ABI's generation to malfunction or fail altogether. Advanced C++ patterns can trip it up and custom types can sometimes cause issues for ABI generation. For this reason, it's **imperative** you understand how ABI files work, so you can debug and fix if and when necessary.

## What is an ABI

The Application Binary Interface (ABI) is a JSON-based description on how to convert user actions between their JSON and Binary representations. The ABI also describes how to convert the database state to/from JSON. Once you have described your contract via an ABI then developers and users will be able to interact with your contract seamlessly via JSON.

[[warning | Security Note]]
| ABI can be bypassed when executing transactions. Messages and actions passed to a contract do not have to conform to the ABI. The ABI is a guide, not a gatekeeper.

## Create an ABI File

Start with an empty ABI, for exemplification we will work based on the `eosio.token` therefore name it `eosio.token.abi`:

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

An ABI enables any client or interface to interpret and even generate a GUI for your contract. For this to work consistently, describe the custom types that are used as a parameter in any public action or struct that needs to be described in the ABI.

[[info | Built-in Types]]
| EOSIO implements a number of custom built-ins. Built-in types don't need to be described in an ABI file. If you would like to familiarize yourself with EOSIO's built-ins, they are defined [here](https://github.com/EOSIO/eos/blob/release/2.1.x/libraries/chain/abi_serializer.cpp#L85)


```json
{
   "new_type_name": "name",
   "type": "name"
}
```
The ABI now looks like this:

```json
{
   "version": "eosio::abi/1.1",
   "types": [{
     "new_type_name": "name",
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

Structs that are exposed to the ABI also need to be described. By looking at eosio.token.hpp, it can be quickly determined which structs are utilized by public actions. This is particularly important for the next step.

A struct's object definition in JSON looks like the following:

```json
{
   "name": "issue", //The name
   "base": "", 			//Inheritance, parent struct
   "fields": []			//Array of field objects describing the struct's fields.
}
```
## Fields

```json
{
   "name":"", // The field's name
   "type":""   // The field's type
}    
```
In the `eosio.token` contract, there's a number of structs that require definition. Please note, not all of the structs are explicitly defined, some correspond to an actions' parameters. Here's a list of structs that require an ABI description for the `eosio.token` contract:

## Implicit Structs

The following structs are implicit in that a struct was never explicitly defined in the contract. Looking at the [create](https://developers.eos.io/manuals/eosio.contracts/latest/action-reference/eosio.token/index/#create) action, you'll find two parameters, `issuer` of type `name ` and `maximum_supply` of type `asset`. For brevity this tutorial won't break down every struct, but applying the same logic, you will end up with the following:

### [create](https://developers.eos.io/manuals/eosio.contracts/latest/action-reference/eosio.token/index/#create)

```json
{
  "name": "create",
  "base": "",
  "fields": [
    {
      "name":"issuer",
      "type":"name"
    },
    {
      "name":"maximum_supply",
      "type":"asset"
    }
  ]
}
```
### [issue](https://developers.eos.io/manuals/eosio.contracts/latest/action-reference/eosio.token/index/#issue)

```json
{
  "name": "issue",
  "base": "",
  "fields": [
    {
      "name":"to",
      "type":"name"
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
### [retire](https://developers.eos.io/manuals/eosio.contracts/latest/action-reference/eosio.token/index/#retire)

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

### [transfer](https://developers.eos.io/manuals/eosio.contracts/latest/action-reference/eosio.token/index/#transfer)

```json
{
  "name": "transfer",
  "base": "",
  "fields": [
    {
      "name":"from",
      "type":"name"
    },
    {
      "name":"to",
      "type":"name"
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

### [close](https://developers.eos.io/manuals/eosio.contracts/latest/action-reference/eosio.token/index/#close)

```json
{
  "name": "close",
  "base": "",
  "fields": [
    {
      "name":"owner",
      "type":"name"
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

### [account](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L120)

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

## Actions

An action's JSON object definition looks like the following:

```json
{
  "name": "transfer", 			//The name of the action as defined in the contract
  "type": "transfer", 			//The name of the implicit struct as described in the ABI
  "ricardian_contract": "" 	//An optional ricardian clause to associate to this action describing its intended functionality.
}
```
Describe the actions of the `eosio.token` contract by aggregating all the public functions described in the `eosio.token` contract's [header file](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp).

Then describe each action's *type* according to its previously described struct. In most situations, the function name and the struct name will be equal, but are not required to be equal.

Below is a list of actions that link to their source code with example JSON provided for how each action would be described.

##  [create](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L35-L37)

```json
{
  "name": "create",
  "type": "create",
  "ricardian_contract": ""
}
```

## [issue](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L45-L46)

```json
{
  "name": "issue",
  "type": "issue",
  "ricardian_contract": ""
}
```

## [retire](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L55-L56)

```json
{
  "name": "retire",
  "type": "retire",
  "ricardian_contract": ""
}
```

## [transfer](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L67-L71)

```json
{
  "name": "transfer",
  "type": "transfer",
  "ricardian_contract": ""
}
```

## [close](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L96-L97)

```json
{
  "name": "close",
  "type": "close",
  "ricardian_contract": ""
}
```

## Tables

Describe the tables. Here's a table's JSON object definition:

```json
{
  "name": "",       //The name of the table, determined during instantiation.
  "type": "", 			//The table's corresponding struct
  "index_type": "", //The type of primary index of this table
  "key_names" : [], //An array of key names, length must equal length of key_types member
  "key_types" : []  //An array of key types that correspond to key names array member, length of array must equal length of key names array.
}
```

The eosio.token contract instantiates two tables, [accounts](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L134) and [stat](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L135).

The `accounts` table is an i64 index, based on the [`account` struct](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L120-L124), has a [`uint64` as it's primary key](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L123)

Here's how the accounts table would be described in the ABI

```json
{
  "name": "accounts",
  "type": "account", // Corresponds to previously defined struct
  "index_type": "i64",
  "key_names" : ["primary_key"],
  "key_types" : ["uint64"]
}
```

The `stat` table is an i64 index, based on the [`currency_stats` struct](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L126-L132), has a [`uint64` as it's primary key](https://github.com/EOSIO/eosio.contracts/blob/master/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L131)

Here's how the stat table would be described in the ABI

```json
{
  "name": "stat",
  "type": "currency_stats",
  "index_type": "i64",
  "key_names" : ["primary_key"],
  "key_types" : ["uint64"]
}
```
You'll notice the above tables have the same "key name." Naming your keys similar names is symbolic in that it can potentially suggest a subjective relationship. As with this implementation, implying that any given value can be used to query different tables.

## Putting it all Together

Finally, an ABI file that accurately describes the `eosio.token` contract.

```json
{
  "version": "eosio::abi/1.1",
  "types": [
    {
      "new_type_name": "name",
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
          "type":"name"
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
            "type":"name"
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
            "type":"name"
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
          "type":"name"
        },
        {
          "name":"to",
          "type":"name"
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
          "type":"name"
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

## Vectors

When describing a vector in your ABI file, simply append the type with `[]`, so if you need to describe a vector of permission levels, you would describe it like so: `permission_level[]`

## Struct Base

It's a rarely used property worth mentioning. You can use **base** ABI struct property to reference another struct for inheritance, as long as that struct is also described in the same ABI file. Base will do nothing or potentially throw an error if your smart contract logic does not support inheritance.

You can see an example of base in use in the system contract [source code](https://github.com/EOSIO/eosio.contracts/blob/4e4a3ca86d5d3482dfac85182e69f33c49e62fa9/eosio.system/include/eosio.system/eosio.system.hpp#L46) and [ABI](https://github.com/EOSIO/eosio.contracts/blob/4e4a3ca86d5d3482dfac85182e69f33c49e62fa9/eosio.system/abi/eosio.system.abi#L262)

## Extra ABI Properties Not Covered Here

A few properties of the ABI specification were skipped here for brevity, however, there is a pending ABI specification that will outline every property of the ABI in its entirety.

## Ricardian Clauses

Ricardian clauses describe the intended outcome of a particular actions. It may also be utilized to establish terms between the sender and the contract.

## ABI Extensions

A generic "future proofing" layer that allows old clients to skip the parsing of "chunks" of extension data. For now, this property is unused. In the future each extension would have its own "chunk" in that vector so that older clients skip it and newer clients that understand how to interpret it.

## Maintenance

Every time you change a struct, add a table, add an action or add parameters to an action, use a new type, you will need to remember to update your ABI file. In many cases failure to update your ABI file will not produce any error.

## Troubleshooting

### Table returns no rows

Check that your table is accurately described in the ABI file. For example, If you use `cleos` to add a table on a contract with a malformed ABI definition and then get rows from that table, you will receive an empty result. `cleos` will not produce an error when adding a row nor reading a row when a contract has failed to properly describe its tables in its ABI.
