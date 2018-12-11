
### Usage
---
#### CMakeLists.txt
To compile an EOSIO smart contract with CMake you'll need a CMake file. The template `CMakeLists.txt` in the examples folder is a good boilerplate.

For example:

In `CMakeLists.txt`:
```
cmake_minimum_required(VERSION 3.5)
project(test_example VERSION 1.0.0)

find_package(eosio.cdt)

add_contract( test test test.cpp )
```


In `test.cpp`:

```
#include <eosiolib/eosio.hpp>
using namespace eosio;

CONTRACT test : public eosio::contract {
public:
   using contract::contract;

   ACTION testact( name test ) {
   }
};

EOSIO_DISPATCH( test, (testact) )
```

To manually compile the source code, use [`eosio-cpp/eosio-cc`](/tools/eosio-cpp.html) and [`eosio-ld`](/tools/eosio-ld.html) as if it were __clang__ and __lld__. All the includes and options specific to EOSIO and CDT are baked in.
