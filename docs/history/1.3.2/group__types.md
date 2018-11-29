---
search:
    keywords: ['types', 'eosio::name', 'eosio::producer_key', 'eosio::public_key', 'capi_public_key', 'capi_signature', 'unsigned_int', 'signed_int', 'EOSIO', 'capi_name', 'ALIGNED', 'ALIGNED', 'ALIGNED', 'ALIGNED']
---

# group types

Specifies builtin types, typedefs and aliases. 
## Classes

|Type|Name|
|-----|-----|
|struct|[**eosio::name**](structeosio_1_1name.md)|
|struct|[**eosio::producer\_key**](structeosio_1_1producer__key.md)|
|struct|[**eosio::public\_key**](structeosio_1_1public__key.md)|
|struct|[**capi\_public\_key**](structcapi__public__key.md)|
|struct|[**capi\_signature**](structcapi__signature.md)|
|struct|[**unsigned\_int**](structunsigned__int.md)|
|struct|[**signed\_int**](structsigned__int.md)|
|struct|[**EOSIO**](struct_e_o_s_i_o.md)|


## Defines

## Typedefs Documentation

### typedef <a id="gad9871b90d15df8db5c660c78569f029f" href="#gad9871b90d15df8db5c660c78569f029f">capi\_name</a>

```cpp
typedef uint64_t capi_name;
```



## Functions Documentation

### function <a id="ga3918acf677acc581e7b3827b409a94cf" href="#ga3918acf677acc581e7b3827b409a94cf">ALIGNED</a>

```cpp
struct types::ALIGNED (
    capi_checksum256 
)
```


256-bit hash 

### function <a id="gae367011e784adcda4a7d890b0f4e477d" href="#gae367011e784adcda4a7d890b0f4e477d">ALIGNED</a>

```cpp
struct types::ALIGNED (
    capi_checksum160 
)
```


160-bit hash 

### function <a id="gacc94b2dd94dabb8a53ac7f7bb3e3ca54" href="#gacc94b2dd94dabb8a53ac7f7bb3e3ca54">ALIGNED</a>

```cpp
struct types::ALIGNED (
    capi_checksum512 
)
```


512-bit hash 

## Defines Documentation

### define <a id="ga9763e79ee25e85ca02c0d05e3aee7135" href="#ga9763e79ee25e85ca02c0d05e3aee7135">ALIGNED</a>

```cpp
define ALIGNED;
```


Macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned 

