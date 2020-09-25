#include <addressbook.hpp>

// creates if not exists, or updates if already exists, a person
[[eosio::action]]
std::pair<int, std::string> addressbook::upsert(
      name account_name,
      string first_name,
      string last_name,
      string street,
      string city,
      string state) {

   require_auth( account_name );
   kv_address_table addresses{"kvaddrbook"_n};

   std::pair<int, std::string> results = {0, "NOP"};

   // retreive the person by account name
   auto itr = addresses.account_name_uidx.find(account_name);

   // upsert into kv_table
   addresses.put({
         account_name, 
         first_name, 
         {account_name, last_name}, 
         street, 
         city, 
         state}, 
      get_self());

   // print customized message for insert vs update
   if (itr == addresses.account_name_uidx.end()) {
      print_f("Person was successfully added to addressbook.");
      results = {1, "New row created."};
   }
   else {
      print_f("Person was successfully updated in addressbook.");
      results = {2, "Existing row updated."};
   }
   return results;
}

// deletes a person based on primary key account_name
[[eosio::action]]
void addressbook::del(name account_name) {

   require_auth(account_name);
   kv_address_table addresses{"kvaddrbook"_n};

   // search for person by primary key account_name
   auto itr = addresses.account_name_uidx.find(account_name);

   // check if person was found
   if (itr != addresses.account_name_uidx.end()) {
      // extract person from iterator and delete it
      const auto& person_found = itr.value();

      // delete it from kv_table
      addresses.erase(person_found);
      print_f("Person was successfully deleted from addressbook.");
   }
   else {
      print_f("Person not found in addressbook.");
   }
}

// retrieves list of persons with the same last name
[[eosio::action]]
std::vector<person> addressbook::getbylastname(string last_name) {

   kv_address_table addresses{"kvaddrbook"_n};

   name min_account_name{0};
   name max_account_name{UINT_MAX};
   auto list_of_persons = addresses.last_name_idx.range(
      {min_account_name, last_name},
      {max_account_name, last_name});
      
   // return found list of person from action
   return list_of_persons;
}
