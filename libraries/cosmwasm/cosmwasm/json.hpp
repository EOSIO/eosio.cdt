#pragma once
#include "memory.hpp"
#include "check.hpp"
#include "primitives.hpp"
#include "base64.hpp"
#include <string_view>
#include <optional>
#include <variant>

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
   using null = picojson::null;

   namespace _detail {
      template<typename T, typename Enable = void>
      struct is_vector : std::false_type {};

      template<typename T>
      struct is_vector<std::vector<T>> : std::true_type {};

      template<typename T, typename Enable = void>
      struct is_optional : std::false_type {};

      template<typename T>
      struct is_optional<std::optional<T>> : std::true_type {};

      template<typename T, typename Enable = void>
      struct is_variant : std::false_type {};

      template<typename... T>
      struct is_variant<std::variant<T...>> : std::true_type {};
   }

   inline value parse_unsafe(const std::string &s) {
      value out;
      std::string err = parse(out, s);
      if (err.size())
         abort();
      return out;
   }

   inline value parse_unsafe(const std::string_view &s) {
      value out;
      std::string err = parse(out, s);
      if (err.size())
         abort();
      return out;
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
      if (v.is<int64_t>()) {
         return v.get<int64_t>();
      } else {
         return std::stoull(v.get<std::string>(), nullptr);
      }
   }

   template<>
   uint128_t from_json(const value& v) {
      return uint128_t(v.get<std::string>());
   }

   template<typename T, std::enable_if_t<std::is_class_v<T> &&
      !_detail::is_optional<T>::value &&
      !_detail::is_vector<T>::value &&
      !_detail::is_variant<T>::value>* = nullptr>
   T from_json(const value& v) {
      return T::from_json(v);
   }

   template<>
   std::string from_json(const value& v) {
      return v.get<std::string>();
   }

   template<typename T, std::enable_if_t<std::is_class_v<T> &&
      !_detail::is_optional<T>::value &&
      _detail::is_vector<T>::value>* = nullptr>
   T from_json(const value& v) {
      value::array arr = v.get<value::array>();
      T out(arr.size());
      for (auto& e: arr) {
         out.push_back(from_json<typename T::value_type>(e));
      }
      return out;
   }

   template<>
   bytes from_json<bytes>(const value& v) {
      std::string s = v.get<std::string>();
      return base64::decode(s);
   }

   template<typename T, std::enable_if_t<_detail::is_optional<T>::value>* = nullptr>
   T from_json(const value& v) {
      if (v.is<null>()) {
         return std::nullopt;
      } else {
         return from_json<typename T::value_type>(v);
      }
   }

   template<typename T, std::enable_if_t<std::is_class_v<T> &&
      !_detail::is_variant<T>::value>* = nullptr>
   std::optional<T> try_from_json(const value& v) {
      if (!v.is<value::object>() || !v.contains(T::__typename))
         return std::nullopt;
      return from_json<T>(v.get(T::__typename));
   }

   namespace _detail {
      template<std::size_t I>
      using index_t = std::integral_constant<std::size_t, I>;
      template<std::size_t I>
      constexpr index_t<I> index{};

      template<std::size_t...Is>
      constexpr std::tuple< index_t<Is>... > make_indexes(std::index_sequence<Is...>){
        return std::make_tuple(index<Is>...);
      }
      template<std::size_t N>
      constexpr auto indexing_tuple = make_indexes(std::make_index_sequence<N>{});

      template<std::size_t...Is, class F, class T>
      auto tuple_foreach( std::index_sequence<Is...>, T&& tup, F&& f ) {
        ( f( std::get<Is>( std::forward<T>(tup) ) ), ... );
      }
      template<class F, class T>
      auto tuple_foreach( T&& tup, F&& f ) {
        auto indexes = std::make_index_sequence< std::tuple_size_v< std::decay_t<T> > >{};
        return tuple_foreach( indexes, std::forward<T>(tup), std::forward<F>(f) );
      }
   }

   template<typename T, std::enable_if_t<std::is_class_v<T> &&
      _detail::is_variant<T>::value>* = nullptr>
   std::optional<T> try_from_json(const value& v) {
      auto indexes = _detail::indexing_tuple<std::variant_size_v<T>>;
      std::optional<T> retval;
      _detail::tuple_foreach(indexes, [&](auto I) {
         if (retval) return;
         auto p = try_from_json<std::variant_alternative_t<I,T>>(v);
         if (p) retval.emplace(std::move(*p));
      });
      return retval;
   }

   template<typename T, std::enable_if_t<_detail::is_variant<T>::value>* = nullptr>
   T from_json(const value& v) {
      return *try_from_json<T>(v);
   }

   template<typename T, std::enable_if_t<std::is_integral_v<std::decay_t<T>>, int> = 0>
   value to_json(T v) {
      return value((int64_t)v);
   }

   template<typename T, std::enable_if_t<std::is_class_v<T> &&
      !_detail::is_optional<T>::value &&
      !_detail::is_vector<T>::value &&
      !_detail::is_variant<T>::value>* = nullptr>
   value to_json(const T& v) {
      return T::to_json(v);
   }

   template<typename T, std::enable_if_t<std::is_class_v<T> &&
      !_detail::is_optional<T>::value &&
      _detail::is_vector<T>::value>* = nullptr>
   value to_json(const T& v) {
      value::array out;
      for (auto& e: v) {
         out.push_back(to_json(e));
      }
      return value(out);
   }

   template<typename T, std::enable_if_t<std::is_class_v<T> &&
      !_detail::is_optional<T>::value &&
      _detail::is_variant<T>::value>* = nullptr>
   value to_json(const T& v) {
      auto indexes = _detail::indexing_tuple<std::variant_size_v<T>>;
      std::optional<json::value> retval;
      _detail::tuple_foreach(indexes, [&](auto I) {
         if (retval) return;
         auto p = std::get_if<I>(&v);
         if (p) retval.emplace(to_json(*p));
      });
      return (retval) ? *retval : json::value();
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

   template<>
   value to_json<bytes>(const bytes& v) {
      return value(base64::encode(v));
   }

   template<typename T, std::enable_if_t<_detail::is_optional<T>::value>* = nullptr>
   value to_json(const T& v) {
      if (!v) {
         return value();
      } else {
         return json::to_json(*v);
      }
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
