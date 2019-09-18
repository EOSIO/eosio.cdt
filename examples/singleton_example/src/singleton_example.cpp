   #include <singleton_example.hpp>

   [[eosio::action]] void singleton_example::set( name user, uint64_t value ) {
      auto entry_stored = singleton_instance.get();
      entry_stored.primary_value = user;
      entry_stored.secondary_value = value;
      singleton_instance.set(entry_stored, user);
   }

   [[eosio::action]] void singleton_example::get( ) {
   eosio::print("Value stored for: {%} is {%}\n", 
      singleton_instance.get().primary_value.value,
      singleton_instance.get().secondary_value);
   }
