## How to restrict access to an action by user

Prerequisites: https://developers.eos.io/eosio-home/docs/your-first-contract

The below code is enforcing the action hi to be executed only by the same user that is sent as paramater to the action.

```cpp
void hi( name user ) {
   require_auth( user );
   print( "Hello, ", name{user} );
}
```

The below code is enforcing the action hi to be executed only by the same user that is sent as paramater to the action and only if the active key is provided, that is, if the same user is signing the transaction with a different key (e.g. code, owner) the execution of the action is halted.

```cpp
void hi( name user ) {
   require_auth2(user, N(active));
   print( "Hello, ", name{user} );
}
```