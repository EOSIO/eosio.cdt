#pragma once

#include <stdint.h>
#include <string>
#include <string_view>
#include <vector>

namespace eosio {

namespace internal_use_do_not_use {
extern "C" [[eosio::wasm_import]] uint32_t get_input_data(void* dest, uint32_t size);
extern "C" [[eosio::wasm_import]] void     set_output_data(const void* data, uint32_t size);
} // namespace internal_use_do_not_use

inline std::vector<char> get_input_data() {
   std::vector<char> result(internal_use_do_not_use::get_input_data(nullptr, 0));
   internal_use_do_not_use::get_input_data(result.data(), result.size());
   return result;
}

inline std::string get_input_data_str() {
   std::string result(internal_use_do_not_use::get_input_data(nullptr, 0), 0);
   internal_use_do_not_use::get_input_data(result.data(), result.size());
   return result;
}

inline void set_output_data(const void* data, uint32_t size) { internal_use_do_not_use::set_output_data(data, size); }
inline void set_output_data(const std::vector<char>& v) { set_output_data(v.data(), v.size()); }
inline void set_output_data(const std::string_view& v) { set_output_data(v.data(), v.size()); }

// Strips off the terminating 0
template <int size>
inline void set_output_data_zstr(const char (&s)[size]) {
   set_output_data(s, size - 1);
}

} // namespace eosio
