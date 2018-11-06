---
search:
    keywords: ['eosio::singleton', 'row', 'singleton', 'exists', 'get', 'get_or_default', 'get_or_create', 'set', 'remove']
---

# class eosio::singleton

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**singleton**](classeosio_1_1singleton.md)


## Classes

|Type|Name|
|-----|-----|
|struct|[**row**](structeosio_1_1singleton_1_1row.md)|


## Public Functions

|Type|Name|
|-----|-----|
||[**singleton**](classeosio_1_1singleton_aacf881ed97b04496be325d74f6b9d25e.md#1aacf881ed97b04496be325d74f6b9d25e) (**[name](structeosio_1_1name.md)** code, uint64\_t scope) |
|bool|[**exists**](classeosio_1_1singleton_ae6eed2e5197e5b4216450e5940ab9141.md#1ae6eed2e5197e5b4216450e5940ab9141) () |
|T|[**get**](classeosio_1_1singleton_a53b56e4e8ed2fddd61175285afdf44d4.md#1a53b56e4e8ed2fddd61175285afdf44d4) () <br>Get the value stored inside the singleton table. |
|T|[**get\_or\_default**](classeosio_1_1singleton_a6a252d888f4c0b20bb3045a443267951.md#1a6a252d888f4c0b20bb3045a443267951) (const T & def = T()) |
|T|[**get\_or\_create**](classeosio_1_1singleton_a7e632785eac3f1d05e08b34c49cb35dc.md#1a7e632785eac3f1d05e08b34c49cb35dc) (**[name](structeosio_1_1name.md)** bill\_to\_account, const T & def = T()) |
|void|[**set**](classeosio_1_1singleton_aada28dbeaf0961614beff67a38553a3c.md#1aada28dbeaf0961614beff67a38553a3c) (const T & value, **[name](structeosio_1_1name.md)** bill\_to\_account) |
|void|[**remove**](classeosio_1_1singleton_afc70106337f2cec280dfd797a1c7920a.md#1afc70106337f2cec280dfd797a1c7920a) () |


## Detailed Description

This wrapper uses a single table to store named objects various types.


**Template parameters:**


* **SingletonName** - the name of this singleton variable 
* **T** - the type of the singleton 


## Public Functions Documentation

### function <a id="1aacf881ed97b04496be325d74f6b9d25e" href="#1aacf881ed97b04496be325d74f6b9d25e">singleton</a>

```cpp
eosio::singleton::singleton (
    name code,
    uint64_t scope
)
```


Construct a new singleton object given the table's owner and the scope


**Parameters:**


* **code** - The table's owner 
* **scope** - The scope of the table 



### function <a id="1ae6eed2e5197e5b4216450e5940ab9141" href="#1ae6eed2e5197e5b4216450e5940ab9141">exists</a>

```cpp
bool eosio::singleton::exists ()
```


Check if the singleton table exists


**Returns:**

true - if exists 




**Returns:**

false - otherwise 




### function <a id="1a53b56e4e8ed2fddd61175285afdf44d4" href="#1a53b56e4e8ed2fddd61175285afdf44d4">get</a>

```cpp
T eosio::singleton::get ()
```

Get the value stored inside the singleton table. 

Get the value stored inside the singleton table. Will throw an exception if it doesn't exist


**Returns:**

T - The value stored 




### function <a id="1a6a252d888f4c0b20bb3045a443267951" href="#1a6a252d888f4c0b20bb3045a443267951">get\_or\_default</a>

```cpp
T eosio::singleton::get_or_default (
    const T & def = T()
)
```


Get the value stored inside the singleton table. If it doesn't exist, it will return the specified default value


**Parameters:**


* **def** - The default value to be returned in case the data doesn't exist 



**Returns:**

T - The value stored 




### function <a id="1a7e632785eac3f1d05e08b34c49cb35dc" href="#1a7e632785eac3f1d05e08b34c49cb35dc">get\_or\_create</a>

```cpp
T eosio::singleton::get_or_create (
    name bill_to_account,
    const T & def = T()
)
```


Get the value stored inside the singleton table. If it doesn't exist, it will create a new one with the specified default value


**Parameters:**


* **bill\_to\_account** - The account to bill for the newly created data if the data doesn't exist 
* **def** - The default value to be created in case the data doesn't exist 



**Returns:**

T - The value stored 




### function <a id="1aada28dbeaf0961614beff67a38553a3c" href="#1aada28dbeaf0961614beff67a38553a3c">set</a>

```cpp
void eosio::singleton::set (
    const T & value,
    name bill_to_account
)
```


Set new value to the singleton table


**Parameters:**


* **value** - New value to be set 
* **bill\_to\_account** - Account to pay for the new value 



### function <a id="1afc70106337f2cec280dfd797a1c7920a" href="#1afc70106337f2cec280dfd797a1c7920a">remove</a>

```cpp
void eosio::singleton::remove ()
```


Remove the only data inside singleton table 



----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/singleton.hpp`
