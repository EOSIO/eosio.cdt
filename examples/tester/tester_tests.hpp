#include <eosio/contract.hpp>
#include <eosio/action.hpp>
#include <eosio/name.hpp>
#include <eosio/multi_index.hpp>
#include <cstdint>

class [[eosio::contract("tester_tests")]] tester_tests : eosio::contract {
public:
   using contract::contract;
   [[eosio::action]] void putdb(int key, int value);
   using putdb_action = eosio::action_wrapper<"putdb"_n, &tester_tests::putdb>;
   struct [[eosio::table]] table_item {
      int key;
      int value;
      std::uint64_t primary_key() const { return key; }
      std::uint64_t secondary() const { return value; }
   };
   using table = eosio::multi_index<"table"_n, table_item, eosio::indexed_by<"value"_n, eosio::const_mem_fun<table_item, std::uint64_t, &table_item::secondary>>>;
};
