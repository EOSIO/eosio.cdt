#include <eosio/eosio.hpp>
#include <eosio/table.hpp>


using namespace eosio;
using namespace std;

using fullname_t = std::tuple<std::string, std::string, eosio::name>;
using address_t = std::tuple<std::string, std::string, std::string, std::string, eosio::name>;
using country_personal_id_t = std::pair<std::string, std::string>;

// this structure defines the data stored in the kv::table
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

class [[eosio::contract]] kv_addr_book : public eosio::contract {

   struct [[eosio::table]] address_table : eosio::kv::table<person, "kvaddrbook"_n> {
      // unique indexes definitions
      // 1. they are defined for just one property of the kv::table parameter type (person)
      // 2. unique indexes for multiple properties of the kv::table parameter type
      //    are defined with the help of a pair or a tuple; a pair if the index has 
      //    two properties or a tuple in case of more than two
      index<name> account_name_uidx {
         name{"accname"_n},
         &person::account_name };
      index<country_personal_id_t> country_personal_id_uidx {
         name{"cntrypersid"_n},
         &person::country_personal_id };
      
      // non-unique indexes definitions
      // 1. non unique indexes need to be defined for at least two properties, 
      // 2. first properties, besides the last, are the ones wanted to be indexed non-uniquely
      // 3. the last one needs to be a property that stores unique values, because 
      //    under the hood every index (non-unique or unique) is stored as an unique 
      //    index, and by providing as the last property one that has unique values
      //    it ensures the uniques of the values combined (including non-unique ones)
      index<fullname_t> full_name_first_last_idx {
         name{"frstnameidx"_n},
         &person::full_name_first_last};
      index<fullname_t> full_name_last_first_idx {
         name{"lastnameidx"_n},
         &person::full_name_last_first};
      index<address_t> address_idx {
         name{"addressidx"_n},
         &person::address};

      address_table(eosio::name contract_name) {
         init(contract_name,
            account_name_uidx,
            country_personal_id_uidx,
            full_name_first_last_idx,
            full_name_last_first_idx,
            address_idx);
      }
   };

   public:
      using contract::contract;
      kv_addr_book(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds)
         : contract(receiver, code, ds) {}

      // retrieves a person based on primary key account_name
      [[eosio::action]]
      person get(eosio::name account_name);

      // retrieves a person based on unique index defined by country and personal_id
      [[eosio::action]]
      person getbycntrpid(std::string country, std::string personal_id);

      // retrieves list of persons with the same last name
      [[eosio::action]]
      std::vector<person> getbylastname(std::string last_name);

      [[eosio::action]]
      std::vector<person> getbyaddress(
         std::string street, 
         std::string city, 
         std::string state, 
         std::string country);

      // creates if not exists, or updates if already exists, a person
      [[eosio::action]]
      void upsert(eosio::name account_name,
         std::string first_name,
         std::string last_name,
         std::string street,
         std::string city,
         std::string state, 
         std::string country,
         std::string personal_id);

      // deletes a person based on primary key account_name
      [[eosio::action]]
      void del(eosio::name account_name);

      // checks if a person exists in addressbook with a specific personal_id and country
      [[eosio::action]]
      bool checkpidcntr(std::string personal_id, std::string country);

      // iterates over the first iterations_count persons in the table 
      // and prints their first and last names
      [[eosio::action]]
      void iterate(int iterations_count);

      using get_action = eosio::action_wrapper<"get"_n, &kv_addr_book::get>;
      using get_by_cntry_pers_id_action = eosio::action_wrapper<"getbycntrpid"_n, &kv_addr_book::getbycntrpid>;
      using get_by_last_name_action = eosio::action_wrapper<"getbylastname"_n, &kv_addr_book::getbylastname>;
      using get_buy_address_action = eosio::action_wrapper<"getbyaddress"_n, &kv_addr_book::getbyaddress>;
      using upsert_action = eosio::action_wrapper<"upsert"_n, &kv_addr_book::upsert>;
      using del_action = eosio::action_wrapper<"del"_n, &kv_addr_book::del>;
      using is_pers_id_in_cntry_action = eosio::action_wrapper<"checkpidcntr"_n, &kv_addr_book::checkpidcntr>;
      using iterate_action = eosio::action_wrapper<"iterate"_n, &kv_addr_book::iterate>;

   private:
      void print_person(const person& person, bool new_line = true);
      address_table addresses{"kvaddrbook"_n};
};
