#pragma once

#include <bluegrass/meta/refl.hpp>

#define CDT_REFLECT(...)             \
   BLUEGRASS_META_REFL(__VA_ARGS__); \
