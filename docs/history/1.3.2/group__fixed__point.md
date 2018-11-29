---
search:
    keywords: ['fixed_point', 'eosio::fixed_point32', 'eosio::fixed_point64', 'eosio::fixed_point128']
---

# group fixed\_point

32,64,128,256 bits version of fixed point variables [More...](#detailed-description)
## Classes

|Type|Name|
|-----|-----|
|struct|[**eosio::fixed\_point32**](structeosio_1_1fixed__point32.md)|
|struct|[**eosio::fixed\_point64**](structeosio_1_1fixed__point64.md)|
|struct|[**eosio::fixed\_point128**](structeosio_1_1fixed__point128.md)|


## Detailed Description

Floating point operations are indeterministic, hence is prevented in smart contract. The smart contract developers should use the appropriate Fixed\_Point template class by passing the number to be represented in integer format and the number of decimals required. These template classes also support the arithmetic operations and basic comparison operators 
