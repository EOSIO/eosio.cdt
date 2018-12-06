#pragma once
#include <eosiolib/eosio.hpp>
#include <native/crt/crt.hpp>
#include <native/intrinsics.hpp>
#include <setjmp.h>
#include <vector>

extern "C" bool ___disable_output;
extern "C" bool ___has_failed;

inline void silence_output(bool t) {
   ___disable_output = t;
}
inline bool has_failed() {
   return ___has_failed;
}

template <size_t N, typename F, typename... Args>
inline void expect_assert(const char (&expected)[N], F&& func, Args... args) {
   std_err.flush();
   __set_env_test();
   int ret = setjmp(*___env_ptr);
   if (ret == 0) {
      func(args...);
      __reset_env();
      eosio_assert(false, "expect_assert, no assert");
   } 
   __reset_env();
   eosio_assert(memcmp(expected, std_err.get(), N) == 0, "expect_assert, wrong assert");
   std_err.flush();
}

#define EOSIO_TEST(X) \
   int X ## _ret = setjmp(*___env_ptr); \
   if ( X ## _ret == 0 ) \
      X(); \
   else { \
      silence_output(false); \
      eosio::print("\033[1;37m", #X, " \033[0;37munit test \033[1;31mfailed\033[0m\n"); \
      ___has_failed = true; \
      silence_output(___disable_output); \
   }


#define EOSIO_TEST_BEGIN(X) \
   void X() { \
      static constexpr const char* __test_name = #X;

#define EOSIO_TEST_END \
      silence_output(false); \
      eosio::print("\033[1;37m",__test_name," \033[0;37munit test \033[1;32mpassed\033[0m\n"); \
      silence_output(___disable_output); \
   }

