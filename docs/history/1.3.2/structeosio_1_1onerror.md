---
search:
    keywords: ['eosio::onerror', 'sender_id', 'sent_trx', 'from_current_action', 'unpack_sent_trx']
---

# struct eosio::onerror

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**onerror**](structeosio_1_1onerror.md)


Contains and sender id and packed transaction. [More...](#detailed-description)
## Public Attributes

|Type|Name|
|-----|-----|
|uint128\_t|[**sender\_id**](group__transactioncppapi_gae6686bc621795ad87f257d0be72f345c.md#gae6686bc621795ad87f257d0be72f345c)|
|std::vector< char >|[**sent\_trx**](group__transactioncppapi_ga1cae304360f79394762ec5040046887e.md#ga1cae304360f79394762ec5040046887e)|


## Public Static Functions

|Type|Name|
|-----|-----|
|static **[onerror](structeosio_1_1onerror.md)**|[**from\_current\_action**](group__transactioncppapi_gabdf7bc0ef45beae2daf8cf3f08a7cd42.md#gabdf7bc0ef45beae2daf8cf3f08a7cd42) () <br>Unpacks and returns a onerror struct. |


## Public Functions

|Type|Name|
|-----|-----|
|**[transaction](classeosio_1_1transaction.md)**|[**unpack\_sent\_trx**](group__transactioncppapi_ga8fe8368f7a3213b5ab0a93407f41cd66.md#ga8fe8368f7a3213b5ab0a93407f41cd66) () const <br>Unpacks and returns a transaction. |


## Detailed Description

Struct onerror contains and sender id and packed transaction 
## Public Attributes Documentation

### variable <a id="gae6686bc621795ad87f257d0be72f345c" href="#gae6686bc621795ad87f257d0be72f345c">sender\_id</a>

```cpp
uint128_t eosio::onerror::sender_id;
```



### variable <a id="ga1cae304360f79394762ec5040046887e" href="#ga1cae304360f79394762ec5040046887e">sent\_trx</a>

```cpp
std::vector<char> eosio::onerror::sent_trx;
```



## Public Static Functions Documentation

### function <a id="gabdf7bc0ef45beae2daf8cf3f08a7cd42" href="#gabdf7bc0ef45beae2daf8cf3f08a7cd42">from\_current\_action</a>

```cpp
static onerror eosio::onerror::from_current_action ()
```

Unpacks and returns a onerror struct. 

from\_current\_action unpacks and returns a onerror struct 

## Public Functions Documentation

### function <a id="ga8fe8368f7a3213b5ab0a93407f41cd66" href="#ga8fe8368f7a3213b5ab0a93407f41cd66">unpack\_sent\_trx</a>

```cpp
transaction eosio::onerror::unpack_sent_trx () const
```

Unpacks and returns a transaction. 

unpack\_sent\_trx unpacks and returns a transaction 



----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/transaction.hpp`
