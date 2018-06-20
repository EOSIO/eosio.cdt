# WasmSDK
## Version : ```Prelease```

WasmSDK is a full toolchain for WebAssembly (WASM).  In addition to being a general WebAssembly toolchain, additional [EOSIO](https://github.com/eosio/eos) specific optimizations are available.  This new toolchain is built around [Clang 7](https://github.com/eosio/llvm), which means that the SDK has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.

## New Features from EOSIO
- Compile (-c) option flag will compile to a WASM-elf object file
- Ranlib and AR support for static libraries for WASM
- \_\_FILE\_\_ and \_\_BASE\_FILE\_\_ will now only return the file name and not the fullpath. This eliminates any non-determinism from location of the compiled binary
- Global constructors and global destructors are now supported
- cppeos and eosio.lld are the two top level tools for compilation and linking.  (UNDER DEVELOPMENT)
- A simple CMake interface to build contracts against WasmSDK
- ABI generator (Coming soon)

### Installation
WasmSDK requires CMake v3.5+ and Boost 1.67.0 to run.

Please note that because we have the compiler as an external project, this will cause it to be cloned during the ```make``` step.  The compiler repo can be cloned separately and installed to either default install prefix or where you specify during the ```cmake``` step.

First clone the repo and create a build directory

```sh
$ git clone --recursive https://github.com/eosio/wasmsdk
$ cd wasmsdk
$ mkdir build && cd build
```

Then configure with cmake to install to ```/usr/local```

```sh
$ cmake ..
```

or configure cmake to install to a specific path
```sh
$ cmake -DCMAKE_INSTALL_PREFIX=<install path> ..
```

*If the compiler is cloned separetly, use the cmake flag ```-DNO_WASM_COMPILER```, this will allow for compilation and installation of the SDK independently of the compiler.
 *If the install path is to ```/usr/local``` ```sudo``` will have to be used even during making, because of the installation of the compiler during the ```make``` step.

Then to install, simply
```sh
$ make && make install
```

### Installed Tools
* cppeos (C/C++ compiler)
* eosio.lld
* eosio.ranlib
* eosio.ar
* eosio.objdump
* eosio.readelf
* stripbss (strips all zeroed initialized data segments, usually the .bss sections)

### Usage
To compile a contract, the perferred method is to use the template CMakeLists.txt in the examples folder.
For example:
```CMakeLists.txt```
```
cmake_minimum_required(VERSION 3.5)
project(test_example VERSION 1.0.0)

if(WASM_ROOT STREQUAL "" OR NOT WASM_ROOT)
    set(WASM_ROOT ${CMAKE_INSTALL_PREFIX})
endif()
list(APPEND CMAKE_MODULE_PATH ${WASM_ROOT}/lib/cmake)
include(EosioWasmToolchain)

add_executable( test test.cpp )
target_include_directories(test PUBLIC ${STANDARD_INCLUDES})
target_link_libraries(test PRIVATE ${STANDARD_LIBS})
```
```test.cpp```
```
#include <eosiolib/eosio.hpp>
using namespace eosio;
class test : public eosio::contract {
public:
   using contract::contract;
   void test_action( account_name test ) {
   }
};
EOSIO_ABI( test, (test_action))
```

The cmake variables ```STANDARD_LIBS``` and ```STANDARD_INCLUDES``` are defined within EosioWasmToolchain.cmake.  Since, EosioWasmToolchain overwrites cmake to cross-compile WASM, standard cmake commands of add_executable/ add_library can be used.

To manually compile source code:
Use ```cppeos``` and ```eosio.lld``` as if it were clang and lld , with all includes and options specific to EOSIO and WasmSDK being baked in.

### Todos

 - Make cppeos and eosio.lld more than just wrapper scripts
 - Add ABI generation to cppeos
 - Integrate any post passes (minimizations) to eosio.lld

License
----

MIT
