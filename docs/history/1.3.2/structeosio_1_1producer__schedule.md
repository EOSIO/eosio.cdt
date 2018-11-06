---
search:
    keywords: ['eosio::producer_schedule', 'version', 'producers']
---

# struct eosio::producer\_schedule

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**producer\_schedule**](structeosio_1_1producer__schedule.md)


## Public Attributes

|Type|Name|
|-----|-----|
|uint32\_t|[**version**](structeosio_1_1producer__schedule_a3958be89c877e9249710a8aafa155829.md#1a3958be89c877e9249710a8aafa155829)|
|std::vector< **[producer\_key](structeosio_1_1producer__key.md)** >|[**producers**](structeosio_1_1producer__schedule_a10628b8023f6db90687a54776d15a117.md#1a10628b8023f6db90687a54776d15a117)|


## Detailed Description

Defines both the order, account name, and signing keys of the active set of producers. 
## Public Attributes Documentation

### variable <a id="1a3958be89c877e9249710a8aafa155829" href="#1a3958be89c877e9249710a8aafa155829">version</a>

```cpp
uint32_t eosio::producer_schedule::version;
```


Version number of the schedule. It is sequentially incrementing version number 

### variable <a id="1a10628b8023f6db90687a54776d15a117" href="#1a10628b8023f6db90687a54776d15a117">producers</a>

```cpp
std::vector<producer_key> eosio::producer_schedule::producers;
```


List of producers for this schedule, including its signing key 



----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/producer\_schedule.hpp`
