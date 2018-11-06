---
search:
    keywords: ['eosio::singleton::row', 'value', 'primary_key']
---

# struct eosio::singleton::row

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**singleton**](classeosio_1_1singleton.md) **::** [**row**](structeosio_1_1singleton_1_1row.md)


## Public Attributes

|Type|Name|
|-----|-----|
|T|[**value**](structeosio_1_1singleton_1_1row_a1453fcd490c4d142635fbb30a01d8d82.md#1a1453fcd490c4d142635fbb30a01d8d82)|


## Public Functions

|Type|Name|
|-----|-----|
|uint64\_t|[**primary\_key**](structeosio_1_1singleton_1_1row_a9849634575cdaa0d994f5bef1f8ee141.md#1a9849634575cdaa0d994f5bef1f8ee141) () const |


## Detailed Description

Structure of data inside the singleton table 
## Public Attributes Documentation

### variable <a id="1a1453fcd490c4d142635fbb30a01d8d82" href="#1a1453fcd490c4d142635fbb30a01d8d82">value</a>

```cpp
T eosio::singleton< SingletonName, T >::row::value;
```


Value to be stored inside the singleton table 

## Public Functions Documentation

### function <a id="1a9849634575cdaa0d994f5bef1f8ee141" href="#1a9849634575cdaa0d994f5bef1f8ee141">primary\_key</a>

```cpp
uint64_t eosio::singleton::row::primary_key () const
```


Get primary key of the data


**Returns:**

uint64\_t - Primary Key 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/singleton.hpp`
