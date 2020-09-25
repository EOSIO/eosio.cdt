#include <kv_addr_book.hpp>

void kv_addr_book::print_person(const person& person) {
   print_f(
      "Person found: {%, %, %, %, %, %, %s}\n", 
      person.get_first_name(), 
      person.get_last_name(), 
      person.get_street(),
      person.get_city(),
      person.get_state(),
      person.get_country(),
      person.get_personal_id());
}

// retrieves a person based on primary key account_name
// we make use of index find function, which returns an iterator, 
//    and then use iterator value
[[eosio::action]]
person kv_addr_book::get(name account_name) {
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
      print_f("Person not found in addressbook.");
      // return empty person from action
      return person{};
   }
}

// retrieves a person based on unique index defined by country and personal_id
[[eosio::action]]
person kv_addr_book::getbycntrpid(string country, string personal_id) {
   address_table addresses{"kvaddrbook"_n};

   const auto& person_found = addresses.country_personal_id_uidx.get({country, personal_id});
   if (person_found.has_value()) {
      print_person(person_found.value());
      // return found person from action
      return person_found.value();
   }
   else {
      print_f("Person not found in addressbook.");
      // return empty person from action
      return person{};
   }
}

// retrieves list of persons with the same last name
[[eosio::action]]
vector<person> kv_addr_book::getbylastname(string last_name) {
   address_table addresses{"kvaddrbook"_n};

   name min_account_name{0};
   name max_account_name{UINT_MAX};
   auto list_of_persons = addresses.last_name_idx.range(
      {min_account_name, last_name},
      {max_account_name, last_name});
   // return found list of person from action
   return list_of_persons;
}

// retrieves list of persons with the same address
[[eosio::action]]
vector<person> kv_addr_book::getbyaddress(
   string street, string city, string state, string country) {
   address_table addresses{"kvaddrbook"_n};

   name min_account_name{0};
   name max_account_name{UINT_MAX};
   auto list_of_persons = addresses.street_city_state_cntry.range(
      {min_account_name, street, city, state, country}, 
      {max_account_name, street, city, state, country});
   // return found list of person from action
   return list_of_persons;
}

// creates if not exists, or updates if already exists, a person
[[eosio::action]]
void kv_addr_book::upsert(
      name account_name,
      string first_name,
      string last_name,
      string street,
      string city,
      string state, 
      string country,
      string personal_id) {
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
   const person& existing_person = get(person_update.get_account_name());

   // upsert into kv::table
   addresses.put(person_update, get_self());

   // print customized message for insert vs update
   if (existing_person.get_account_name().value == 0) {
      print_f("Person was successfully added to addressbook.");
   }
   else {
      print_f("Person was successfully updated in addressbook.");
   }
}

// deletes a person based on primary key account_name
[[eosio::action]]
void kv_addr_book::del(name account_name) {
   address_table addresses{"kvaddrbook"_n};

   // search for person by primary key account_name
   auto itr = addresses.account_name_uidx.find(account_name);

   // check if person was found
   if (itr != addresses.account_name_uidx.end()) {
      // extract person from iterator and delete it
      const auto& person_found = itr.value();

      // delete it from kv::table
      addresses.erase(person_found);
      print_f("Person was successfully deleted from addressbook.");
   }
   else {
      print_f("Person not found in addressbook.");
   }
}

// checks if a person exists in addressbook with a specific personal_id and country
[[eosio::action]]
bool kv_addr_book::checkpidcntr(string personal_id, string country) {
   address_table addresses{"kvaddrbook"_n};

   return addresses.country_personal_id_uidx.exists({personal_id, country});
}
