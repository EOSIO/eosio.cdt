#pragma once
#include "builtin.hpp"
#include "print.hpp"

namespace cosmwasm {
   inline void check(bool pred, const char* msg = nullptr) {
      if (!pred) {
         if (msg) {
            print(msg);
         }
         abort();
      }
   }

   inline void check(bool pred, const std::string& msg) {
      if (!pred) {
         if (!msg.empty()) {
            print(msg.data());
         }
         abort();
      }
   }
}
