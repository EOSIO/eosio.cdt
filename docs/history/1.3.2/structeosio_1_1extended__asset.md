---
search:
    keywords: ['eosio::extended_asset', 'quantity', 'contract', 'get_extended_symbol', 'extended_asset', 'extended_asset', 'extended_asset', 'print', 'operator-', 'operator-', 'operator+', 'operator+=', 'operator-=', 'operator<', 'operator==', 'operator!=', 'operator<=', 'operator>=']
---

# struct eosio::extended\_asset

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**extended\_asset**](structeosio_1_1extended__asset.md)


## Public Attributes

|Type|Name|
|-----|-----|
|**[asset](structeosio_1_1asset.md)**|[**quantity**](structeosio_1_1extended__asset_aacd83a028c55e22db2e8f14027827ad7.md#1aacd83a028c55e22db2e8f14027827ad7)|
|**[name](structeosio_1_1name.md)**|[**contract**](structeosio_1_1extended__asset_acac503620c8c59864b065360fb50868b.md#1acac503620c8c59864b065360fb50868b)|


## Public Functions

|Type|Name|
|-----|-----|
|**[extended\_symbol](classeosio_1_1extended__symbol.md)**|[**get\_extended\_symbol**](structeosio_1_1extended__asset_a7604109863c6d45bc54347c27bd632a4.md#1a7604109863c6d45bc54347c27bd632a4) () const |
||[**extended\_asset**](structeosio_1_1extended__asset_a7747448ddea804e84fe72decd40ee3a2.md#1a7747448ddea804e84fe72decd40ee3a2) () = default |
||[**extended\_asset**](structeosio_1_1extended__asset_a831a169f25c972af14a01e0305ab8d74.md#1a831a169f25c972af14a01e0305ab8d74) (int64\_t v, **[extended\_symbol](classeosio_1_1extended__symbol.md)** s) |
||[**extended\_asset**](structeosio_1_1extended__asset_a29f3c5f054f754ab4fac481cce633682.md#1a29f3c5f054f754ab4fac481cce633682) (**[asset](structeosio_1_1asset.md)** a, **[name](structeosio_1_1name.md)** c) |
|void|[**print**](structeosio_1_1extended__asset_af5993cb01622505de48870ba728cec3b.md#1af5993cb01622505de48870ba728cec3b) () const |
|**[extended\_asset](structeosio_1_1extended__asset.md)**|[**operator-**](structeosio_1_1extended__asset_a6ed16665f2ef5843b3dea2e2429f7147.md#1a6ed16665f2ef5843b3dea2e2429f7147) () const |


## Friends

|Type|Name|
|-----|-----|
|friend **[extended\_asset](structeosio_1_1extended__asset.md)**|[**operator-**](structeosio_1_1extended__asset_ab4d2266e6f926377337a626e990a5e7c.md#1ab4d2266e6f926377337a626e990a5e7c)<br>Subtraction operator. |
|friend **[extended\_asset](structeosio_1_1extended__asset.md)**|[**operator+**](structeosio_1_1extended__asset_a407c4e55064a39fa993303aa2d428244.md#1a407c4e55064a39fa993303aa2d428244)<br>Addition operator. |
|friend **[extended\_asset](structeosio_1_1extended__asset.md)** &|[**operator+=**](structeosio_1_1extended__asset_a905237f4cfc32d33e8c89223461fa466.md#1a905237f4cfc32d33e8c89223461fa466)<br>Addition operator. |
|friend **[extended\_asset](structeosio_1_1extended__asset.md)** &|[**operator-=**](structeosio_1_1extended__asset_a93c002db9cb9b3137fba4dbe2ee3c87b.md#1a93c002db9cb9b3137fba4dbe2ee3c87b)<br>Subtraction operator. |
|friend bool|[**operator<**](structeosio_1_1extended__asset_a8a4fd918c008063c91c5dc1c47a7379b.md#1a8a4fd918c008063c91c5dc1c47a7379b)<br>Less than operator. |
|friend bool|[**operator==**](structeosio_1_1extended__asset_a3aa0a4107431f276a87ee9d375057f9c.md#1a3aa0a4107431f276a87ee9d375057f9c)<br>Comparison operator. |
|friend bool|[**operator!=**](structeosio_1_1extended__asset_a804dc3c59aaec0a3c6a4b8c516fda329.md#1a804dc3c59aaec0a3c6a4b8c516fda329)<br>Comparison operator. |
|friend bool|[**operator<=**](structeosio_1_1extended__asset_a76fd9f6a03d7be197796bdcf7875c399.md#1a76fd9f6a03d7be197796bdcf7875c399)<br>Comparison operator. |
|friend bool|[**operator>=**](structeosio_1_1extended__asset_a4969e409e5f8d32e48b40ebc4ba4f6af.md#1a4969e409e5f8d32e48b40ebc4ba4f6af)<br>Comparison operator. |


## Public Attributes Documentation

### variable <a id="1aacd83a028c55e22db2e8f14027827ad7" href="#1aacd83a028c55e22db2e8f14027827ad7">quantity</a>

```cpp
asset eosio::extended_asset::quantity;
```


The asset 

### variable <a id="1acac503620c8c59864b065360fb50868b" href="#1acac503620c8c59864b065360fb50868b">contract</a>

```cpp
name eosio::extended_asset::contract;
```


The owner of the asset 

## Public Functions Documentation

### function <a id="1a7604109863c6d45bc54347c27bd632a4" href="#1a7604109863c6d45bc54347c27bd632a4">get\_extended\_symbol</a>

```cpp
extended_symbol eosio::extended_asset::get_extended_symbol () const
```


Get the extended symbol of the asset


**Returns:**

**[extended\_symbol](classeosio_1_1extended__symbol.md)** - The extended symbol of the asset 




### function <a id="1a7747448ddea804e84fe72decd40ee3a2" href="#1a7747448ddea804e84fe72decd40ee3a2">extended\_asset (1/3)</a>

```cpp
eosio::extended_asset::extended_asset () = default
```


Default constructor 

### function <a id="1a831a169f25c972af14a01e0305ab8d74" href="#1a831a169f25c972af14a01e0305ab8d74">extended\_asset (2/3)</a>

```cpp
eosio::extended_asset::extended_asset (
    int64_t v,
    extended_symbol s
)
```


Construct a new extended asset given the amount and extended symbol 

### function <a id="1a29f3c5f054f754ab4fac481cce633682" href="#1a29f3c5f054f754ab4fac481cce633682">extended\_asset (3/3)</a>

```cpp
eosio::extended_asset::extended_asset (
    asset a,
    name c
)
```


Construct a new extended asset given the asset and owner name 

### function <a id="1af5993cb01622505de48870ba728cec3b" href="#1af5993cb01622505de48870ba728cec3b">print</a>

```cpp
void eosio::extended_asset::print () const
```


Print the extended asset 

### function <a id="1a6ed16665f2ef5843b3dea2e2429f7147" href="#1a6ed16665f2ef5843b3dea2e2429f7147">operator- (1/2)</a>

```cpp
extended_asset eosio::extended_asset::operator- () const
```


Unary minus operator


**Returns:**

**[extended\_asset](structeosio_1_1extended__asset.md)** - New extended asset with its amount is the negative amount of this extended asset 




## Friends Documentation

### friend <a id="1ab4d2266e6f926377337a626e990a5e7c" href="#1ab4d2266e6f926377337a626e990a5e7c">operator- (2/2)</a>

```cpp
friend extended_asset eosio::extended_asset::operator- (
    const extended_asset & a,
    const extended_asset & b
)
```

Subtraction operator. 

Subtraction operator. This subtracts the amount of the extended asset. 

**Parameters:**


* **a** - The extended asset to be subtracted 
* **b** - The extended asset used to subtract 



**Returns:**

**[extended\_asset](structeosio_1_1extended__asset.md)** - New extended asset as the result of subtraction 




**Precondition:**

The owner of both extended asset need to be the same 




### friend <a id="1a407c4e55064a39fa993303aa2d428244" href="#1a407c4e55064a39fa993303aa2d428244">operator+</a>

```cpp
friend extended_asset eosio::extended_asset::operator+ (
    const extended_asset & a,
    const extended_asset & b
)
```

Addition operator. 

Addition operator. This adds the amount of the extended asset. 

**Parameters:**


* **a** - The extended asset to be added 
* **b** - The extended asset to be added 



**Returns:**

**[extended\_asset](structeosio_1_1extended__asset.md)** - New extended asset as the result of addition 




**Precondition:**

The owner of both extended asset need to be the same 




### friend <a id="1a905237f4cfc32d33e8c89223461fa466" href="#1a905237f4cfc32d33e8c89223461fa466">operator+=</a>

```cpp
friend extended_asset & eosio::extended_asset::operator+= (
    extended_asset & a,
    const extended_asset & b
)
```

Addition operator. 


### friend <a id="1a93c002db9cb9b3137fba4dbe2ee3c87b" href="#1a93c002db9cb9b3137fba4dbe2ee3c87b">operator-=</a>

```cpp
friend extended_asset & eosio::extended_asset::operator-= (
    extended_asset & a,
    const extended_asset & b
)
```

Subtraction operator. 


### friend <a id="1a8a4fd918c008063c91c5dc1c47a7379b" href="#1a8a4fd918c008063c91c5dc1c47a7379b">operator<</a>

```cpp
friend bool eosio::extended_asset::operator< (
    const extended_asset & a,
    const extended_asset & b
)
```

Less than operator. 


### friend <a id="1a3aa0a4107431f276a87ee9d375057f9c" href="#1a3aa0a4107431f276a87ee9d375057f9c">operator==</a>

```cpp
friend bool eosio::extended_asset::operator== (
    const extended_asset & a,
    const extended_asset & b
)
```

Comparison operator. 


### friend <a id="1a804dc3c59aaec0a3c6a4b8c516fda329" href="#1a804dc3c59aaec0a3c6a4b8c516fda329">operator!=</a>

```cpp
friend bool eosio::extended_asset::operator!= (
    const extended_asset & a,
    const extended_asset & b
)
```

Comparison operator. 


### friend <a id="1a76fd9f6a03d7be197796bdcf7875c399" href="#1a76fd9f6a03d7be197796bdcf7875c399">operator<=</a>

```cpp
friend bool eosio::extended_asset::operator<= (
    const extended_asset & a,
    const extended_asset & b
)
```

Comparison operator. 


### friend <a id="1a4969e409e5f8d32e48b40ebc4ba4f6af" href="#1a4969e409e5f8d32e48b40ebc4ba4f6af">operator>=</a>

```cpp
friend bool eosio::extended_asset::operator>= (
    const extended_asset & a,
    const extended_asset & b
)
```

Comparison operator. 




----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/asset.hpp`
