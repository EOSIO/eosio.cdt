# EOSIO.CDT (Contract Development Toolkit)
## Version : 1.3.0

EOSIO.CDT is a toolchain for WebAssembly (WASM) and set of tools to facilitate contract writing for the EOSIO platform.  In addition to being a general purpose WebAssembly toolchain, [EOSIO](https://github.com/eosio/eos) specific optimizations are available to support building EOSIO smart contracts.  This new toolchain is built around [Clang 7](https://github.com/eosio/llvm), which means that EOSIO.CDT has the most currently available optimizations and analyses from LLVM, but as the WASM target is still considered experimental, some optimizations are not available or incomplete.

## Important!
EOSIO.CDT Version 1.3.0 introduces quite a few breaking changes.  To have binary releases we needed to remove the concept of core symbol from EOSIO.CDT, this meant drastic changes to symbol, asset and other types/functions that were connected to them. Since these changes would be disruptive, we decided to add as many disruptive changes needed for future contract writing, so that disruption should only occur once. Please read the **_Differences between Version 1.2.x and Version 1.3.0_** section of this readme.

### Binary Releases
EOSIO.CDT currently supports Mac OS X brew, Linux x86_64 Debian packages, and Linux x86_64 RPM packages.
#### Mac OS X Brew Install
```sh
$ brew tap eosio/eosio.cdt
$ brew install eosio.cdt
```
#### Mac OS X Brew Uninstall
```sh
$ brew remove eosio.cdt
```
#### Debian Package Install
```sh
$ wget https://github.com/eosio/eosio.cdt/releases/download/v1.3.0/eosio.cdt-1.3.0.x86_64.deb
$ sudo apt install ./eosio.cdt-1.3.0.x86_64.deb
```
#### Debian Package Uninstall
```sh
$ sudo apt uninstall eosio.cdt
```

#### RPM Package Install
```sh
$ wget https://github.com/eosio/eosio.cdt/releases/download/v1.3.0/eosio.cdt-1.3.0.x86_64-0.x86_64.rpm
$ sudo yum install ./eosio.cdt-1.3.0.x86_64-0.x86_64.rpm
```

#### RPM Package Uninstall
```sh
$ sudo yum remove eosio.cdt
```

### Guided Installation (Building from Scratch)
```sh
$ git clone --recursive https://github.com/eosio/eosio.cdt
$ cd eosio.cdt
$ ./build.sh
$ sudo ./install.sh
```
### Building your first smart contract
- Navigate to the hello folder in examples (./examples/hello).
- You should then see the hello.cpp file
- Now run the compiler
```sh
$ eosio-cpp -abigen hello.cpp -o hello.wasm
```
- Or with CMake
  ```sh
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  ```
This will generate two files:
* The compiled binary wasm (hello.wasm)
* The generated abi file (hello.abi)

#### using eosio-abigen alone
To generate an abi with ```eosio-abigen```, only requires that you give the main '.cpp' file to compile and the output filename `--output` and generating against the contract name `--contract`.

Example:
```bash
$ eosio-abigen hello.cpp --contract=hello --output=hello.abi
```

This will generate one file:
* The generated abi file (hello.abi)

## Differences between Version 1.2.x and Version 1.3.0
### eosiolib C API
- addition of `uint64_t` typedef `capi_name`
- removal of `uint64_t` typedefs 
  - `account_name`
  - `permission_name`
  - `scope_name`
  - `table_name`
  - `action_name`
    - these have been replaced by `capi_name`, and as a practice should not be used.  The new version of the `name` type should be used to replace these instances. This decision was made because of bad implict casting issues with `uint64_t` and the new pattern should allow for better type safety.
  - `symbol_name`
    - this has no C equivalent and is superceded by the `symbol_code` struct.  As with the previously mentioned named types, this was removed and replaced with `symbol_code` to allow for better type safety in contracts.  To use a symbol, i.e. symbol name and precision, use the `symbol` class.
- removal of `time` and `weight_type` typedefs
- removal of the typedefs `transaction_id_type` and `block_id_type`
- removal of the `account_permission` struct
- renaming of typedefs
  - `checksum160` -> `capi_checksum160`
  - `checksum256` -> `capi_checksum256`
  - `checksum512` -> `capi_checksum512`
  - `public_key`  -> `capi_public_key`
  - `signature`   -> `capi_signature`
- removal of non-existent intrinsics declarations `require_write_lock` and `require_read_lock`
### eosiolib C++ API
- removal of eosiolib/vector.hpp
  - removed alias `eosio::vector` and typedef `bytes`
  - going forward contract writers should include `<vector>` from the STL and use `std::vector<char>` instead of bytes.
- removal of eosiolib/types.hpp
- removal of eosiolib/optional.hpp, use `std::optional` as a replacement.
- removal of eosiolib/core_symbol.hpp, the contract writer should explicitly specify the symbol.
- added eosiolib/name.hpp

#### eosiolib/types.hpp
- moved the typedef `eosio::extensions_types` to eosiolib/transaction.hpp
- removed comparison functions for `checksum` structs
- removal of `eosio::char_to_symbol`, `eosio::string_to_name`, `eosio::name_suffix` functions
- removal of the `N` macro. The ""\_n operator or the `name` constructor should be used as a type safe replacement. Example: `N(foo)` -> `"foo"\_n`, or `N(foo)` -> `name("foo")`.
- moved `eosio::name` struct definition and ""\_n operator to eosiolib/name.hpp

#### eosiolib/name.hpp
- removal of implicit and explicit conversions to `uint64_t`.
- addition of `enum class` `eosio::name::raw` which will implicitly convert to `uint64_t` (used for template non-type parameters).
- added `bool` conversion operator for conditionally testing if a name is empty
- all constructors are now `constexpr`
  - these take either a `uint64_t`, an `eosio::name::raw` or a `std::string_view`
- added `constexpr` methods `eosio::name::length`, `eosio::name::suffix`
- added equivalence, inverted equivalence and less than operators to `eosio::name`

#### eosiolib/symbol.hpp
- removed `eosio::symbol_type` struct and replaced with `eosio::symbol` class
- added struct `eosio::symbol_code`
  - added two `constexpr` constructors that take either a raw `uint64_t` or an `std::string_view`
  - added `constexpr` methods `is_valid`, `length` and `raw`
  - added a print method
  - added `bool` conversion operator to test is `symbol_code` is empty
- removal of `eosio::string_to_symbol`, `eosio::is_valid_symbol`, `eosio::symbol_name_length` functions
- removal of the `S` macro. The symbol constructor should be used as a type safe replacement. Example: `S(4,SYS)` -> `symbol("SYS", 4)`.
- added struct `eosio::symbol`
  - added two `constexpr` constructors that take either a raw `uint64_t` or a `symbol_code` and a `uint8_t` precision.
  - added `constexpr` methods `is_valid`, `precision`, `code`, and `raw`. These call the `symbol_code` is_valid, get the `uint8_t` precision, get the `symbol_code` and get the raw `uint64_t`.
  - added equivalence, inverted equivalence and less than operators to `eosio::symbol`
- modified struct `eosio::extended_symbol`
  - restricted fields to private
  - added `constexpr` constructor that takes a `eosio::symbol` and an `eosio::name`.
  - added `constexpr` methods `get_symbol` and `get_contract`.
  - made existing comparison operators `constexpr`

#### eosiolib/asset.hpp
- The main constructor now requires a `int64_t` (quantity) and `eosio::symbol` explicitly.
- The default constructor no longer initializes the instance to a valid zero quantity asset with a symbol equivalent to "core symbol". Instead the default constructed `eosio::asset` is a bit representation of all zeros (which will cause `is_valid` to fail), so that check is bypassed to allow for `multi_index` and `datastream` to work.
- Old contracts that use `eosio::asset()` should be changed to either use the core symbol of the specific chain they are targeting i.e. `eosio::asset(0, symbol(4,SYS))` to reduce writing `symbol(4,SYS)` over and over, a `constexpr` function to return the symbol or `constexpr` global variable should be used.

#### eosiolib/contract.hpp
- The constructor for `eosio::contract` now takes an `eosio::name` for the receiver, an `eosio::name` for the code, and a `eosio::datastream<const char*>` for the datastream used for the contract.  The last argument is for manually unpacking an action, see the section on `eosio::ignore` for a more indepth usage.

#### eosiolib/dispatcher.hpp
- renamed the macro `EOSIO_ABI` to `EOSIO_DISPATCH`, as this makes more sense as to what this macro does.
- modified the definition of `EOSIO_DISPATCH` to work with the new constructor for `eosio::contract`

#### eosiolib/multi_index.hpp
- the first template parameter for `indexed_by` now requires the argument be convertible to `eosio::name::raw` (replacing `uint64_t`.
- the first template parameter for `multi_index` now requires the argument be convertible to `eosio::name::raw` (replacing `uint64_t`.
- the constructor now takes an `eosio::name` type for the code (replacing `uint64_t`), scope is still `uint64_t`.
- various other replacements of `uint64_t` to `eosio::name`

#### eosiolib/singleton.hpp
- the first template parameter for `eosio::singleton` now requires the argument be convertible to `eosio::name::raw` (replacing `uint64_t`.
- the constructor now takes an `eosio::name` type for the code.
- in the methods `get_or_create` and `set` the argument `bill_to_account` is now of type `eosio::name` (replacing `uint64_t`).

#### eosiolib/action.hpp
- added C++ function `eosio::require_auth`
- added C++ function `eosio::has_auth`
- added C++ function `eosio::is_account`
- redefined `eosio::permission_level` to use `eosio::name` in place of `uint64_t`
- removed the macro `ACTION`

#### eosiolib/permission.hpp
 - The optional provided_keys argument of the function `eosio::check_transaction_authorization` is now of the type `std::set<eosio::public_key>` rather than the type `std::set<capi_public_key>`. C++ contract code should most likely be using the eosio::public_key struct (defined in "eosiolib/public_key.hpp") if they need to deal with EOSIO-compatible public keys rather than the capi_public_key struct (now renamed from its original name of ::public_key) from the eosiolib C API. Note that existing contract code that just referred to the type public_key without namespace qualification may have accidentally been using the capi_public_key struct and therefore should ideally be modified to use the eosio::public_key C++ type.
 - the `account` and `permission` arguments of `eosio::check_transaction_authorization` are both `eosio::name` now instead of `uint64_t`

#### eosiolib/ignore.hpp
- added new type `ignore`
  - this type acts as a placeholder for actions that don't want to deserialize their fields but what the types to be reflected in the abi.
    ```c
   	ACTION action(ignore<some_type>) { some_type st; _ds >> st; }
    ```
- added new type `ignore_wrapper`
  - this allows for calling `SEND_INLINE_ACTION` with `ignore_wrapper(some_value)` against an action with an `ignore` of matching types.

### macros
- added `ACTION` macro, this is simply a wrapper for `[[eosio::action]] void`
- added `TABLE` macro, this is simply a wrapper for `struct [[eosio::table]]` 
- added `CONTRACT` macro, this is simply a wrapper for `class [[eosio::contract]]`

### CMake
- added `eosio.cdt-config.cmake` to allow for `find_package(eosio.cdt)`, see eosio.cdt/examples/hello or eosio.cdt/examples/template for an example
- added new macro `add_contract`, this new contract takes a contract name, cmake target, then any normal arguments you would give to `add_executable`, see eosio.cdt/examples/hello or eosio.cdt/examples/template
- new version checking mechanism is included, see eosio.contracts/CMakeLists.txt to see this in use.

### libc
- replaced `printf`, `sprintf`, and `snprintf` with new minimal variants, and allows contracts to use these functions without causing stack overflow issues.

### libcxx
- removed `sstream`, with the intent to return this after more has been done.
- added `__cxa_pure_virtual` to allow for pure virtual methods in contract classes.
- `std::to_string` now works without the issues of stack overflows.

### attributes
- added `[[eosio::ignore]]` attribute to flag a type as being ignored by the deserializer, this attribute is primarily only used for internal use within eosiolib.
- added `[[eosio::contract]]` attribute, this new attribute is used to mark a contract class as "contract" with the name being either the C++ name of the class or a user specified name (i.e. [[eosio::contract("somecontract")]]), this attribute can also be used in conjunction with the `eosio::action` and `eosio::table` attributes for tables that you would like to define outside of the `eosio::contract` class.  This is used in conjunction with either the raw `eosio-cpp` option `--contract <name>`, `-o <name>/.wasm` or with CMake `add_contract`.  This acts as a filter, so that users who want to include a header file with attributes and generate an abi devoid of those actions and tables (e.g. eosio.token).
  ```c
  CONTRACT test {
  	ACTION acta(){}
	TABLE taba {
	  uint64_t a;
	  float b;
	  uint64_t primary_key() { return a; }
	};
  };
  struct [[eosio::table, eosio::contract("test")]]
  tabb {
    uint64_t a;
    int b
  };
  typedef eosio::multi_index<"testtaba"_n, test::taba> table_a;
  typedef eosio::multi_index<"testtabb"_n, taba> table_b;
  ```
  this will produce the tables `testtaba` and `testtabb` in your abi. Example: `eosio-cpp -abigen test.cpp -o test.wasm` will mark this compilation and abi generation for the `eosio::contract` `test`, so will `eosio-cpp -abigen test.cpp -o <some else> --contract test` and finally CMake `add_contract( test, test_contract, test.cpp )`, this will produce test_contract.wasm, test_contract.abi generated against `test` contract name.

### boost
- is now part of the library (Boost will be removed in a future release), no more external dependence on Boost and all system inclusion are within it's `sysroot`.


## abi generator attributes
Unlike the old abi generator tool, the new tool uses C++11 or GNU style attributes to mark ```actions``` and ```tables```.
#### [[eosio::action]]
this attribute marks either a struct or a method as an action.
Example (four ways to declare an action for ABI generation):
```c++
// this is the C++11 and greater style attribute
[[eosio::action]]
void testa( name n ) {
	// do something
}

// this is the GNU style attribute, this can be used in C code and prior to C++ 11
__attribute__((eosio_action))
void testa( name n ){
	// do something
}

struct [[eosio::action]] testa {
	name n;
    EOSLIB_SERIALIZE( testa, (n) )
};

struct __attribute__((eosio_action)) testa {
	name n;
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

### Fixing an ABI, or Writing an ABI
- The sections to the abi are pretty simple to understand and the syntax is purely JSON, so we are going to write our own ABI file.
- Even after ABI generation is available, an important note should be made that the generator will never be completely perfect for every contract written, advanced features of the newest version of the ABI will require manual construction of the ABI, and odd and advanced C++ patterns could capsize the generators type deductions, so having a good knowledge of how to write an ABI should be an essential piece of knowledge of a smart contract writer.
- Please refer to [developers.eos.io "How to write an abi"](https://developers.eos.io/eosio-cpp/docs/how-to-write-an-abi), to learn about the different sections of an ABI.


### Installed Tools
---
* [eosio-cpp](#eosio-cpp)
* [eosio-cc](#eosio-cc)
* [eosio-ld](#eosio-ld)
* eosio-abigen
* eosio-pp (post processing pass for WASM, automatically runs with eosio-cpp and eosio-ld)
* eosio-wasm2wast
* eosio-wast2wasm
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

find_package(eosio.cdt)

add_contract( test test test.cpp )
```
```test.cpp```
```
#include <eosiolib/eosio.hpp>
using namespace eosio;
CONTRACT test : public eosio::contract {
public:
   using contract::contract;

   ACTION testact( account_name test ) {
   }
};
EOSIO_ABI( test, (testact))
```

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

### eosio-abigen
---
```bash
USAGE: eosio-abigen [options] <source0> [... <sourceN>]

OPTIONS:

Generic Options:

  -help                      - Display available options (-help-hidden for more)
  -help-list                 - Display list of available options (-help-list-hidden for more)
  -version                   - Display the version of this program

eosio-abigen:
generates an abi from C++ project input

  -extra-arg=<string>        - Additional argument to append to the compiler command line
  -extra-arg-before=<string> - Additional argument to prepend to the compiler command line
  -output=<string>           - Set the output filename and fullpath
  -p=<string>                - Build path
```

License
----

MIT
