#pragma once
#include "json.hpp"

namespace cosmwasm {

   template<typename T>
   T from_bytes(const bytes& b) {
      std::string_view s(b.data(), b.size());
      json::value v = json::parse_unsafe(s);
      return json::from_json<T>(v);
   }

   template<typename T>
   bytes to_bytes(const T& v) {
      return v.as_bytes();
   }

   template<typename T, std::enable_if_t<std::is_integral_v<std::decay_t<T>>, int> = 0>
   bytes to_bytes(T v) {
      union {
         T u;
         unsigned char u8[sizeof(T)];
      } source, dest;

      source.u = v;

      for (size_t k = 0; k < sizeof(T); ++k) {
         dest.u8[k] = source.u8[sizeof(T) - k - 1];
      }

      return {dest.u8};
   }
}
