---
content_title: How to define a singleton
---

## Overview

This guide provides instructions to define a singleton.

## Reference

See the following code reference:

* The [`singleton`](../../classeosio_1_1singleton) class.

## Before you begin

Make sure you have the following prerequisites in place:

* An EOSIO development environment, for details consult the [Get Started Guide](https://developers.eos.io/welcome/latest/getting-started-guide/index),

## Procedure

A singleton uses a single multi-index table to store named objects of various types. To define a simple singleton, which is storing an account `name` as primary value and a `uint64_t` as secondary value in structure `testtable`, follow the steps below:

### 1. Preparation And Initialization

Include the `eosio.hpp` and `singleton.hpp` headers and declare the `eosio` namespace usage

   ```cpp
   #include <eosio/eosio.hpp>
   #include <eosio/singleton.hpp>
   using namespace eosio;
   ```

### 2. Define The Table Data Structure

Define the data structure for the multi-index table:

   ```cpp
   struct [[eosio::table]] testtable {
      name primary_value;
      uint64_t secondary_value;
   };
   ```

### 3. Define A Singleton Type Alias

For ease of use, define a type alias `singleton_type` based on the `eosio::singleton` template type, parametarized with a random name `"testtable"` and the `testtable` data structure. The names must adhere to `EOSIO` account name restrictions.

   ```diff
   struct [[eosio::table]] testtable {
      name primary_value;
      uint64_t secondary_value;
   };
   +using singleton_type = eosio::singleton<"testtable"_n, testtable>;
   ```

### 4. Define The Singleton Instance

Define the singleton table instance as a data member of type `singleton_type`.

   ```diff
   struct [[eosio::table]] testtable {
      name primary_value;
      uint64_t secondary_value;
   };

   using singleton_type = eosio::singleton<"testtable"_n, testtable>;
   +singleton_type singleton_instance;
   ```

### 5. Initialize And Use The Singleton Instance

Initialize the `singleton_instance` using the constructor with the parameters `receiver` and `code` (the last one in in this case is `receiver.value`). These parameters, combined with `testtable`, provide access to the partition of the RAM cache used by this singleton. In our example you initialize the `singleton_instance` data member in the smart contract constructor, see below:

   ```diff
   // singleton contract constructor
   singleton_example( name receiver, name code, datastream<const char*> ds ) :
      contract(receiver, code, ds),
   +  singleton_instance(receiver, receiver.value)
      { }
   }
   ```

Now you have defined and initialized a singleton as a data member for the smart contract class. You can access it from any of the smart contract methods via `singleton_instance` data member. Below you can find a possible implementation for the full class singleton example contract.

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
         {}

      [[eosio::action]]
      void set( name user, uint64_t value );
      [[eosio::action]]
      void get( );

      struct [[eosio::table]] testtable {
         name primary_value;
         uint64_t secondary_value;
         uint64_t primary_key() const { return primary_value.value; }
      } testtablerow;

      using singleton_type = eosio::singleton<"testtable"_n, testtable>;
      singleton_type singleton_instance;

      using set_action = action_wrapper<"set"_n, &singleton_example::set>;
      using get_action = action_wrapper<"get"_n, &singleton_example::get>;
};
```

Below is an example for the `get` and `set` actions. It also demonstrates the usage of the `get` and `set` singleton methods. Note that the `set` action makes use of the singleton's `set` method, for which the second parameter is the payer account for the RAM needed to store the new value.

__singleton_example.cpp__

```cpp
#include <singleton_example.hpp>

[[eosio::action]] void singleton_example::set( name user, uint64_t value ) {
   auto entry_stored = singleton_instance.get_or_create(user, testtablerow);
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

## Summary

In conclusion, the above instructions show how to define a singleton.

## Next Steps

* Singleton uses as underlying structure a multi-index table therefore you can [iterate and retrieve data](./how-to-iterate-and-retrieve-a-multi_index-table) from the singleton the same way you would with a multi-index table.
