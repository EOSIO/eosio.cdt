#include <eosio/eosio.hpp>

class [[eosio::contract]] separate_cpp_hpp : eosio::contract {
public:
   using contract::contract;

   [[eosio::action]] void act();
};
