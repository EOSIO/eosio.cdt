#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] print_link_dirs : public contract {
  public:
      using contract::contract;
      
      [[eosio::action]]
      void test() {
      }
};