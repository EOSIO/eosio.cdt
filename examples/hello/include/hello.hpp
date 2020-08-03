#include <eosio/eosio.hpp>
using namespace eosio;

class [[eosio::contract]] hello : public contract {
   public:
      using contract::contract;

      [[eosio::action]] 
      void hi( name nm );
      [[eosio::action]] 
      void check( name nm );
      [[eosio::action]]
      std::vector<std::string> checkwithrv( name nm );
      [[eosio::action]]
      void checkwithrv2( name nm );

      using hi_action = action_wrapper<"hi"_n, &hello::hi>;
      using check_action = action_wrapper<"check"_n, &hello::check>;
      using checkwithrv_action = action_wrapper<"checkwithrv"_n, &hello::checkwithrv>;
      using checkwithrv2_action = action_wrapper<"checkwithrv2"_n, &hello::checkwithrv2>;
};
