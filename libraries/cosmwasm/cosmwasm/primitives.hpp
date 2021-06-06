#pragma once
#include <cstddef>
#include <vector>
#include <string>

#ifdef BLANC_NATIVE
using uint128_t = __uint128_t;
#else
#include "uint128_t/uint128_t.h"
#endif

namespace cosmwasm {
   typedef std::vector<char> bytes;
}

namespace std {
   template<>
   inline constexpr bool is_integral_v<uint128_t> = true;

   template<>
   struct is_class<uint128_t> : false_type {};

   std::string to_string(uint128_t v) {
      return v.str();
   }
}
