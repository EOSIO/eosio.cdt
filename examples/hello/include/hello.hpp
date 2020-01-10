#include <eosio/eosio.hpp>
using namespace eosio;

class [[eosio::contract]] hello : public contract {
   public:
      using contract::contract;

      [[eosio::action]] 
      void hi( name nm );
      [[eosio::action]] 
      void check( name nm );

      using hi_action = action_wrapper<"hi"_n, &hello::hi>;
      using check_action = action_wrapper<"check"_n, &hello::check>;
};
