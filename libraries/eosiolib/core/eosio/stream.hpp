#pragma once

#include "../../core/eosio/check.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <type_traits>
#include <string.h>
#include <algorithm>
#include <system_error>
#include <string_view>

namespace eosio {

enum class stream_error {
   no_error,
   overrun,
   underrun,
   float_error,
   varuint_too_big,
   invalid_varuint_encoding,
   bad_variant_index,
   invalid_asset_format,
   array_size_mismatch,
   invalid_name_char,
   invalid_name_char13,
   name_too_long,
   json_writer_error, // !!!
}; // stream_error

constexpr inline std::string_view convert_stream_error(stream_error e) {
   switch (e) {
         // clang-format off
      case stream_error::no_error:                 return "No error";
      case stream_error::overrun:                  return "Stream overrun";
      case stream_error::underrun:                 return "Stream underrun";
      case stream_error::float_error:              return "Float error";
      case stream_error::varuint_too_big:          return "Varuint too big";
      case stream_error::invalid_varuint_encoding: return "Invalid varuint encoding";
      case stream_error::bad_variant_index:        return "Bad variant index";
      case stream_error::invalid_asset_format:     return "Invalid asset format";
      case stream_error::array_size_mismatch:      return "T[] size and unpacked size don't match";
      case stream_error::invalid_name_char:        return "character is not in allowed character set for names";
      case stream_error::invalid_name_char13:      return "thirteenth character in name cannot be a letter that comes after j";
      case stream_error::name_too_long:            return "string is too long to be a valid name";
      case stream_error::json_writer_error: return "Error writing json";
         // clang-format on

      default: return "unknown";
   }
}

template<typename T>
constexpr bool has_bitwise_serialization() {
   if constexpr (std::is_arithmetic_v<T>) {
      return true;
   } else if constexpr (std::is_enum_v<T>) {
      static_assert(!std::is_convertible_v<T, std::underlying_type_t<T>>, "Serializing unscoped enum");
      return true;
   } else {
      return false;
   }
}

template <int max_size>
struct small_buffer {
   char  data[max_size];
   char* pos{ data };

   void reverse() { std::reverse(data, pos); }
};

struct vector_stream {
   std::vector<char>& data;
   vector_stream(std::vector<char>& data) : data(data) {}

   void write(char c) {
      data.push_back(c);
   }
   void write(const void* src, std::size_t sz) {
      auto s = reinterpret_cast<const char*>(src);
      data.insert( data.end(), s, s + sz );
   }
   template <typename T>
   void write_raw(const T& v) {
      write(&v, sizeof(v));
   }
};

struct fixed_buf_stream {
   char* pos;
   char* end;

   fixed_buf_stream(char* pos, size_t size) : pos{ pos }, end{ pos + size } {}

   void write(char c) {
      check( pos < end, convert_stream_error(stream_error::overrun) );
      *pos++ = c;
   }

   void write(const void* src, std::size_t sz) {
      check( pos + sz <= end, convert_stream_error(stream_error::overrun) );
      memcpy(pos, src, sz);
      pos += sz;
   }

   template <int Size>
   void write(const char (&src)[Size]) {
      write(src, Size);
   }

   template <typename T>
   void write_raw(const T& v) {
      write(&v, sizeof(v));
   }
};

struct size_stream {
   size_t size = 0;

   void write(char c) {
      ++size;
   }

   void write(const void* src, std::size_t sz) {
      size += sz;
   }

   template <int Size>
   void write(const char (&src)[Size]) {
      size += Size;
   }

   template <typename T>
   void write_raw(const T& v) {
      size += sizeof(v);
   }
};

template <typename S>
void increase_indent(S&) {
}

template <typename S>
void decrease_indent(S&) {
}

template <typename S>
void write_colon(S& s) {
   s.write(':');
}

template <typename S>
void write_newline(S&) {
}

template <typename Base>
struct pretty_stream : Base {
   using Base::Base;
   int               indent_size = 4;
   std::vector<char> current_indent;
};

template <typename S>
void increase_indent(pretty_stream<S>& s) {
   s.current_indent.resize(s.current_indent.size() + s.indent_size, ' ');
}

template <typename S>
void decrease_indent(pretty_stream<S>& s) {
   check( s.current_indent.size() >= s.indent_size,
         convert_stream_error(stream_error::overrun) );
   s.current_indent.resize(s.current_indent.size() - s.indent_size);
}

template <typename S>
void write_colon(pretty_stream<S>& s) {
   s.write(": ", 2);
}

template <typename S>
void write_newline(pretty_stream<S>& s) {
   s.write('\n');
   s.write(s.current_indent.data(), s.current_indent.size());
}

struct input_stream {
   const char* pos;
   const char* end;

   input_stream() : pos{ nullptr }, end{ nullptr } {}
   input_stream(const char* pos, size_t size) : pos{ pos }, end{ pos + size } {}
   input_stream(const char* pos, const char* end) : pos{ pos }, end{ end } {}
   input_stream(const std::vector<char>& v) : pos{ v.data() }, end{ v.data() + v.size() } {}
   input_stream(std::string_view v) : pos{ v.data() }, end{ v.data() + v.size() } {}
   input_stream(const input_stream&) = default;

   input_stream& operator=(const input_stream&) = default;

   size_t remaining() const { return end - pos; }

   void check_available(size_t size) const {
      check( size <= std::size_t(end-pos), convert_stream_error(stream_error::overrun) );
   }

   auto get_pos()const { return pos; }

   void read(void* dest, size_t size) {
      check( size <= size_t(end-pos), convert_stream_error(stream_error::overrun) );
      memcpy(dest, pos, size);
      pos += size;
   }

   template <typename T>
   void read_raw(T& dest) {
      read(&dest, sizeof(dest));
   }

   void skip(size_t size) {
      check( size <= size_t(end-pos), convert_stream_error(stream_error::overrun) );
      pos += size;
   }

   void read_reuse_storage(const char*& result, size_t size) {
      check( size <= size_t(end-pos), convert_stream_error(stream_error::overrun) );
      result = pos;
      pos += size;
   }
};

} // namespace eosio
