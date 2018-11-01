---
search:
    keywords: ['eosio::const_mem_fun', 'result_type', 'operator()', 'operator()', 'operator()', 'operator()']
---

# struct eosio::const\_mem\_fun

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**const\_mem\_fun**](structeosio_1_1const__mem__fun.md)


## Public Types

|Type|Name|
|-----|-----|
|typedef std::remove\_reference< Type >::type|[**result\_type**](structeosio_1_1const__mem__fun_a12f7a54a640c4224bccfbbe401ffb16f.md#1a12f7a54a640c4224bccfbbe401ffb16f)|


## Public Functions

|Type|Name|
|-----|-----|
|auto|[**operator()**](structeosio_1_1const__mem__fun_a3c026ddc053c6c87d0c466830cc691c9.md#1a3c026ddc053c6c87d0c466830cc691c9) (const ChainedPtr & x) const |
|Type|[**operator()**](structeosio_1_1const__mem__fun_a04df970f69c75d7fc9d4949c3c0b0d19.md#1a04df970f69c75d7fc9d4949c3c0b0d19) (const Class & x) const |
|Type|[**operator()**](structeosio_1_1const__mem__fun_a35e521958c3954d0eacac56ce37153d7.md#1a35e521958c3954d0eacac56ce37153d7) (const std::reference\_wrapper< const Class > & x) const |
|Type|[**operator()**](structeosio_1_1const__mem__fun_a8191e327113ef08c101ae22d495315ae.md#1a8191e327113ef08c101ae22d495315ae) (const std::reference\_wrapper< Class > & x) const |


## Public Types Documentation

### typedef <a id="1a12f7a54a640c4224bccfbbe401ffb16f" href="#1a12f7a54a640c4224bccfbbe401ffb16f">result\_type</a>

```cpp
typedef std::remove_reference<Type>::type eosio::const_mem_fun< Class, Type, PtrToMemberFunction >::result_type;
```



## Public Functions Documentation

### function <a id="1a3c026ddc053c6c87d0c466830cc691c9" href="#1a3c026ddc053c6c87d0c466830cc691c9">operator() (1/4)</a>

```cpp
auto eosio::const_mem_fun::operator() (
    const ChainedPtr & x
) const
```



### function <a id="1a04df970f69c75d7fc9d4949c3c0b0d19" href="#1a04df970f69c75d7fc9d4949c3c0b0d19">operator() (2/4)</a>

```cpp
Type eosio::const_mem_fun::operator() (
    const Class & x
) const
```



### function <a id="1a35e521958c3954d0eacac56ce37153d7" href="#1a35e521958c3954d0eacac56ce37153d7">operator() (3/4)</a>

```cpp
Type eosio::const_mem_fun::operator() (
    const std::reference_wrapper< const Class > & x
) const
```



### function <a id="1a8191e327113ef08c101ae22d495315ae" href="#1a8191e327113ef08c101ae22d495315ae">operator() (4/4)</a>

```cpp
Type eosio::const_mem_fun::operator() (
    const std::reference_wrapper< Class > & x
) const
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/multi\_index.hpp`
