#pragma once
#include "base64/base64.h"
#include "primitives.hpp"

namespace cosmwasm { namespace base64 {

   std::string encode(const cosmwasm::bytes& b) {
      std::string_view sv(reinterpret_cast<const char*>(b.data()), b.size());
      return ::base64_encode(sv);
   }

   cosmwasm::bytes decode(const std::string& s) {
      std::string dec = ::base64_decode(s);
      cosmwasm::bytes out(dec.size());
      std::copy_n(dec.data(), dec.size(), out.data());
      return out;
   }

} }
