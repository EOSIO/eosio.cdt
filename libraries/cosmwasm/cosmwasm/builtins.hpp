#pragma once
#include <cstddef>
#include <cstdint>

#ifndef BLANC_NATIVE
# define WASM_EXPORT(x) __attribute__((export_name(x)))
# define WASM_IMPORT(x) __attribute__((import_name(x)))
# define POINTER_WIDTH_TYPE uint32_t
#else
# define WASM_EXPORT(x)
# define WASM_IMPORT(x)
# define POINTER_WIDTH_TYPE uint64_t
#endif

namespace cosmwasm {
   using ptr_t = POINTER_WIDTH_TYPE;
}

extern "C" {
   void __wasm_call_ctors();

   struct region {
      cosmwasm::ptr_t offset;
      uint32_t capacity;
      uint32_t length;

      region() = default;
      region(const region&) = default;
      region(region&&) = default;
      region& operator=(const region&) = default;

      region(uint32_t offset, uint32_t capacity, uint32_t length)
      : offset(offset), capacity(capacity), length(length) {}
   };

#ifndef BLANC_NATIVE
   void abort();
#endif

   WASM_EXPORT("interface_version_5")
   void interface_version_5();

   WASM_EXPORT("allocate")
   region* allocate(size_t size);

   WASM_EXPORT("deallocate")
   void deallocate(region* reg);

   WASM_IMPORT("debug")
   void debug(region*);
}
