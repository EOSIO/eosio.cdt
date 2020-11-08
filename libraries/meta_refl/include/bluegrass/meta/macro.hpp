#pragma once

#include "preprocessor.hpp"

#ifndef BLUEGRASS_IGNORE_HELPERS
#include "helpers.hpp"
#endif

#define BLUEGRASS_META_ADDRESS(ignore, FIELD) (void*)&FIELD
#define BLUEGRASS_META_DECLTYPE(ignore, FIELD) decltype(FIELD)
#define BLUEGRASS_META_PASS_STR(ignore, FIELD) #FIELD
#define BLUEGRASS_META_PASS_STRVIEW(ignore, FIELD) bluegrass::meta::str_view{#FIELD}

#define BLUEGRASS_META_VA_ARGS_SIZE(...)  \
   bluegrass::meta::va_args_count_helper( \
         BLUEGRASS_META_FOREACH(          \
            BLUEGRASS_META_PASS_STR, "ignored", ##__VA_ARGS__))

#define BLUEGRASS_META_REFL(...)                                                            \
   public:                                                                                  \
      constexpr void _bluegrass_meta_refl_valid();                                             \
      void _bluegrass_meta_refl_fields                                                         \
         (BLUEGRASS_META_FOREACH(BLUEGRASS_META_DECLTYPE, "ignored", ##__VA_ARGS__)) {}        \
      inline auto _bluegrass_meta_refl_field_ptrs() const {                                    \
         return bluegrass::meta::base_array<void*, BLUEGRASS_META_VA_ARGS_SIZE(__VA_ARGS__)> { \
            BLUEGRASS_META_FOREACH(BLUEGRASS_META_ADDRESS, "ignored", ##__VA_ARGS__)};         \
      }                                                                                        \
      template <unsigned long long _BLUEGRASS_N>                                               \
      inline void* _bluegrass_meta_refl_field_ptr() const {                                    \
         return _bluegrass_meta_refl_field_ptrs()[_BLUEGRASS_N];                               \
      }                                                                                        \
      constexpr inline static auto _bluegrass_meta_refl_field_names() {                        \
         using namespace bluegrass::meta;                                                      \
         return base_array<str_view, BLUEGRASS_META_VA_ARGS_SIZE(__VA_ARGS__)> {               \
            BLUEGRASS_META_FOREACH(BLUEGRASS_META_PASS_STRVIEW, "ignored", ##__VA_ARGS__)      \
         };                                                                                    \
      }
