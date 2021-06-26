#pragma once
#include "json.hpp"

namespace cosmwasm {
   template<typename T>
   T from_bytes(const bytes& b) { std::string_view s(b.data(), b.size());
      json::value v = json::parse_unsafe(s);
      return json::from_json<T>(v);
   }

   template<typename T, std::enable_if_t<!std::is_integral_v<std::decay_t<T>>, int> = 0>
   bytes to_bytes(const T& v) {
      return v.as_bytes();
   }

   template<>
   bytes to_bytes(const bytes& v) {
      return v;
   }

   template<>
   bytes to_bytes(const std::string& v) {
      bytes out(v.size());
      std::copy(v.begin(), v.end(), out.begin());
      return out;
   }

   template<typename T, std::enable_if_t<std::is_integral_v<std::decay_t<T>>, int> = 0>
   bytes to_bytes(T v) {
      auto s = std::to_string(v);
      return to_bytes(s);
   }

   template<>
   bytes to_bytes(uint128_t v) {
      auto s = "\"" + std::to_string(v) + "\"";
      return to_bytes(s);
   }

   template<size_t N>
   bytes to_bytes(char const (&v)[N]) {
      bytes out(N-1);
      std::copy_n(std::begin(v), N-1, out.begin());
      return out;
   }
}
