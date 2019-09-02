## How to define a singleton

This an example of how you can define a simple singleton, which is storing an account name as primary value and a uint64_t as secondary value in structure `testtable`; the structure testtable can be extended to be defined by multiple data members, here we defined only two for demonstration purposes.

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

And below is a possible implementation for the two `get` and `set` actions defined above and demonstrate the usage of a couple of singleton methods. Note that the `set` action makes use of the singleton's `set` method for which parameter is the account to pay for the new value stored, in this case, the same account name that is stored in the primary value, however, it can be a different account if the so required.

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