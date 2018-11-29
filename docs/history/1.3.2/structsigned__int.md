---
search:
    keywords: ['signed_int', 'value', 'signed_int', 'operator int32_t', 'operator=', 'operator++', 'operator++', 'operator==', 'operator==', 'operator==', 'operator!=', 'operator!=', 'operator!=', 'operator<', 'operator<', 'operator<', 'operator>=', 'operator>=', 'operator>=', 'operator<<', 'operator>>']
---

# struct signed\_int

[**Class List**](annotated.md) **>** [**signed\_int**](structsigned__int.md)


## Public Attributes

|Type|Name|
|-----|-----|
|int32\_t|[**value**](structsigned__int_aa943a3c55006fb2cc4e88afd6edc1755.md#1aa943a3c55006fb2cc4e88afd6edc1755)|


## Public Functions

|Type|Name|
|-----|-----|
||[**signed\_int**](structsigned__int_aeb8e4505fb74ac10c901ef905ff82d2e.md#1aeb8e4505fb74ac10c901ef905ff82d2e) (int32\_t v = 0) |
||[**operator int32\_t**](structsigned__int_afe5db7c1297bb56cc454fe413959c07c.md#1afe5db7c1297bb56cc454fe413959c07c) () const |
|**[signed\_int](structsigned__int.md)** &|[**operator=**](structsigned__int_a94d683e2056158e266f64ac0cd60ecaf.md#1a94d683e2056158e266f64ac0cd60ecaf) (const T & v) |
|**[signed\_int](structsigned__int.md)**|[**operator++**](structsigned__int_a21a8dab0e714a1bac2cbd91810ac6a77.md#1a21a8dab0e714a1bac2cbd91810ac6a77) (int) |
|**[signed\_int](structsigned__int.md)** &|[**operator++**](structsigned__int_a364a6dedb3372b0c318d0954ba0909d3.md#1a364a6dedb3372b0c318d0954ba0909d3) () |


## Friends

|Type|Name|
|-----|-----|
|friend bool|[**operator==**](structsigned__int_a7e5690c7ad2505e2297e0e536a3ad740.md#1a7e5690c7ad2505e2297e0e536a3ad740)|
|friend bool|[**operator==**](structsigned__int_aec106017391627bd0e273592c18a0eb5.md#1aec106017391627bd0e273592c18a0eb5)|
|friend bool|[**operator==**](structsigned__int_a5612a3dbcbfd3d1fa335aa402c29ac35.md#1a5612a3dbcbfd3d1fa335aa402c29ac35)|
|friend bool|[**operator!=**](structsigned__int_a3ee2f31815a95ec9ffd2ab7a7f02d524.md#1a3ee2f31815a95ec9ffd2ab7a7f02d524)|
|friend bool|[**operator!=**](structsigned__int_a6abfcdc55171168e5679ccd9f37ba471.md#1a6abfcdc55171168e5679ccd9f37ba471)|
|friend bool|[**operator!=**](structsigned__int_a6e25feb014836d0288a262e5d488533c.md#1a6e25feb014836d0288a262e5d488533c)|
|friend bool|[**operator<**](structsigned__int_adbae5ebcac1cb6ed6a7547a427c7cf95.md#1adbae5ebcac1cb6ed6a7547a427c7cf95)|
|friend bool|[**operator<**](structsigned__int_ae0ebf886eb4f8bd58f41626bfc2d88b7.md#1ae0ebf886eb4f8bd58f41626bfc2d88b7)|
|friend bool|[**operator<**](structsigned__int_ac2eea5a817b5774997100c0cdb8c81d2.md#1ac2eea5a817b5774997100c0cdb8c81d2)|
|friend bool|[**operator>=**](structsigned__int_a0b7eb30fe9fdbc1f375309719ad8248c.md#1a0b7eb30fe9fdbc1f375309719ad8248c)|
|friend bool|[**operator>=**](structsigned__int_accb4bcf54dbdec11cdcbd06167d677c9.md#1accb4bcf54dbdec11cdcbd06167d677c9)|
|friend bool|[**operator>=**](structsigned__int_ab249950f493a3623cf845633e07df502.md#1ab249950f493a3623cf845633e07df502)|
|friend DataStream &|[**operator<<**](structsigned__int_afabc024d2020505d135d9efbcf9e16a8.md#1afabc024d2020505d135d9efbcf9e16a8)|
|friend DataStream &|[**operator>>**](structsigned__int_a7eef08b0d7662636e1b0fa0f90bc8ccd.md#1a7eef08b0d7662636e1b0fa0f90bc8ccd)|


## Detailed Description

Variable Length Signed Integer. This provides more efficient serialization of 32-bit signed int. It serializes a 32-bit signed integer in as few bytes as possible.


**Note:**

`varint32' is signed and uses [Zig-Zag encoding](https://developers.google.com/protocol-buffers/docs/encoding#signed-integers) 



## Public Attributes Documentation

### variable <a id="1aa943a3c55006fb2cc4e88afd6edc1755" href="#1aa943a3c55006fb2cc4e88afd6edc1755">value</a>

```cpp
int32_t signed_int::value;
```


Contained value 

## Public Functions Documentation

### function <a id="1aeb8e4505fb74ac10c901ef905ff82d2e" href="#1aeb8e4505fb74ac10c901ef905ff82d2e">signed\_int</a>

```cpp
signed_int::signed_int (
    int32_t v = 0
)
```


Construct a new signed int object


**Parameters:**


* **v** - Source 



### function <a id="1afe5db7c1297bb56cc454fe413959c07c" href="#1afe5db7c1297bb56cc454fe413959c07c">operator int32\_t</a>

```cpp
signed_int::operator int32_t () const
```


Convert **[signed\_int](structsigned__int.md)** to primitive 32-bit signed integer


**Returns:**

int32\_t - The converted result 




### function <a id="1a94d683e2056158e266f64ac0cd60ecaf" href="#1a94d683e2056158e266f64ac0cd60ecaf">operator=</a>

```cpp
signed_int & signed_int::operator= (
    const T & v
)
```


Assign an object that is convertible to int32\_t


**Template parameters:**


* **T** - Type of the assignment object 



**Parameters:**


* **v** - Source 



**Returns:**

**[unsigned\_int](structunsigned__int.md)**& - Reference to this object 




### function <a id="1a21a8dab0e714a1bac2cbd91810ac6a77" href="#1a21a8dab0e714a1bac2cbd91810ac6a77">operator++ (1/2)</a>

```cpp
signed_int signed_int::operator++ (
    int 
)
```


Increment operator


**Returns:**

**[signed\_int](structsigned__int.md)** - New **[signed\_int](structsigned__int.md)** with value incremented from the current object's value 




### function <a id="1a364a6dedb3372b0c318d0954ba0909d3" href="#1a364a6dedb3372b0c318d0954ba0909d3">operator++ (2/2)</a>

```cpp
signed_int & signed_int::operator++ ()
```


Increment operator


**Returns:**

**[signed\_int](structsigned__int.md)** - Reference to current object 




## Friends Documentation

### friend <a id="1a7e5690c7ad2505e2297e0e536a3ad740" href="#1a7e5690c7ad2505e2297e0e536a3ad740">operator== (1/3)</a>

```cpp
friend bool signed_int::operator== (
    const signed_int & i,
    const int32_t & v
)
```


Check equality between a **[signed\_int](structsigned__int.md)** object and 32-bit integer


**Parameters:**


* **i** - **[signed\_int](structsigned__int.md)** object to compare 
* **v** - 32-bit integer to compare 



**Returns:**

true - if equal 




**Returns:**

false - otherwise 




### friend <a id="1aec106017391627bd0e273592c18a0eb5" href="#1aec106017391627bd0e273592c18a0eb5">operator== (2/3)</a>

```cpp
friend bool signed_int::operator== (
    const int32_t & i,
    const signed_int & v
)
```


Check equality between 32-bit integer and a **[signed\_int](structsigned__int.md)** object


**Parameters:**


* **i** - 32-bit integer to compare 
* **v** - **[signed\_int](structsigned__int.md)** object to compare 



**Returns:**

true - if equal 




**Returns:**

false - otherwise 




### friend <a id="1a5612a3dbcbfd3d1fa335aa402c29ac35" href="#1a5612a3dbcbfd3d1fa335aa402c29ac35">operator== (3/3)</a>

```cpp
friend bool signed_int::operator== (
    const signed_int & i,
    const signed_int & v
)
```


Check equality between two **[signed\_int](structsigned__int.md)** objects


**Parameters:**


* **i** - First **[signed\_int](structsigned__int.md)** object to compare 
* **v** - Second **[signed\_int](structsigned__int.md)** object to compare 



**Returns:**

true - if equal 




**Returns:**

false - otherwise 




### friend <a id="1a3ee2f31815a95ec9ffd2ab7a7f02d524" href="#1a3ee2f31815a95ec9ffd2ab7a7f02d524">operator!= (1/3)</a>

```cpp
friend bool signed_int::operator!= (
    const signed_int & i,
    const int32_t & v
)
```


Check inequality between a **[signed\_int](structsigned__int.md)** object and 32-bit integer


**Parameters:**


* **i** - **[signed\_int](structsigned__int.md)** object to compare 
* **v** - 32-bit integer to compare 



**Returns:**

true - if inequal 




**Returns:**

false - otherwise 




### friend <a id="1a6abfcdc55171168e5679ccd9f37ba471" href="#1a6abfcdc55171168e5679ccd9f37ba471">operator!= (2/3)</a>

```cpp
friend bool signed_int::operator!= (
    const int32_t & i,
    const signed_int & v
)
```


Check inequality between 32-bit integer and a **[signed\_int](structsigned__int.md)** object


**Parameters:**


* **i** - 32-bit integer to compare 
* **v** - **[signed\_int](structsigned__int.md)** object to compare 



**Returns:**

true - if unequal 




**Returns:**

false - otherwise 




### friend <a id="1a6e25feb014836d0288a262e5d488533c" href="#1a6e25feb014836d0288a262e5d488533c">operator!= (3/3)</a>

```cpp
friend bool signed_int::operator!= (
    const signed_int & i,
    const signed_int & v
)
```


Check inequality between two **[signed\_int](structsigned__int.md)** objects


**Parameters:**


* **i** - First **[signed\_int](structsigned__int.md)** object to compare 
* **v** - Second **[signed\_int](structsigned__int.md)** object to compare 



**Returns:**

true - if inequal 




**Returns:**

false - otherwise 




### friend <a id="1adbae5ebcac1cb6ed6a7547a427c7cf95" href="#1adbae5ebcac1cb6ed6a7547a427c7cf95">operator< (1/3)</a>

```cpp
friend bool signed_int::operator< (
    const signed_int & i,
    const int32_t & v
)
```


Check if the given **[signed\_int](structsigned__int.md)** object is less than the given 32-bit integer


**Parameters:**


* **i** - **[signed\_int](structsigned__int.md)** object to compare 
* **v** - 32-bit integer to compare 



**Returns:**

true - if i less than v 




**Returns:**

false - otherwise 




### friend <a id="1ae0ebf886eb4f8bd58f41626bfc2d88b7" href="#1ae0ebf886eb4f8bd58f41626bfc2d88b7">operator< (2/3)</a>

```cpp
friend bool signed_int::operator< (
    const int32_t & i,
    const signed_int & v
)
```


Check if the given 32-bit integer is less than the given **[signed\_int](structsigned__int.md)** object


**Parameters:**


* **i** - 32-bit integer to compare 
* **v** - **[signed\_int](structsigned__int.md)** object to compare 



**Returns:**

true - if i less than v 




**Returns:**

false - otherwise 




### friend <a id="1ac2eea5a817b5774997100c0cdb8c81d2" href="#1ac2eea5a817b5774997100c0cdb8c81d2">operator< (3/3)</a>

```cpp
friend bool signed_int::operator< (
    const signed_int & i,
    const signed_int & v
)
```


Check if the first given **[signed\_int](structsigned__int.md)** is less than the second given **[signed\_int](structsigned__int.md)** object


**Parameters:**


* **i** - First **[signed\_int](structsigned__int.md)** object to compare 
* **v** - Second **[signed\_int](structsigned__int.md)** object to compare 



**Returns:**

true - if i less than v 




**Returns:**

false - otherwise 




### friend <a id="1a0b7eb30fe9fdbc1f375309719ad8248c" href="#1a0b7eb30fe9fdbc1f375309719ad8248c">operator>= (1/3)</a>

```cpp
friend bool signed_int::operator>= (
    const signed_int & i,
    const int32_t & v
)
```


Check if the given **[signed\_int](structsigned__int.md)** object is greater or equal to the given 32-bit integer


**Parameters:**


* **i** - **[signed\_int](structsigned__int.md)** object to compare 
* **v** - 32-bit integer to compare 



**Returns:**

true - if i is greater or equal to v 




**Returns:**

false - otherwise 




### friend <a id="1accb4bcf54dbdec11cdcbd06167d677c9" href="#1accb4bcf54dbdec11cdcbd06167d677c9">operator>= (2/3)</a>

```cpp
friend bool signed_int::operator>= (
    const int32_t & i,
    const signed_int & v
)
```


Check if the given 32-bit integer is greater or equal to the given **[signed\_int](structsigned__int.md)** object


**Parameters:**


* **i** - 32-bit integer to compare 
* **v** - **[signed\_int](structsigned__int.md)** object to compare 



**Returns:**

true - if i is greater or equal to v 




**Returns:**

false - otherwise 




### friend <a id="1ab249950f493a3623cf845633e07df502" href="#1ab249950f493a3623cf845633e07df502">operator>= (3/3)</a>

```cpp
friend bool signed_int::operator>= (
    const signed_int & i,
    const signed_int & v
)
```


Check if the first given **[signed\_int](structsigned__int.md)** is greater or equal to the second given **[signed\_int](structsigned__int.md)** object


**Parameters:**


* **i** - First **[signed\_int](structsigned__int.md)** object to compare 
* **v** - Second **[signed\_int](structsigned__int.md)** object to compare 



**Returns:**

true - if i is greater or equal to v 




**Returns:**

false - otherwise 




### friend <a id="1afabc024d2020505d135d9efbcf9e16a8" href="#1afabc024d2020505d135d9efbcf9e16a8">operator<<</a>

```cpp
friend DataStream & signed_int::operator<< (
    DataStream & ds,
    const signed_int & v
)
```


Serialize an **[signed\_int](structsigned__int.md)** object with as few bytes as possible


**Parameters:**


* **ds** - The stream to write 
* **v** - The value to serialize 



**Template parameters:**


* **DataStream** - Type of datastream 



**Returns:**

DataStream& - Reference to the datastream 




### friend <a id="1a7eef08b0d7662636e1b0fa0f90bc8ccd" href="#1a7eef08b0d7662636e1b0fa0f90bc8ccd">operator>></a>

```cpp
friend DataStream & signed_int::operator>> (
    DataStream & ds,
    signed_int & vi
)
```


Deserialize an **[signed\_int](structsigned__int.md)** object


**Parameters:**


* **ds** - The stream to read 
* **vi** - The destination for deserialized value 



**Template parameters:**


* **DataStream** - Type of datastream 



**Returns:**

DataStream& - Reference to the datastream 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/varint.hpp`
