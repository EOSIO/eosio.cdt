#include <eosio/contract.hpp>
#include <eosio/name.hpp>
#include <eosio/privileged.hpp>

using namespace eosio;

class [[eosio::contract]] resource_limits_test_contract : eosio::contract {
public:
   using contract::contract;
   [[eosio::action]] void setrslimit(name account, name resource, int64_t limit) {
      set_resource_limit(account, resource, limit);
   }

   [[eosio::action]] void printrslm(name account, name resource) {
      int64_t limit = get_resource_limit(account, resource);
      eosio::cout<< "account = "<< account.to_string() << ", resource = " << resource.to_string() << ", limit = "<< limit << '\n';
   }
};

