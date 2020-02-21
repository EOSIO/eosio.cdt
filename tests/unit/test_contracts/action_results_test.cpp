#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] action_results_test : public contract {
   public:
   using contract::contract;

   [[eosio::action]]
   void action1() {}

   [[eosio::action]]
   uint32_t action2() { return 42; }

   [[eosio::action]]
   std::string action3() { return "foo"; }
};
