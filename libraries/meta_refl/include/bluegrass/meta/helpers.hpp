#pragma once

namespace bluegrass::meta {
   template <typename... Args>
   constexpr inline auto va_args_count_helper(Args&&...) { return sizeof...(Args); }

   // adding some types for injection, this file can't include anything
   struct str_view {
      template <unsigned long long N>
      constexpr inline str_view(const char (&arr)[N])
         : dat(arr),
           sz(N) {}

      constexpr inline const char* data() const { return dat; }
      constexpr inline unsigned long long size() const { return sz; }
      constexpr inline char operator[](unsigned long long n) const { return data()[n]; }

      constexpr void _bluegrass_meta_refl_valid(); // keep this from ever being reflected
      const char* dat;
      unsigned long long sz;
   };

   template <typename T, unsigned long long Sz>
   struct base_array {
      template <typename... Args>
      constexpr inline base_array(Args&&... args)
         : dat({(Args&&)(args)...}) {}
      using iterator = T*;
      using const_iterator = const iterator;

      constexpr inline const T* data() const { return dat; }
      constexpr inline unsigned long long size() const { return sz; }
      constexpr inline const T& operator[](unsigned long long n) const { return data()[n]; }
      constexpr void _bluegrass_meta_refl_valid(); // keep this from ever being reflected
      T dat[Sz];
      unsigned long long sz = Sz;
   };
} // ns bluegrass::meta
