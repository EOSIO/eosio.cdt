#include <eosiolib/name.hpp>
#include <eosiolib/action.hpp>
#include "../intrinsics.hpp"
#include "crt.hpp"
#include <cstdint>
#include <functional>
#include <stdio.h>
#include <setjmp.h>

eosio::cdt::output_stream std_out;
eosio::cdt::output_stream std_err;

extern "C" {
   int main(int, char**);
   char* _mmap();
   
   static jmp_buf env;
   static jmp_buf test_env;
   static volatile int jmp_ret;
   jmp_buf* ___env_ptr = &env;
   char* ___heap;
   char* ___heap_ptr;
   void ___putc(char c);
   bool ___disable_output;
   bool ___has_failed;

   void _prints_l(const char* cstr, uint32_t len, uint8_t which) {
      for (int i=0; i < len; i++) {
         if (which == eosio::cdt::output_stream_kind::std_out)
            std_out.push(cstr[i]);
         else if (which == eosio::cdt::output_stream_kind::std_err)
            std_err.push(cstr[i]);
         if (!___disable_output)
            ___putc(cstr[i]);
      }
   }

   void _prints(const char* cstr, uint8_t which) {
      for (int i=0; cstr[i] != '\0'; i++) {
         if (which == eosio::cdt::output_stream_kind::std_out)
            std_out.push(cstr[i]);
         else if (which == eosio::cdt::output_stream_kind::std_err)
            std_err.push(cstr[i]);
         if (!___disable_output)
            ___putc(cstr[i]);
      }
   }

   void prints_l(const char* cstr, uint32_t len) {
      _prints_l(cstr, len, eosio::cdt::output_stream_kind::std_out);
   }

   void prints(const char* cstr) {
      _prints(cstr, eosio::cdt::output_stream_kind::std_out);
   }

   void printi(int64_t value) {
      printf("%lli\n", value);
   }

   void printui(uint64_t value) {
      printf("%llu\n", value);
   }
   
   void printi128(const int128_t* value) {
      int* tmp = (int*)value;
      printf("0x%04x%04x%04x%04x\n", tmp[0], tmp[1], tmp[2], tmp[3]);
   }

    void printui128(const uint128_t* value) {
      int* tmp = (int*)value;
      printf("0x%04x%04x%04x%04x\n", tmp[0], tmp[1], tmp[2], tmp[3]);
   }
  
   void printsf(float value) {
      printf("%f\n", value);
   }

   void printdf(double value) {
      printf("%f\n", value);
   }

   void printqf(const long double* value) {
      int* tmp = (int*)value;
      printf("0x%04x%04x%04x%04x\n", tmp[0], tmp[1], tmp[2], tmp[3]);
   }
   
   void printn(uint64_t nm) {
      std::string s = eosio::name(nm).to_string();
      prints_l(s.c_str(), s.length());
   }
   
   void printhex(const void* data, uint32_t len) {
   }

   void* memset ( void* ptr, int value, size_t num ) {
      unsigned char v = value;
      char* _ptr = (char*)ptr;
      for (int i=0; i < num; i++) {
         _ptr[i] = v;
      }
      return (void*)_ptr;
   }
   void* memcpy ( void* destination, const void* source, size_t num ) {
      char* dest = (char*)destination;
      char* src = (char*)source;
      for (int i=0; i < num; i++) {
         dest[i] = src[i];
      }
      return (void*)dest;
   }

   void* memmove ( void* destination, const void* source, size_t num ) {
      char tmp_buf[num];
      char* dest = (char*)destination;
      char* src = (char*)source;
      for (int i=0; i < num; i++)
         tmp_buf[i] = src[i];
      for (int i=0; i < num; i++)
         dest[i] = tmp_buf[i];
      return (void*)dest;
   }
   
   void eosio_assert(uint32_t test, const char* msg) {
      if (test == 0) {
         _prints(msg, eosio::cdt::output_stream_kind::std_err);
         _prints_l("\n", 1, eosio::cdt::output_stream_kind::none);
         longjmp(*___env_ptr, 1);
      }
   }

   void eosio_assert_message(uint32_t test, const char* msg, uint32_t len) {
      if (test == 0) {
         _prints_l(msg, len, eosio::cdt::output_stream_kind::std_err);
         _prints_l("\n", 1, eosio::cdt::output_stream_kind::none);
         longjmp(*___env_ptr, 1);
      }
   }

   void eosio_assert_code(uint32_t test, uint64_t code) {
      if (test == 0) {
         char buff[32];
         snprintf(buff, 32, "%llu\n", code);
         _prints(buff, eosio::cdt::output_stream_kind::std_err);
         _prints_l("\n", 1, eosio::cdt::output_stream_kind::none);
         longjmp(*___env_ptr, 1);
      }
   }
   
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-noreturn" 
   void abort() {
      eosio_assert(false, "abort");
   }
#pragma clang diagnostic pop
   
   size_t __builtin_wasm_current_memory() {
      return (size_t)___heap_ptr;
   }

   size_t __builtin_wasm_grow_memory(size_t size) {
      if ((___heap_ptr + (size*64*1024)) > (___heap_ptr + 100*1024*1024))
         eosio_assert(false, "__builtin_wasm_grow_memory");
      ___heap_ptr += (size*64*1024);
      return (size_t)___heap_ptr;
   }
   
   void __set_env_test() {
      ___env_ptr = &test_env;
   }
   void __reset_env() {
      ___env_ptr = &env;
   }
   int _wrap_main(int argc, char** argv) {
      int ret_val = 0;
      ___heap = _mmap();
      ___heap_ptr = ___heap;
      ___disable_output = false;
      ___has_failed = false;
      jmp_ret = setjmp(env); 
      if (jmp_ret == 0) {
         ret_val = main(argc, argv);
      } else {
         ret_val = -1;
      }
      return ret_val;
   }
}
