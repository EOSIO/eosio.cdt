---
search:
    keywords: ['eosio::permission_level', 'actor', 'permission', 'permission_level', 'permission_level', 'operator==']
---

# struct eosio::permission\_level

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**permission\_level**](structeosio_1_1permission__level.md)


Packed representation of a permission level (Authorization) [More...](#detailed-description)
## Public Attributes

|Type|Name|
|-----|-----|
|**[name](structeosio_1_1name.md)**|[**actor**](structeosio_1_1permission__level_ab5d07611f5ba5e7b5dff9e749c5696e0.md#1ab5d07611f5ba5e7b5dff9e749c5696e0)<br>Name of the account who owns this permission. |
|**[name](structeosio_1_1name.md)**|[**permission**](structeosio_1_1permission__level_af30baa92f0b3d6b611967c0dfb7555a7.md#1af30baa92f0b3d6b611967c0dfb7555a7)<br>Name of the permission. |


## Public Functions

|Type|Name|
|-----|-----|
||[**permission\_level**](structeosio_1_1permission__level_a5b23258308433be05a03ab4e2fce9b36.md#1a5b23258308433be05a03ab4e2fce9b36) (**[name](structeosio_1_1name.md)** a, **[name](structeosio_1_1name.md)** p) <br>Construct a new permission level object. |
||[**permission\_level**](structeosio_1_1permission__level_a80e38737c6e727a27e9ff7a195f74d5d.md#1a80e38737c6e727a27e9ff7a195f74d5d) () <br>Construct a new permission level object. |


## Friends

|Type|Name|
|-----|-----|
|friend constexpr bool|[**operator==**](structeosio_1_1permission__level_a40e360b0bfdb5aa413812d5cb171ff51.md#1a40e360b0bfdb5aa413812d5cb171ff51)<br>Check equality of two permissions. |


## Detailed Description

Packed representation of a permission level (Authorization) 
## Public Attributes Documentation

### variable <a id="1ab5d07611f5ba5e7b5dff9e749c5696e0" href="#1ab5d07611f5ba5e7b5dff9e749c5696e0">actor</a>

```cpp
name eosio::permission_level::actor;
```

Name of the account who owns this permission. 

Name of the account who owns this permission 

### variable <a id="1af30baa92f0b3d6b611967c0dfb7555a7" href="#1af30baa92f0b3d6b611967c0dfb7555a7">permission</a>

```cpp
name eosio::permission_level::permission;
```

Name of the permission. 

Name of the permission 

## Public Functions Documentation

### function <a id="1a5b23258308433be05a03ab4e2fce9b36" href="#1a5b23258308433be05a03ab4e2fce9b36">permission\_level (1/2)</a>

```cpp
eosio::permission_level::permission_level (
    name a,
    name p
)
```

Construct a new permission level object. 

Construct a new permission level object with actor name and permission name


**Parameters:**


* **a** - Name of the account who owns this authorization 
* **p** - Name of the permission 



### function <a id="1a80e38737c6e727a27e9ff7a195f74d5d" href="#1a80e38737c6e727a27e9ff7a195f74d5d">permission\_level (2/2)</a>

```cpp
eosio::permission_level::permission_level ()
```

Construct a new permission level object. 

Default Constructor 

## Friends Documentation

### friend <a id="1a40e360b0bfdb5aa413812d5cb171ff51" href="#1a40e360b0bfdb5aa413812d5cb171ff51">operator==</a>

```cpp
friend constexpr bool eosio::permission_level::operator== (
    const permission_level & a,
    const permission_level & b
)
```

Check equality of two permissions. 

Check equality of two permissions


**Parameters:**


* **a** - first permission to compare 
* **b** - second permission to compare 



**Returns:**

true if equal 




**Returns:**

false if unequal 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/action.hpp`
