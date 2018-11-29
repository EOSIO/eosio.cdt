---
search:
    keywords: ['eosio::_datastream_detail', 'is_pointer', 'is_primitive']
---

# namespace eosio::\_datastream\_detail

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**\_datastream\_detail**](namespaceeosio_1_1__datastream__detail.md)


|Type|Name|
|-----|-----|
|constexpr bool|[**is\_pointer**](namespaceeosio_1_1__datastream__detail_a756af9772c5c98915bbdacafa7165278.md#1a756af9772c5c98915bbdacafa7165278) () <br>Check if type T is a pointer. |
|constexpr bool|[**is\_primitive**](namespaceeosio_1_1__datastream__detail_ab1b99c65e9a4c3ec9a0077b17466043d.md#1ab1b99c65e9a4c3ec9a0077b17466043d) () <br>Check if type T is a primitive type. |


## Functions Documentation

### function <a id="1a756af9772c5c98915bbdacafa7165278" href="#1a756af9772c5c98915bbdacafa7165278">is\_pointer</a>

```cpp
constexpr bool eosio::_datastream_detail::is_pointer ()
```

Check if type T is a pointer. 

Check if type T is a pointer


**Template parameters:**


* **T** - The type to be checked 



**Returns:**

true if T is a pointer 




**Returns:**

false otherwise 




### function <a id="1ab1b99c65e9a4c3ec9a0077b17466043d" href="#1ab1b99c65e9a4c3ec9a0077b17466043d">is\_primitive</a>

```cpp
constexpr bool eosio::_datastream_detail::is_primitive ()
```

Check if type T is a primitive type. 

Check if type T is a primitive type


**Template parameters:**


* **T** - The type to be checked 



**Returns:**

true if T is a primitive type 




**Returns:**

false otherwise 






----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/datastream.hpp`
