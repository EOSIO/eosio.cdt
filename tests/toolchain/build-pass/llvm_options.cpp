#include <eosio/eosio.hpp>

class [[eosio::contract]] llvm_options : public eosio::contract {
  public:
      using contract::contract;
      [[eosio::action]] void test1() {}
};
