#include "cosmwasm/builtins.hpp"
#include <memory>

void abort() {
   __builtin_trap();
}

void interface_version_5() {
}

void cosmwasm_init() {
   static bool initialized = false;
   if (!initialized) {
      __wasm_call_ctors();
      initialized = true;
   }
}

region* allocate(size_t size) {
   cosmwasm_init();
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

// Override eosio import by strong symbol
void prints_l(const char* ptr, size_t len) {
   region* reg = (region*)malloc(sizeof(region));
   reg->offset = (uint32_t)ptr;
   reg->capacity = (uint32_t)len;
   reg->length = (uint32_t)len;
   debug(reg);
}

// Override eosio import by strong symbol
uint64_t current_time() {
   return 0;
}
