---
search:
    keywords: ['contract', 'eosio::contract', '_self', '_code', '_ds', 'contract', 'get_self', 'get_code', 'get_datastream']
---

# group contract

Defines contract type which is base class for every **[EOSIO](struct_e_o_s_i_o.md)** contract. 
## Classes

|Type|Name|
|-----|-----|
|class|[**eosio::contract**](classeosio_1_1contract.md)|


## Variables

## Variables Documentation

### variable <a id="ga29c0c98c72ab578a5965d13b00dca0c0" href="#ga29c0c98c72ab578a5965d13b00dca0c0">\_self</a>

```cpp
name eosio::contract::_self;
```


The name of this contract 

### variable <a id="ga7bcf83f433ecb0b305dfd7d217c2dde8" href="#ga7bcf83f433ecb0b305dfd7d217c2dde8">\_code</a>

```cpp
name eosio::contract::_code;
```


The code name of the action this contract is processing. 

### variable <a id="ga66b526b89bc6d35b63b874615f1d27b8" href="#ga66b526b89bc6d35b63b874615f1d27b8">\_ds</a>

```cpp
datastream<const char*> eosio::contract::_ds;
```


The datastream for this contract 

## Functions Documentation

### function <a id="ga592e8ee6dabf85e272cc0a833c3fe453" href="#ga592e8ee6dabf85e272cc0a833c3fe453">contract</a>

```cpp
contract::contract (
    name receiver,
    name code,
    datastream< const char *> ds
)
```


Construct a new contract given the contract name


**Parameters:**


* **receiver** - The name of this contract 
* **code** - The code name of the action this contract is processing. 
* **ds** - The datastream used 



### function <a id="ga7564cad9be4ee5dcaac832511d9a0e05" href="#ga7564cad9be4ee5dcaac832511d9a0e05">get\_self</a>

```cpp
name contract::get_self () const
```


Get this contract name


**Returns:**

name - The name of this contract 




### function <a id="ga4efcd5638d26dad3e5dc075bad152d45" href="#ga4efcd5638d26dad3e5dc075bad152d45">get\_code</a>

```cpp
name contract::get_code () const
```


The code name of the action this contract is processing.


**Returns:**

name - The code name of the action this contract is processing. 




### function <a id="ga3bb1107c2675ef69f3eca8e3793701c7" href="#ga3bb1107c2675ef69f3eca8e3793701c7">get\_datastream</a>

```cpp
datastream< const char * > contract::get_datastream () const
```


Get the datastream for this contract


**Returns:**

datastream<const char\*> - The datastream for this contract 




