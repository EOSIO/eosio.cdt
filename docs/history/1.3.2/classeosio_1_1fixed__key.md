---
search:
    keywords: ['eosio::fixed_key', 'bool_pack', 'word_t', 'arr_itr', 'arr', 'num_words', 'padded_bytes', 'fixed_key', 'fixed_key', 'operator==', 'operator!=', 'operator>', 'operator<']
---

# class eosio::fixed\_key

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**fixed\_key**](classeosio_1_1fixed__key.md)


## Classes

|Type|Name|
|-----|-----|
|struct|[**bool\_pack**](structeosio_1_1fixed__key_1_1bool__pack.md)|


## Public Types

|Type|Name|
|-----|-----|
|typedef uint128\_t|[**word\_t**](classeosio_1_1fixed__key_a9a6e8a685e7bafe688fce78adb6b4c43.md#1a9a6e8a685e7bafe688fce78adb6b4c43)|


## Public Attributes

|Type|Name|
|-----|-----|
||[**arr\_itr**](classeosio_1_1fixed__key_a0f8f02c751a182e95804504f1748cadc.md#1a0f8f02c751a182e95804504f1748cadc)|
|return|[**arr**](classeosio_1_1fixed__key_a3dd16555ceab5388156a62fbca11f7af.md#1a3dd16555ceab5388156a62fbca11f7af)|


## Public Static Functions

|Type|Name|
|-----|-----|
|static constexpr size\_t|[**num\_words**](classeosio_1_1fixed__key_aa948895b18141aa14b1fa20326396d71.md#1aa948895b18141aa14b1fa20326396d71) () |
|static constexpr size\_t|[**padded\_bytes**](classeosio_1_1fixed__key_afa54ad415d104999704b6278f4701e66.md#1afa54ad415d104999704b6278f4701e66) () |


## Public Functions

|Type|Name|
|-----|-----|
|constexpr|[**fixed\_key**](classeosio_1_1fixed__key_a892c6c60af975d98d6ff6a8e3bb7c822.md#1a892c6c60af975d98d6ff6a8e3bb7c822) () |
||[**fixed\_key**](classeosio_1_1fixed__key_a1e21d6bc807df3fe3148e1df7af12472.md#1a1e21d6bc807df3fe3148e1df7af12472) (const std::array< **[word\_t](classeosio_1_1fixed__key_a9a6e8a685e7bafe688fce78adb6b4c43.md#1a9a6e8a685e7bafe688fce78adb6b4c43)**, **[num\_words](classeosio_1_1fixed__key_aa948895b18141aa14b1fa20326396d71.md#1aa948895b18141aa14b1fa20326396d71)**()> & arr) |


## Friends

|Type|Name|
|-----|-----|
|friend bool|[**operator==**](classeosio_1_1fixed__key_a30f70a7e52cc0469c1e48d261fec6f48.md#1a30f70a7e52cc0469c1e48d261fec6f48)|
|friend bool|[**operator!=**](classeosio_1_1fixed__key_ac19901f66d7942ad087edef0daef485d.md#1ac19901f66d7942ad087edef0daef485d)|
|friend bool|[**operator>**](classeosio_1_1fixed__key_a0d80d7fcf875276760867c2be8b66438.md#1a0d80d7fcf875276760867c2be8b66438)|
|friend bool|[**operator<**](classeosio_1_1fixed__key_aed9cc9961cc5c84cdefa193a7baf3f9a.md#1aed9cc9961cc5c84cdefa193a7baf3f9a)|


## Detailed Description

Fixed size key sorted lexicographically for Multi Index Table


**Template parameters:**


* **Size** - Size of the **[fixed\_key](classeosio_1_1fixed__key.md)** object 


## Public Types Documentation

### typedef <a id="1a9a6e8a685e7bafe688fce78adb6b4c43" href="#1a9a6e8a685e7bafe688fce78adb6b4c43">word\_t</a>

```cpp
typedef uint128_t eosio::fixed_key< Size >::word_t;
```



## Public Attributes Documentation

### variable <a id="1a0f8f02c751a182e95804504f1748cadc" href="#1a0f8f02c751a182e95804504f1748cadc">arr\_itr</a>

```cpp
eosio::fixed_key< Size >::arr_itr;
```


Constructor to **[fixed\_key](classeosio_1_1fixed__key.md)** object from std::array of **[num\_words()](classeosio_1_1fixed__key_aa948895b18141aa14b1fa20326396d71.md#1aa948895b18141aa14b1fa20326396d71)** words


**Parameters:**


* **arr** - Source data 



### variable <a id="1a3dd16555ceab5388156a62fbca11f7af" href="#1a3dd16555ceab5388156a62fbca11f7af">arr</a>

```cpp
return eosio::fixed_key< Size >::arr;
```



## Public Static Functions Documentation

### function <a id="1aa948895b18141aa14b1fa20326396d71" href="#1aa948895b18141aa14b1fa20326396d71">num\_words</a>

```cpp
static constexpr size_t eosio::fixed_key::num_words ()
```


Get number of words contained in this **[fixed\_key](classeosio_1_1fixed__key.md)** object. A word is defined to be 16 bytes in size \ 

### function <a id="1afa54ad415d104999704b6278f4701e66" href="#1afa54ad415d104999704b6278f4701e66">padded\_bytes</a>

```cpp
static constexpr size_t eosio::fixed_key::padded_bytes ()
```


Get number of padded bytes contained in this **[fixed\_key](classeosio_1_1fixed__key.md)** object. Padded bytes are the remaining bytes inside the **[fixed\_key](classeosio_1_1fixed__key.md)** object after all the words are allocated \ 

## Public Functions Documentation

### function <a id="1a892c6c60af975d98d6ff6a8e3bb7c822" href="#1a892c6c60af975d98d6ff6a8e3bb7c822">fixed\_key (1/2)</a>

```cpp
constexpr eosio::fixed_key::fixed_key ()
```


constructor to **[fixed\_key](classeosio_1_1fixed__key.md)** object \ 

### function <a id="1a1e21d6bc807df3fe3148e1df7af12472" href="#1a1e21d6bc807df3fe3148e1df7af12472">fixed\_key (2/2)</a>

```cpp
eosio::fixed_key::fixed_key (
    const std::array< word_t, num_words()> & arr
)
```


Constructor to **[fixed\_key](classeosio_1_1fixed__key.md)** object from std::array of **[num\_words()](classeosio_1_1fixed__key_aa948895b18141aa14b1fa20326396d71.md#1aa948895b18141aa14b1fa20326396d71)** words
\ \* 

**Parameters:**


* **arr** data 



## Friends Documentation

### friend <a id="1a30f70a7e52cc0469c1e48d261fec6f48" href="#1a30f70a7e52cc0469c1e48d261fec6f48">operator==</a>

```cpp
friend bool eosio::fixed_key::operator== (
    const fixed_key< Size > & c1,
    const fixed_key< Size > & c2
)
```


Lexicographically compares two **[fixed\_key](classeosio_1_1fixed__key.md)** variables c1 and c2


**Parameters:**


* **c1** - First **[fixed\_key](classeosio_1_1fixed__key.md)** object to compare 
* **c2** - Second **[fixed\_key](classeosio_1_1fixed__key.md)** object to compare 



**Returns:**

if c1 == c2, return true, otherwise false 




### friend <a id="1ac19901f66d7942ad087edef0daef485d" href="#1ac19901f66d7942ad087edef0daef485d">operator!=</a>

```cpp
friend bool eosio::fixed_key::operator!= (
    const fixed_key< Size > & c1,
    const fixed_key< Size > & c2
)
```


Lexicographically compares two **[fixed\_key](classeosio_1_1fixed__key.md)** variables c1 and c2


**Parameters:**


* **c1** - First **[fixed\_key](classeosio_1_1fixed__key.md)** object to compare 
* **c2** - Second **[fixed\_key](classeosio_1_1fixed__key.md)** object to compare 



**Returns:**

if c1 != c2, return true, otherwise false 




### friend <a id="1a0d80d7fcf875276760867c2be8b66438" href="#1a0d80d7fcf875276760867c2be8b66438">operator></a>

```cpp
friend bool eosio::fixed_key::operator> (
    const fixed_key< Size > & c1,
    const fixed_key< Size > & c2
)
```


Lexicographically compares two **[fixed\_key](classeosio_1_1fixed__key.md)** variables c1 and c2


**Parameters:**


* **c1** - First **[fixed\_key](classeosio_1_1fixed__key.md)** object to compare 
* **c2** - Second **[fixed\_key](classeosio_1_1fixed__key.md)** object to compare 



**Returns:**

if c1 > c2, return true, otherwise false 




### friend <a id="1aed9cc9961cc5c84cdefa193a7baf3f9a" href="#1aed9cc9961cc5c84cdefa193a7baf3f9a">operator<</a>

```cpp
friend bool eosio::fixed_key::operator< (
    const fixed_key< Size > & c1,
    const fixed_key< Size > & c2
)
```


Lexicographically compares two **[fixed\_key](classeosio_1_1fixed__key.md)** variables c1 and c2


**Parameters:**


* **c1** - First **[fixed\_key](classeosio_1_1fixed__key.md)** object to compare 
* **c2** - Second **[fixed\_key](classeosio_1_1fixed__key.md)** object to compare 



**Returns:**

if c1 < c2, return true, otherwise false 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/fixed\_key.hpp`
