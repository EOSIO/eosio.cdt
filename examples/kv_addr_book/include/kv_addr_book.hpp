#include <eosio/eosio.hpp>
using namespace std;
using namespace eosio;

// this structure defines the data stored in the kv_table
struct person {
   eosio::name account_name;
   eosio::non_unique<eosio::name, string> first_name;
   eosio::non_unique<eosio::name, string> last_name;
   eosio::non_unique<eosio::name, string, string, string, string> street_city_state_cntry;
   eosio::non_unique<eosio::name, string> personal_id;
   std::pair<string, string> country_personal_id;

   eosio::name get_account_name() const {
      return account_name;
   }
   string get_first_name() const {
      // from the eosio::non_unique tuple we extract the value with key 1, the first name
      return get<1>(first_name);
   }
   string get_last_name() const {
      // from the eosio::non_unique tuple we extract the value with key 1, the last name
      return get<1>(last_name);
   }
   string get_street() const {
      // from the eosio::non_unique tuple we extract the value with key 1, the street
      return get<1>(street_city_state_cntry);
   }
   string get_city() const {
      // from the eosio::non_unique tuple we extract the value with key 2, the city
      return get<2>(street_city_state_cntry);
   }
   string get_state() const {
      // from the eosio::non_unique tuple we extract the value with key 3, the state
      return get<3>(street_city_state_cntry);
   }
   string get_country() const {
      // from the eosio::non_unique tuple we extract the value with key 4, the country
      return get<4>(street_city_state_cntry);
   }
   string get_personal_id() const {
      // from the eosio::non_unique tuple we extract the value with key 1, the personal id
      return get<1>(personal_id);
   }
};

// helper factory to easily build person objects
struct person_factory {
   static person get_person(
      eosio::name account_name,
      string first_name,
      string last_name,
      string street,
      string city,
      string state, 
      string country,
      string personal_id) {
         return person {
            .account_name = account_name,
            .first_name = {account_name, first_name},
            .last_name = {account_name, last_name},
            .street_city_state_cntry = {account_name, street, city, state, country},
            .personal_id = {account_name, personal_id},
            .country_personal_id = {country, personal_id}
         };
      }
};

class [[eosio::contract]] kv_addr_book : public eosio::contract {

   [[eosio::table]]
   struct address_table : kv_table<person> {
      // unique indexes definitions
      // 1. they are defined for just one property of the kv_table parameter type (person)
      // 2. unique indexes for multiple properties of the kv_table parameter type
      //    are defined with the help of a pair or a tuple; a pair if the index has 
      //    two properties or a tuple in case of more than two
      index<name>   account_name_uidx {"accname"_n, &person::account_name};
      index<pair<string, string>> country_personal_id_uidx {"cntrypersid"_n, &person::country_personal_id};
      
      // non-unique indexes definitions
      // 1. non unique indexes need to be defined for at least two properties, 
      // 2. the first one needs to be a property that stores unique values, because 
      //    under the hood every index (non-unique or unique) is stored as an unique 
      //    index, and by providing as the first property one that has unique values
      //    it ensures the uniques of the values combined (including non-unique ones)
      // 3. the rest of the properties are the ones wanted to be indexed non-uniquely
      index<non_unique<eosio::name, string>> first_name_idx {
         "firstname"_n, &person::first_name};
      index<non_unique<eosio::name, string>> last_name_idx {
         "lastname"_n, &person::last_name};
      index<non_unique<eosio::name, string>> personal_id_idx {
         "persid"_n, &person::personal_id};
      // non-unique index defined using the KV_NAMED_INDEX macro
      // note: you can not name your index like you were able to do before (ending in `_idx`),
      // instead when using the macro you have to pass the name of the data member which 
      // is indexed and that will give the name of the index as well
      KV_NAMED_INDEX("address"_n, street_city_state_cntry)

      address_table(eosio::name contract_name) {
         init(contract_name, 
            "addrtable"_n, 
            eosio::kv_ram, 
            account_name_uidx, 
            country_personal_id_uidx, 
            first_name_idx, 
            last_name_idx,
            personal_id_idx,
            street_city_state_cntry);
      }
   };

   public:
      using contract::contract;
      kv_addr_book(name receiver, name code, datastream<const char*> ds)
         : contract(receiver, code, ds) {}

      // retrieves a person based on primary key account_name
      [[eosio::action]]
      person get(name account_name);

      // retrieves a person based on unique index defined by country and personal_id
      [[eosio::action]]
      person getbycntrpid(string country, string personal_id);

      // retrieves list of persons with the same last name
      [[eosio::action]]
      std::vector<person> getbylastname(string last_name);

      // retrieves list of persons with the same address
      [[eosio::action]]
      std::vector<person> getbyaddress(string street, string city, string state, string country);

      // creates if not exists, or updates if already exists, a person
      [[eosio::action]]
      void upsert(eosio::name account_name,
         string first_name,
         string last_name,
         string street,
         string city,
         string state, 
         string country,
         string personal_id);

      // deletes a person based on primary key account_name
      [[eosio::action]]
      void del(name account_name);

      // checks if a person exists in addressbook with a specific personal_id and country
      [[eosio::action]]
      bool checkpidcntr(string personal_id, string country);

      using get_action = action_wrapper<"get"_n, &kv_addr_book::get>;
      using get_by_cntry_pers_id_action = action_wrapper<"getbycntrpid"_n, &kv_addr_book::getbycntrpid>;
      using get_by_last_name_action = action_wrapper<"getbylastname"_n, &kv_addr_book::getbylastname>;
      using get_buy_address_action = action_wrapper<"getbyaddress"_n, &kv_addr_book::getbyaddress>;
      using upsert_action = action_wrapper<"upsert"_n, &kv_addr_book::upsert>;
      using del_action = action_wrapper<"del"_n, &kv_addr_book::del>;
      using is_pers_id_in_cntry_action = action_wrapper<"checkpidcntr"_n, &kv_addr_book::checkpidcntr>;

   private:
      void print_person(const person& person);

      address_table addresses{"kvaddrbook"_n};
};
