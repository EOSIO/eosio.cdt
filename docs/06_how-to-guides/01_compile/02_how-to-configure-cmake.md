---
content_title: How to configure CMake
---

## CMake Configuration

### Automatic generation of CMake configuration

To compile an EOSIO smart contract with CMake, you'll need a CMake file. To use the new `eosio-init` tool to generate the directory structure stub .hpp/.cpp files and the cmake configuration files follow these steps:

1. cd ~
2. eosio-init --path=. --project=test_contract
3. cd test_contract
4. cd build
5. cmake ..
6. make
7. ls -al test_contract

At this point, you'll have the `test_contract.abi` and `test_contract.wasm` files in `~/test_contract/test_contract`. These files are ready to be deployed.

### Manual generation of CMake configuration

To create manually the cmake configuration, the template `CMakeLists.txt` in the examples folder is a good boilerplate for manual usage.

1. In `CMakeLists.txt`:
```
cmake_minimum_required(VERSION 3.5)
project(test_example VERSION 1.0.0)

find_package(eosio.cdt)

add_contract( test test test.cpp )
```

2. In `test.cpp`:
```
#include <eosio/eosio.hpp>
using namespace eosio;

class [[eosio::contract]] test : public eosio::contract {
public:
   using contract::contract;

   [[eosio::action]] void testact( name test ) {
   }
};

EOSIO_DISPATCH( test, (testact) )
```

3. The following CMake macros are provided:
- `add_contract` is used to build your smart contract and generate an ABI. The first parameter is the contract name, the second is the cmake target name, and the rest are the CPP files needed to build the contract.
- `target_ricardian_directory` can be used to add the directory where your ricardian contracts live to a specific cmake target.
- `add_native_library` and `add_native_executable` are CMake macros for the native tester. They are drop in replacements for `add_library` and `add_executable`.
