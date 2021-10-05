#include <eosio/contract.hpp>
#include <eosio/name.hpp>
#include <eosio/privileged.hpp>

using namespace eosio;

class [[eosio::contract]] kv_params_test_contract : eosio::contract {
public:
   using contract::contract;
   [[eosio::action]] void kvlimitstst(uint32_t k, uint32_t v, uint32_t i) {
      uint32_t limits[4];
      eosio::kv_parameters params;
      params.max_key_size = k;
      params.max_value_size = v;
      params.max_iterators = i;
      set_kv_parameters(params);

      // put high values on the params to check later if we get the same values
      params.max_key_size = 0xFFFFFFFFu;
      params.max_value_size = 0xFFFFFFFFu;
      params.max_iterators = 0xFFFFFFFFu;

      int sz = get_kv_parameters(params);

      check(sz == 16, "wrong kv parameters result");
      check(params.max_key_size == k, "wrong key");
      check(params.max_value_size == v, "wrong value");
      check(params.max_iterators == i, "wrong iter limit");
   }
};
