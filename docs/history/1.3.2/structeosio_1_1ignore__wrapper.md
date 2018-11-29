---
search:
    keywords: ['eosio::ignore_wrapper', 'value', 'ignore_wrapper', 'ignore_wrapper', 'ignore_wrapper', 'get', 'operator T', 'operator ignore< T >']
---

# struct eosio::ignore\_wrapper

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**ignore\_wrapper**](structeosio_1_1ignore__wrapper.md)


## Public Attributes

|Type|Name|
|-----|-----|
|T|[**value**](structeosio_1_1ignore__wrapper_a4f84e678095bec6a25ce72256b4cbfce.md#1a4f84e678095bec6a25ce72256b4cbfce)|


## Public Functions

|Type|Name|
|-----|-----|
|constexpr|[**ignore\_wrapper**](structeosio_1_1ignore__wrapper_a89db1ce457bcd345d9c0403ad6c4936f.md#1a89db1ce457bcd345d9c0403ad6c4936f) () |
|constexpr|[**ignore\_wrapper**](structeosio_1_1ignore__wrapper_ac6cf7a6be9c0cc5013c68c1e39a0bab8.md#1ac6cf7a6be9c0cc5013c68c1e39a0bab8) (T val) |
|constexpr|[**ignore\_wrapper**](structeosio_1_1ignore__wrapper_a0dcb6bbda0a915c87264dc0119fc9ded.md#1a0dcb6bbda0a915c87264dc0119fc9ded) (**[ignore](structeosio_1_1ignore.md)**< T > val) |
|constexpr T|[**get**](structeosio_1_1ignore__wrapper_adcb05b331265687383a2324c8e47c428.md#1adcb05b331265687383a2324c8e47c428) () |
|constexpr|[**operator T**](structeosio_1_1ignore__wrapper_ac438b2240f6513b7ccac4d7c6876bd95.md#1ac438b2240f6513b7ccac4d7c6876bd95) () |
|constexpr|[**operator ignore< T >**](structeosio_1_1ignore__wrapper_a79d94e050efb9c7b3cd6a01fe1c8350b.md#1a79d94e050efb9c7b3cd6a01fe1c8350b) () |


## Detailed Description

Wrapper class to allow sending inline actions with the correct payload 
## Public Attributes Documentation

### variable <a id="1a4f84e678095bec6a25ce72256b4cbfce" href="#1a4f84e678095bec6a25ce72256b4cbfce">value</a>

```cpp
T eosio::ignore_wrapper< T >::value;
```



## Public Functions Documentation

### function <a id="1a89db1ce457bcd345d9c0403ad6c4936f" href="#1a89db1ce457bcd345d9c0403ad6c4936f">ignore\_wrapper (1/3)</a>

```cpp
constexpr eosio::ignore_wrapper::ignore_wrapper ()
```



### function <a id="1ac6cf7a6be9c0cc5013c68c1e39a0bab8" href="#1ac6cf7a6be9c0cc5013c68c1e39a0bab8">ignore\_wrapper (2/3)</a>

```cpp
constexpr eosio::ignore_wrapper::ignore_wrapper (
    T val
)
```



### function <a id="1a0dcb6bbda0a915c87264dc0119fc9ded" href="#1a0dcb6bbda0a915c87264dc0119fc9ded">ignore\_wrapper (3/3)</a>

```cpp
constexpr eosio::ignore_wrapper::ignore_wrapper (
    ignore< T > val
)
```



### function <a id="1adcb05b331265687383a2324c8e47c428" href="#1adcb05b331265687383a2324c8e47c428">get</a>

```cpp
constexpr T eosio::ignore_wrapper::get ()
```



### function <a id="1ac438b2240f6513b7ccac4d7c6876bd95" href="#1ac438b2240f6513b7ccac4d7c6876bd95">operator T</a>

```cpp
constexpr eosio::ignore_wrapper::operator T ()
```



### function <a id="1a79d94e050efb9c7b3cd6a01fe1c8350b" href="#1a79d94e050efb9c7b3cd6a01fe1c8350b">operator ignore< T ></a>

```cpp
constexpr eosio::ignore_wrapper::operator ignore< T > ()
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/ignore.hpp`
