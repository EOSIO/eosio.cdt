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

      template <typename R, typename C>
      auto member_pointer_type(R (C::*)) -> R;
      template <typename R, typename C>
      auto member_pointer_class(R (C::*)) -> C;

      template <typename... Args>
      constexpr inline std::size_t total_bytes_size() { return (sizeof(Args) + ...); }

      // TODO rework the to_key and datastream logic to be constexpr/consteval friendly to get rid of this
      template <std::size_t I, typename Arg, typename... Args>
      inline void const_pack_helper(eosio::key_type& s, Arg&& arg, Args&&... args) {
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
      inline eosio::key_type const_pack(Args&&... args) {
         eosio::key_type s;
         s.resize(total_bytes_size<Args...>());
         const_pack_helper<0>(s, std::forward<Args>(args)...);
         return s;
      }
   } // namespace eosio::detail


} // namespace eosio
