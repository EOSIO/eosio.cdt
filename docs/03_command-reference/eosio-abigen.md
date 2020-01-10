---
content_title: eosio-abigen tool
---

## This tool is deprecated, use `eosio-cpp` for generation of your ABIs

To generate an ABI with ```eosio-abigen```, only requires that you give the main '.cpp' file to compile and the output filename `--output` and generating against the contract name `--contract`.

Example:
```bash
$ eosio-abigen hello.cpp --contract=hello --output=hello.abi
```

This will generate one file:
* The generated ABI file (hello.abi)

```
USAGE: eosio-abigen [options] <source0> [... <sourceN>]

OPTIONS:

Generic Options:

  -help                      - Display available options (-help-hidden for more)
  -help-list                 - Display list of available options (-help-list-hidden for more)
  -version                   - Display the version of this program

eosio-abigen:
generates an ABI from C++ project input

  -extra-arg=<string>        - Additional argument to append to the compiler command line
  -extra-arg-before=<string> - Additional argument to prepend to the compiler command line
  -output=<string>           - Set the output filename and fullpath
  -p=<string>                - Build path
```
