## How to define a singleton

This is how you define a simple singleton, which is storing an account name as primary data and a uint64_t as secondary data in structure test table; the structure testtable can be extended to be defined by multiple data members, here we defined only to for demonstration purposes.

```cpp
#include <eosio/eosio.hpp>
#include <eosio/name.hpp>
#include <eosio/singleton.hpp>
using namespace eosio;

CONTRACT singleton_example : public contract {
   public:
      using contract::contract;
      singleton_example( name receiver, name code, datastream<const char*> ds ) :
         contract(receiver, code, ds),
         singleton_instance(receiver, receiver.value)
         {}

      ACTION set( name user, uint64_t value );
      ACTION get( );

      TABLE testtable {
         name primary_value;
         uint64_t secondary_value;
      };

      using singleton_type = eosio::singleton<"testtable"_n, testtable>;
      singleton_type singleton_instance;

      using set_action = action_wrapper<"set"_n, &singleton_example::set>;
      using get_action = action_wrapper<"get"_n, &singleton_example::get>;
};
```

And this is a possible implementation for the two `get` and `set` actions. Note that the `set` action makes use of the singleton's set method which has as second parameter the account to pay for the new value, in this case, the same account name that is stored in the primary value, however, it can be a different account.

```cpp
   #include <singleton_example.hpp>

   ACTION singleton_example::set( name user, uint64_t value ) {
      auto entry_stored = singleton_instance.get();
      entry_stored.primary_value = user;
      entry_stored.secondary_value = value;
      singleton_instance.set(entry_stored, user);
   }

   ACTION singleton_example::get( ) {
   eosio::print("Value stored for: {%} is {%}\n", 
      singleton_instance.get().primary_value.value,
      singleton_instance.get().secondary_value);
   }
```