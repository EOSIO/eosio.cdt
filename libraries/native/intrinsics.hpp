#include <eosiolib/action.hpp>
#include <native/intrinsics_def.hpp>

#pragma once

namespace eosio { namespace native {
   enum intrinsic_name {
      INTRINSICS(CREATE_ENUM)
      INTRINSICS_SIZE
   };

   struct intrinsics {
      INTRINSICS(CREATE_STUB)
      static constexpr auto ___stub_null = [](){};
      std::tuple< 
         INTRINSICS(GET_TYPE)
         decltype(___stub_null)
         > funcs = {
            INTRINSICS(REGISTER_INTRINSIC)
            ___stub_null };

      template <intrinsic_name IN, typename... Args>
      auto call(Args... args) -> decltype(std::get<IN>(funcs)(args...)) {
         return std::get<IN>(funcs)(args...); 
      }
   };

}} //ns eosio::native

extern eosio::native::intrinsics __intrins;

template <eosio::native::intrinsic_name IN, typename F>
void set_intrinsic(F&& func) {
   std::get<IN>(__intrins.funcs) = func;
}
