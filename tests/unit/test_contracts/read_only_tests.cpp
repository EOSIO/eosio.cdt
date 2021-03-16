#include <eosio/eosio.hpp>
#include <eosio/privileged.hpp>

using namespace eosio;
//extern "C" __attribute__((eosio_wasm_import)) void set_resource_limit(int64_t, int64_t, int64_t);

class [[eosio::contract]]  read_only_tests : public contract {
  public:

      [[eosio::action, eosio::read_only]]
      void testreadonly( name user, int64_t limit ) {
         internal_use_do_not_use::set_resource_limits(user.value, 0, 0, 0);
         foo(user);
      }

      [[eosio::action, eosio::read_only]]
      void test2( name user, int64_t limit ) {
         internal_use_do_not_use::set_resource_limits(user.value, 0, 0, 0);
      }

      void foo(name user) {
         internal_use_do_not_use::set_resource_limits(user.value, 0, 0, 0);
      }

      [[eosio::action, eosio::read_only]]
      void test3( name user, int64_t limit ) {
         foo1(user);
      }

      void foo1(name user) {
         foo(user);
      }

      [[eosio::action]]
      void hi( name user ) {
         print( "Hello, ", user);
      }
};
