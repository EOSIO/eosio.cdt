#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] no_missing_ricardian_clause : public contract {
public:
   using contract::contract;

   [[eosio::action]]
   void test1( name user ) {
   }
};
