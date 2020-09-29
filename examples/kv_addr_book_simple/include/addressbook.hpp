#include <eosio/eosio.hpp>

struct person {
 eosio::name account_name;
 std::string first_name;
 eosio::non_unique<eosio::name, std::string> last_name;
 std::string street;
 std::string city;
 std::string state;
};

class [[eosio::contract]] addressbook : public eosio::contract {
   public:
      using contract::contract;

      addressbook(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds)
         : contract(receiver, code, ds) {}

      struct [[eosio::table]] kv_address_table : eosio::kv::table<person, "kvaddrbook"_n> {

      index<eosio::name> account_name_uidx {
         eosio::name{"accname"_n},
         &person::account_name };
      index<eosio::non_unique<eosio::name, std::string>> last_name_idx {
         eosio::name{"lastnameidx"_n},
         &person::last_name };

         kv_address_table(eosio::name contract_name) {
            init(contract_name,
               account_name_uidx,
               last_name_idx);
         }
      };

      // creates if not exists, or updates if already exists, a person
      [[eosio::action]]
      std::pair<int, std::string> upsert(
         eosio::name account_name,
         std::string first_name,
         std::string last_name,
         std::string street,
         std::string city,
         std::string state);

      // deletes a person based on primary key account_name
      [[eosio::action]]
      void del(eosio::name account_name);

      // retrieves list of persons with the same last name
      [[eosio::action]]
      std::vector<person> getbylastname(std::string last_name);

      using upsert_action = eosio::action_wrapper<"upsert"_n, &addressbook::upsert>;
      using del_action = eosio::action_wrapper<"del"_n, &addressbook::del>;
      using get_by_last_name_action = eosio::action_wrapper<"getbylastname"_n, &addressbook::getbylastname>;

   private:
      kv_address_table addresses{"kvaddrbook"_n};
};