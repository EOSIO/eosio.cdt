#include <singleton_example.hpp>

[[eosio::action]]
void singleton_example::set( name user, uint64_t value ) {
   if (!singleton_instance.exists())
   {
      singleton_instance.get_or_create(user, tt);
   }
   auto entry_stored = singleton_instance.get();
   entry_stored.primary_value = user;
   entry_stored.secondary_value = value;
   singleton_instance.set(entry_stored, user);
}

[[eosio::action]]
void singleton_example::get( ) {
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
