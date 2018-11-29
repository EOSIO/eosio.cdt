#pragma once
#include <eosiolib/eosio.hpp>
#include <setjmp.h>

template <typename F, typename... Args>
void expect_assert(F&& func, Args... args) {
   extern jmp_buf* ___test_env_ptr;
   extern int*     ___test_ret_ptr;
   *___test_ret_ptr = setjmp(*___test_env_ptr);
   if (*___test_ret_ptr == 0) {
      func(args...);
      *___test_ret_ptr = 1;
      eosio_assert(false, "expect_assert");
   }
}
