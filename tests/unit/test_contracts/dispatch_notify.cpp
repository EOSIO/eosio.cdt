#include <eosio/contract.hpp>
#include <eosio/action.hpp>
#include <eosio/name.hpp>

using namespace eosio;

class [[eosio::contract]] dispatch_notify : contract {
 public:
   using contract::contract;
   [[eosio::action]] void forward(name dest, int res) { require_recipient(dest); }
};
