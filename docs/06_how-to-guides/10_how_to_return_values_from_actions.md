---
content_title: How-To Return Values From Action
---

## Summary

This how-to demonstrates how a smart contract developer implements return values from an action.

In order to accomplish this, use the `return` statement and pass the desired returned value, which can be of any C++ primitive type, a standard library type, or a user defined type.

## Prerequisites

* Set your EOSIO development environment. Refer to the [Get Started](https://developers.eos.io/welcome/latest/getting-started/development-environment/introduction) Guide.
* You have a smart contract, let’s call it `smrtcontract`, and it builds without error.
* You have an action, let’s call it `checkwithrv`, from which you want to return a value of a user defined type `action_response`.

Refer to the following  example.

```cpp
struct action_response
{
  uint16_t id;
  std::pair<int, std::string> status;
};

class [[eosio::contract]] smrtcontract : public contract {
  public:
     using contract::contract;

     [[eosio::action]]
     action_response checkwithrv( name nm );
};
```

## Procedure

Complete the following steps to return an instance of `action_response` from the `checkwithrv` action:

1. Create an instance of the `action_response` C++ defined structure.
2. Initialize its data members based on the action’s business logic.
3. Use `return` statement and pass as a parameter the instance created and initialized in previous steps.

```cpp
[[eosio::action]]
action_response smrtcontract::checkwithrv( name nm ) {
  print_f("Name : %\n", nm);

  action_response results;
  results.id = 1;
  if (nm == "hello"_n) {
     results.status = {0, "Validation has passed."};
  }
  else {
     results.status = {1, "Input param `name` not equal to `hello`."};
  }
  return results; // the `set_action_return_value` intrinsic is invoked automatically here
}
```

For a complete example of a smart contract that implements an action which returns a value see the [hello example smart contract](https://github.com/EOSIO/eosio.cdt/blob/develop/examples/hello/src/hello.cpp#L16).

## Next Steps

The following options are available when you complete the procedure:

* Compile the smart contract and deploy it to the EOSIO testnet or any EOSIO based blockchain.
* Use the `cleos` command to send the `checkwithrv` action to the smart contract and observe the returned value in the `cleos` output.
* Use other means (e.g. programmatically) to send the  `checkwithrv` action to the smart contract and observe the returned value in the action trace.

[[info | Returned values from actions availability]]
The returned values from actions are only available to the clients sending the action via the RPC API. Currently, there is no support for an inline action to be able to use the return value, because inline actions don't execute synchronously.
