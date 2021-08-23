/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include_next <eosio/fixed_bytes.hpp>
#include "print.hpp"

namespace eosio {

   template<std::size_t Size>
   inline void print(const fixed_bytes<Size>& d) {
       auto arr = d.extract_as_byte_array();
       printhex(static_cast<const void*>(arr.data()), arr.size());
   }
}
