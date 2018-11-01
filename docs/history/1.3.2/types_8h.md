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
||[**ALIGNED**](types_8h_a9763e79ee25e85ca02c0d05e3aee7135.md#1a9763e79ee25e85ca02c0d05e3aee7135)<br>Macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned. |


## Typedefs

|Type|Name|
|-----|-----|
|typedef uint64\_t|[**capi\_name**](types_8h_ad9871b90d15df8db5c660c78569f029f.md#1ad9871b90d15df8db5c660c78569f029f)|


## Functions

|Type|Name|
|-----|-----|
|struct|[**ALIGNED**](types_8h_a3918acf677acc581e7b3827b409a94cf.md#1a3918acf677acc581e7b3827b409a94cf) (capi\_checksum256) <br>256-bit hash |
|struct|[**ALIGNED**](types_8h_ae367011e784adcda4a7d890b0f4e477d.md#1ae367011e784adcda4a7d890b0f4e477d) (capi\_checksum160) <br>160-bit hash |
|struct|[**ALIGNED**](types_8h_acc94b2dd94dabb8a53ac7f7bb3e3ca54.md#1acc94b2dd94dabb8a53ac7f7bb3e3ca54) (capi\_checksum512) <br>512-bit hash |


## Detailed Description



**Copyright**

defined in eos/LICENSE.txt 



## Defines Documentation

### define <a id="1a9763e79ee25e85ca02c0d05e3aee7135" href="#1a9763e79ee25e85ca02c0d05e3aee7135">ALIGNED</a>

```cpp
define ALIGNED;
```

Macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned. 

Macro to align/overalign a type to ensure calls to intrinsics with pointers/references are properly aligned 

## Typedefs Documentation

### typedef <a id="1ad9871b90d15df8db5c660c78569f029f" href="#1ad9871b90d15df8db5c660c78569f029f">capi\_name</a>

```cpp
typedef uint64_t capi_name;
```



## Functions Documentation

### function <a id="1a3918acf677acc581e7b3827b409a94cf" href="#1a3918acf677acc581e7b3827b409a94cf">ALIGNED</a>

```cpp
struct types.h::ALIGNED (
    capi_checksum256 
)
```

256-bit hash 

256-bit hash 

### function <a id="1ae367011e784adcda4a7d890b0f4e477d" href="#1ae367011e784adcda4a7d890b0f4e477d">ALIGNED</a>

```cpp
struct types.h::ALIGNED (
    capi_checksum160 
)
```

160-bit hash 

160-bit hash 

### function <a id="1acc94b2dd94dabb8a53ac7f7bb3e3ca54" href="#1acc94b2dd94dabb8a53ac7f7bb3e3ca54">ALIGNED</a>

```cpp
struct types.h::ALIGNED (
    capi_checksum512 
)
```

512-bit hash 

512-bit hash 



----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/types.h`
