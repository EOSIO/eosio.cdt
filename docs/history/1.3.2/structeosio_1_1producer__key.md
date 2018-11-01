---
search:
    keywords: ['eosio::producer_key', 'producer_name', 'block_signing_key', 'operator<']
---

# struct eosio::producer\_key

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**producer\_key**](structeosio_1_1producer__key.md)


Maps producer with its signing key. [More...](#detailed-description)
## Public Attributes

|Type|Name|
|-----|-----|
|**[name](structeosio_1_1name.md)**|[**producer\_name**](structeosio_1_1producer__key_abec477141e08f7648fbdb5d7c2e69824.md#1abec477141e08f7648fbdb5d7c2e69824)<br>Name of the producer. |
|**[public\_key](structeosio_1_1public__key.md)**|[**block\_signing\_key**](structeosio_1_1producer__key_af5899a46ea38009f20643792af38d26b.md#1af5899a46ea38009f20643792af38d26b)<br>Block signing key used by this producer. |


## Friends

|Type|Name|
|-----|-----|
|friend constexpr bool|[**operator<**](structeosio_1_1producer__key_a90ba7a85612a19a42fb7c598cd0d3406.md#1a90ba7a85612a19a42fb7c598cd0d3406)|


## Detailed Description

Maps producer with its signing key, used for producer schedule 
## Public Attributes Documentation

### variable <a id="1abec477141e08f7648fbdb5d7c2e69824" href="#1abec477141e08f7648fbdb5d7c2e69824">producer\_name</a>

```cpp
name eosio::producer_key::producer_name;
```

Name of the producer. 

Name of the producer 

### variable <a id="1af5899a46ea38009f20643792af38d26b" href="#1af5899a46ea38009f20643792af38d26b">block\_signing\_key</a>

```cpp
public_key eosio::producer_key::block_signing_key;
```

Block signing key used by this producer. 

Block signing key used by this producer 

## Friends Documentation

### friend <a id="1a90ba7a85612a19a42fb7c598cd0d3406" href="#1a90ba7a85612a19a42fb7c598cd0d3406">operator<</a>

```cpp
friend constexpr bool eosio::producer_key::operator< (
    const producer_key & a,
    const producer_key & b
)
```





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/privileged.hpp`
