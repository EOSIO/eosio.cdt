#include <eosio/eosio.hpp>
#include <eosio/privileged.hpp>

using namespace eosio;
extern "C" __attribute__((weak)) __attribute__((eosio_wasm_import)) void set_resource_limit(int64_t, int64_t, int64_t);

class [[eosio::contract]] warn_action_read_only : public contract {
  public:
      using contract::contract;
      
      [[eosio::action, eosio::read_only]]
      void test1( name user ) {
	      set_resource_limit(user.value, 0, 0);
      }
};
