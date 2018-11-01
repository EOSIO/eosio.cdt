---
search:
    keywords: ['eosio::multi_index::item_ptr', '_item', '_primary_key', '_primary_itr', 'item_ptr']
---

# struct eosio::multi\_index::item\_ptr

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**multi\_index**](classeosio_1_1multi__index.md) **::** [**item\_ptr**](structeosio_1_1multi__index_1_1item__ptr.md)


## Public Attributes

|Type|Name|
|-----|-----|
|std::unique\_ptr< item >|[**\_item**](structeosio_1_1multi__index_1_1item__ptr_a76caf0c83e2d01052f6f9f9a1202779a.md#1a76caf0c83e2d01052f6f9f9a1202779a)|
|uint64\_t|[**\_primary\_key**](structeosio_1_1multi__index_1_1item__ptr_a4f0ea92f1a3f1a161339bab5f1684490.md#1a4f0ea92f1a3f1a161339bab5f1684490)|
|int32\_t|[**\_primary\_itr**](structeosio_1_1multi__index_1_1item__ptr_a3de098b77400e8b8c2be71f841f979b1.md#1a3de098b77400e8b8c2be71f841f979b1)|


## Public Functions

|Type|Name|
|-----|-----|
||[**item\_ptr**](structeosio_1_1multi__index_1_1item__ptr_aae582bb6e081ba5fd90afd5874c13f1c.md#1aae582bb6e081ba5fd90afd5874c13f1c) (std::unique\_ptr< item > && i, uint64\_t pk, int32\_t pitr) |


## Public Attributes Documentation

### variable <a id="1a76caf0c83e2d01052f6f9f9a1202779a" href="#1a76caf0c83e2d01052f6f9f9a1202779a">\_item</a>

```cpp
std::unique_ptr<item> eosio::multi_index< TableName, T, Indices >::item_ptr::_item;
```



### variable <a id="1a4f0ea92f1a3f1a161339bab5f1684490" href="#1a4f0ea92f1a3f1a161339bab5f1684490">\_primary\_key</a>

```cpp
uint64_t eosio::multi_index< TableName, T, Indices >::item_ptr::_primary_key;
```



### variable <a id="1a3de098b77400e8b8c2be71f841f979b1" href="#1a3de098b77400e8b8c2be71f841f979b1">\_primary\_itr</a>

```cpp
int32_t eosio::multi_index< TableName, T, Indices >::item_ptr::_primary_itr;
```



## Public Functions Documentation

### function <a id="1aae582bb6e081ba5fd90afd5874c13f1c" href="#1aae582bb6e081ba5fd90afd5874c13f1c">item\_ptr</a>

```cpp
eosio::multi_index::item_ptr::item_ptr (
    std::unique_ptr< item > && i,
    uint64_t pk,
    int32_t pitr
)
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/multi\_index.hpp`
