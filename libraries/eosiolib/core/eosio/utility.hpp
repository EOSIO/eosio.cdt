#pragma once
#include <string>
#include <locale>
#include <codecvt>

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

   inline std::wstring string_to_wstring(const std::string& str) {
      using convert_wchar = std::codecvt_utf8<wchar_t>;
      std::wstring_convert<convert_wchar, wchar_t> conv;

      return conv.from_bytes(str);
   }

   inline std::string wstring_to_string(const std::wstring& wstr) {
      using convert_char = std::codecvt_utf8<wchar_t>;
      std::wstring_convert<convert_char, wchar_t> conv;

      return conv.to_bytes(wstr);
   }

   inline std::string to_upper(const std::string& str) {
      setlocale(LC_CTYPE, "en_US.UTF-8");
      auto wstr = string_to_wstring(str);

      for (std::wstring::iterator it = wstr.begin(); it != wstr.end(); ++it)
         *it = towupper(*it);

      return wstring_to_string(wstr);
   }
}
