---
search:
    keywords: ['eosio::multi_index::const_iterator', 'multi_index', 'operator==', 'operator!=', 'operator*', 'operator->', 'operator++', 'operator--', 'operator++', 'operator--']
---

# struct eosio::multi\_index::const\_iterator

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**multi\_index**](classeosio_1_1multi__index.md) **::** [**const\_iterator**](structeosio_1_1multi__index_1_1const__iterator.md)




Inherits the following classes: **std::iterator< std::bidirectional\_iterator\_tag, const T >**

## Friends

|Type|Name|
|-----|-----|
|friend class|[**multi\_index**](structeosio_1_1multi__index_1_1const__iterator_a474ad2ee1333aa717a29bb55a9d5fc44.md#1a474ad2ee1333aa717a29bb55a9d5fc44)|
|friend bool|[**operator==**](structeosio_1_1multi__index_1_1const__iterator_ab0ac7de651351823aece097eb3be1cfb.md#1ab0ac7de651351823aece097eb3be1cfb)|
|friend bool|[**operator!=**](structeosio_1_1multi__index_1_1const__iterator_a5fbca5b71055f2b793ae1607239b13a0.md#1a5fbca5b71055f2b793ae1607239b13a0)|


## Public Functions

|Type|Name|
|-----|-----|
|const T &|[**operator\***](structeosio_1_1multi__index_1_1const__iterator_a41b221797d68b605c8811c21249c37f7.md#1a41b221797d68b605c8811c21249c37f7) () const |
|const T \*|[**operator->**](structeosio_1_1multi__index_1_1const__iterator_a24a83ecc4a2288ac35afb785b64dff12.md#1a24a83ecc4a2288ac35afb785b64dff12) () const |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**operator++**](structeosio_1_1multi__index_1_1const__iterator_a751d1e7eedadb5b3033cedc6f8e8078e.md#1a751d1e7eedadb5b3033cedc6f8e8078e) (int) |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)**|[**operator--**](structeosio_1_1multi__index_1_1const__iterator_a3a0faabfa94970b0d94d9b2977c69f43.md#1a3a0faabfa94970b0d94d9b2977c69f43) (int) |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)** &|[**operator++**](structeosio_1_1multi__index_1_1const__iterator_aaec476e83abc7018972986594509a477.md#1aaec476e83abc7018972986594509a477) () |
|**[const\_iterator](structeosio_1_1multi__index_1_1const__iterator.md)** &|[**operator--**](structeosio_1_1multi__index_1_1const__iterator_a3f5b8d0651cef32ef99565be6fc1de23.md#1a3f5b8d0651cef32ef99565be6fc1de23) () |


## Friends Documentation

### friend <a id="1a474ad2ee1333aa717a29bb55a9d5fc44" href="#1a474ad2ee1333aa717a29bb55a9d5fc44">multi\_index</a>

```cpp
friend class multi_index;
```



### friend <a id="1ab0ac7de651351823aece097eb3be1cfb" href="#1ab0ac7de651351823aece097eb3be1cfb">operator==</a>

```cpp
friend bool eosio::multi_index::const_iterator::operator== (
    const const_iterator & a,
    const const_iterator & b
)
```



### friend <a id="1a5fbca5b71055f2b793ae1607239b13a0" href="#1a5fbca5b71055f2b793ae1607239b13a0">operator!=</a>

```cpp
friend bool eosio::multi_index::const_iterator::operator!= (
    const const_iterator & a,
    const const_iterator & b
)
```



## Public Functions Documentation

### function <a id="1a41b221797d68b605c8811c21249c37f7" href="#1a41b221797d68b605c8811c21249c37f7">operator\*</a>

```cpp
const T & eosio::multi_index::const_iterator::operator* () const
```



### function <a id="1a24a83ecc4a2288ac35afb785b64dff12" href="#1a24a83ecc4a2288ac35afb785b64dff12">operator-></a>

```cpp
const T * eosio::multi_index::const_iterator::operator-> () const
```



### function <a id="1a751d1e7eedadb5b3033cedc6f8e8078e" href="#1a751d1e7eedadb5b3033cedc6f8e8078e">operator++ (1/2)</a>

```cpp
const_iterator eosio::multi_index::const_iterator::operator++ (
    int 
)
```



### function <a id="1a3a0faabfa94970b0d94d9b2977c69f43" href="#1a3a0faabfa94970b0d94d9b2977c69f43">operator-- (1/2)</a>

```cpp
const_iterator eosio::multi_index::const_iterator::operator-- (
    int 
)
```



### function <a id="1aaec476e83abc7018972986594509a477" href="#1aaec476e83abc7018972986594509a477">operator++ (2/2)</a>

```cpp
const_iterator & eosio::multi_index::const_iterator::operator++ ()
```



### function <a id="1a3f5b8d0651cef32ef99565be6fc1de23" href="#1a3f5b8d0651cef32ef99565be6fc1de23">operator-- (2/2)</a>

```cpp
const_iterator & eosio::multi_index::const_iterator::operator-- ()
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/multi\_index.hpp`
