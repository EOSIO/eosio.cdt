#pragma once

#include "name.hpp"

namespace eosio {
   namespace internal_use_do_not_use {
      extern "C" uint64_t eosio_contract_name;
   }

   inline name current_context_contract() { return name{internal_use_do_not_use::eosio_contract_name}; }
}
