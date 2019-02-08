#pragma once
#include <setjmp.h>

#warning "<eosio/native/crt.hpp> is deprecated use <eosio/crt.hpp>"
namespace eosio { namespace cdt {
   enum output_stream_kind {
      std_out,
      std_err,
      none
   };
   struct output_stream {
      char output[1024*2];
      size_t index = 0;
      std::string to_string()const { return std::string((const char*)output, index); }
      const char* get()const { return output; }
      void push(char c) { output[index++] = c; }
      void clear() { index = 0; }
   };
}} //ns eosio::cdt

extern eosio::cdt::output_stream std_out;
extern eosio::cdt::output_stream std_err;
extern "C" jmp_buf* ___env_ptr;
extern "C" char*    ___heap_ptr;

extern "C" {
   void __set_env_test();
   void __reset_env();
   void _prints_l(const char* cstr, uint32_t len, uint8_t which);
   void _prints(const char* cstr, uint8_t which);
}
