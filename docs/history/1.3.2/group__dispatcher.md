---
search:
    keywords: ['dispatcher', 'execute_action']
---

# group dispatcher

Defines C++ functions to dispatch action to proper action handler inside a contract. [More...](#detailed-description)
## Detailed Description

Convenient macro to create contract apply handler


**Note:**

To be able to use this macro, the contract needs to be derived from **[eosio::contract](classeosio_1_1contract.md)** 




**Parameters:**


* **TYPE** - The class name of the contract 
* **MEMBERS** - The sequence of available actions supported by this contract

Example: 
```cpp
EOSIO_DISPATCH( eosio::bios, (setpriv)(setalimits)(setglimits)(setprods)(reqauth) )
```

 
## Functions Documentation

### function <a id="ga8c4928c29096799ef6ddabf148dc9cf9" href="#ga8c4928c29096799ef6ddabf148dc9cf9">execute\_action</a>

```cpp
bool dispatcher::execute_action (
    name self,
    name code,
    void(T::*)(Args...) func
)
```


Unpack the received action and execute the correponding action handler


**Template parameters:**


* **T** - The contract class that has the correponding action handler, this contract should be derived from **[eosio::contract](classeosio_1_1contract.md)** 
* **Q** - The namespace of the action handler function 
* **Args** - The arguments that the action handler accepts, i.e. members of the action 



**Parameters:**


* **obj** - The contract object that has the correponding action handler 
* **func** - The action handler 



**Returns:**

true 




