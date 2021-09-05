// This is adapted from https://github.com/eoscommunity/Eden/blob/ebdd1a0c/libraries/eosiolib/simple_malloc.cpp
#include <errno.h>
#include <memory>

#ifdef BLANC_NATIVE
extern "C" {
   size_t _current_memory();
   size_t _grow_memory(size_t);
}
#define CURRENT_MEMORY _current_memory()
#define GROW_MEMORY(X) _grow_memory(X)
#else
#define CURRENT_MEMORY __builtin_wasm_memory_size(0)
#define GROW_MEMORY(X) __builtin_wasm_memory_grow(0, X)
#endif

extern "C" char __heap_base;

namespace eosio {
   struct dsmalloc {
      inline size_t align(size_t ptr, size_t align_amt) {
         return (ptr + align_amt - 1) & ~(align_amt - 1);
      }

      static constexpr uint32_t wasm_page_size = 64 * 1024;

      dsmalloc() {
         next_addr = (size_t)&__heap_base;
         next_page = CURRENT_MEMORY;
      }

      void* operator()(size_t sz, size_t align_amt = 16) {
         [[clang::import_name("eosio_assert"), noreturn]] void eosio_assert(uint32_t, const char*);
         if (sz == 0)
            return NULL;

         size_t ret = align(next_addr, align_amt);
         next_addr = ret + sz;

         if (next_addr > next_page) {
            size_t new_next_page = align(next_addr, wasm_page_size);
            if (GROW_MEMORY((new_next_page - next_page) >> 16) == -1)
               eosio_assert(false, "failed to allocate pages");
            next_page = new_next_page;
         }

         return (void*)ret;
      }

      size_t next_addr;
      size_t next_page;
   };

   // TODO: if other code uses this priority (gcc documents 101 as the highest priority), static initialization may malfunction
   dsmalloc _dsmalloc __attribute__((init_priority(101)));
}  // namespace eosio

extern "C" {
   void* malloc(size_t size) {
      void* ret = eosio::_dsmalloc(size);
      return ret;
   }

   int posix_memalign(void** memptr, size_t alignment, size_t size) {
      if (alignment < sizeof(void*) || (alignment & (alignment - size_t(1))) != 0)
         return EINVAL;
      *memptr = eosio::_dsmalloc(size, alignment > 16 ? alignment : 16);
      return 0;
   }

   void* memset(void*, int, size_t);
   void* calloc(size_t count, size_t size) {
      if (void* ptr = eosio::_dsmalloc(count * size)) {
         memset(ptr, 0, count * size);
         return ptr;
      }
      return nullptr;
   }

   void* realloc(void* ptr, size_t size) {
      if (void* result = eosio::_dsmalloc(size)) {
         // May read out of bounds, but that's okay, as the
         // contents of the memory are undefined anyway.
         memmove(result, ptr, size);
         return result;
      }
      return nullptr;
   }

   void free(void* ptr) {}
}
