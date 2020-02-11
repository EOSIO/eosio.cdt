#pragma once

#include_next <eosio/asset.hpp>
#include <eosio/print.hpp>

namespace eosio {

void print(asset obj) {
   print(obj.to_string());
}

void print(extended_asset obj) {
   print(obj.to_string());
}

}
