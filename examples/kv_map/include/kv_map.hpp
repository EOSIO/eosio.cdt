#include <eosio/eosio.hpp>
#include <eosio/map.hpp>

using namespace eosio;
using namespace std;

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
            .personal_id = personal_id
         };
      }
};

class [[eosio::contract]] kv_map : public eosio::contract {

   using my_map_t = eosio::kv::map<"kvmap"_n, int, person>;

   public:
      using contract::contract;
      kv_map(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds)
         : contract(receiver, code, ds) {}

      // retrieves a person based on map key.
      [[eosio::action]]
      person get(int id);

      // inserts a person if not exists, or updates it if already exists.
      // the payer for the resources consumed is the account that created the kv::map
      // object in the first place, the account that owns this smart contract.
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

      // same as upsert only that it takes a user defined type as input parameter
      [[eosio::action]]
      void upsert2(int id, person pers);

      // inserts a person if not exists, or updates it if already exists.
      // the payer is the account_name, specified as input parameter.
      [[eosio::action]]
      void upsertwpayer(int id,
         eosio::name account_name,
         std::string first_name,
         std::string last_name,
         std::string street,
         std::string city,
         std::string state, 
         std::string country,
         std::string personal_id);

      // deletes a person based on unique id
      [[eosio::action]]
      void erase(int id);

      // checks if a person exists with a given personal_id and country
      [[eosio::action]]
      bool checkpidcntr(std::string personal_id, std::string country);

      // iterates over the first iterations_count persons in the table using for loop
      // and prints their first and last names
      [[eosio::action]]
      void fiterate(int iterations_count);

      // iterates over the first iterations_count persons in the table using while loop
      // and prints their first and last names
      [[eosio::action]]
      void witerate(int iterations_count);

      using get_action = eosio::action_wrapper<"get"_n, &kv_map::get>;
      using upsert_action = eosio::action_wrapper<"upsert"_n, &kv_map::upsert>;
      using upsert_action2 = eosio::action_wrapper<"upsert2"_n, &kv_map::upsert2>;
      using upsertwpayer_action = eosio::action_wrapper<"upsertwpayer"_n, &kv_map::upsertwpayer>;
      using erase_action = eosio::action_wrapper<"erase"_n, &kv_map::erase>;
      using is_pers_id_in_cntry_action = eosio::action_wrapper<"checkpidcntr"_n, &kv_map::checkpidcntr>;
      using witerate_action = eosio::action_wrapper<"witerate"_n, &kv_map::witerate>;
      using fiterate_action = eosio::action_wrapper<"fiterate"_n, &kv_map::fiterate>;

   private:
      void print_person(const person& person, bool new_line = true);

      my_map_t my_map{};
};
