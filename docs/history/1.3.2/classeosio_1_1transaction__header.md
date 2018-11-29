---
search:
    keywords: ['eosio::transaction_header', 'expiration', 'ref_block_num', 'ref_block_prefix', 'max_net_usage_words', 'max_cpu_usage_ms', 'delay_sec', 'transaction_header']
---

# class eosio::transaction\_header

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**transaction\_header**](classeosio_1_1transaction__header.md)


Contains details about the transaction. [More...](#detailed-description)


Inherited by the following classes: **[eosio::transaction](classeosio_1_1transaction.md)**

## Public Attributes

|Type|Name|
|-----|-----|
|**[time\_point\_sec](classeosio_1_1time__point__sec.md)**|[**expiration**](group__transactioncppapi_ga32a532f21d5a5eeadd892723d9171e05.md#ga32a532f21d5a5eeadd892723d9171e05)|
|uint16\_t|[**ref\_block\_num**](group__transactioncppapi_ga7bd1f446f3a9a212183787d223d89766.md#ga7bd1f446f3a9a212183787d223d89766)|
|uint32\_t|[**ref\_block\_prefix**](group__transactioncppapi_gaed4b528cd8a73ac9fa9cb5b7c429f90c.md#gaed4b528cd8a73ac9fa9cb5b7c429f90c)|
|**[unsigned\_int](structunsigned__int.md)**|[**max\_net\_usage\_words**](group__transactioncppapi_gaad5eeaa9d26473ae8faa0d9c5cf44c0e.md#gaad5eeaa9d26473ae8faa0d9c5cf44c0e)|
|uint8\_t|[**max\_cpu\_usage\_ms**](group__transactioncppapi_ga5fbc5f8eef0093ad65598953156d4359.md#ga5fbc5f8eef0093ad65598953156d4359)<br>number of 8 byte words this transaction can serialize into after compressions |
|**[unsigned\_int](structunsigned__int.md)**|[**delay\_sec**](group__transactioncppapi_gadbe5ee5f38a82db68162188c49d62f39.md#gadbe5ee5f38a82db68162188c49d62f39)<br>number of CPU usage units to bill transaction for |


## Public Functions

|Type|Name|
|-----|-----|
||[**transaction\_header**](group__transactioncppapi_ga27106464af5cbbfd09b223eeb48d1e37.md#ga27106464af5cbbfd09b223eeb48d1e37) (**[time\_point\_sec](classeosio_1_1time__point__sec.md)** exp = **[time\_point\_sec](classeosio_1_1time__point__sec.md)**(now()+60)) <br>Construct a new **[transaction\_header](classeosio_1_1transaction__header.md)** object initialising the transaction header expiration to now + 60 seconds. |


## Detailed Description

Class **[transaction\_header](classeosio_1_1transaction__header.md)** contains details about the transaction 
## Public Attributes Documentation

### variable <a id="ga32a532f21d5a5eeadd892723d9171e05" href="#ga32a532f21d5a5eeadd892723d9171e05">expiration</a>

```cpp
time_point_sec eosio::transaction_header::expiration;
```



### variable <a id="ga7bd1f446f3a9a212183787d223d89766" href="#ga7bd1f446f3a9a212183787d223d89766">ref\_block\_num</a>

```cpp
uint16_t eosio::transaction_header::ref_block_num;
```



### variable <a id="gaed4b528cd8a73ac9fa9cb5b7c429f90c" href="#gaed4b528cd8a73ac9fa9cb5b7c429f90c">ref\_block\_prefix</a>

```cpp
uint32_t eosio::transaction_header::ref_block_prefix;
```



### variable <a id="gaad5eeaa9d26473ae8faa0d9c5cf44c0e" href="#gaad5eeaa9d26473ae8faa0d9c5cf44c0e">max\_net\_usage\_words</a>

```cpp
unsigned_int eosio::transaction_header::max_net_usage_words;
```



### variable <a id="ga5fbc5f8eef0093ad65598953156d4359" href="#ga5fbc5f8eef0093ad65598953156d4359">max\_cpu\_usage\_ms</a>

```cpp
uint8_t eosio::transaction_header::max_cpu_usage_ms;
```

number of 8 byte words this transaction can serialize into after compressions 


### variable <a id="gadbe5ee5f38a82db68162188c49d62f39" href="#gadbe5ee5f38a82db68162188c49d62f39">delay\_sec</a>

```cpp
unsigned_int eosio::transaction_header::delay_sec;
```

number of CPU usage units to bill transaction for 


## Public Functions Documentation

### function <a id="ga27106464af5cbbfd09b223eeb48d1e37" href="#ga27106464af5cbbfd09b223eeb48d1e37">transaction\_header</a>

```cpp
eosio::transaction_header::transaction_header (
    time_point_sec exp = time_point_sec(now()+60)
)
```

Construct a new **[transaction\_header](classeosio_1_1transaction__header.md)** object initialising the transaction header expiration to now + 60 seconds. 

Construct a new **[transaction\_header](classeosio_1_1transaction__header.md)** with an expiration of now + 60 seconds. 



----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/transaction.hpp`
