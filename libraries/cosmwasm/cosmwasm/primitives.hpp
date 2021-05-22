#pragma once
#include <cstddef>
#include <vector>

namespace cosmwasm {
   typedef std::vector<char> bytes;
}

namespace std {

   template<>
   struct is_integral<uint128_t> {
      constexpr static bool value = true;
   };

   template<>
   struct is_signed<uint128_t> {
      constexpr static bool value = false;
   };

   std::string to_string(uint128_t v) {
      if (v <= std::numeric_limits<uint64_t>::max()) {
         uint64_t _v = v;
         return std::to_string(_v);
      } else {
         constexpr uint64_t P10_UINT64 = 10'000'000'000'000'000'000ULL;
         constexpr uint64_t E10_UINT64 = 19;

         std::string out;
         uint128_t leading = v / P10_UINT64;
         uint64_t trailing = v % P10_UINT64;
         out = std::to_string(leading);
         auto s = std::to_string(trailing);
         out += std::string(19 - s.size(), '0') + s;
         return out;
      }
   }

}
