#pragma once

namespace eosio {
   /* utility function to swap the endianness of a variable */
   template <typename T>
   inline T swap_endian(T val) {
      static_assert(sizeof(T) <= 16);
      if constexpr (sizeof(T) == 1)
         return val;
      else if constexpr (sizeof(T) == 2)
         return (val << 8) | (val >> 8);
      else if constexpr (sizeof(T) == 4) {
         T value;
         auto swapped = __builtin_bswap32(val);
         memcpy((char*)&value, (char*)&swapped, sizeof(T));
         return value;
      } else if constexpr (sizeof(T) == 8) {
         T value;
         auto swapped = __builtin_bswap64(val);
         memcpy((char*)&value, (char*)&swapped, sizeof(T));
         return value;
      } else {
         T value;
         uint128_t swapped_high = __builtin_bswap64((uint64_t)val);
         uint64_t swapped_low = __builtin_bswap64((uint64_t)(val >> 64));
         T swapped = (swapped_high << 64) | swapped_low;
         memcpy((char*)&value, (char*)&swapped, sizeof(T));
         return value;
      }
   }
}
