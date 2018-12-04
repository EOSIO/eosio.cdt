#pragma once

namespace eosio { namespace cdt {
   enum output_stream_kind {
      std_out,
      std_err
   };
   struct output_stream {
      char output[1024*2];
      size_t index = 0;
      std::string to_string()const { return std::string((const char*)output, index); }
      void push(char c) { output[index++] = c; }
      void flush() { index = 0; }
   };
}} //ns eosio::cdt

extern eosio::cdt::output_stream std_out;
extern eosio::cdt::output_stream std_err;
