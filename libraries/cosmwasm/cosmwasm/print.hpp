#pragma once
#include "memory.hpp"
#include <string>
#include <string_view>

namespace cosmwasm {
   namespace internal_use_do_not_use {
      extern "C" {
         __attribute__((import_name("debug")))
         void debug(region*);
      }
   }

   template<typename T, std::enable_if_t<!std::is_arithmetic<std::decay_t<T>>::value, int> = 0>
   inline void print(T&& t) {
      if constexpr (std::is_same<std::decay_t<T>, std::string>::value ||
                    std::is_same<std::decay_t<T>, std::string_view>::value)
         internal_use_do_not_use::debug(build_region_dup(t.data(), t.size()).release());
      else if constexpr (std::is_same<std::decay_t<T>, const char*>::value)
         internal_use_do_not_use::debug(build_region_dup(t, strlen(t)).release());
      else
         t.print();
   }

   template<typename T, std::enable_if_t<std::is_arithmetic<std::decay_t<T>>::value, int> = 0>
   inline void print(T t) {
      print(std::to_string(t));
   }
}
