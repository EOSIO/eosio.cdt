#include <eosio/eosio.hpp>

using namespace eosio;

struct some_record {
   std::string full_name;
   uint8_t     age;
   std::tuple<int, float, char> arbitrary;
};

class [[eosio::contract]] key_value : eosio::contract {
   public:
      using eosio::contract::contract;

      [[eosio::action]]
      void act1(int k, std::string s) {
         test[k] = s;

         test3[s] = some_record{"Block McChain", 42, {13, 42.42f, 'a'}};
      }

      [[eosio::action]]
      void act2(float f, uint64_t v) {
         test2[f] = v;
      }

      using test_map  = kv::map<"test.map"_n, int, std::string>;
      using test2_map = kv::map<"test2.map"_n, float, uint64_t>;
      using test3_map = kv::map<"test3.map"_n, std::string, some_record>;

   private:
      test_map  test;
      test2_map test2;
      test3_map test3;
};
