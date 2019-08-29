## How to create and use action wrappers

### Create action wrappers
One of the good examples for creating action wrappers is the `eosio.token` contract [code](https://github.com/EOSIO/eosio.contracts/blob/867f6f291db05cb663505f21ba8f7c3adca20678/contracts/eosio.token/include/eosio.token/eosio.token.hpp#L150).

This is how are defined the action wrappers for the following actions `create`, `issue`, `transfer` in token contract:

```cpp
[[eosio::action]]
void create( name issuer, asset  maximum_supply );

[[eosio::action]]
void issue( name to, asset quantity, string memo );

[[eosio::action]]
void transfer( name    from,
              name    to,
              asset   quantity,
              string  memo );

// The first parameter of the action wrapper template is the action eosio::name and second parameter is the reference to the action method
using create_action = eosio::action_wrapper<"create"_n, &token::create>;
using issue_action = eosio::action_wrapper<"issue"_n, &token::issue>;
using transfer_action = eosio::action_wrapper<"transfer"_n, &token::transfer>;

// ...
```
__Note__: // The first parameter of the action wrapper template is the action eosio::name and second parameter is the reference to the action method.


### Use action wrappers
And this is how you use the previously created action wrapper for `transfer` action:
```cpp
#include <eosio_token/include/eosio_token.hpp>

// specify the contract to send the action to as the first argument
token::transfer_action payout("eosio.token"_n, {get_self(), "active"_n});

// specify the transfer arguments as postional arguments
payout.send(get_self(), to, quantity, memo);
```

__Note:__ You have to include the header file where the action wrappers are defined.