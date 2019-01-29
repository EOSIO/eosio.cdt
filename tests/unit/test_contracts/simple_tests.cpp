#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] simple_test : public contract {
   public:
      using contract::contract;

      [[eosio::action]] void test1(name nm) {
         check(nm == "bucky"_n, "not bucky");
      }
};
