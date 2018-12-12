#pragma once
#include <eosiolib/eosio.hpp>
#include "crt.hpp"
#include "intrinsics.hpp"
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

template <typename Pred, typename F, typename... Args>
inline bool expect_assert(bool check, const std::string& li, Pred&& pred, F&& func, Args... args) {
   std_err.clear();
   __set_env_test();
   int ret = setjmp(*___env_ptr);
   if (ret == 0) {
      func(args...);
      __reset_env();
      if (!check)
         eosio_assert(false, std::string("expect_assert, no assert {"+li+"}").c_str());
      eosio::print("expect_assert, no assert {"+li+"}\n");
      return false;
   } 
   __reset_env();
   bool failed = pred(std_err.get());
   std_err.clear();
   if (!check)
      eosio_assert(failed, std::string("expect_assert, wrong assert {"+li+"}").c_str());
   if (!failed)
      eosio::print("expect_assert, wrong assert {"+li+"}\n");
   return failed;   
}

template <size_t N, typename F, typename... Args>
inline bool expect_assert(bool check, const std::string& li, const char (&expected)[N], F&& func, Args... args) {
   return expect_assert(check, li, [&](const std::string& s) { return memcmp(expected, s.c_str(), N-1) == 0; }, func, args...);
}

#define CHECK_ASSERT(...) \
   ___has_failed |= !expect_assert(true, std::string(__FILE__)+":"+__func__+":"+(std::to_string(__LINE__)), __VA_ARGS__);

#define REQUIRE_ASSERT(...) \
   expect_assert(false, std::string(__FILE__)+":"+__func__+":"+(std::to_string(__LINE__)),  __VA_ARGS__);

#define CHECK_EQUAL(X, Y) \
   if (X != Y) { \
      ___has_failed = true; \
      eosio::print(std::string("CHECK_EQUAL failed (")+#X+" != "+#Y+") {"+__FILE__+":"+std::to_string(__LINE__)+"}\n"); \
   }

#define REQUIRE_EQUAL(X, Y) \
   eosio_assert(X == Y, std::string(std::string("REQUIRE_EQUAL failed (")+#X+" != "+#Y+") {"+__FILE__+":"+std::to_string(__LINE__)+"}").c_str());
 
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
