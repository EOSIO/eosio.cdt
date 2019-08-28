## How to restrict access to an action by user

Prerequisites: https://developers.eos.io/eosio-home/docs/your-first-contract

The below code is enforcing the action `hi` to be executed only by the account that is sent as paramater to the action, no matter what permission the account is using to sign the transaction (e.g. owner, active, code).

```cpp
void hi( name user ) {
   require_auth( user );
   print( "Hello, ", name{user} );
}
```

The below code is enforcing the action hi to be executed only by the account that is sent as paramater to the action and only if the permission used to sign the transaction is the 'active' one, that is, if the same user is signing the transaction with a different permission (e.g. code, owner) the execution of the action is halted.

```cpp
void hi( name user ) {
   require_auth2(user, N(active));
   print( "Hello, ", name{user} );
}
```