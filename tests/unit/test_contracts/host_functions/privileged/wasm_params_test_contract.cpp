#include <eosio/contract.hpp>
#include <eosio/name.hpp>
#include <eosio/privileged.hpp>

using namespace eosio;

class [[eosio::contract]] wasm_params_test_contract : eosio::contract {
public:
   using contract::contract;

   [[eosio::action]] void printwasmp(void) {
      eosio::wasm_parameters params;
      int sz = eosio::get_wasm_parameters(params);
      eosio::cout<< "sz = "<< sz << '\n';
      eosio::cout<< "max_mutable_global_bytes = "<< params.max_mutable_global_bytes << '\n';
      eosio::cout<< "max_table_elements = "<< params.max_table_elements << '\n';
      eosio::cout<< "max_section_elements = "<< params.max_section_elements << '\n';
      eosio::cout<< "max_linear_memory_init = "<< params.max_linear_memory_init << '\n';
      eosio::cout<< "max_func_local_bytes = "<< params.max_func_local_bytes << '\n';
      eosio::cout<< "max_nested_structures = "<< params.max_nested_structures << '\n';
      eosio::cout<< "max_symbol_bytes = "<< params.max_symbol_bytes << '\n';
      eosio::cout<< "max_module_bytes = "<< params.max_module_bytes << '\n';
      eosio::cout<< "max_code_bytes = "<< params.max_code_bytes << '\n';
      eosio::cout<< "max_pages = "<< params.max_pages << '\n';
      eosio::cout<< "max_call_depth = "<< params.max_call_depth << '\n';

      params.max_mutable_global_bytes = 101147;
      params.max_section_elements = 4;
      params.max_func_local_bytes = 8;
      params.max_nested_structures = 1;
      params.max_symbol_bytes = 32;
      params.max_module_bytes = 256;
      params.max_code_bytes = 32;
      params.max_pages = 1;
      params.max_call_depth = 2;

      eosio::set_wasm_parameters(params);

      params.max_mutable_global_bytes = 0;

      eosio::get_wasm_parameters(params);
      eosio::cout<< "max_mutable_global_bytes = "<< params.max_mutable_global_bytes << '\n';
      eosio::cout<< "max_table_elements = "<< params.max_table_elements << '\n';
      eosio::cout<< "max_section_elements = "<< params.max_section_elements << '\n';
      eosio::cout<< "max_linear_memory_init = "<< params.max_linear_memory_init << '\n';
      eosio::cout<< "max_func_local_bytes = "<< params.max_func_local_bytes << '\n';
      eosio::cout<< "max_nested_structures = "<< params.max_nested_structures << '\n';
      eosio::cout<< "max_symbol_bytes = "<< params.max_symbol_bytes << '\n';
      eosio::cout<< "max_module_bytes = "<< params.max_module_bytes << '\n';
      eosio::cout<< "max_code_bytes = "<< params.max_code_bytes << '\n';
      eosio::cout<< "max_pages = "<< params.max_pages << '\n';
      eosio::cout<< "max_call_depth = "<< params.max_call_depth << '\n';

   }
};

