#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] structure_test : public eosio::contract {
public:
   using contract::contract;

   struct [[eosio::structure]] fee_payer {
      name payer;
      uint64_t quantity;
   };

   [[eosio::action]]
   fee_payer getpayer() {
      fee_payer pyr;
      pyr.payer = _self;
      pyr.quantity = 1000;

      return pyr;
   }

   [[eosio::action]]
   void setfeepyr(fee_payer& pyr) {
      pyr.payer = "respyr"_n;
      pyr.quantity = 10;
   }

   [[eosio::action]]
   void printfeepyr(fee_payer pyr) {
      print(pyr.payer, pyr.quantity);
   }
};
