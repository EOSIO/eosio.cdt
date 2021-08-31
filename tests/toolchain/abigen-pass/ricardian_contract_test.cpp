#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] ricardian_contract_test : public contract {
  public:
      using contract::contract;
      
      [[eosio::action]]
      void test() {
      }
};