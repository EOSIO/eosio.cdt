#pragma once

namespace cosmwasm {

   template<typename T>
   std::string to_string(T&& v) {
      constexpr bool has_to_string = requires(const T& t) {
         t.to_string();
      };
      if constexpr (has_to_string) {
         return v.to_string();
      } else {
         return std::to_string(v);
      }
   }

}
