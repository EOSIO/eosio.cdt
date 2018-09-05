#CDT 
## Version : 1.1.1

WasmSDK is a toolchain for WebAssembly (WASM).  In addition to being a general purpose WebAssembly toolchain, [EOSIO](https://github.com/eosio/eos) specific optimizations are available to support building EOSIO smart contracts.  This new toolchain is built around [Clang 7](https://github.com/eosio/llvm), which means that the SDK has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.

## New Features from EOSIO
- Compile (-c) option flag will compile to a WASM-elf object file
- ranlib and ar support for static libraries for WASM
- \_\_FILE\_\_ and \_\_BASE\_FILE\_\_ will now only return the file name and not the fullpath. This eliminates any non-determinism from location of the compiled binary
- Global constructors and global destructors are now supported
- _eosio-cpp_, _eosio-cc_, _eosio-ld_, and _eosio-pp_ are set the core set of tools that you will interact with.
    * These are the C++ compiler, C compiler, linker and postpass tools.
- A simple CMake interface to build EOSIO smart contracts against WasmSDK
- ABI generator

# How to use eosio-abigen
## using with eosio-cpp
To generate an abi with ```eosio-cpp```, the only flag you need to pass to ```eosio-cpp``` is `-abigen`, this will tell the compiler to run `eosio-abigen` after compilation and linking stages.  If the output filename is specified as a '.wasm' file with the `-o` option (e.g. \<filename\>.wasm) then eosio-cpp will tell the abi generator to create the abi with the name \<filename\>.abi, if no '.wasm' suffix is used then the resulting output filename is still \<filename\>.abi 

Example:
```bash
$ eosio-cpp hello.cpp -o hello.wasm --abigen
```
This will generate two files:
* The compiled binary wasm (hello.wasm)
* The generated abi file (hello.abi)

## using eosio-abigen alone
To generate an abi with ```eosio-abigen```, only requires that you give the main '.cpp' file to compile and the output filename `--output`.

Example:
```bash
$ eosio-abigen hello.cpp --output=hello.abi
```

This will generate one file:
* The generated abi file (hello.abi)

# Difference from old abi generator
Unlike the old abi generator tool, the new tool uses C++11 or GNU style attributes to mark ```actions``` and ```tables```.

Example (four ways to declare an action for ABI generation):
```c++
// this is the C++11 and greater style attribute
[[eosio::action]]
void testa( account_name n ) {
	// do something
}

// this is the GNU style attribute, this can be used in C code and prior to C++ 11
__attribute__((eosio_action)) 
void testa( account_name n ){
	// do something
}

struct [[eosio::action]] testa {
	account_name n;
    EOSLIB_SERIALIZE( testa, (n) )
};

struct __attribute__((eosio_action)) testa {
	account_name n;
    EOSLIB_SERIALIZE( testa, (n) )
};
```
If your action name is not a valid [EOSIO name](https://developers.eos.io/eosio-cpp/docs/naming-conventions) you can explicitly specify the name in the attribute ```c++ [[eosio::action("<valid action name>")]]```

Example (Two ways to declare a table for abi generation):
```c++
struct [[eosio::table]] testtable {
	uint64_t owner;
  	/* all other fields */
};

struct __attribute__((eosio_table)) testtable {
	uint64_t owner;
    /* all other fields */
};

typedef eosio::multi_index<N(tablename), testtable> testtable_t;
```
If you don't want to use the multi-index you can explicitly specify the name in the attribute ```c++ [[eosio::table("<valid action name>")]]```

For an example contract of abi generation please see the file ./examples/abigen_test/test.cpp, you can generate the abi for this file with `eosio-abigen test.cpp --output=test.abi`.

### Guided Installation
First clone

```sh
$ git clone --recursive https://github.com/eosio/eosio.cdt
$ cd eosio.cdt
```

Now run `build.sh` and give the core symbol for the EOSIO blockchain that intend to deploy to.
    *`build.sh` will install any dependencies that are needed.
```sh
$ ./build.sh <CORE_SYMBOL>
```

Finally, install the build
    *This install will install the core to ```/usr/local/eosio.cdt``` and symlinks to the top level tools (compiler, ld, etc.) to ```/usr/local/bin```
```sh
$ sudo ./install.sh
```

### Installed Tools
---
* [eosio-cpp](#eosio-cpp)
* [eosio-cc](#eosio-cc)
* [eosio-ld](#eosio-ld)
* eosio-pp (post processing pass for WASM, automatically runs with eosio-cpp and eosio-ld)
* eosio-ranlib
* eosio-ar
* eosio-objdump
* eosio-readelf

### Usage
---
To compile an EOSIO smart contract, the perferred method is to use the template CMakeLists.txt in the examples folder.
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

Since, EosioWasmToolchain overwrites `cmake` to cross-compile WASM, standard cmake commands of _add\_executable/ add\_library_ can then be used.  Also note, the __WASM_ROOT__ variable, this needs to be set if you decided to install to the non-default location.

To manually compile source code:
Use ```eosio-cpp/eosio-cc``` and ```eosio-ld``` as if it were __clang__ and __lld__ , with all includes and options specific to EOSIO and CDT being baked in.

### eosio-cpp
---
```bash
OVERVIEW: eosio-cpp (Eosio C++ -> WebAssembly compiler)
USAGE: eosio-cpp [options] <input file> ...

OPTIONS:
  -C                       - Include comments in preprocessed output
  -CC                      - Include comments from within macros in preprocessed output
  -D=<string>              - Define <macro> to <value> (or 1 if <value> omitted)
  -E                       - Only run the preprocessor
  -I=<string>              - Add directory to include search path
  -L=<string>              - Add directory to library search path
  -O=<string>              - Optimization level s, 0-3
  -S                       - Only run preprocess and compilation steps
  -U=<string>              - Undefine macro <macro>
  -W=<string>              - Enable the specified warning
  -c                       - Only run preprocess, compile, and assemble steps
  -dD                      - Print macro definitions in -E mode in addition to normal output
  -dI                      - Print include directives in -E mode in addition to normal outpu
  -dM                      - Print macro definitions in -E mode instead to normal output
  -emit-ast                - Emit Clang AST files for source inputs
  -emit-llvm               - Use the LLVM representation for assembler and object files
  -faligned-allocation     - Enable C++17 aligned allocation functions
  -fcoroutine-ts           - Enable support for the C++ Coroutines TS
  -finline-functions       - Inline suitable functions
  -finline-hint-functions  - Inline functions which are (explicitly or implicitly) marked inline
  -fmerge-all-constants    - Allow merging of constants
  -fno-cfl-aa              - Disable CFL Alias Analysis
  -fno-elide-constructors  - Disable C++ copy constructor elision
  -fno-lto                 - Disable LTO
  -fstack-protector        - Enable stack protectors for functions potentially vulnerable to stack smashing
  -fstack-protector-all    - Force the usage of stack protectors for all functions
  -fstack-protector-strong - Use a strong heuristic to apply stack protectors to functions
  -fstrict-enums           - Enable optimizations based on the strict definition of an enum's value range
  -fstrict-return          - Always treat control flow paths that fall off the end of a non-void function as unreachable
  -fstrict-vtable-pointers - Enable optimizations based on the strict rules for overwriting polymorphic C++ objects
  -include=<string>        - Include file before parsing
  -isysroot=<string>       - Set the system root directory (usually /)
  -l=<string>              - Root name of library to link
  -lto-opt=<string>        - LTO Optimization level (O0-O3)
  -o=<string>              - Write output to <file>
  -std=<string>            - Language standard to compile for
  -v                       - Show commands to run and use verbose output
  -w                       - Suppress all warnings

Generic Options:

  -help                    - Display available options (-help-hidden for more)
  -help-list               - Display list of available options (-help-list-hidden for more)
  -version                 - Display the version of this program
```

### eosio-ld
---
```bash
OVERVIEW: eosio-ld (WebAssembly linker)
USAGE: eosio-ld [options] <input file> ...

OPTIONS:

Generic Options:

  -help             - Display available options (-help-hidden for more)
  -help-list        - Display list of available options (-help-list-hidden for more)
  -version          - Display the version of this program

eosio.ld options:

  -L=<string>       - Add directory to library search path
  -fno-cfl-aa       - Disable CFL Alias Analysis
  -fno-lto          - Disable LTO
  -fno-post-pass    - Don't run post processing pass
  -fno-stack-first  - Don't set the stack first in memory
  -l=<string>       - Root name of library to link
  -lto-opt=<string> - LTO Optimization level (O0-O3)
  -o=<string>       - Write output to <file>
 ```
 
 ### Todos
 ---
 - Add ABI generation to eosio-cpp

License
----

MIT
