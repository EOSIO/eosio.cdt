#pragma once

namespace eosio {
   /* utility function to swap the endianness of a variable */
   template <typename T>
   inline T swap_endian(T val) {
      static_assert(sizeof(T) <= 16);
      if constexpr (sizeof(T) == 1)
         return val;
      else if constexpr (sizeof(T) == 2)
         return (val << 8) |(val >> 8);
      else if constexpr (sizeof(T) == 4) {
         union i32_swap {
            T value;
            uint32_t ui;
            i32_swap(T v) : value(v) { ui = __builtin_bswap32(ui); }
         };
         return i32_swap(val).value;
      } else if constexpr (sizeof(T) == 8) {
         union i64_swap {
            T value;
            uint64_t ui;
            i64_swap(T v) : value(v) { ui = __builtin_bswap64(ui); }
         };
         return i64_swap(val).value;
      } else {
         union i128_swap {
            uint64_t high;
            uint64_t low;
            T        value;
            i128_swap(const T& val) : value(val) {
               uint64_t tmp = high;
               high = __builtin_bswap64(low);
               low = __builtin_bswap64(tmp);
            }
         };
         return i128_swap(val).value;
      }
   }
}
