#include <kv_addr_book.hpp>

void kv_addr_book::print_person(const person& person, bool new_line) {
   // when using "/n" in the string printed by eosio everything else
   // printed after this print for the current action will not be shown
   // in the console, therefore use it as the last print statement only.
   eosio::print_f(
      new_line 
         ? "Person found: {%, %, %, %, %, %, %}\n"
         : "Person found: {%, %, %, %, %, %, %}. ", 
      person.first_name, 
      person.last_name, 
      person.street,
      person.city,
      person.state,
      person.country,
      person.personal_id);
}

// retrieves a person based on primary key account_name
// we make use of index find function, which returns an iterator, 
//    and then use iterator value
[[eosio::action]]
person kv_addr_book::get(eosio::name account_name) {
   address_table addresses{"kvaddrbook"_n};

   // search for person by primary key account_name
   auto itr = addresses.account_name_uidx.find(account_name);

   // check if person was found
   if (itr != addresses.account_name_uidx.end()) {
      // extract person from iterator, print it and return it to the action sender
      const auto& person_found = itr.value();

      print_person(person_found);
      
      // return found person from action
      return person_found;
   }
   else {
      eosio::print_f("Person not found in addressbook.");
      // return empty person from action
      return person{};
   }
}

// retrieves a person based on unique index defined by country and personal_id
[[eosio::action]]
person kv_addr_book::getbycntrpid(std::string country, std::string personal_id) {
   address_table addresses{"kvaddrbook"_n};

   const auto& person_found = addresses.country_personal_id_uidx.get({country, personal_id});
   if (person_found.has_value()) {
      print_person(person_found.value());
      // return found person from action
      return person_found.value();
   }
   else {
      eosio::print_f("Person not found in addressbook.");
      // return empty person from action
      return person{};
   }
}

// retrieves list of persons with the same last name
[[eosio::action]]
std::vector<person> kv_addr_book::getbylastname(std::string last_name) {
   address_table addresses{"kvaddrbook"_n};

   std::string min_first_name("A");
   std::string max_first_name(100, 'z');
   eosio::name min_account_name{0};
   eosio::name max_account_name{UINT_MAX};

   auto list_of_persons = addresses.full_name_last_first_idx.range(
      {last_name, min_first_name, min_account_name},
      {last_name, max_first_name, max_account_name});

   eosio::print_f("Found % person(s). ", list_of_persons.size());

   for (auto& person : list_of_persons) {
      print_person(person, false);
   }
   // return found list of person from action
   return list_of_persons;
}

// retrieves list of persons with the same address
[[eosio::action]]
std::vector<person> kv_addr_book::getbyaddress(
   std::string street, std::string city, std::string state, std::string country) {
   address_table addresses{"kvaddrbook"_n};

   eosio::name min_account_name{0};
   eosio::name max_account_name{UINT_MAX};

   auto list_of_persons = addresses.address_idx.range(
      {street, city, state, country, min_account_name}, 
      {street, city, state, country, max_account_name});

   eosio::print_f("Found % person(s). ", list_of_persons.size());

   for (auto& person : list_of_persons) {
      print_person(person, false);
   }
   // return found list of persons from action
   return list_of_persons;
}

// creates if not exists, or updates if already exists, a person
[[eosio::action]]
void kv_addr_book::upsert(
      eosio::name account_name,
      std::string first_name,
      std::string last_name,
      std::string street,
      std::string city,
      std::string state, 
      std::string country,
      std::string personal_id) {
   address_table addresses{"kvaddrbook"_n};

   // create the person object which will be stored in kv::table
   const person& person_update = person_factory::get_person(
      account_name,
      first_name,
      last_name,
      street,
      city,
      state, 
      country,
      personal_id);

   // retreive the person by account name, if it doesn't exist we get an emtpy person
   const person& existing_person = get(person_update.account_name);

   // upsert into kv::table
   addresses.put(person_update, get_self());

   // print customized message for insert vs update
   if (existing_person.account_name.value == 0) {
      eosio::print_f("Person was successfully added to addressbook.");
   }
   else {
      eosio::print_f("Person was successfully updated in addressbook.");
   }
}

// deletes a person based on primary key account_name
[[eosio::action]]
void kv_addr_book::del(eosio::name account_name) {
   address_table addresses{"kvaddrbook"_n};

   // search for person by primary key account_name
   auto itr = addresses.account_name_uidx.find(account_name);

   // check if person was found
   if (itr != addresses.account_name_uidx.end()) {
      // extract person from iterator and delete it
      const auto& person_found = itr.value();

      // delete it from kv::table
      addresses.erase(person_found);
      eosio::print_f("Person was successfully deleted from addressbook.");
   }
   else {
      eosio::print_f("Person not found in addressbook.");
   }
}

// checks if a person exists in addressbook with a specific personal_id and country
[[eosio::action]]
bool kv_addr_book::checkpidcntr(std::string personal_id, std::string country) {
   address_table addresses{"kvaddrbook"_n};

   return addresses.country_personal_id_uidx.exists({personal_id, country});
}

// iterates over the first iterations_count persons in the table 
// and prints their first and last names
[[eosio::action]]
void kv_addr_book::iterate(int iterations_count) {
   address_table addresses{"kvaddrbook"_n};

   auto begin_itr = addresses.account_name_uidx.begin();
   auto end_itr = addresses.account_name_uidx.end();

   int current_iteration = 0;
   while (begin_itr != end_itr && current_iteration < iterations_count) {
      eosio::print_f(
         "Person found: {%, %}. ", 
         begin_itr.value().first_name,
         begin_itr.value().last_name);

      ++ begin_itr;
      ++ current_iteration;
   }
}
