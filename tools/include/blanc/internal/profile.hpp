#pragma once
#include <string>

namespace blanc {
    enum profile {
        eosio = 0,
        cosmwasm,
    };

    std::string default_stack_size(blanc::profile pf) {
        switch (pf) {
        case blanc::profile::eosio:
            return "8192"; // 8 KiB
        case blanc::profile::cosmwasm:
            return "1048576"; // 1 MiB
        default:
            __builtin_unreachable();
        }
    }
}

namespace std {
   string to_string(blanc::profile pf) {
       switch (pf) {
       case blanc::profile::eosio:
           return "eosio";
       case blanc::profile::cosmwasm:
           return "cosmwasm";
       default:
           __builtin_unreachable();
       }
   }
}