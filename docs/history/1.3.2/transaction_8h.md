---
search:
    keywords: ['transaction.h', 'send_deferred', 'cancel_deferred', 'read_transaction', 'transaction_size', 'tapos_block_num', 'tapos_block_prefix', 'expiration', 'get_action', 'get_context_free_data']
---

# file transaction.h

**[Go to the source code of this file.](transaction_8h_source.md)**
|Type|Name|
|-----|-----|
|void|[**send\_deferred**](group__transaction__c_ga07e6ca6c01d86389ee6a09bbc79d5771.md#ga07e6ca6c01d86389ee6a09bbc79d5771) (const uint128\_t & sender\_id, **[capi\_name](group__types_gad9871b90d15df8db5c660c78569f029f.md#gad9871b90d15df8db5c660c78569f029f)** payer, const char \* serialized\_transaction, size\_t size, uint32\_t replace\_existing = 0) |
|int|[**cancel\_deferred**](group__transaction__c_ga03df5ed306c904081f03601350a81b67.md#ga03df5ed306c904081f03601350a81b67) (const uint128\_t & sender\_id) <br>Cancels a deferred transaction. |
|size\_t|[**read\_transaction**](group__transaction__c_ga73fa4253ae0aec55399c26b740223bc6.md#ga73fa4253ae0aec55399c26b740223bc6) (char \* buffer, size\_t size) <br>Access a copy of the currently executing transaction. |
|size\_t|[**transaction\_size**](group__transaction__c_gadfcd2e00f00461eac891ee8433508609.md#gadfcd2e00f00461eac891ee8433508609) () <br>Gets the size of the currently executing transaction. |
|int|[**tapos\_block\_num**](group__transaction__c_gac0b858a766ca73c6415bdb799d6ef45a.md#gac0b858a766ca73c6415bdb799d6ef45a) () <br>Gets the block number used for TAPOS on the currently executing transaction. |
|int|[**tapos\_block\_prefix**](group__transaction__c_gaef5458ccfe8ca5d6b273e35d04c56371.md#gaef5458ccfe8ca5d6b273e35d04c56371) () <br>Gets the block prefix used for TAPOS on the currently executing transaction. |
|uint32\_t|[**expiration**](group__transaction__c_ga3843de6e5838982eb47c3806cfd0739a.md#ga3843de6e5838982eb47c3806cfd0739a) () <br>Gets the expiration of the currently executing transaction. |
|int|[**get\_action**](group__transaction__c_gaf1fe78f3caf24010302e5cac1edad61d.md#gaf1fe78f3caf24010302e5cac1edad61d) (uint32\_t type, uint32\_t index, char \* buff, size\_t size) <br>Retrieves the indicated action from the active transaction. |
|int|[**get\_context\_free\_data**](group__transaction__c_gad0f4f1bd76077b3ee4289aadfb84c2b2.md#gad0f4f1bd76077b3ee4289aadfb84c2b2) (uint32\_t index, char \* buff, size\_t size) <br>Retrieve the signed\_transaction.context\_free\_data[index]. |


## Detailed Description



**Copyright**

defined in eos/LICENSE.txt 



## Functions Documentation

### function <a id="ga07e6ca6c01d86389ee6a09bbc79d5771" href="#ga07e6ca6c01d86389ee6a09bbc79d5771">send\_deferred</a>

```cpp
void transaction.h::send_deferred (
    const uint128_t & sender_id,
    capi_name payer,
    const char * serialized_transaction,
    size_t size,
    uint32_t replace_existing = 0
)
```


Sends a deferred transaction.


**Parameters:**


* **sender\_id** - ID of sender 
* **payer** - Account paying for RAM 
* **serialized\_transaction** - Pointer of serialized transaction to be deferred 
* **size** - Size to reserve 
* **replace\_existing** - f this is `0` then if the provided sender\_id is already in use by an in-flight transaction from this contract, which will be a failing assert. If `1` then transaction will atomically cancel/replace the inflight transaction 



### function <a id="ga03df5ed306c904081f03601350a81b67" href="#ga03df5ed306c904081f03601350a81b67">cancel\_deferred</a>

```cpp
int transaction.h::cancel_deferred (
    const uint128_t & sender_id
)
```

Cancels a deferred transaction. 

Cancels a deferred transaction.


**Parameters:**


* **sender\_id** - The id of the sender



**Precondition:**

The deferred transaction ID exists. 




**Precondition:**

The deferred transaction ID has not yet been published. 




**Post**

Deferred transaction canceled.




**Returns:**

1 if transaction was canceled, 0 if transaction was not found


Example:

```cpp
id = 0xffffffffffffffff
cancel_deferred( id );
```

 

### function <a id="ga73fa4253ae0aec55399c26b740223bc6" href="#ga73fa4253ae0aec55399c26b740223bc6">read\_transaction</a>

```cpp
size_t transaction.h::read_transaction (
    char * buffer,
    size_t size
)
```

Access a copy of the currently executing transaction. 

Access a copy of the currently executing transaction.


**Parameters:**


* **buffer** - a buffer to write the current transaction to 
* **size** - the size of the buffer, 0 to return required size 



**Returns:**

the size of the transaction written to the buffer, or number of bytes that can be copied if size==0 passed 




### function <a id="gadfcd2e00f00461eac891ee8433508609" href="#gadfcd2e00f00461eac891ee8433508609">transaction\_size</a>

```cpp
size_t transaction.h::transaction_size ()
```

Gets the size of the currently executing transaction. 

Gets the size of the currently executing transaction.


**Returns:**

size of the currently executing transaction 




### function <a id="gac0b858a766ca73c6415bdb799d6ef45a" href="#gac0b858a766ca73c6415bdb799d6ef45a">tapos\_block\_num</a>

```cpp
int transaction.h::tapos_block_num ()
```

Gets the block number used for TAPOS on the currently executing transaction. 

Gets the block number used for TAPOS on the currently executing transaction.


**Returns:**

block number used for TAPOS on the currently executing transaction Example: 
```cpp
int tbn = tapos_block_num();
```

 




### function <a id="gaef5458ccfe8ca5d6b273e35d04c56371" href="#gaef5458ccfe8ca5d6b273e35d04c56371">tapos\_block\_prefix</a>

```cpp
int transaction.h::tapos_block_prefix ()
```

Gets the block prefix used for TAPOS on the currently executing transaction. 

Gets the block prefix used for TAPOS on the currently executing transaction.


**Returns:**

block prefix used for TAPOS on the currently executing transaction Example: 
```cpp
int tbp = tapos_block_prefix();
```

 




### function <a id="ga3843de6e5838982eb47c3806cfd0739a" href="#ga3843de6e5838982eb47c3806cfd0739a">expiration</a>

```cpp
uint32_t transaction.h::expiration ()
```

Gets the expiration of the currently executing transaction. 

Gets the expiration of the currently executing transaction.


**Returns:**

expiration of the currently executing transaction in seconds since Unix epoch Example: 
```cpp
uint32_t tm = expiration();
eosio_print(tm);
```

 




### function <a id="gaf1fe78f3caf24010302e5cac1edad61d" href="#gaf1fe78f3caf24010302e5cac1edad61d">get\_action</a>

```cpp
int transaction.h::get_action (
    uint32_t type,
    uint32_t index,
    char * buff,
    size_t size
)
```

Retrieves the indicated action from the active transaction. 

Retrieves the indicated action from the active transaction.


**Parameters:**


* **type** - 0 for context free action, 1 for action 
* **index** - the index of the requested action 
* **buff** - output packed buff of the action 
* **size** - amount of buff read, pass 0 to have size returned 



**Returns:**

the size of the action, -1 on failure 




### function <a id="gad0f4f1bd76077b3ee4289aadfb84c2b2" href="#gad0f4f1bd76077b3ee4289aadfb84c2b2">get\_context\_free\_data</a>

```cpp
int transaction.h::get_context_free_data (
    uint32_t index,
    char * buff,
    size_t size
)
```

Retrieve the signed\_transaction.context\_free\_data[index]. 

Retrieve the signed\_transaction.context\_free\_data[index].


**Parameters:**


* **index** - the index of the context\_free\_data entry to retrieve 
* **buff** - output buff of the context\_free\_data entry 
* **size** - amount of context\_free\_data[index] to retrieve into buff, 0 to report required size 



**Returns:**

size copied, or context\_free\_data[index].size() if 0 passed for size, or -1 if index not valid 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/transaction.h`
