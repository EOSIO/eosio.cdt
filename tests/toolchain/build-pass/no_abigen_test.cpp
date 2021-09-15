#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] no_abigen_test : public contract {
  public:
      using contract::contract;
      
      [[eosio::action]]
      void test() {
      }
};
