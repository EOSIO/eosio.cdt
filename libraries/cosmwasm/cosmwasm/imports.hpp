#pragma once
#include "memory.hpp"
#include "types.hpp"

namespace cosmwasm {
   namespace internal_use_do_not_use {
      WASM_IMPORT("addr_validate")
      region* addr_validate(region*);
   }

   addr addr_validate(const std::string& address) {
      auto source = build_region(address.data(), address.size());
      auto res = internal_use_do_not_use::addr_validate(source.get());
      if (res) {
         abort();
      }
      return address;
   }
}

