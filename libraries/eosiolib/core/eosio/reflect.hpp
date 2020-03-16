#pragma once

#include <string>

namespace eosio {

   struct reflect_error_t {};
   inline static constexpr reflect_error_t reflect_error;

   template <char... Str>
   struct compile_time_string {
      static constexpr const char value[] = {Str..., '\0'};
   };

   template <typename Type, typename Name>
   struct type_name_pair {
      using type_t = Type;
      using name_t = Name;
   };

   template <typename Pair>
   inline constexpr auto pair_name_v = Pair::name_t::value;

   template <typename Pair>
   using pair_type_t = typename Pair::type_t;

   template <std::size_t N, typename T, typename R>
   constexpr R meta_get(const T& t);

   template <typename Compile_Str, typename T, typename R>
   constexpr R meta_get(const T& t);

   template <typename T>
   constexpr std::size_t meta_arity();

   template <typename T, typename Attr>
   constexpr bool meta_has_attribute();

   template <typename T, typename R>
   constexpr R meta_get_attributes();
} // ns eosio

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
template <typename T, T... Str>
inline constexpr eosio::compile_time_string<Str...> operator""_cts() {
   return eosio::compile_time_string<Str...>{};
}
#pragma clang diagnostic pop
