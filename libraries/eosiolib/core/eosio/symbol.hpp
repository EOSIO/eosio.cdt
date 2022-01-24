/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include_next <eosio/symbol.hpp>
#include <eosio/print.hpp>

namespace eosio {

inline void print(symbol obj) {
   print(obj.to_string());
}

}
