---
content_title: How To Perform Authorization Checks
link_text: How To Perform Authorization Checks
---

## Overview

This guide provides instructions how to perform authorization checks in a smart contract.

## Before you begin

1. You have the sources of a contract with a `hi` action defined and implemented.
2. The `hi` action has defined one input parameter `user` of type `name`.

## Code Reference

See the following code reference guides for functions which can be used to implement authorization checks in a smart contract:

* function [has_auth(name n)](https://developers.eos.io/manuals/eosio.cdt/v1.8/namespaceeosio#function-has_auth)
* function [require_auth(name n)](https://developers.eos.io/manuals/eosio.cdt/v1.8/namespaceeosio/#function-require_auth-12)
* function [require_auth2(capi_name name, capi_name permission)](https://developers.eos.io/manuals/eosio.cdt/v1.8/group__action__c#function-require_auth2)
* function [check(bool pred, ...)](https://developers.eos.io/manuals/eosio.cdt/v1.8/group__system/#function-check)

## Procedure

The following steps show how to check authorization for `user` account for the `hi` action. There are three ways to accomplish an authorization check in a smart contract action implementation. You can use any of the methods provided below depending on your needs:

* [Use check(...) in combination with has_auth(...)](#1-use-checkhas_auth)
* [Use require_auth(...)](#2-use-require_auth)
* [Use require_auth2(...)](#3-use-require_auth2)

### 1. Use check(has_auth(...)...)

The following code example enforces the action `hi` to be executed only by the account that is sent as parameter to the action, no matter what permission the account uses to sign the transaction (e.g. owner, active, code).

[[info | Error message is custom]]
| Observe that in this case the yielded error message is a custom one and thus it can be used to provide a better experience for the user.

```cpp
#include <capi/eosio/action.h>

void hi( name user ) {
   check(has_auth(user), "User is not authorized to perform this action.");
   print( "Hello, ", name{user} );
}
```

Another example can be found in the [Tic Tac Toe Tutorial](https://developers.eos.io/welcome/latest/tutorials/tic-tac-toe-game-contract/#action-handler---move).

### 2. Use require_auth

The below code enforces the action `hi` to be executed only by the account that is sent as parameter to the action, no matter what permission the account uses to sign the transaction (e.g. owner, active, code).

```cpp
void hi( name user ) {
   require_auth( user );
   print( "Hello, ", name{user} );
}
```

[[info | Error message is not custom]]
| Note that this time you can not customize the yielded error message, it will be a generic authorization error message.

### 3. Use require_auth2

The below code is enforces the action `hi` to be executed only by the account that is sent as parameter to the action and only if the permission used to sign the transaction is the 'active' one. In other words, if the same user uses the transaction with a different permission (e.g. code, owner) the execution of the action is halted.

```cpp
#include <capi/eosio/action.h>

void hi( name user ) {
   require_auth2(user.value, "active"_n.value);
   print( "Hello, ", name{user} );
}
```

[[info | Error message is not custom]]
| Note that this time, as well as previous method, you can not customize the yielded error message, it will be a generic authorization error message.

## Summary

In conclusion, the above instructions show how to perform authorization checks in a smart contract.
