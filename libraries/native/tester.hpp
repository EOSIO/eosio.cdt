#pragma once
#include <setjmp.h>
#include <eosiolib/print.hpp>

extern "C" {
   int ___jmp_ret;
   jmp_buf ___env;
}

//#define EOSIO_TEST_BEGIN              \
//___test_jmp_ret = setjmp(___test_env); \
//if (___test_jmp_ret == 0) {            \
//      
//#define EOSIO_TEST_END                        \
//}                                      

template <typename T, typename... Args>
constexpr void expect_assert( T&& func, Args... args ) {
   bool asserted = true;
   ___jmp_ret = setjmp(___env);
   if (___jmp_ret == 0) {
      func(args...);
      asserted = false;
   }
   eosio::print("Asserted!\n");
   eosio_assert(asserted, "Test did not assert");
}
