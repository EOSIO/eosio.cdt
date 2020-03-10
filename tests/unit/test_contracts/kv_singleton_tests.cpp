#include <eosio/eosio.hpp>
#include <eosio/key_value_singleton.hpp>

using namespace eosio;

struct my_contract_config {
   int a;
   std::string b;

   bool operator==(const my_contract_config& rhs) const {
      return std::tie(a, b) == std::tie(rhs.a, rhs.b);
   }
};

class [[eosio::contract]] kv_singleton_tests : public eosio::contract {
public:
   using contract::contract;

   [[eosio::action]]
   void setup() {
      kv_singleton<uint64_t, "count"_n> t{get_self()};
      t.set(4);
   }

   [[eosio::action]]
   void test() {
      kv_singleton<uint64_t, "count"_n> myglob{get_self()};
      kv_singleton<my_contract_config, "config"_n> gl{get_self()};

      eosio::check(myglob.get() == 4, "Get returned an incorrect value: 1");
      myglob.set(8);
      eosio::check(myglob.get() == 8, "Get returned an incorrect value: 2");

      gl.modify([&](auto& x) {
         x.a = 2;
         x.b = "abcd";
      });
      my_contract_config expected{2, "abcd"};
      eosio::check(gl.get() == expected, "Get returned an incorrect value: 3");
   }

   [[eosio::action]]
   void erase() {
      kv_singleton<uint64_t, "count"_n> myglob{get_self()};
      eosio::check(myglob.get() == 4, "Get returned an incorrect value: 1");

      myglob.erase();

      myglob.get();
   }
};
