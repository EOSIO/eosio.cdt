---
search:
    keywords: ['crypto.h', 'assert_sha256', 'assert_sha1', 'assert_sha512', 'assert_ripemd160', 'sha256', 'sha1', 'sha512', 'ripemd160', 'recover_key', 'assert_recover_key']
---

# file crypto.h

**[Go to the source code of this file.](crypto_8h_source.md)**
|Type|Name|
|-----|-----|
|void|[**assert\_sha256**](group__crypto_ga72b4fe70a6c503bf37ccf7285e03d4bb.md#ga72b4fe70a6c503bf37ccf7285e03d4bb) (const char \* data, uint32\_t length, const capi\_checksum256 \* hash) |
|void|[**assert\_sha1**](group__crypto_gab870a63902f83fcf0c8346206e0e1f4c.md#gab870a63902f83fcf0c8346206e0e1f4c) (const char \* data, uint32\_t length, const capi\_checksum160 \* hash) |
|void|[**assert\_sha512**](group__crypto_ga11c2f4f3e6b2e0a61d0b40f0e9f29d14.md#ga11c2f4f3e6b2e0a61d0b40f0e9f29d14) (const char \* data, uint32\_t length, const capi\_checksum512 \* hash) |
|void|[**assert\_ripemd160**](group__crypto_ga5fa71c353dbb47f9f2068e33a1a79d71.md#ga5fa71c353dbb47f9f2068e33a1a79d71) (const char \* data, uint32\_t length, const capi\_checksum160 \* hash) |
|void|[**sha256**](group__crypto_ga7e93f6228e21884c348ad6589050b9eb.md#ga7e93f6228e21884c348ad6589050b9eb) (const char \* data, uint32\_t length, capi\_checksum256 \* hash) |
|void|[**sha1**](group__crypto_ga382d8329db6727bd460e81c72bdc926a.md#ga382d8329db6727bd460e81c72bdc926a) (const char \* data, uint32\_t length, capi\_checksum160 \* hash) |
|void|[**sha512**](group__crypto_gace935d4721e179120b86cbfdc86590b9.md#gace935d4721e179120b86cbfdc86590b9) (const char \* data, uint32\_t length, capi\_checksum512 \* hash) |
|void|[**ripemd160**](group__crypto_ga156b5b599dfafe44c64b2e923e0ad114.md#ga156b5b599dfafe44c64b2e923e0ad114) (const char \* data, uint32\_t length, capi\_checksum160 \* hash) |
|int|[**recover\_key**](group__crypto_gaa9f54e1ba406bdb5a509c8fe91d214c2.md#gaa9f54e1ba406bdb5a509c8fe91d214c2) (const capi\_checksum256 \* digest, const char \* sig, size\_t siglen, char \* pub, size\_t publen) |
|void|[**assert\_recover\_key**](group__crypto_gacc12ec8a9ab4b0135a4fd743a6d9b4e4.md#gacc12ec8a9ab4b0135a4fd743a6d9b4e4) (const capi\_checksum256 \* digest, const char \* sig, size\_t siglen, const char \* pub, size\_t publen) |


## Detailed Description



**Copyright**

defined in eos/LICENSE.txt 



## Functions Documentation

### function <a id="ga72b4fe70a6c503bf37ccf7285e03d4bb" href="#ga72b4fe70a6c503bf37ccf7285e03d4bb">assert\_sha256</a>

```cpp
void crypto.h::assert_sha256 (
    const char * data,
    uint32_t length,
    const capi_checksum256 * hash
)
```


Tests if the sha256 hash generated from data matches the provided checksum.


**Parameters:**


* **data** - Data you want to hash 
* **length** - Data length 
* **hash** - `capi\_checksum256\*` hash to compare to



**Precondition:**

**assert256 hash** of `data` equals provided `hash` parameter. 




**Post**

Executes next statement. If was not `true`, hard return.




**Note:**

This method is optimized to a NO-OP when in fast evaluation mode.


Example:

```cpp
checksum hash;
char data;
uint32_t length;
assert_sha256( data, length, hash )
//If the sha256 hash generated from data does not equal provided hash, anything below will never fire.
eosio::print("sha256 hash generated from data equals provided hash");
```

 

### function <a id="gab870a63902f83fcf0c8346206e0e1f4c" href="#gab870a63902f83fcf0c8346206e0e1f4c">assert\_sha1</a>

```cpp
void crypto.h::assert_sha1 (
    const char * data,
    uint32_t length,
    const capi_checksum160 * hash
)
```


Tests if the sha1 hash generated from data matches the provided checksum.


**Note:**

This method is optimized to a NO-OP when in fast evaluation mode. 




**Parameters:**


* **data** - Data you want to hash 
* **length** - Data length 
* **hash** - `capi\_checksum160\*` hash to compare to



**Precondition:**

**sha1 hash** of `data` equals provided `hash` parameter. 




**Post**

Executes next statement. If was not `true`, hard return.


Example:

```cpp
checksum hash;
char data;
uint32_t length;
assert_sha1( data, length, hash )
//If the sha1 hash generated from data does not equal provided hash, anything below will never fire.
eosio::print("sha1 hash generated from data equals provided hash");
```

 

### function <a id="ga11c2f4f3e6b2e0a61d0b40f0e9f29d14" href="#ga11c2f4f3e6b2e0a61d0b40f0e9f29d14">assert\_sha512</a>

```cpp
void crypto.h::assert_sha512 (
    const char * data,
    uint32_t length,
    const capi_checksum512 * hash
)
```


Tests if the sha512 hash generated from data matches the provided checksum.


**Note:**

This method is optimized to a NO-OP when in fast evaluation mode. 




**Parameters:**


* **data** - Data you want to hash 
* **length** - Data length 
* **hash** - `capi\_checksum512\*` hash to compare to



**Precondition:**

**assert512 hash** of `data` equals provided `hash` parameter. 




**Post**

Executes next statement. If was not `true`, hard return.


Example:

```cpp
checksum hash;
char data;
uint32_t length;
assert_sha512( data, length, hash )
//If the sha512 hash generated from data does not equal provided hash, anything below will never fire.
eosio::print("sha512 hash generated from data equals provided hash");
```

 

### function <a id="ga5fa71c353dbb47f9f2068e33a1a79d71" href="#ga5fa71c353dbb47f9f2068e33a1a79d71">assert\_ripemd160</a>

```cpp
void crypto.h::assert_ripemd160 (
    const char * data,
    uint32_t length,
    const capi_checksum160 * hash
)
```


Tests if the ripemod160 hash generated from data matches the provided checksum.


**Parameters:**


* **data** - Data you want to hash 
* **length** - Data length 
* **hash** - `capi\_checksum160\*` hash to compare to



**Precondition:**

**assert160 hash** of `data` equals provided `hash` parameter. 




**Post**

Executes next statement. If was not `true`, hard return.


Example:

```cpp
checksum hash;
char data;
uint32_t length;
assert_ripemod160( data, length, hash )
//If the ripemod160 hash generated from data does not equal provided hash, anything below will never fire.
eosio::print("ripemod160 hash generated from data equals provided hash");
```

 

### function <a id="ga7e93f6228e21884c348ad6589050b9eb" href="#ga7e93f6228e21884c348ad6589050b9eb">sha256</a>

```cpp
void crypto.h::sha256 (
    const char * data,
    uint32_t length,
    capi_checksum256 * hash
)
```


Hashes `data` using `sha256` and stores result in memory pointed to by hash.


**Parameters:**


* **data** - Data you want to hash 
* **length** - Data length 
* **hash** - Hash pointer

Example:

```cpp
checksum calc_hash;
sha256( data, length, &calc_hash );
eos_assert( calc_hash == hash, "invalid hash" );
```

 

### function <a id="ga382d8329db6727bd460e81c72bdc926a" href="#ga382d8329db6727bd460e81c72bdc926a">sha1</a>

```cpp
void crypto.h::sha1 (
    const char * data,
    uint32_t length,
    capi_checksum160 * hash
)
```


Hashes `data` using `sha1` and stores result in memory pointed to by hash.


**Parameters:**


* **data** - Data you want to hash 
* **length** - Data length 
* **hash** - Hash pointer

Example:

```cpp
checksum calc_hash;
sha1( data, length, &calc_hash );
eos_assert( calc_hash == hash, "invalid hash" );
```

 

### function <a id="gace935d4721e179120b86cbfdc86590b9" href="#gace935d4721e179120b86cbfdc86590b9">sha512</a>

```cpp
void crypto.h::sha512 (
    const char * data,
    uint32_t length,
    capi_checksum512 * hash
)
```


Hashes `data` using `sha512` and stores result in memory pointed to by hash.


**Parameters:**


* **data** - Data you want to hash 
* **length** - Data length 
* **hash** - Hash pointer

Example:

```cpp
checksum calc_hash;
sha512( data, length, &calc_hash );
eos_assert( calc_hash == hash, "invalid hash" );
```

 

### function <a id="ga156b5b599dfafe44c64b2e923e0ad114" href="#ga156b5b599dfafe44c64b2e923e0ad114">ripemd160</a>

```cpp
void crypto.h::ripemd160 (
    const char * data,
    uint32_t length,
    capi_checksum160 * hash
)
```


Hashes `data` using `ripemod160` and stores result in memory pointed to by hash.


**Parameters:**


* **data** - Data you want to hash 
* **length** - Data length 
* **hash** - Hash pointer

Example:

```cpp
checksum calc_hash;
ripemod160( data, length, &calc_hash );
eos_assert( calc_hash == hash, "invalid hash" );
```

 

### function <a id="gaa9f54e1ba406bdb5a509c8fe91d214c2" href="#gaa9f54e1ba406bdb5a509c8fe91d214c2">recover\_key</a>

```cpp
int crypto.h::recover_key (
    const capi_checksum256 * digest,
    const char * sig,
    size_t siglen,
    char * pub,
    size_t publen
)
```


Calculates the public key used for a given signature and hash used to create a message.


**Parameters:**


* **digest** - Hash used to create a message 
* **sig** - Signature 
* **siglen** - Signature length 
* **pub** - Public key 
* **publen** - Public key length

Example:

```cpp
```

 

### function <a id="gacc12ec8a9ab4b0135a4fd743a6d9b4e4" href="#gacc12ec8a9ab4b0135a4fd743a6d9b4e4">assert\_recover\_key</a>

```cpp
void crypto.h::assert_recover_key (
    const capi_checksum256 * digest,
    const char * sig,
    size_t siglen,
    const char * pub,
    size_t publen
)
```


Tests a given public key with the generated key from digest and the signature.


**Parameters:**


* **digest** - What the key will be generated from 
* **sig** - Signature 
* **siglen** - Signature length 
* **pub** - Public key 
* **publen** - Public key length



**Precondition:**

**assert recovery key** of `pub` equals the key generated from the `digest` parameter 




**Post**

Executes next statement. If was not `true`, hard return.


Example:

```cpp
checksum digest;
char sig;
size_t siglen;
char pub;
size_t publen;
assert_recover_key( digest, sig, siglen, pub, publen )
// If the given public key does not match with the generated key from digest and the signature, anything below will never fire.
eosio::print("pub key matches the pub key generated from digest");
```

 



----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/crypto.h`
