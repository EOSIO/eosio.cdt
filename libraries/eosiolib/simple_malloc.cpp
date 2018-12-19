#include "system.hpp"
namespace eosio {   
   struct dsmalloc {
      // align to 8 bytes
      inline char* align(char* ptr) {
         return (char*)((((size_t)ptr) + 7) & ~7);
      }
      static constexpr uint32_t wasm_page_size = 64*1024;
      static constexpr uint32_t max_size = 32*1024*1024;

      dsmalloc() {
         heap = (char*)(__builtin_wasm_current_memory() << 16);
         eosio::check(__builtin_wasm_grow_memory(1) != -1, "failed to allocate pages");  
         offset = heap;
         next_page = 2;
      }
       
      char* operator()(size_t sz) {
         size_t pages_to_alloc = sz >> 16;
         next_page += pages_to_alloc;
         if (((next_page << 16)+sz) < (size_t)(align(offset+sz))) {
            next_page++;
            eosio::check((size_t)(align(offset+sz)) < max_size, "exceeding max heap size");
            eosio::check(__builtin_wasm_grow_memory(pages_to_alloc+1) != -1, "failed to allocate pages");  
         } else { 
            eosio::check((size_t)(align(offset+sz)) < max_size, "exceeding max heap size");
         }
         offset = align(offset+sz);
         return offset;
      }

      char*  heap;
      char*  offset;
      size_t next_page;
   }; 
   dsmalloc _dsmalloc;
} // ns eosio

extern "C" {

void* malloc(size_t size) {
   return eosio::_dsmalloc(size);
}

void* memset(void*,int,size_t);
void* calloc(size_t count, size_t size) {
   void* ptr = eosio::_dsmalloc(count*size);
   memset(ptr, 0, count*size);
   return ptr;
}

void* realloc(void* ptr, size_t size) {
   return eosio::_dsmalloc(size);
}

void free(void* ptr) {}
}

