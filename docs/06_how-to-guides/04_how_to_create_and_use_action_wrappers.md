## How to create and use action wrappers

1. Let's assume you have a contract `multi_index_example` with an action `mod` defined like below in file `multi_index_example.hpp`, it modifies
```cpp
class [[eosio::contract]] multi_index_example : public contract {
  // ...
  [[eosio::action]] void mod( name user, uint32_t n );
  // ...
}
```
2. To define an action wrapper for the `mod` action, make use of the `eosio::action_wrapper` template, with  the first parameter the action name as a `eosio::name` and second parameter as the reference to the action method
```diff
class [[eosio::contract]] multi_index_example : public contract {
  // ...
  [[eosio::action]] void mod(name user);
  // ...
+  using mod_action = action_wrapper<"mod"_n, &multi_index_example::mod>;
  // ...
}
```
3. To use the action wrapper, you have to include the header file where the action wrapper is defined
```cpp
#include <multi_index_example.hpp>
```
4. And then instantiate the `mod_action` defined above specifying the contract to send the action to as the first argument, in this case we assume the contract is deployed to `multiindexex` account, and a stucture which is defined by self account and the `active` permission, you can modify these based on your requirements.
```diff
#include <multi_index_example.hpp>

+multi_index_example::mod_action modaction("multiindexex"_n, {get_self(), "active"_n});
```
5. And finally call the `send` method of the action wrapper and pass in the `mod` action's parameters as positional arguments
```diff
#include <multi_index_example.hpp>

multi_index_example::mod_action modaction("multiindexex"_n, {get_self(), 1});

+modaction.send(get_self(), "eostutorial"_n, 1, memo);
```

For a full example see the [`multi_index` contract implementation](https://github.com/EOSIO/eosio.cdt/tree/master/examples/multi_index_example).
