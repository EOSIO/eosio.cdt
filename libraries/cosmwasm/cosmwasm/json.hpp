#pragma once
#include "memory.hpp"
#include "check.hpp"
#include "uint128_t.hpp"
#include <string_view>

#define PICOJSON_USE_INT64
#define PICOJSON_USE_LOCALE 0
#define PICOJSON_ASSERT(e) \
   do { \
      if (!(e)) { \
         abort(); \
      } \
   } while (0)

#include "picojson/picojson.h"

namespace cosmwasm { namespace json {

   using value = picojson::value;

   namespace _detail {
      uint32_t char_to_digit(char c) {
         if (c >= '0' && c <= '9')
            return c - '0';
         if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
         if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
         abort();
      }

      template<typename T>
      T from_str_radix(const std::string& s, uint32_t radix) {
         T v = 0;
         int is_signed = -1;

         check(s.size() && (radix >= 2 && radix <= 16));

         if (s[0] == '+') {
            is_signed = 0;
         } else if (s[0] == '-') {
            check(std::is_signed_v<T>);
            is_signed = 1;
         }

         for (auto i = (is_signed >= 0 ? 1 : 0); i < s.size(); ++i) {
            T tmp = v;
            v *= radix;
            check(v >= tmp);

            tmp = v;
            v += char_to_digit(s[i]);
            check(v >= tmp);
         }
         if (is_signed == 1) {
            v *= -1;
         }
         return v;
      }

      template<typename T>
      struct is_vector {
         static constexpr bool value = false;
      };

      template<template<typename...> class C, typename U>
      struct is_vector<C<U>> {
         static constexpr bool value = std::is_same<C<U>,std::vector<U>>::value;
      };
   }

   template<typename T, std::enable_if_t<std::is_integral_v<std::decay_t<T>>, int> = 0>
   T from_json(const value& v) {
      return v.get<int64_t>();
   }

   template<>
   bool from_json(const value& v) {
      return v.get<bool>();
   }

   template<>
   uint64_t from_json(const value& v) {
      uint64_t out = 0;
      std::stoull(v.get<std::string>(), reinterpret_cast<size_t*>(&out));
      return out;
   }

   template<>
   uint128_t from_json(const value& v) {
      return _detail::from_str_radix<uint128_t>(v.get<std::string>(), 10);
   }

   template<typename T, std::enable_if_t<std::is_class_v<T> && !_detail::is_vector<T>::value>* = nullptr>
   T from_json(const value& v) {
      return T::from_json(v);
   }

   template<>
   std::string from_json(const value& v) {
      return v.get<std::string>();
   }

   template<typename T, std::enable_if_t<std::is_class_v<T> && _detail::is_vector<T>::value>* = nullptr>
   T from_json(const value& v) {
      value::array arr = v.get<value::array>();
      T out(arr.size());
      for (auto& e: arr) {
         out.push_back(from_json<typename T::value_type>(e));
      }
      return out;
   }

   template<typename T, std::enable_if_t<std::is_integral_v<std::decay_t<T>>, int> = 0>
   value to_json(T v) {
      return value(v);
   }

   template<typename T, std::enable_if_t<std::is_class_v<T> && !_detail::is_vector<T>::value>* = nullptr>
   value to_json(const T& v) {
      return T::to_json(v);
   }

   template<typename T, std::enable_if_t<std::is_class_v<T> && _detail::is_vector<T>::value>* = nullptr>
   value to_json(const T& v) {
      value::array out;
      for (auto& e: v) {
         out.push_back(to_json(e));
      }
      return value(out);
   }

   template<>
   value to_json(uint64_t v) {
      return value(std::to_string(v));
   }

   template<>
   value to_json(uint128_t v) {
      return value(std::to_string(v));
   }

   template<>
   value to_json(const std::string& v) {
      return value(v);
   }

   value parse_region(region* reg) {
      std::string_view s((const char*)reg->offset, reg->length);
      value v;
      std::string err = parse(v, s);
      check(err.empty());
      return v;
   }

   template<typename T>
   T from_region(region* reg) {
      return from_json<T>(parse_region(reg));
   }

   template<typename T>
   std::unique_ptr<region> to_region(const T& v) {
      std::string s = to_json(v).serialize();
      return build_region_dup(s.data(), s.size());
   }

} }
