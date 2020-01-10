---
content_title: How to define a singleton
---

To define a simple singleton, which is storing an account name as primary value and a uint64_t as secondary value in structure `testtable`, follow the steps below:

1. Include the `eosio.hpp` and `singleton.hpp` headers and declare the `eosio` namespace usage
```
#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>
using namespace eosio;
```

2. Define the data structure for the multi index table
```cpp
struct [[eosio::table]] testtable {
   name primary_value;
   uint64_t secondary_value;
};
```

3. For ease of use, define a type alias `singleton_type` based on the `eosio::singleton` template type, parametarized with a random name `"testsingletona"` and the `testtable` data structure defined above
```diff
struct [[eosio::table]] testtable {
   name primary_value;
   uint64_t secondary_value;
};
+using singleton_type = eosio::singleton<"testsingletona"_n, testtable>;
```

4. Define the singleton table instance declared as a data member of type `singleton_type` defined in the privious step
```diff
struct [[eosio::table]] testtable {
   name primary_value;
   uint64_t secondary_value;
};

using singleton_type = eosio::singleton<"testsingletona"_n, testtable>;
+singleton_type singleton_instance;
```

5. Instantiate the data member `singleton_instance` by passing to its constructor the `receiver` and the `code` (in this case `receiver.value`) parameters; these two combined with "testsingletona" provide access to the partition of the RAM cache used by this singleton. In this example you will initialize the `singleton_instance` data member in the smart contract constructor, see below:
```diff
// singleton contract constructor
singleton_example( name receiver, name code, datastream<const char*> ds ) :
   contract(receiver, code, ds),
+  singleton_instance(receiver, receiver.value)
   { }
}
```

Now you have defined and instantiated a singleton. Below you can find a possible implementation for the full class singleton example contract.

__singleton_example.hpp__
```cpp
#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>
using namespace eosio;

class [[eosio::contract]] singleton_example : public contract {
   public:
      using contract::contract;
      singleton_example( name receiver, name code, datastream<const char*> ds ) :
         contract(receiver, code, ds),
         singleton_instance(receiver, receiver.value)
         { }

      [[eosio::action]] void set( name user, uint64_t value );
      [[eosio::action]] void get( );

      struct [[eosio::table]] testtable {
         name primary_value;
         uint64_t secondary_value;
      } tt;

      using singleton_type = eosio::singleton<"testsingletona"_n, testtable>;
      singleton_type singleton_instance;

      using set_action = action_wrapper<"set"_n, &singleton_example::set>;
      using get_action = action_wrapper<"get"_n, &singleton_example::get>;
};
```

And below is a possible implementation for the two `get` and `set` actions defined above. It also demonstrates the usage of a couple of singleton methods. Note that the `set` action makes use of the singleton's `set` method, for which parameter is the account to pay for the new value stored. In this case, the same account name that is stored in the primary value is the payer. However, it can be a different account if so required.

__singleton_example.cpp__
```cpp
#include <singleton_example.hpp>

[[eosio::action]] void singleton_example::set( name user, uint64_t value ) {
   if (!singleton_instance.exists())
   {
      singleton_instance.get_or_create(user, tt);
   }
   auto entry_stored = singleton_instance.get();
   entry_stored.primary_value = user;
   entry_stored.secondary_value = value;
   singleton_instance.set(entry_stored, user);
}

[[eosio::action]] void singleton_example::get( ) {
   if (singleton_instance.exists())
      eosio::print(
         "Value stored for: ", 
         name{singleton_instance.get().primary_value.value},
         " is ",
         singleton_instance.get().secondary_value,
         "\n");
   else
      eosio::print("Singleton is empty\n");
}
```


[[info | Full example location]]
| A full example project demonstrating the instantiation and usage of singleton can be found [here](https://github.com/EOSIO/eosio.cdt/tree/master/examples/singleton_example).
