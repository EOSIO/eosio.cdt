#pragma once

#include "map_macro.h"
#include <type_traits>

namespace eosio { namespace reflection {

   template <typename T>
   struct has_for_each_field {
    private:
      struct F {
         template <typename A, typename B>
         void operator()(const A&, const B&);
      };

      template <typename C>
      static char test(decltype(eosio_for_each_field((C*)nullptr, std::declval<F>()))*);

      template <typename C>
      static long test(...);

    public:
      static constexpr bool value = sizeof(test<T>((void*)nullptr)) == sizeof(char);
   };

   template <typename T>
   inline constexpr bool has_for_each_field_v = has_for_each_field<T>::value;

#define EOSIO_REFLECT_MEMBER(STRUCT, FIELD)                                                                            \
   f(#FIELD, [](auto p) -> decltype(&std::decay_t<decltype(*p)>::FIELD) { return &std::decay_t<decltype(*p)>::FIELD; });

#define EOSIO_REFLECT_STRIP_BASEbase
#define EOSIO_REFLECT_BASE(STRUCT, BASE)                                                                               \
   static_assert(std::is_base_of_v<EOSIO_REFLECT_STRIP_BASE##BASE, STRUCT>, #BASE " is not a base class of " #STRUCT); \
   eosio_for_each_field((EOSIO_REFLECT_STRIP_BASE##BASE*)nullptr, f);

#define EOSIO_REFLECT_SIGNATURE(STRUCT, ...)                                                                           \
   [[maybe_unused]] inline const char* get_type_name(STRUCT*) { return #STRUCT; }                                      \
   template <typename F>                                                                                               \
   constexpr void eosio_for_each_field(STRUCT*, F f)

/**
 * EOSIO_REFLECT(<struct>, <member or base spec>...)
 * Each parameter should be either the keyword 'base' followed by a base class of the struct or
 * an identifier which names a non-static data member of the struct.
 */
#define EOSIO_REFLECT(...)                                                                                             \
   EOSIO_REFLECT_SIGNATURE(__VA_ARGS__) { EOSIO_MAP_REUSE_ARG0(EOSIO_REFLECT_INTERNAL, __VA_ARGS__) }

// Identity the keyword 'base' followed by at least one token
#define EOSIO_REFLECT_SELECT_I(a, b, c, d, ...) EOSIO_REFLECT_##d
#define EOSIO_REFLECT_IS_BASE() ~, ~
#define EOSIO_REFLECT_IS_BASE_TESTbase ~, EOSIO_REFLECT_IS_BASE

#define EOSIO_APPLY(m, x) m x
#define EOSIO_CAT(x, y) x##y
#define EOSIO_REFLECT_INTERNAL(STRUCT, FIELD)                                                                          \
   EOSIO_APPLY(EOSIO_REFLECT_SELECT_I, (EOSIO_CAT(EOSIO_REFLECT_IS_BASE_TEST, FIELD()), MEMBER, BASE, MEMBER))         \
   (STRUCT, FIELD)

}} // namespace eosio::reflection
