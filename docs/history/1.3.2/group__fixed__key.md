---
search:
    keywords: ['fixed_key', 'eosio::fixed_key', 'key256', 'operator==', 'operator!=', 'operator>', 'operator<']
---

# group fixed\_key

Fixed size key sorted lexicographically for Multi Index Table. 
## Classes

|Type|Name|
|-----|-----|
|class|[**eosio::fixed\_key**](classeosio_1_1fixed__key.md)|


## Typedefs Documentation

### typedef <a id="ga9ae210ffc1e7d1e4f1514431b26aa3d1" href="#ga9ae210ffc1e7d1e4f1514431b26aa3d1">key256</a>

```cpp
typedef fixed_key<32> eosio::key256;
```



## Functions Documentation

### function <a id="gabbac43a956f81fd4bd4379ede0f82dd7" href="#gabbac43a956f81fd4bd4379ede0f82dd7">operator==</a>

```cpp
bool fixed_key::operator== (
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




### function <a id="ga27b953728fd2a29e49f84e35135a70de" href="#ga27b953728fd2a29e49f84e35135a70de">operator!=</a>

```cpp
bool fixed_key::operator!= (
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




### function <a id="gada6d080f1501ee085aabf5c7736461ca" href="#gada6d080f1501ee085aabf5c7736461ca">operator></a>

```cpp
bool fixed_key::operator> (
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




### function <a id="ga0d8567b962caa06cf0994134725e3f8d" href="#ga0d8567b962caa06cf0994134725e3f8d">operator<</a>

```cpp
bool fixed_key::operator< (
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




