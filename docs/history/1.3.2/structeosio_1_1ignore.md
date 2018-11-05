---
search:
    keywords: ['eosio::ignore']
---

# struct eosio::ignore

[**Class List**](annotated.md) **>** [**eosio**](namespaceeosio.md) **::** [**ignore**](structeosio_1_1ignore.md)


Tells the datastream to ignore this type, but allows the abi generator to add the correct type. [More...](#detailed-description)
## Detailed Description

Currently non-ignore types can not succeed an ignore type in a method definition, i.e. void foo(float, ignore<int>) is allowed and void foo(float, ignore<int>, int) is not allowed. 

**Note:**

This restriction will be relaxed in a later release. Currently non-ignore types can not succeed an ignore type in a method definition, i.e. void foo(float, ignore<int>) is allowed and void foo(float, ignore<int>, int) is not allowed. This restriction will be relaxed in a later release. 





----------------------------------------
The documentation for this class was generated from the following file: `libraries/eosiolib/ignore.hpp`
