/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include_next <eosio/name.hpp>

/// @cond IMPLEMENTATIONS

namespace eosio {
   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((eosio_wasm_import))
         void printn(uint64_t);
      }
   }

   inline void print(name obj) {
      internal_use_do_not_use::printn(obj.value);
   }

}

using namespace eosio::literals;

/// @endcond
