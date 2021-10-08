#pragma once

#include <cstdint>
#include <type_traits>
#include <tuple>
#include <utility>

namespace bluegrass { namespace meta {
   // helpers for std::visit
   template <class... Ts>
   struct overloaded : Ts... {
      using Ts::operator()...;
   };
   template <class... Ts>
   overloaded(Ts...)->overloaded<Ts...>;

   template <char... Str>
   struct ct_string {
      constexpr static const char value[] = {Str..., '\0'};
      constexpr static std::size_t size = sizeof...(Str)+1;
   };

   namespace detail {
      template <char C, typename CTS>
      struct ct_string_appender;
      template <char C, template <char...> class CTS, char... Str>
      struct ct_string_appender<C, CTS<Str...>> {
         using type = ct_string<Str..., C>;
      };
      template <typename New, typename O>
      struct ct_string_reverser;
      template <typename New, template <char> class O, char C>
      struct ct_string_reverser<New, O<C>> {
         using type = typename ct_string_appender<C, New>::type;
      };
      template <typename New, template <char, char...> class O, char C, char... Rest>
      struct ct_string_reverser<New, O<C, Rest...>> {
         using type = typename ct_string_reverser<typename ct_string_appender<C, New>::type, ct_string<Rest...>>::type;
      };
      template <char Test, typename T>
      struct trim_front;
      template <template <char, char...> class CTS, char C, char... Rest>
      struct trim_front<C, CTS<C, Rest...>> {
         using value = ct_string<Rest...>;
      };
      template <char Test, typename T>
      struct trim_back {
         using type = typename ct_string_reverser<ct_string<>, typename trim_front<Test, typename ct_string_reverser<ct_string<>, T>::type>::type>::type;
      };
   } // ns bluegrass::meta::detail

   template <typename T>
   [[deprecated]]
   constexpr inline T check_type() { return T{}; } // you can this to print the type at compile type as part of the deprecation warning

}} // ns bluegrass::meta

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
   template <typename T, T... Str>
   constexpr inline bluegrass::meta::ct_string<Str...> operator""_cts() { return bluegrass::meta::ct_string<Str...>{}; }
#pragma clang diagnostic pop

