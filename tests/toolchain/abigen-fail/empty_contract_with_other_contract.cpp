#include <eosio/eosio.hpp>
using namespace eosio;

CONTRACT hello : public contract {};

CONTRACT another_hello : public contract {
   public:
      using contract::contract;

      ACTION hi( name nm ) {
         print_f("Name : %\n", nm);
      }
};
