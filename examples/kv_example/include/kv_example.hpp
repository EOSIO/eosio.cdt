#include <eosio/eosio.hpp>
using namespace eosio;

class [[eosio::contract]] kv_example : public contract {
   public:
      using contract::contract;
      kv_example( name receiver, name code, datastream<const char*> ds )
         : contract(receiver, code, ds), testtab(receiver, receiver.value) {}

      struct [[eosio::table]] test_table {
         name test_primary;
         name secondary;
         uint64_t datum;
         uint64_t primary_key()const { return test_primary.value; }
         uint64_t by_secondary()const { return secondary.value; }
      };

      typedef eosio::multi_index<"testtaba"_n, test_table, eosio::indexed_by<"secid"_n, eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_tables;

      test_tables testtab;

      [[eosio::action]] 
      void set(name user);
      [[eosio::action]] 
      void print( name user );
      [[eosio::action]] 
      void bysec( name secid );
      [[eosio::action]] 
      void mod( name user, uint32_t n );
      [[eosio::action]] 
      void del( name user );

      using set_action = action_wrapper<"set"_n, &kv_example::set>;
      using print_action = action_wrapper<"print"_n, &kv_example::print>;
      using bysec_action = action_wrapper<"bysec"_n, &kv_example::bysec>;
      using mod_action = action_wrapper<"mod"_n, &kv_example::mod>;
      using del_action = action_wrapper<"del"_n, &kv_example::del>;
};
