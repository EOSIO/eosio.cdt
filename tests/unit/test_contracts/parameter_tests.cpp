#include <eosio/contract.hpp>
#include <eosio/name.hpp>
#include <eosio/privileged.hpp>


using namespace eosio;

namespace eosio {
   extern "C" {
     __attribute__((eosio_wasm_import))
     void set_blockchain_parameters_packed(char*, uint32_t);
     __attribute__((eosio_wasm_import))
     uint32_t get_blockchain_parameters_packed(char*, uint32_t);
     __attribute__((eosio_wasm_import))
     void set_parameters_packed(const char*, uint32_t);
     __attribute__((eosio_wasm_import))
     uint32_t get_parameters_packed(const char*, uint32_t, char*, uint32_t);
   }
}
// test set_parameters and get_parameters which flexibly set or get chain parameters by ids
class [[eosio::contract]] parameter_tests : eosio::contract {
 public:
   using contract::contract;

   // usage: cleos -v push action eosio getall '[]' -p eosio@active
   [[eosio::action]] eosio::blockchain_parameters getall(){
      eosio::blockchain_parameters params;
      get_blockchain_parameters(params);
      int id = 0;
      eosio::cout << "id =" << id++ << ", max_block_net_usage=" << params.max_block_net_usage << "\n";
      eosio::cout << "id =" << id++ << ", target_block_net_usage_pct=" << params.target_block_net_usage_pct << "\n";
      eosio::cout << "id =" << id++ << ", max_transaction_net_usage=" << params.max_transaction_net_usage << "\n";
      eosio::cout << "id =" << id++ << ", base_per_transaction_net_usage=" << params.base_per_transaction_net_usage << "\n";
      eosio::cout << "id =" << id++ << ", net_usage_leeway=" << params.net_usage_leeway << "\n";
      eosio::cout << "id =" << id++ << ", context_free_discount_net_usage_num=" << params.context_free_discount_net_usage_num << "\n";
      eosio::cout << "id =" << id++ << ", context_free_discount_net_usage_den=" << params.context_free_discount_net_usage_den << "\n";
      eosio::cout << "id =" << id++ << ", max_block_cpu_usage=" << params.max_block_cpu_usage << "\n";
      eosio::cout << "id =" << id++ << ", target_block_cpu_usage_pct=" << params.target_block_cpu_usage_pct << "\n";
      eosio::cout << "id =" << id++ << ", max_transaction_cpu_usage=" << params.max_transaction_cpu_usage << "\n";
      eosio::cout << "id =" << id++ << ", min_transaction_cpu_usage=" << params.min_transaction_cpu_usage << "\n";
      eosio::cout << "id =" << id++ << ", max_transaction_lifetime=" << params.max_transaction_lifetime << "\n";
      eosio::cout << "id =" << id++ << ", deferred_trx_expiration_window=" << params.deferred_trx_expiration_window << "\n";
      eosio::cout << "id =" << id++ << ", max_transaction_delay=" << params.max_transaction_delay << "\n";
      eosio::cout << "id =" << id++ << ", max_inline_action_size=" << params.max_inline_action_size << "\n";
      eosio::cout << "id =" << id++ << ", max_inline_action_depth=" << params.max_inline_action_depth << "\n";
      eosio::cout << "id =" << id++ << ", max_authority_depth=" << params.max_authority_depth << "\n";
      return params;
   }
   /*
   enum {
      max_block_net_usage_id,
      target_block_net_usage_pct_id,
      max_transaction_net_usage_id,
      base_per_transaction_net_usage_id,
      net_usage_leeway_id,
      context_free_discount_net_usage_num_id,
      context_free_discount_net_usage_den_id,
      max_block_cpu_usage_id,
      target_block_cpu_usage_pct_id,
      max_transaction_cpu_usage_id,
      min_transaction_cpu_usage_id,
      max_transaction_lifetime_id,
      deferred_trx_expiration_window_id,
      max_transaction_delay_id,
      max_inline_action_size_id,
      max_inline_action_depth_id,
      max_authority_depth_id,
      max_action_return_value_size_id
   };
*/
   //usage:  cleos -v push action eosio get '[[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17]]' -p eosio@active
   [[eosio::action]] std::vector<std::pair<uint32_t, std::variant<uint16_t, uint32_t, uint64_t>>> get(  std::vector<uint32_t>  param_ids){
      std::vector<std::pair<uint32_t, std::variant<uint16_t, uint32_t, uint64_t>>>  params;
      get_parameters(param_ids, params);
      for(const auto & p : params){
         std::variant<uint16_t, uint32_t, uint64_t> var = p.second;
         eosio::cout << "id = " << p.first << ", value= " <<  (std::holds_alternative<uint16_t>(var) ? std::get<uint16_t>(var) :
                                             std::holds_alternative<uint32_t>(var) ? std::get<uint32_t>(var) : std::get<uint64_t>(var) ) << "\n";
      }
      return params;
   }
   //usage: cleos -v push action eosio set '[[{"first":0,"second":["uint64",1048800]},{"first":1,"second":["uint32",1008]}, {"first":2,"second":["uint32",524290]},{"first":15,"second":["uint16",5]}, {"first":16,"second":["uint16",7]}]]' -p eosio@active
   [[eosio::action]] void set( std::vector<std::pair<uint32_t, std::variant<uint16_t, uint32_t, uint64_t>>>  params  ){
      set_parameters(params);
      return;
   }
};
