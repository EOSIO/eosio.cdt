---
search:
    keywords: ['transaction_c', 'send_deferred', 'cancel_deferred', 'read_transaction', 'transaction_size', 'tapos_block_num', 'tapos_block_prefix', 'expiration', 'get_action', 'get_context_free_data']
---

# group transaction\_c

Defines C API for sending transactions and inline actions. [More...](#detailed-description)
## Detailed Description

Deferred transactions will not be processed until a future block. They can therefore have no effect on the success of failure of their parent transaction so long as they appear well formed. If any other condition causes the parent transaction to be marked as failing, then the deferred transaction will never be processed.
Deferred transactions must adhere to the permissions available to the parent transaction or, in the future, delegated to the contract account for future use.
An inline message allows one contract to send another contract a message which is processed immediately after the current message's processing ends such that the success or failure of the parent transaction is dependent on the success of the message. If an inline message fails in processing then the whole tree of transactions and actions rooted in the block will me marked as failing and none of effects on the database will persist.
Inline actions and Deferred transactions must adhere to the permissions available to the parent transaction or, in the future, delegated to the contract account for future use. 
## Functions Documentation

### function <a id="ga07e6ca6c01d86389ee6a09bbc79d5771" href="#ga07e6ca6c01d86389ee6a09bbc79d5771">send\_deferred</a>

```cpp
void transaction_c::send_deferred (
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
int transaction_c::cancel_deferred (
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
size_t transaction_c::read_transaction (
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
size_t transaction_c::transaction_size ()
```

Gets the size of the currently executing transaction. 

Gets the size of the currently executing transaction.


**Returns:**

size of the currently executing transaction 




### function <a id="gac0b858a766ca73c6415bdb799d6ef45a" href="#gac0b858a766ca73c6415bdb799d6ef45a">tapos\_block\_num</a>

```cpp
int transaction_c::tapos_block_num ()
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
int transaction_c::tapos_block_prefix ()
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
uint32_t transaction_c::expiration ()
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
int transaction_c::get_action (
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
int transaction_c::get_context_free_data (
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




