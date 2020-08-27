---
content_title: Debugging a smart contract
---

In order to be able to debug your smart contract, you will need to setup a local nodeos node. This local nodeos node can be run as separate private testnet or as an extension of a public testnet.  This local node also needs to be run with the contracts-console option on, either `--contracts-console` via the command line or `contracts-console = true` via the config.ini and/or by setting up logging on your running nodeos node and checking the output logs. See below for details on logging.

When you are creating your smart contract for the first time, it is recommended to test and debug your smart contract on a private testnet first, since you have full control of the whole blockchain and can easily add suitable logging. This enables you to have unlimited amount of eos needed and you can just reset the state of the blockchain whenever you want. When it is ready for production, debugging  on the public testnet (or official testnet) can be done by connecting your local nodeos to the public testnet (or official testnet) so you can see the log of the testnet in your local nodeos.

The concept is the same, so for the following guide, debugging on the private testnet will be covered.

If you haven't set up your own local nodeos, follow the [setup guide](https://developers.eos.io/eosio-home/docs/getting-the-software). By default, your local nodeos will just run in a private testnet unless you modify the config.ini file to connect with public testnet (or official testnet) nodes. 

# Method
The main method used to debug smart contract is **Caveman Debugging**. Printing is utilized to inspect the value of a variable and check the flow of the contract. Printing in smart contracts can be done through the Print API. The C++ API is a wrapper for C API and is the recommended API.

# Print
Print C API supports the following data type that you can print:
- prints - a null terminated char array (string)
- prints_l - any char array (string) with given size
- printi - 64-bit signed integer
- printui - 64-bit unsigned integer 
- printi128 - 128-bit signed integer
- printui128 - 128-bit unsigned integer
- printsf - single-precision floating point number
- printdf - double encoded as 64-bit unsigned integer
- printqf - quadruple encoded as 64-bit unsigned integer
- printn - 64 bit names as base32 encoded string
- printhex - hex given binary of data and its size 

The Print C++ API wraps some of the above C API by overriding the print() function, so the user doesn't need to determine which specific print function to use. Print C++ API supports:
- a null terminated char array (string)
- integer (128-bit unsigned, 64-bit unsigned, 32-bit unsigned, signed, unsigned)
- base32 string encoded as 64-bit unsigned integer
- struct that has print() method

# Example
Here's an example contract for debugging

## debug.hpp

```cpp
namespace debug {
    struct foo {
        account_name from;
        account_name to;
        uint64_t amount;
        void print() const {
            eosio::print("Foo from ", eosio::name(from), " to ", eosio::name(to), " with amount ", amount, "\n");
        }
    };
}
```
## debug.cpp

```cpp
#include <debug.hpp>

extern "C" {

    void apply( uint64_t code, uint64_t action ) {
        if (code == N(debug)) {
            eosio::print("Code is debug\n");
            if (action == N(foo)) {
                 eosio::print("Action is foo\n");
                debug::foo f = eosio::unpack_action_data<debug::foo>();
               if (f.amount >= 100) {
                    eosio::print("Amount is larger or equal than 100\n");
                } else {
                    eosio::print("Amount is smaller than 100\n");
                    eosio::print("Increase amount by 10\n");
                    f.amount += 10;
                    eosio::print(f);
                }
            }
        }
    }
} // extern "C"
```
## debug.abi

```json
{
  "structs": [{
      "name": "foo",
      "base": "",
      "fields": {
        "from": "account_name",
        "to": "account_name",
        "amount": "uint64"
      }
    }
  ],
  "actions": [{
      "action_name": "foo",
      "type": "foo"
    }
  ]
}
```
Deploy it and push an action to it. It is assumed you have a `debug` account created and have its key in your wallet.

```bash
$ eosio-cpp -abigen debug.cpp -o debug.wasm
$ cleos set contract debug CONTRACT_DIR/debug -p youraccount@active
$ cleos push action debug foo '{"from":"inita", "to":"initb", "amount":10}' --scope debug
```

When you check your local `nodeos` node log, you will see the following lines after the above message is sent.

```
Code is debug
Action is foo
Amount is smaller than 100
Increase amount by 10
Foo from inita to initb with amount 20
```

There, you can confirm that your message is going to the right control flow and the amount is updated correctly. You might see the above message at least 2 times and that's normal because each transaction is being applied during verification, block generation, and block application.