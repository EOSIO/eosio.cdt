#include <memory>
#include "core/eosio/check.hpp"

#ifdef EOSIO_NATIVE
   extern "C" {
      size_t _current_memory();
      size_t _grow_memory(size_t);
   }
#define CURRENT_MEMORY _current_memory()
#define GROW_MEMORY(X) _grow_memory(X)
#else
#define CURRENT_MEMORY __builtin_wasm_current_memory()
#define GROW_MEMORY(X) __builtin_wasm_grow_memory(X)
#endif

namespace eosio {   
   struct dsmalloc {
      inline char* align(char* ptr, uint8_t align_amt) {
         return (char*)((((size_t)ptr) + align_amt-1) & ~(align_amt-1));
      }

      inline size_t align(size_t ptr, uint8_t align_amt) {
         return (ptr + align_amt-1) & ~(align_amt-1);
      }

      static constexpr uint32_t wasm_page_size = 64*1024;

      dsmalloc() {
         volatile uintptr_t heap_base = 0; // linker places this at address 0
         heap = align(*(char**)heap_base, 8);
         last_ptr = heap;

         next_page = CURRENT_MEMORY;
      }
       
      char* operator()(size_t sz, uint8_t align_amt=8) {
         if (sz == 0)
            return NULL;

         char* ret = last_ptr;
         last_ptr = align(last_ptr+sz, align_amt);

         size_t pages_to_alloc = sz >> 16;
         next_page += pages_to_alloc;
         if ((next_page << 16) <= (size_t)last_ptr) {
            next_page++;
            pages_to_alloc++;
         }         
         eosio::check(GROW_MEMORY(pages_to_alloc) != -1, "failed to allocate pages");  
         return ret;
      }

      char*  heap;
      char*  last_ptr;
      size_t offset;
      size_t next_page;
   }; 
   dsmalloc _dsmalloc;
} // ns eosio

extern "C" {

void* malloc(size_t size) {
   void* ret = eosio::_dsmalloc(size);
   return ret;
}

void* memset(void*,int,size_t);
void* calloc(size_t count, size_t size) {
   if (void* ptr = eosio::_dsmalloc(count*size)) {
      memset(ptr, 0, count*size);
      return ptr;
   }
   return nullptr;
}

void* realloc(void* ptr, size_t size) {
   return eosio::_dsmalloc(size);
}

void free(void* ptr) {}
}

