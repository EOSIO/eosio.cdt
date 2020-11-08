#pragma once

//#define BLUEGRASS_IGNORE_HELPERS
#include <bluegrass/meta/macro.hpp>

#define EOSIO_CDT_REFLECT(BASES, ...) BLUEGRASS_META_REFL(__VA_ARGS__)
#define EOSIO_CDT_NO_REFLECT constexpr void _bluegrass_meta_refl_valid(); // force the reflection to skip over

#include <bluegrass/meta/refl.hpp>
namespace eosio {
   using bluegrass::meta::meta_object;
   using bluegrass::meta::str_view;
   using bluegrass::meta::base_array;
   using bluegrass::meta::is_reflected_v;
} // namespace eosio
