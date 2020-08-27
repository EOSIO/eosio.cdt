#include <eosio/eosio.hpp>
using namespace std;
using namespace eosio;

struct person {
   eosio::name account_name;
   string first_name;
   string last_name;
   string street;
   string city;
   string state;
   string country;
   string personal_id;

   non_unique<string> get_first_name() const {
      return first_name;
   }
   non_unique<string> get_last_name() const {
      return last_name;
   }
   non_unique<string> get_full_name() const {
      return {first_name + " " + last_name};
   }
   string get_personal_id_and_country() const {
      return country + " " + personal_id; // unique combination
   }
};

struct address_table : kv_table<person> {
   // unique indexes
   index<name>               account_name_uidx {"accname"_n, &person::account_name};
   index<string>             pers_id_cntry_uidx {"uniqueid"_n, &person::get_personal_id_and_country};
   // non unique indexes
   index<non_unique<string>> first_name_idx {"firstname"_n, &person::get_first_name};
   index<non_unique<string>> last_name_idx {"lastname"_n, &person::get_last_name};
   index<non_unique<string>> full_name_idx {"fullname"_n, &person::get_full_name};

   address_table(eosio::name contract_name) {
      init(contract_name, "addrtable"_n, eosio::kv_ram, 
      account_name_uidx, pers_id_cntry_uidx, first_name_idx, 
      last_name_idx, full_name_idx);
   }
};

class [[eosio::contract]] kv_addr_book : public eosio::contract {
   public:
      using contract::contract;
      kv_addr_book(name receiver, name code, datastream<const char*> ds)
         : contract(receiver, code, ds) {}

      [[eosio::action]]
      person get(name account_name);
      [[eosio::action]]
      person getby(string country, string personal_id);
      [[eosio::action]]
      void upsert(person user);
      [[eosio::action]]
      void del(person user);

      using get_action = action_wrapper<"get"_n, &kv_addr_book::get>;
      using get_by_action = action_wrapper<"getby"_n, &kv_addr_book::getby>;
      using upsert_action = action_wrapper<"upsert"_n, &kv_addr_book::upsert>;
      using del_action = action_wrapper<"del"_n, &kv_addr_book::del>;

   private:
      void print_person(const person& person);

      address_table addresses{"kvaddrbook"_n};
};
