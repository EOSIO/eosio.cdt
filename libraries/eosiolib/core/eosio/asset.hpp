#pragma once

#include_next <eosio/asset.hpp>
#include <eosio/print.hpp>

namespace eosio {

inline void print(asset obj) {
   print(obj.to_string());
}

inline void print(extended_asset obj) {
   print(obj.to_string());
}

}
