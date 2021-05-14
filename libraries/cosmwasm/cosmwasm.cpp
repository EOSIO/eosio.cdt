#include "cosmwasm/builtin.hpp"
#include <memory>

void abort() {
   __builtin_trap();
}

void cosmwasm_vm_version_4() {
}

region* allocate(size_t size) {
   void* data = malloc(size);
   region* reg = (region*)malloc(sizeof(region));
   reg->offset = (uint32_t)data;
   reg->capacity = (uint32_t)size;
   reg->length = 0;
   return reg;
}

void deallocate(region* reg) {
   free(reinterpret_cast<void*>(reg->offset));
}
