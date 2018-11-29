---
search:
    keywords: ['privileged', 'eosio::blockchain_parameters', 'set_blockchain_parameters', 'get_blockchain_parameters']
---

# group privileged

## Classes

|Type|Name|
|-----|-----|
|struct|[**eosio::blockchain\_parameters**](structeosio_1_1blockchain__parameters.md)|


## Detailed Description

Defines C++ Privileged API 
## Functions Documentation

### function <a id="ga4b6f34ade1f06f6583b4a17738bebe0c" href="#ga4b6f34ade1f06f6583b4a17738bebe0c">set\_blockchain\_parameters</a>

```cpp
void privileged::set_blockchain_parameters (
    const eosio::blockchain_parameters & params
)
```

Set the blockchain parameters Set the blockchain parameters. 



**Parameters:**


* **params** - New blockchain parameters to set 



### function <a id="ga673da670d201ba73461eedd8bd1aec8f" href="#ga673da670d201ba73461eedd8bd1aec8f">get\_blockchain\_parameters</a>

```cpp
void privileged::get_blockchain_parameters (
    eosio::blockchain_parameters & params
)
```

Retrieve the blolckchain parameters Retrieve the blolckchain parameters. 



**Parameters:**


* **params** - It will be replaced with the retrieved blockchain params 



