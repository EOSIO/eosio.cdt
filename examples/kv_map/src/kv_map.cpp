#include <kv_map.hpp>

void kv_map::print_person(const person& person, bool new_line) {
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

// retrieves a person based on unique id
[[eosio::action]]
person kv_map::get(int id) {

   auto itr = my_map.find(id);

   // check if person was found
   if (itr != my_map.end()) {
      // extract person from iterator, print it and return it to the action sender
      const auto& person_found = itr->second();

      print_person(person_found, false);
      
      // return found person from action
      return person_found;
   }
   else {
      eosio::print_f("Person with ID % not found.", id);
      // return empty person from action
      return person{};
   }
}

// inserts a person if not exists, or updates it if already exists
[[eosio::action]]
void kv_map::upsert(
      int id,
      eosio::name account_name,
      std::string first_name,
      std::string last_name,
      std::string street,
      std::string city,
      std::string state, 
      std::string country,
      std::string personal_id) {

   // create the person object which will be stored in kv::map
   const person& person_update = person_factory::get_person(
      account_name,
      first_name,
      last_name,
      street,
      city,
      state, 
      country,
      personal_id);

   // retrieve the person by account name, if it doesn't exist we get an emtpy person
   const person& existing_person = get(id);

   // upsert into kv::map
   my_map[id] = person_update;

   // print customized message for insert vs update
   if (existing_person.account_name.value == 0) {
      eosio::print_f("Person (%, %, %) was successfully added.",
         person_update.first_name, person_update.last_name, person_update.personal_id);
   }
   else {
      eosio::print_f("Person with ID % was successfully updated.", id);
   }
}

// deletes a person based on unique id
[[eosio::action]]
void kv_map::del(int id) {

   // search for person by unique id
   auto itr = my_map.find(id);

   // check if person was found
   if (itr != my_map.end()) {
      // extract person from iterator and delete it
      const auto person_found = itr->second();

      // delete it from kv::map
      my_map.erase(id);
      eosio::print_f("Person (%, %, %) was successfully deleted.",
         person_found.first_name, person_found.last_name, person_found.personal_id);
   }
   else {
      eosio::print_f("Person with ID % not found.", id);
   }
}

// checks if a person exists with a given personal_id and country
[[eosio::action]]
bool kv_map::checkpidcntr(std::string personal_id, std::string country) {
   
   for ( const auto& person_detail : my_map ) {
      if (person_detail.second().country == country && 
          person_detail.second().personal_id == personal_id) {
         eosio::print_f("Person with personal id % and country % found in db with ID %d.",
                        personal_id, 
                        country, 
                        person_detail.key);
         return true;
      }
   }
   eosio::print_f("Person with personal id % not found in country %.", personal_id, country);
   return false;
}

// iterates over the first iterations_count persons using while loop
// and prints their first and last names
[[eosio::action]]
void kv_map::witerate(int iterations_count) {

   auto begin_itr = my_map.begin();
   auto end_itr = my_map.end();

   int current_iteration = 0;
   while (begin_itr != end_itr && current_iteration < iterations_count) {
      eosio::print_f(
         "Person %: {%, %}. ",
         current_iteration + 1,
         begin_itr->second().first_name,
         begin_itr->second().last_name);

      ++ begin_itr;
      ++ current_iteration;
   }
}

// iterates over the first iterations_count persons using for loop
// and prints their first and last names
[[eosio::action]]
void kv_map::fiterate(int iterations_count) {

   int current_iteration = 0;
   for ( const auto& person_detail : my_map ) {
      if (++ current_iteration < iterations_count) {
         eosio::print_f(
            "Person %: {%, %}. ",
            current_iteration + 1,
            person_detail.second().first_name,
            person_detail.second().last_name);
      }
      else {
         break;
      }
   }
}