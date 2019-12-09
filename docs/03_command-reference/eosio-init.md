---
content_title: eosio-init tool
---

This tool is used to generate a skeleton smart contract and directory structure.
To generate a new smart contract project you can either generate a "bare" project (no CMake) or the default is to generate a CMake project.
Example:
```bash
$ eosio-abigen hello.cpp --contract=hello --output=hello.abi
```

This will generate one file:
* The generated ABI file (hello.abi)

```
USAGE: eosio-init [options]

OPTIONS:

Generic Options:

  -help             - Display available options (-help-hidden for more)
  -help-list        - Display list of available options (-help-list-hidden for more)
  -version          - Display the version of this program

eosio-init:
generates an eosio smart contract project

  -bare             - produces only a skeleton smart contract without CMake support
  -path=<string>    - directory to place the project
  -project=<string> - output project name
```
