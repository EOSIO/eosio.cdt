#pragma once

#include "../../core/eosio/reflection.hpp"
#include <utility>

// This causes compilation erros, but everything builds fine without it.
#if 0
#if __has_include(<boost/pfr/precise/core.hpp>)

#include <boost/pfr/precise/core.hpp>

namespace eosio {
template <typename T, typename F>
constexpr auto for_each_field(T&& t, F&& f) -> std::enable_if_t<!reflection::has_for_each_field_v<std::decay_t<T>>> {
   return boost::pfr::for_each_field(static_cast<T&&>(t), static_cast<F&&>(f));

// namespace eosio
#endif
#endif

namespace eosio {

template <typename T, typename F>
constexpr auto for_each_field(T&& t, F&& f) -> std::enable_if_t<reflection::has_for_each_field_v<std::decay_t<T>>> {
   eosio_for_each_field((std::decay_t<T>*)nullptr, [&](const char*, auto member) {
      if constexpr (std::is_member_object_pointer_v<decltype(member(&t))>) {
         f(t.*member(&t));
      }
   });
}

template <typename T, typename F>
constexpr void for_each_field(F&& f) {
   eosio_for_each_field((T*)nullptr, [&f](const char* name, auto member) {
      if constexpr (std::is_member_object_pointer_v<decltype(member((T*)nullptr))>) {
         f(name, [member](auto p) -> decltype((p->*member(p))) { return p->*member(p); });
      }
   });
}

// Calls f(#fn_name, &T::fn_name) for every reflected member function of T.
template <typename T, typename F>
constexpr void for_each_method(F&& f) {
   eosio_for_each_field((T*)nullptr, [&f](const char* name, auto member) {
      if constexpr (std::is_member_function_pointer_v<decltype(member((T*)nullptr))>) {
         f(name, member((T*)nullptr));
      }
   });
}

} // namespace eosio
