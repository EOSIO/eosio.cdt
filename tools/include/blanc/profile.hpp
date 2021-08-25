#pragma once
#include <string>

namespace blanc {
   enum profile {
      eosio = 0,
      cosmwasm,
   };

   std::string default_stack_size(profile p) {
      switch (p) {
      case profile::eosio:
         return "8192"; // 8 KiB
      case profile::cosmwasm:
         return "1048576"; // 1 MiB
      default:
         __builtin_unreachable();
      }
   }
}

namespace std {
   string to_string(blanc::profile p) {
      switch (p) {
      case blanc::profile::eosio:
         return "eosio";
      case blanc::profile::cosmwasm:
         return "cosmwasm";
      default:
         __builtin_unreachable();
      }
   }
}
