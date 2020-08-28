#include <kv_addr_book.hpp>

void kv_addr_book::print_person(const person& person) {
   eosio::print_f(
      "Person found: {%, %, %, %, %, %, %s}\n", 
      person.get_first_name(), 
      person.get_last_name(), 
      person.get_street(),
      person.get_city(),
      person.get_state(),
      person.get_country(),
      person.get_personal_id());
}

[[eosio::action]]
person kv_addr_book::get(name account_name) {
   address_table addresses{"kvaddrbook"_n};

   auto itr = addresses.account_name_uidx.find(account_name);
   if (itr != addresses.account_name_uidx.end()) {
      const auto& person_found = itr.value();
      print_person(person_found);
      return person_found;
   }
   else {
      eosio::print_f("Person not found in addressbook.");
      return person{};
   }
}

[[eosio::action]] 
person kv_addr_book::getby(string country, string personal_id) {
   address_table addresses{"kvaddrbook"_n};

   auto itr = addresses.country_personal_id_uidx.find({country, personal_id});
   if (itr != addresses.country_personal_id_uidx.end()) {
      const auto& person_found = itr.value();
      print_person(person_found);
      return person_found;
   }
   else {
      eosio::print_f("Person not found in addressbook.");
      return person{};
   }
}

[[eosio::action]] 
void kv_addr_book::upsert(
      eosio::name account_name,
      string first_name,
      string last_name,
      string street,
      string city,
      string state, 
      string country,
      string personal_id) {
   address_table addresses{"kvaddrbook"_n};

   const person& person_update = person_factory::get_person(
      account_name,
      first_name,
      last_name,
      street,
      city,
      state, 
      country,
      personal_id);

   const person& existing_person = get(person_update.get_account_name());
   addresses.put(person_update, get_self());

   if (existing_person.get_account_name().value == 0) {
      eosio::print_f("Person was successfully added to addressbook.");
   }
   else {
      eosio::print_f("Person was successfully updated in addressbook.");
   }
}

[[eosio::action]]
void kv_addr_book::del(person user) {
   address_table addresses{"kvaddrbook"_n};

   addresses.erase(user);
   eosio::print_f("Person was successfully deleted.");
}
