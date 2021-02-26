#include "transfer.hpp" 
#include <eosio/eosio.hpp>
using namespace eosio;

class [[eosio::contract]]  read_only_tests : public contract {
  public:
      using contract::contract;
      [[eosio::action, eosio::read_only]]
      void hi( name user ) {
         print( "Hello, ", user);
      }
};
