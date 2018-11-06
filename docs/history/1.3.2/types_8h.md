---
search:
    keywords: ['types.h', 'capi_public_key', 'capi_signature', 'ALIGNED', 'capi_name', 'ALIGNED', 'ALIGNED', 'ALIGNED']
---

# file types.h

**[Go to the source code of this file.](types_8h_source.md)**
## Classes

|Type|Name|
|-----|-----|
|struct|[**capi\_public\_key**](structcapi__public__key.md)|
|struct|[**capi\_signature**](structcapi__signature.md)|


## Defines

|Type|Name|
|-----|-----|
||[**ALIGNED**](group__types_ga9763e79ee25e85ca02c0d05e3aee7135.md#ga9763e79ee25e85ca02c0d05e3aee7135)|



|Type|Name|
|-----|-----|
|typedef uint64\_t|[**capi\_name**](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)|



|Type|Name|
|-----|-----|
|struct|[**ALIGNED**](group__types_ga3918acf677acc581e7b3827b409a94cf.md#ga3918acf677acc581e7b3827b409a94cf) (capi\_checksum256) |
|struct|[**ALIGNED**](group__types_gae367011e784adcda4a7d890b0f4e477d.md#gae367011e784adcda4a7d890b0f4e477d) (capi\_checksum160) |
|struct|[**ALIGNED**](group__types_gacc94b2dd94dabb8a53ac7f7bb3e3ca54.md#gacc94b2dd94dabb8a53ac7f7bb3e3ca54) (capi\_checksum512) |


## Detailed Description



**Copyright**

defined in eos/LICENSE.txt 



## Defines Documentation

### define <a id="ga9763e79ee25e85ca02c0d05e3aee7135" href="#ga9763e79ee25e85ca02c0d05e3aee7135">ALIGNED</a>

```cpp
define ALIGNED;
```


Macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned 

## Typedefs Documentation

### typedef <a id="gad9871b90d15df8db5c660c78569f029f" href="#gad9871b90d15df8db5c660c78569f029f">capi\_name</a>

```cpp
typedef uint64_t capi_name;
```



## Functions Documentation

### function <a id="ga3918acf677acc581e7b3827b409a94cf" href="#ga3918acf677acc581e7b3827b409a94cf">ALIGNED</a>

```cpp
struct types.h::ALIGNED (
    capi_checksum256 
)
```


256-bit hash 

### function <a id="gae367011e784adcda4a7d890b0f4e477d" href="#gae367011e784adcda4a7d890b0f4e477d">ALIGNED</a>

```cpp
struct types.h::ALIGNED (
    capi_checksum160 
)
```


160-bit hash 

### function <a id="gacc94b2dd94dabb8a53ac7f7bb3e3ca54" href="#gacc94b2dd94dabb8a53ac7f7bb3e3ca54">ALIGNED</a>

```cpp
struct types.h::ALIGNED (
    capi_checksum512 
)
```


512-bit hash 



----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/types.h`
