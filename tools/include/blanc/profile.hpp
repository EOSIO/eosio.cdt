#pragma once
#include <string>

namespace blanc {
   enum profile {
      eosio = 0,
      cosmwasm,
   };

   blanc::profile to_profile(const std::string& p) {
      if (p == "eosio") {
         return blanc::profile::eosio;
      } else if (p == "cosmwasm") {
         return blanc::profile::cosmwasm;
      } else {
         throw std::runtime_error("unsupported profile: "+p);
      }
   }

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
