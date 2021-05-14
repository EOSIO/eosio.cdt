#pragma once
#include <cstdint>

extern "C" {

   struct region {
      uint32_t offset;
      uint32_t capacity;
      uint32_t length;

      region() = default;
      region(const region&) = default;
      region(region&&) = default;
      region& operator=(const region&) = default;

      region(uint32_t offset, uint32_t capacity, uint32_t length)
      : offset(offset), capacity(capacity), length(length) {}
   };

   void abort();

   __attribute__((export_name("cosmwasm_vm_version_4")))
   void cosmwasm_vm_version_4();

   __attribute__((export_name("allocate")))
   region* allocate(size_t size);

   __attribute__((export_name("deallocate")))
   void deallocate(region* reg);

}
