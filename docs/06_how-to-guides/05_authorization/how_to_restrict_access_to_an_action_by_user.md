---
content_title: How to restrict access to an action by a user
---

## Preconditions
- It is assumed you have the sources for a contract and one of the actions defined is getting as a parameter an account name and it is printing the account name.

To restrict access to the `hi` action, you can do it in two ways:

1. Using require_auth
The below code is enforcing the action `hi` to be executed only by the account that is sent as parameter to the action, no matter what permission the account is using to sign the transaction (e.g. owner, active, code).

```cpp
void hi( name user ) {
   require_auth( user );
   print( "Hello, ", name{user} );
}
```

2. Or using require_auth2

The below code is enforcing the action `hi` to be executed only by the account that is sent as parameter to the action and only if the permission used to sign the transaction is the 'active' one. In other words, if the same user is signing the transaction with a different permission (e.g. code, owner) the execution of the action is halted.

```cpp
#include <capi/eosio/action.h>

void hi( name user ) {
   require_auth2(nm.value, "active"_n.value);
   print( "Hello, ", name{user} );
}
```

An example of this contract can be found [here](https://github.com/EOSIO/eosio.cdt/blob/master/examples/hello/src/hello.cpp)
