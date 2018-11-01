---
search:
    keywords: ['eosio::multi_index::index::const_iterator', 'index', 'operator==', 'operator!=', 'operator*', 'operator->', 'operator++', 'operator--', 'operator++', 'operator--', 'const_iterator']
---

# struct eosio::multi\_index::index::const\_iterator

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**multi\_index**](classeosio_1_1multi__index.md) **::** [**index**](structeosio_1_1multi__index_1_1index.md) **::** [**const\_iterator**](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)




Inherits the following classes: **std::iterator< std::bidirectional\_iterator\_tag, const T >**

## Friends

|Type|Name|
|-----|-----|
|friend struct|[**index**](structeosio_1_1multi__index_1_1index_1_1const__iterator_a98d9477848d2c563b310010a4f7b8fed.md#1a98d9477848d2c563b310010a4f7b8fed)|
|friend bool|[**operator==**](structeosio_1_1multi__index_1_1index_1_1const__iterator_ab0ac7de651351823aece097eb3be1cfb.md#1ab0ac7de651351823aece097eb3be1cfb)|
|friend bool|[**operator!=**](structeosio_1_1multi__index_1_1index_1_1const__iterator_a5fbca5b71055f2b793ae1607239b13a0.md#1a5fbca5b71055f2b793ae1607239b13a0)|


## Public Functions

|Type|Name|
|-----|-----|
|const T &|[**operator\***](structeosio_1_1multi__index_1_1index_1_1const__iterator_ae717dad91e163ae8a1d04d9043261211.md#1ae717dad91e163ae8a1d04d9043261211) () const |
|const T \*|[**operator->**](structeosio_1_1multi__index_1_1index_1_1const__iterator_ac945b05a32096e86a99718728e944b09.md#1ac945b05a32096e86a99718728e944b09) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**operator++**](structeosio_1_1multi__index_1_1index_1_1const__iterator_aaa017dc5dc3ecac344a590f409371216.md#1aaa017dc5dc3ecac344a590f409371216) (int) |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)**|[**operator--**](structeosio_1_1multi__index_1_1index_1_1const__iterator_a058bad034470139d1e1518d5533122bf.md#1a058bad034470139d1e1518d5533122bf) (int) |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)** &|[**operator++**](structeosio_1_1multi__index_1_1index_1_1const__iterator_ac21740a04210f02f1fad68e03724c932.md#1ac21740a04210f02f1fad68e03724c932) () |
|**[const\_iterator](structeosio_1_1multi__index_1_1index_1_1const__iterator.md)** &|[**operator--**](structeosio_1_1multi__index_1_1index_1_1const__iterator_aec23aecad7c4f2fc8bd3fb4c84de4b4f.md#1aec23aecad7c4f2fc8bd3fb4c84de4b4f) () |
||[**const\_iterator**](structeosio_1_1multi__index_1_1index_1_1const__iterator_a6d0b7e7cb0410b12d4333e5b53eb9ea8.md#1a6d0b7e7cb0410b12d4333e5b53eb9ea8) () |


## Friends Documentation

### friend <a id="1a98d9477848d2c563b310010a4f7b8fed" href="#1a98d9477848d2c563b310010a4f7b8fed">index</a>

```cpp
friend struct index;
```



### friend <a id="1ab0ac7de651351823aece097eb3be1cfb" href="#1ab0ac7de651351823aece097eb3be1cfb">operator==</a>

```cpp
friend bool eosio::multi_index::index::const_iterator::operator== (
    const const_iterator & a,
    const const_iterator & b
)
```



### friend <a id="1a5fbca5b71055f2b793ae1607239b13a0" href="#1a5fbca5b71055f2b793ae1607239b13a0">operator!=</a>

```cpp
friend bool eosio::multi_index::index::const_iterator::operator!= (
    const const_iterator & a,
    const const_iterator & b
)
```



## Public Functions Documentation

### function <a id="1ae717dad91e163ae8a1d04d9043261211" href="#1ae717dad91e163ae8a1d04d9043261211">operator\*</a>

```cpp
const T & eosio::multi_index::index::const_iterator::operator* () const
```



### function <a id="1ac945b05a32096e86a99718728e944b09" href="#1ac945b05a32096e86a99718728e944b09">operator-></a>

```cpp
const T * eosio::multi_index::index::const_iterator::operator-> () const
```



### function <a id="1aaa017dc5dc3ecac344a590f409371216" href="#1aaa017dc5dc3ecac344a590f409371216">operator++ (1/2)</a>

```cpp
const_iterator eosio::multi_index::index::const_iterator::operator++ (
    int 
)
```



### function <a id="1a058bad034470139d1e1518d5533122bf" href="#1a058bad034470139d1e1518d5533122bf">operator-- (1/2)</a>

```cpp
const_iterator eosio::multi_index::index::const_iterator::operator-- (
    int 
)
```



### function <a id="1ac21740a04210f02f1fad68e03724c932" href="#1ac21740a04210f02f1fad68e03724c932">operator++ (2/2)</a>

```cpp
const_iterator & eosio::multi_index::index::const_iterator::operator++ ()
```



### function <a id="1aec23aecad7c4f2fc8bd3fb4c84de4b4f" href="#1aec23aecad7c4f2fc8bd3fb4c84de4b4f">operator-- (2/2)</a>

```cpp
const_iterator & eosio::multi_index::index::const_iterator::operator-- ()
```



### function <a id="1a6d0b7e7cb0410b12d4333e5b53eb9ea8" href="#1a6d0b7e7cb0410b12d4333e5b53eb9ea8">const\_iterator (1/2)</a>

```cpp
eosio::multi_index::index::const_iterator::const_iterator ()
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/multi\_index.hpp`
