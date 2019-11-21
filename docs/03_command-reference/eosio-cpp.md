---
content_title: eosio-cpp tool
---

To manually compile the source code, use `eosio-cpp` and `eosio-ld` as if it were __clang__ and __lld__. All the includes and options specific to EOSIO and CDT are baked in.

```
USAGE: eosio-cpp [options] <input file> ...

OPTIONS:

Generic Options:

  -help                    - Display available options (-help-hidden for more)
  -help-list               - Display list of available options (-help-list-hidden for more)
  -version                 - Display the version of this program

compiler options:

  -C                       - Include comments in preprocessed output
  -CC                      - Include comments from within macros in preprocessed output
  -D=<string>              - Define <macro> to <value> (or 1 if <value> omitted)
  -E                       - Only run the preprocessor
  -I=<string>              - Add directory to include search path
  -L=<string>              - Add directory to library search path
  -MD                      - Write depfile containing user and system headers
  -MF=<string>             - Write depfile output
  -MMD                     - Write depfile containing user
  -MT=<string>             - Specify name of main file output in depfile
  -O=<string>              - Optimization level s, 0-3
  -R=<string>              - Add a resource path for inclusion
  -S                       - Only run preprocess and compilation steps
  -U=<string>              - Undefine macro <macro>
  -W=<string>              - Enable the specified warning
  -abigen                  - Generate ABI
  -abigen_output=<string>  - ABIGEN output
  -c                       - Only run preprocess, compile, and assemble steps
  -contract=<string>       - Contract name
  -dD                      - Print macro definitions in -E mode in addition to normal output
  -dI                      - Print include directives in -E mode in addition to normal output
  -dM                      - Print macro definitions in -E mode instead to normal output
  -emit-ast                - Emit Clang AST files for source inputs
  -emit-llvm               - Use the LLVM representation for assembler and object files
  -faligned-allocation     - Enable C++17 aligned allocation functions
  -fasm                    - Assemble file for x86-64
  -fcolor-diagnostics      - Use colors in diagnostics
  -fcoroutine-ts           - Enable support for the C++ Coroutines TS
  -finline-functions       - Inline suitable functions
  -finline-hint-functions  - Inline functions which are (explicitly or implicitly) marked inline
  -fmerge-all-constants    - Allow merging of constants
  -fnative                 - Compile and link for x86-64
  -fno-cfl-aa              - Disable CFL Alias Analysis
  -fno-elide-constructors  - Disable C++ copy constructor elision
  -fno-lto                 - Disable LTO
  -fno-post-pass           - Don't run post processing pass
  -fno-stack-first         - Don't set the stack first in memory
  -stack-size              - Specifies the maximum stack size for the contract
  -fstack-protector        - Enable stack protectors for functions potentially vulnerable to stack smashing
  -fstack-protector-all    - Force the usage of stack protectors for all functions
  -fstack-protector-strong - Use a strong heuristic to apply stack protectors to functions
  -fstrict-enums           - Enable optimizations based on the strict definition of an enum's value range
  -fstrict-return          - Always treat control flow paths that fall off the end of a non-void function as unreachable
  -fstrict-vtable-pointers - Enable optimizations based on the strict rules for overwriting polymorphic C++ objects
  -fuse-main               - Use main as entry
  -include=<string>        - Include file before parsing
  -isystem=<string>        - Add directory to SYSTEM include search path
  -l=<string>              - Root name of library to link
  -lto-opt=<string>        - LTO Optimization level (O0-O3)
  -o=<string>              - Write output to <file>
  -std=<string>            - Language standard to compile for
  -sysroot=<string>        - Set the system root directory
  -v                       - Show commands to run and use verbose output
  -w                       - Suppress all warnings
```
