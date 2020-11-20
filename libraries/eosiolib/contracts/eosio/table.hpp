#pragma once

#include "map.hpp"

#include <functional>

namespace eosio::kv {
   namespace detail {
      template <typename Index>
      static inline name name_forwarder(name owner) { return owner; }

      // there is a bug in clang that is preventing this alias from being used, therefore I am going old fashioned
      // using record_type = decltype(eosio::detail::member_pointer_class(Index::member));
      template <typename Index, typename...>
      struct record_type {
         using type = decltype(eosio::detail::member_pointer_class(Index::member));
      };

      // there is a bug in clang that is preventing this alias from being used, therefore I am going old fashioned
      // using record_type = decltype(eosio::detail::member_pointer_class(Index::member));
      template <std::size_t N, typename Index, typename... Indices>
      constexpr inline static auto index_type_at() -> std::tuple_element_t<N, std::tuple<Index, Indices...>>;

      template <typename Index, typename... Indices>
      struct first_index_name {
         constexpr inline static eosio::name::raw name = Index::name;
      };

      template <eosio::name::raw Test, typename Index, typename... Indices>
      constexpr inline static std::size_t index_ordinal_helper() {
         if constexpr (sizeof...(Indices) > 0)
            if constexpr (Test == Index::name)
               return 0;
            else
               return index_ordinal_helper<Test, Indices...>() + 1;
         else
            return std::numeric_limits<std::size_t>::max();
      }

      template <eosio::name::raw Test, typename... Indices>
      constexpr inline static std::size_t index_ordinal() {
         return index_ordinal_helper<Test, Indices...>();
      }

      struct failed_to_find_field {};

      template <typename RT, std::size_t N, std::size_t I, eosio::name::raw Name, typename Index, typename... Indices>
      constexpr inline decltype(auto) get_field_helper(const RT& rt) {
         if constexpr (N != I)
            if constexpr (sizeof...(Indices) > 0)
               return get_field_helper<RT, N, I+1, Name, Indices...>(rt);
            else
               return failed_to_find_field{};
         else
            return std::invoke(Index::member, &rt);
      }

      template <typename RT, eosio::name::raw Name, typename... Indices>
      constexpr inline decltype(auto) get_field(const RT& rt) {
         return get_field_helper<RT, index_ordinal<Name, Indices...>(), 0, Name, Indices...>(rt);
      }
   } // namespace eosio::kv::detail

   template <eosio::name::raw Name, auto MemberPtr>
   struct index {
      constexpr inline static eosio::name::raw name = Name;
      constexpr inline static auto member           = MemberPtr;
   };

   template <eosio::name::raw TableName, typename... Indices>
   class [[eosio::table]] table {
         template <std::size_t N>
         constexpr static inline auto member_pointer_t = decltype(index_type_at<N>())::member;
      public:
         constexpr inline static name primary_index = name{detail::first_index_name<Indices...>::name};
         using record_t = typename detail::record_type<Indices...>::type;

         using primary_index_t = decltype(detail::index_type_at<0, Indices...>());

         template <std::size_t N>
         using index_t = decltype(detail::index_type_at<N, Indices...>());

         template <eosio::name::raw Name>
         constexpr static inline std::size_t index_ordinal_v = detail::index_ordinal<Name, Indices...>();

         template <eosio::name::raw Name>
         using index_from_name_t = index_t<index_ordinal_v<Name>>;

         template <eosio::name::raw Name>
         using index_mp_t = decltype(eosio::detail::member_pointer_type(index_from_name_t<Name>::member));


         inline table(name owner=current_context_contract())
            : owner(owner),
              indices({detail::name_forwarder<Indices>(owner)...}) {}

         inline void put(const record_t& v, name payer=current_context_contract()) {
            std::pair key = {detail::get_field<record_t, primary_index, Indices...>(v), payer};
            // first update the "primary"
            auto val = std::get<index_ordinal_v<primary_index>>(indices)[key];
            record_t old = (record_t)val;
            val = v;

            // then update the "secondaries"
            put_secondaries<1>(std::move(old), std::move(val), v, payer);
         }

         template <eosio::name::raw Index=primary_index>
         inline decltype(auto) get(const index_mp_t<Index>& k, name payer=current_context_contract()) {
            constexpr std::size_t ord = index_ordinal_v<Index>;
            if constexpr (name{Index} == primary_index) {
               return std::get<ord>(indices).at(k);
            } else {
               auto& sec_ind = std::get<ord>(indices);
               detail::packed_view pv;
               sec_ind.get_raw(k, pv);
               auto& pk_ind  = std::get<index_ordinal_v<primary_index>>(indices);
               return pk_ind.at({pv.data(), pv.size()}, payer);
            }
         }

         using indices_t = std::tuple<map<TableName,
                                           decltype(eosio::detail::member_pointer_type(Indices::member)),
                                           decltype(eosio::detail::member_pointer_class(Indices::member)),
                                           Indices::name> ...>;
      private:

         template <std::size_t N, typename W>
         inline void put_secondaries(record_t&& ov, W&& w, const record_t& v, name payer) {
            if constexpr (N < std::tuple_size_v<indices_t>) {
               constexpr auto sec_name = index_t<N>::name;
               std::pair key = {detail::get_field<record_t, sec_name, Indices...>(v), payer};
               auto& sec_ind = std::get<N>(indices);
               check(sec_ind.contains(key.first), "attempting to store an existing secondary index");
               sec_ind.erase(detail::get_field<record_t, sec_name, Indices...>(ov)); // erase the index from the "old value"
               auto sec_val = std::get<N>(indices).raw_write(key.first, {w.element.key.data(), w.element.key.size()}, payer);
               put_secondaries<N+1>(std::move(ov), std::forward<W>(w), v, payer);
            }
         }

         name owner;
         indices_t indices;
   };
} // namespace eosio::kv
