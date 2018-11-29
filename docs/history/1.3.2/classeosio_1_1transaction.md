---
search:
    keywords: ['eosio::transaction', 'context_free_actions', 'actions', 'transaction_extensions', 'transaction', 'send']
---

# class eosio::transaction

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**transaction**](classeosio_1_1transaction.md)


Contains the actions, context\_free\_actions and extensions type for a transaction. [More...](#detailed-description)


Inherits the following classes: **[eosio::transaction\_header](classeosio_1_1transaction__header.md)**

## Public Attributes

|Type|Name|
|-----|-----|
|std::vector< **[action](structeosio_1_1action.md)** >|[**context\_free\_actions**](group__transactioncppapi_ga38263b68fda9925f2c6472777a04179c.md#ga38263b68fda9925f2c6472777a04179c)|
|std::vector< **[action](structeosio_1_1action.md)** >|[**actions**](group__transactioncppapi_gad53cc19e62742bf205934509c1363e42.md#gad53cc19e62742bf205934509c1363e42)|
|**[extensions\_type](group__transaction_ga19c346b66f7145a5336177ac3eb0849e.md#ga19c346b66f7145a5336177ac3eb0849e)**|[**transaction\_extensions**](group__transactioncppapi_ga445fd87e3465a1ca647c711c1ac0c711.md#ga445fd87e3465a1ca647c711c1ac0c711)|


## Public Functions

|Type|Name|
|-----|-----|
||[**transaction**](group__transactioncppapi_gad090d569f520dce4919de93bcec2e260.md#gad090d569f520dce4919de93bcec2e260) (**[time\_point\_sec](classeosio_1_1time__point__sec.md)** exp = **[time\_point\_sec](classeosio_1_1time__point__sec.md)**(now()+60)) <br>Construct a new transaction object initialising the transaction header expiration to now + 60 seconds. |
|void|[**send**](group__transactioncppapi_gaff16436e52fff284fabc0b748be342b4.md#gaff16436e52fff284fabc0b748be342b4) (const uint128\_t & sender\_id, **[name](structeosio_1_1name.md)** payer, bool replace\_existing = false) const <br>Writes the **[symbol\_code](classeosio_1_1symbol__code.md)** as a string to the provided char buffer. |


## Detailed Description

Class transaction contains the actions, context\_free\_actions and extensions type for a transaction 
## Public Attributes Documentation

### variable <a id="ga38263b68fda9925f2c6472777a04179c" href="#ga38263b68fda9925f2c6472777a04179c">context\_free\_actions</a>

```cpp
std::vector<action> eosio::transaction::context_free_actions;
```



### variable <a id="gad53cc19e62742bf205934509c1363e42" href="#gad53cc19e62742bf205934509c1363e42">actions</a>

```cpp
std::vector<action> eosio::transaction::actions;
```



### variable <a id="ga445fd87e3465a1ca647c711c1ac0c711" href="#ga445fd87e3465a1ca647c711c1ac0c711">transaction\_extensions</a>

```cpp
extensions_type eosio::transaction::transaction_extensions;
```



## Public Functions Documentation

### function <a id="gad090d569f520dce4919de93bcec2e260" href="#gad090d569f520dce4919de93bcec2e260">transaction</a>

```cpp
eosio::transaction::transaction (
    time_point_sec exp = time_point_sec(now()+60)
)
```

Construct a new transaction object initialising the transaction header expiration to now + 60 seconds. 

Construct a new transaction with an expiration of now + 60 seconds. 

### function <a id="gaff16436e52fff284fabc0b748be342b4" href="#gaff16436e52fff284fabc0b748be342b4">send</a>

```cpp
void eosio::transaction::send (
    const uint128_t & sender_id,
    name payer,
    bool replace_existing = false
) const
```

Writes the **[symbol\_code](classeosio_1_1symbol__code.md)** as a string to the provided char buffer. 

Sends this transaction, packs the transaction then sends it as a deferred transaction


**Parameters:**


* **sender\_id** - ID of sender 
* **payer** - Account paying for RAM 
* **replace\_existing** - Defaults to false, if this is `0`/false then if the provided sender\_id is already in use by an in-flight transaction from this contract, which will be a failing assert. If `1` then transaction will atomically cancel/replace the inflight transaction 





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/transaction.hpp`
