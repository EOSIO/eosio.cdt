---
search:
    keywords: ['eosio::multi_index::item', '__idx', '__primary_itr', '__iters', 'item']
---

# struct eosio::multi\_index::item

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**multi\_index**](classeosio_1_1multi__index.md) **::** [**item**](structeosio_1_1multi__index_1_1item.md)




Inherits the following classes: **T**

## Public Attributes

|Type|Name|
|-----|-----|
|const **[multi\_index](classeosio_1_1multi__index.md)** \*|[**\_\_idx**](structeosio_1_1multi__index_1_1item_a99cf43d99bdedf53df84ae32799d2d09.md#1a99cf43d99bdedf53df84ae32799d2d09)|
|int32\_t|[**\_\_primary\_itr**](structeosio_1_1multi__index_1_1item_a6105bc1647981c2c79aed428a8ec02e1.md#1a6105bc1647981c2c79aed428a8ec02e1)|
|int32\_t|[**\_\_iters**](structeosio_1_1multi__index_1_1item_a07b7d499080ec3df9ee3087fb9002b43.md#1a07b7d499080ec3df9ee3087fb9002b43)|


## Public Functions

|Type|Name|
|-----|-----|
||[**item**](structeosio_1_1multi__index_1_1item_a9834dde25b2bf497571b4ffffb7cc682.md#1a9834dde25b2bf497571b4ffffb7cc682) (const **[multi\_index](classeosio_1_1multi__index.md)** \* idx, Constructor && c) |


## Public Attributes Documentation

### variable <a id="1a99cf43d99bdedf53df84ae32799d2d09" href="#1a99cf43d99bdedf53df84ae32799d2d09">\_\_idx</a>

```cpp
const multi_index* eosio::multi_index< TableName, T, Indices >::item::__idx;
```



### variable <a id="1a6105bc1647981c2c79aed428a8ec02e1" href="#1a6105bc1647981c2c79aed428a8ec02e1">\_\_primary\_itr</a>

```cpp
int32_t eosio::multi_index< TableName, T, Indices >::item::__primary_itr;
```



### variable <a id="1a07b7d499080ec3df9ee3087fb9002b43" href="#1a07b7d499080ec3df9ee3087fb9002b43">\_\_iters</a>

```cpp
int32_t eosio::multi_index< TableName, T, Indices >::item::__iters[sizeof...(Indices)+(sizeof...(Indices)==0)];
```



## Public Functions Documentation

### function <a id="1a9834dde25b2bf497571b4ffffb7cc682" href="#1a9834dde25b2bf497571b4ffffb7cc682">item</a>

```cpp
eosio::multi_index::item::item (
    const multi_index * idx,
    Constructor && c
)
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/multi\_index.hpp`
