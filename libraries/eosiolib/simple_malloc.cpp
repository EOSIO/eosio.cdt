#include "system.hpp"
#include "print.hpp"

extern "C" size_t _get_heap_base();
namespace eosio {   
   struct dsmalloc {
      static constexpr uint8_t align_amt = 8;
      inline char* align(char* ptr) {
         return (char*)((((size_t)ptr) + align_amt-1) & ~(align_amt-1));
      }
      static constexpr uint32_t wasm_page_size = 64*1024;
      static constexpr uint32_t max_size = 32*1024*1024;

      dsmalloc() :
         heap((char*)(__builtin_wasm_current_memory() << 16)),
         offset(0),
         last_size(0),
         next_page(__builtin_wasm_current_memory()+1) {
         eosio::check(__builtin_wasm_grow_memory(1) != -1, "failed to allocate pages");
      }
       
      char* operator()(size_t sz) {
         if (sz == 0)
            return NULL;

         offset += last_size;

         size_t pages_to_alloc = sz >> 16;
         next_page += pages_to_alloc;
         last_size = sz;
         const char* aligned = align(heap+offset+sz);
         eosio::check((size_t)aligned < max_size, "exceeding max heap size");
         if ((next_page << 16) <= (size_t)aligned) {
            next_page++;
            pages_to_alloc++;
         }         

         eosio::check(__builtin_wasm_grow_memory(pages_to_alloc) != -1, "failed to allocate pages");  
         return heap+offset;
      }

      char*  heap;
      size_t offset;
      size_t last_size;
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
   void* ptr = eosio::_dsmalloc(count*size);
   memset(ptr, 0, count*size);
   return ptr;
}

void* realloc(void* ptr, size_t size) {
   return eosio::_dsmalloc(size);
}

void free(void* ptr) {}
}

