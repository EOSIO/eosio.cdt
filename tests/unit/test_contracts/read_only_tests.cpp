#include "transfer.hpp"
#include <eosio/eosio.hpp>
#include <eosio/privileged.hpp>

extern "C" __attribute__((eosio_wasm_import)) void set_resource_limit(int64_t, int64_t, int64_t);

using namespace eosio;

class [[eosio::contract]]  read_only_tests : public contract {
  public:
      using contract::contract;
      void foo1() { bar(); }
      void bar() {}
      void foo2( name user, int64_t limit ) { set_resource_limit(user.value, "disk"_n.value, limit); }
      
      [[eosio::action, eosio::read_only]]
      void testreadonly( name user, int64_t limit ) {
         print( "Hello, ", user);
         int i;
         foo1();
         set_resource_limit(user.value, "disk"_n.value, limit);
         foo2(user, limit);
      }

      [[eosio::action, eosio::read_only]]
      void hireadonly( name user ) {
         print( "Hello, ", user);
         int i;
         foo1();
      }

      [[eosio::action]]
      void hi( name user ) {
         print( "Hello, ", user);
      }
};
