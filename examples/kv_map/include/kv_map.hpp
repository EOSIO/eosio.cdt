#include <eosio/eosio.hpp>
#include <eosio/map.hpp>

using namespace eosio;
using namespace std;

using fullname_t = std::tuple<std::string, std::string, eosio::name>;
using address_t = std::tuple<std::string, std::string, std::string, std::string, eosio::name>;
using country_personal_id_t = std::pair<std::string, std::string>;

// this structure defines the data stored in the kv::map
struct person {
   eosio::name account_name;
   std::string first_name;
   std::string last_name;
   std::string street;
   std::string city;
   std::string state; 
   std::string country;
   std::string personal_id;

   // data members supporting the indexes built for this structure
   fullname_t full_name_first_last;
   fullname_t full_name_last_first;
   address_t address;
   country_personal_id_t country_personal_id;
};

// helper factory to easily build person objects
struct person_factory {
   static person get_person(
      eosio::name account_name,
      std::string first_name,
      std::string last_name,
      std::string street,
      std::string city,
      std::string state, 
      std::string country,
      std::string personal_id) {
         return person {
            .account_name = account_name,
            .first_name = first_name,
            .last_name = last_name,
            .street = street,
            .city = city,
            .state = state,
            .country = country,
            .personal_id = personal_id,
            .full_name_first_last = {first_name, last_name, account_name},
            .full_name_last_first = {last_name, first_name, account_name},
            .address = {street, city, state, country, account_name},
            .country_personal_id = {country, personal_id},
         };
      }
};

class [[eosio::contract]] kv_map : public eosio::contract {

   using my_map_t = eosio::kv::map<"kvmap"_n, int, person>;

   public:
      using contract::contract;
      kv_map(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds)
         : contract(receiver, code, ds) {}

      // retrieves a person based on map key
      [[eosio::action]]
      person get(int id);

      // creates if not exists, or updates if already exists, a person
      [[eosio::action]]
      void upsert(int id,
         eosio::name account_name,
         std::string first_name,
         std::string last_name,
         std::string street,
         std::string city,
         std::string state, 
         std::string country,
         std::string personal_id);

      // deletes a person based on primary key account_name
      [[eosio::action]]
      void del(int id);

      // checks if a person exists with a given personal_id and country
      [[eosio::action]]
      bool checkpidcntr(std::string personal_id, std::string country);

      // iterates over the first iterations_count persons in the table 
      // and prints their first and last names
      [[eosio::action]]
      void iterate(int iterations_count);

      using get_action = eosio::action_wrapper<"get"_n, &kv_map::get>;
      using upsert_action = eosio::action_wrapper<"upsert"_n, &kv_map::upsert>;
      using del_action = eosio::action_wrapper<"del"_n, &kv_map::del>;
      using is_pers_id_in_cntry_action = eosio::action_wrapper<"checkpidcntr"_n, &kv_map::checkpidcntr>;
      using iterate_action = eosio::action_wrapper<"iterate"_n, &kv_map::iterate>;

   private:
      void print_person(const person& person, bool new_line = true);

      my_map_t my_map{};
};
