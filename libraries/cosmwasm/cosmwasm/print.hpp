#pragma once
#include "memory.hpp"
#include "primitives.hpp"
#include "stringize.hpp"
#include <string_view>

#ifndef BLANC_NATIVE
namespace cosmwasm {
   template<typename T>
   inline void print(T&& t) {
      if constexpr (std::is_same<std::decay_t<T>, std::string>::value ||
                    std::is_same<std::decay_t<T>, std::string_view>::value)
         ::debug(build_region(t.data(), t.size()).release());
      else if constexpr (std::is_same<std::decay_t<T>, const char*>::value)
         ::debug(build_region(t, strlen(t)).release());
      else
         print(to_string(t));
   }
}
#else
#include <iostream>
namespace cosmwasm {
   template<typename T>
   inline void print(T&& t) {
      std::cout << t << std::endl;
   }
}
#endif
