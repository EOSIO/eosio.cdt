#include <eosio/eosio.hpp>
using namespace std;
using namespace eosio;

struct person {
 name account_name;
 std::string first_name;
 non_unique<name, std::string> last_name;
 std::string street;
 std::string city;
 std::string state;
};

class [[eosio::contract]] addressbook : public contract {
   public:
      using contract::contract;

      addressbook(name receiver, name code, datastream<const char*> ds)
         : contract(receiver, code, ds) {}

      struct [[eosio::table]] kv_address_table : eosio::kv::table<person, "kvaddrbook"_n> {

         index<name> account_name_uidx {"accnameuidx"_n, &person::account_name};
         index<non_unique<name, string>> last_name_idx {"lastnameidx"_n, &person::last_name};

         kv_address_table(name contract_name) {
            init(contract_name,
               account_name_uidx,
               last_name_idx);
         }
      };

      // creates if not exists, or updates if already exists, a person
      [[eosio::action]]
      std::pair<int, std::string> upsert(
         name account_name,
         string first_name,
         string last_name,
         string street,
         string city,
         string state);

      // deletes a person based on primary key account_name
      [[eosio::action]]
      void del(name account_name);

      // retrieves list of persons with the same last name
      [[eosio::action]]
      std::vector<person> getbylastname(string last_name);

      using upsert_action = action_wrapper<"upsert"_n, &addressbook::upsert>;
      using del_action = action_wrapper<"del"_n, &addressbook::del>;
      using get_by_last_name_action = action_wrapper<"getbylastname"_n, &addressbook::getbylastname>;

   private:
      kv_address_table addresses{"kvaddrbook"_n};
};