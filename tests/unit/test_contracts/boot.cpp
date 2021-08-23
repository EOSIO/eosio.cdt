#include <eosio/contract.hpp>
#include <eosio/privileged.hpp>
#include <eosio/fixed_bytes.hpp>

class [[eosio::contract]] boot : eosio::contract {
 public:
   using eosio::contract::contract;
   [[eosio::action]] void activate(const eosio::checksum256& id) {
      preactivate_feature(id);
   }
};
