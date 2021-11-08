#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] print_include_dirs : public contract {
  public:
      using contract::contract;
      
      [[eosio::action]]
      void test() {
      }
};