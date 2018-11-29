---
search:
    keywords: ['eosio::public_key', 'type', 'data', 'operator==', 'operator!=']
---

# struct eosio::public\_key

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**public\_key**](structeosio_1_1public__key.md)


## Public Attributes

|Type|Name|
|-----|-----|
|**[unsigned\_int](structunsigned__int.md)**|[**type**](structeosio_1_1public__key_aaba0301ecc27240508120958503b2361.md#1aaba0301ecc27240508120958503b2361)|
|std::array< char, 33 >|[**data**](structeosio_1_1public__key_af05e926b56c5f221dc502a6f68dbba58.md#1af05e926b56c5f221dc502a6f68dbba58)|


## Friends

|Type|Name|
|-----|-----|
|friend bool|[**operator==**](structeosio_1_1public__key_a4f2610bdec460bf2b0203684a9599da5.md#1a4f2610bdec460bf2b0203684a9599da5)|
|friend bool|[**operator!=**](structeosio_1_1public__key_a5faa639eb95ed12c1be5d8ae65c7cb54.md#1a5faa639eb95ed12c1be5d8ae65c7cb54)|


## Public Attributes Documentation

### variable <a id="1aaba0301ecc27240508120958503b2361" href="#1aaba0301ecc27240508120958503b2361">type</a>

```cpp
unsigned_int eosio::public_key::type;
```


Type of the public key, could be either K1 or R1 

### variable <a id="1af05e926b56c5f221dc502a6f68dbba58" href="#1af05e926b56c5f221dc502a6f68dbba58">data</a>

```cpp
std::array<char,33> eosio::public_key::data;
```


Bytes of the public key 

## Friends Documentation

### friend <a id="1a4f2610bdec460bf2b0203684a9599da5" href="#1a4f2610bdec460bf2b0203684a9599da5">operator==</a>

```cpp
friend bool eosio::public_key::operator== (
    const public_key & a,
    const public_key & b
)
```



### friend <a id="1a5faa639eb95ed12c1be5d8ae65c7cb54" href="#1a5faa639eb95ed12c1be5d8ae65c7cb54">operator!=</a>

```cpp
friend bool eosio::public_key::operator!= (
    const public_key & a,
    const public_key & b
)
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/public\_key.hpp`
