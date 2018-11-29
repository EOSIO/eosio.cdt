---
search:
    keywords: ['transaction', 'eosio::transaction_header', 'eosio::transaction', 'eosio::onerror', 'expiration', 'ref_block_num', 'ref_block_prefix', 'max_net_usage_words', 'max_cpu_usage_ms', 'delay_sec', 'context_free_actions', 'actions', 'transaction_extensions', 'sender_id', 'sent_trx', 'transaction_header', 'transaction', 'send', 'from_current_action', 'unpack_sent_trx', 'get_action']
---

# group transaction

Type-safe C++ wrappers for transaction C API. [More...](#detailed-description)
## Modules

* [**Transaction C++ API**](group__transactioncppapi.md) Type-safe C++ wrappers for transaction C API. 


## Classes

|Type|Name|
|-----|-----|
|class|[**eosio::transaction\_header**](classeosio_1_1transaction__header.md)|
|class|[**eosio::transaction**](classeosio_1_1transaction.md)|
|struct|[**eosio::onerror**](structeosio_1_1onerror.md)|


## Variables

## Detailed Description



**Note:**

There are some methods from the transactioncapi that can be used directly from C++ 



## Variables Documentation

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



### variable <a id="gae6686bc621795ad87f257d0be72f345c" href="#gae6686bc621795ad87f257d0be72f345c">sender\_id</a>

```cpp
uint128_t eosio::onerror::sender_id;
```



### variable <a id="ga1cae304360f79394762ec5040046887e" href="#ga1cae304360f79394762ec5040046887e">sent\_trx</a>

```cpp
std::vector<char> eosio::onerror::sent_trx;
```



## Functions Documentation

### function <a id="ga27106464af5cbbfd09b223eeb48d1e37" href="#ga27106464af5cbbfd09b223eeb48d1e37">transaction\_header</a>

```cpp
transaction::transaction_header (
    time_point_sec exp = time_point_sec(now()+60)
)
```

Construct a new **[transaction\_header](classeosio_1_1transaction__header.md)** object initialising the transaction header expiration to now + 60 seconds. 

Construct a new **[transaction\_header](classeosio_1_1transaction__header.md)** with an expiration of now + 60 seconds. 

### function <a id="gad090d569f520dce4919de93bcec2e260" href="#gad090d569f520dce4919de93bcec2e260">transaction</a>

```cpp
transaction::transaction (
    time_point_sec exp = time_point_sec(now()+60)
)
```

Construct a new transaction object initialising the transaction header expiration to now + 60 seconds. 

Construct a new transaction with an expiration of now + 60 seconds. 

### function <a id="gaff16436e52fff284fabc0b748be342b4" href="#gaff16436e52fff284fabc0b748be342b4">send</a>

```cpp
void transaction::send (
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



### function <a id="gabdf7bc0ef45beae2daf8cf3f08a7cd42" href="#gabdf7bc0ef45beae2daf8cf3f08a7cd42">from\_current\_action</a>

```cpp
static onerror transaction::from_current_action ()
```

Unpacks and returns a onerror struct. 

from\_current\_action unpacks and returns a onerror struct 

### function <a id="ga8fe8368f7a3213b5ab0a93407f41cd66" href="#ga8fe8368f7a3213b5ab0a93407f41cd66">unpack\_sent\_trx</a>

```cpp
transaction transaction::unpack_sent_trx () const
```

Unpacks and returns a transaction. 

unpack\_sent\_trx unpacks and returns a transaction 

### function <a id="gac01a00482f2dc95bf4850b2e8213ee90" href="#gac01a00482f2dc95bf4850b2e8213ee90">get\_action</a>

```cpp
action transaction::get_action (
    uint32_t type,
    uint32_t index
)
```


Retrieve the indicated action from the active transaction.


**Parameters:**


* **type** - 0 for context free action, 1 for action 
* **index** - the index of the requested action 



**Returns:**

the indicated action 




