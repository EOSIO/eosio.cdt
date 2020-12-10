#pragma once

#include <deque>
#include <list>
#include <map>
#include <optional>
#include <set>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>
#include <string>
#include <cstring>

#include "datastream.hpp"
#include "reflect.hpp"

namespace eosio {

   namespace detail {
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

      template <template <typename> class C, typename T>
      constexpr bool is_ranged_type(C<T>) {
         using type = std::decay_t<C<T>>;
         return
            std::is_same_v<std::vector<T>, type> ||
            std::is_same_v<std::list<T>, type>   ||
            std::is_same_v<std::deque<T>, type>  ||
            std::is_same_v<std::set<T>, type>;
      }

      template <typename R, typename C>
      auto member_pointer_type(R (C::*)) -> R;
      template <typename R, typename C>
      auto member_pointer_class(R (C::*)) -> C;

      template <typename... Args>
      constexpr inline std::size_t total_bytes_size() { return (sizeof(Args) + ...); }

      // TODO rework the to_key and datastream logic to be constexpr/consteval friendly to get rid of this
      template <std::size_t I, typename Arg, typename... Args>
      inline void const_pack_helper(std::string& s, Arg&& arg, Args&&... args) {
         std::memcpy(s.data()+I, &arg, sizeof(Arg));
         if constexpr (std::is_integral_v<std::decay_t<Arg>> ||
                  std::is_same_v<std::decay_t<Arg>, eosio::name>) {
            std::reverse(s.data()+I, s.data()+I+sizeof(Arg));
         }
         if constexpr (sizeof...(Args) > 0) {
            return const_pack_helper<I+sizeof(Arg)>(s, std::forward<Args>(args)...);
         }
      }

      // TODO rework the to_key and datastream logic to be constexpr/consteval friendly to get rid of this
      template <typename... Args>
      inline std::string const_pack(Args&&... args) {
         std::string s;
         s.resize(total_bytes_size<Args...>());
         const_pack_helper<0>(s, std::forward<Args>(args)...);
         return s;
      }
   } // namespace eosio::detail

using key_type = std::string;

// to_key defines a conversion from a type to a sequence of bytes whose lexicograpical
// ordering is the same as the ordering of the original type.
//
// For any two objects of type T, a and b:
//
// - key(a) < key(b) iff a < b
// - key(a) is not a prefix of key(b)
//
// Overloads of to_key for user-defined types can be found by Koenig (ADL) lookup.
//
// to_key is specialized for the following types
// - std::string and std::string_view
// - std::vector, std::list, std::deque
// - std::tuple
// - std::array
// - std::optional
// - std::variant
// - Arithmetic types
// - Scoped enumeration types
// - Reflected structs
// - All smart-contract related types defined by abieos
template <typename T, typename S>
void to_key(const T& obj, datastream<S>& stream);

template <int I, typename T, typename S>
void to_key_tuple(const T& obj, datastream<S>& stream) {
   if constexpr (I < std::tuple_size_v<T>) {
      to_key(std::get<I>(obj), stream);
      to_key_tuple<I + 1>(obj, stream);
   }
}

template <typename... Ts, typename S>
void to_key(const std::tuple<Ts...>& obj, datastream<S>& stream) {
   to_key_tuple<0>(obj, stream);
}

template <typename T, std::size_t N, typename S>
void to_key(const std::array<T, N>& obj, datastream<S>& stream) {
   for (const T& elem : obj) { to_key(elem, stream); }
}

template <typename T, typename S>
void to_key_optional(const bool* obj, datastream<S>& stream) {
   if (obj == nullptr)
      stream.write('\0');
   else if (!*obj)
      stream.write('\1');
   else {
      stream.write('\2');
   }
}

template <typename T, typename S>
void to_key_optional(const T* obj, datastream<S>& stream) {
   if constexpr (detail::has_bitwise_serialization<T>() && sizeof(T) == 1) {
      if (obj == nullptr)
         stream.write("\0", 2);
      else {
         char             buf[1];
         datastream<char*> tmp_stream(buf, 1);
         to_key(*obj, tmp_stream);
         stream.write(buf[0]);
         if (buf[0] == '\0')
            stream.write('\1');
      }
   } else {
      if (obj) {
         stream.write('\1');
         to_key(*obj, stream);
      } else {
         stream.write('\0');
      }
   }
}

template <typename T, typename U, typename S>
void to_key(const std::pair<T, U>& obj, datastream<S>& stream) {
   to_key(obj.first, stream);
   to_key(obj.second, stream);
}

template <typename T, typename S>
void to_key_range(const T& obj, datastream<S>& stream) {
   for (const auto& elem : obj) { to_key_optional(&elem, stream); }
   to_key_optional((std::add_pointer_t<decltype(*std::begin(obj))>) nullptr, stream);
}

template <typename T, typename S>
auto to_key(const T& obj, datastream<S>& stream) -> std::enable_if_t<is_ranged_type(std::declval<T>()), void> {
   to_key_range(obj, stream);
}

template <typename T, typename U, typename S>
void to_key(const std::map<T, U>& obj, datastream<S>& stream) {
   to_key_range(obj, stream);
}

template <typename T, typename S>
void to_key(const std::optional<T>& obj, datastream<S>& stream) {
   to_key_optional(obj ? &*obj : nullptr, stream);
}

// The first byte holds:
// 0-4 1's (number of additional bytes) 0 (terminator) bits
//
// The number is represented as big-endian using the low order
// bits of the first byte and all of the remaining bytes.
//
// Notes:
// - values must be encoded using the minimum number of bytes,
//   as non-canonical representations will break the sort order.
template <typename S>
void to_key_varuint32(std::uint32_t obj, datastream<S>& stream) {
   int num_bytes;
   if (obj < 0x80u) {
      num_bytes = 1;
   } else if (obj < 0x4000u) {
      num_bytes = 2;
   } else if (obj < 0x200000u) {
      num_bytes = 3;
   } else if (obj < 0x10000000u) {
      num_bytes = 4;
   } else {
      num_bytes = 5;
   }

   stream.write(
         static_cast<char>(~(0xFFu >> (num_bytes - 1)) | (num_bytes == 5 ? 0 : (obj >> ((num_bytes - 1) * 8)))));
   for (int i = num_bytes - 2; i >= 0; --i) { stream.write(static_cast<char>((obj >> i * 8) & 0xFFu)); }
}

// for non-negative values
//  The first byte holds:
//   1 (signbit) 0-4 1's (number of additional bytes) 0 (terminator) bits
//  The value is represented as big endian
// for negative values
//  The first byte holds:
//   0 (signbit) 0-4 0's (number of additional bytes) 1 (terminator) bits
//   The value is adjusted to be positive based on the range that can
//   be represented with this number of bytes and then encoded as big endian.
//
// Notes:
// - negative values must sort before positive values
// - For negative value, numbers that need more bytes are smaller, hence
//   the encoding of the width must be opposite the encoding used for
//   non-negative values.
// - A 5-byte varint can represent values in $[-2^34, 2^34)$.  In this case,
//   the argument will be sign-extended.
template <typename S>
void to_key_varint32(std::int32_t obj, datastream<S>& stream) {
   static_assert(std::is_same_v<S, void>, "to_key for varint32 has been temporarily disabled");
   int  num_bytes;
   bool sign = (obj < 0);
   if (obj < 0x40 && obj >= -0x40) {
      num_bytes = 1;
   } else if (obj < 0x2000 && obj >= -0x2000) {
      num_bytes = 2;
   } else if (obj < 0x100000 && obj >= -0x100000) {
      num_bytes = 3;
   } else if (obj < 0x08000000 && obj >= -0x08000000) {
      num_bytes = 4;
   } else {
      num_bytes = 5;
   }

   unsigned char width_field;
   if (sign) {
      width_field = 0x80u >> num_bytes;
   } else {
      width_field = 0x80u | ~(0xFFu >> num_bytes);
   }
   auto          uobj       = static_cast<std::uint32_t>(obj);
   unsigned char value_mask = (0xFFu >> (num_bytes + 1));
   unsigned char high_byte  = (num_bytes == 5 ? (sign ? 0xFF : 0) : (uobj >> ((num_bytes - 1) * 8)));
   stream.write(width_field | (high_byte & value_mask));
   for (int i = num_bytes - 2; i >= 0; --i) { stream.write(static_cast<char>((uobj >> i * 8) & 0xFFu)); }
}

template <typename... Ts, typename S>
void to_key(const std::variant<Ts...>& obj, datastream<S>& stream) {
   to_key_varuint32(static_cast<uint32_t>(obj.index()), stream);
   std::visit([&](const auto& item) { to_key(item, stream); }, obj);
}

template <std::size_t N, typename S>
void to_key(const char (&str)[N], datastream<S>& stream) {
   to_key(std::string_view{str, N-1}, stream);
}

template <typename S>
void to_key(std::string_view obj, datastream<S>& stream) {
   for (char ch : obj) {
      stream.write(ch);
      if (ch == '\0') {
         stream.write('\1');
      }
   }
   stream.write("\0", 2);
}

template <typename S>
void to_key(const std::string& obj, datastream<S>& stream) {
   to_key(std::string_view(obj), stream);
}

template <typename S>
void to_key(bool obj, datastream<S>& stream) {
   stream.write(static_cast<char>(obj ? 1 : 0));
}

template <typename UInt, typename T>
UInt float_to_key(T value) {
   static_assert(sizeof(T) == sizeof(UInt), "Expected unsigned int of the same size");
   UInt result;
   std::memcpy(&result, &value, sizeof(T));
   UInt signbit = (static_cast<UInt>(1) << (std::numeric_limits<UInt>::digits - 1));
   UInt mask    = 0;
   if (result == signbit)
      result = 0;
   if (result & signbit)
      mask = ~mask;
   return result ^ (mask | signbit);
}

template <typename T, typename S>
void to_key(const T& obj, datastream<S>& stream) {
   if constexpr (std::is_floating_point_v<T>) {
      if constexpr (sizeof(T) == 4) {
         to_key(float_to_key<uint32_t>(obj), stream);
      } else {
         static_assert(sizeof(T) == 8, "Unknown floating point type");
         to_key(float_to_key<uint64_t>(obj), stream);
      }
   } else if constexpr (std::is_integral_v<T>) {
      auto v = static_cast<std::make_unsigned_t<T>>(obj);
      v -= static_cast<std::make_unsigned_t<T>>(std::numeric_limits<T>::min());
      std::reverse(reinterpret_cast<char*>(&v), reinterpret_cast<char*>(&v + 1));
      stream.write(&v, sizeof(v));
   } else if constexpr (std::is_enum_v<T>) {
      static_assert(!std::is_convertible_v<T, std::underlying_type_t<T>>, "Serializing unscoped enum");
      to_key(static_cast<std::underlying_type_t<T>>(obj), stream);
   } else {
      using ts_meta = bluegrass::meta::meta_object<T>;
      ts_meta::for_each_field(obj, [&](const auto& member) {
         to_key(member, stream);
      });
   }
}

template <typename T>
void convert_to_key(const T& t, key_type& bin) {
   datastream<size_t> ss;
   to_key(t, ss);
   auto orig_size = bin.size();
   bin.resize(orig_size + ss.tellp());
   datastream<char*> fbs(bin.data() + orig_size, ss.tellp());
   to_key(t, fbs);
   check( fbs.valid(), "Stream overrun" );
}

template <typename T>
key_type convert_to_key(const T& t) {
   key_type result;
   convert_to_key(t, result);
   return result;
}
} // namespace eosio
