#include <eosio/eosio.hpp>

using namespace eosio;

// simple macro to add line info to string
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define X(STR) \
   STR " at line # " TOSTRING(__LINE__)

CONTRACT malloc_tests : public contract{
   public:
      using contract::contract;

      inline char* align(char* ptr, uint8_t align_amt) {
         return (char*)((((size_t)ptr) + align_amt-1) & ~(align_amt-1));
      }

      inline size_t align(size_t ptr, uint8_t align_amt) {
         return (ptr + align_amt-1) & ~(align_amt-1);
      }

      inline char* get_heap_base() {

         volatile uintptr_t address = 0;
         volatile char* heap_base = align(*(char**)address, 8);
         return (char*)heap_base;
      }

      static constexpr size_t max_heap = 33*1024*1024;
   
      ACTION mallocalot() {
         char* ret = (char*)malloc(10);
         char* last_ret = ret;
         for (size_t i=0; i < ((max_heap-(64*1024))/align(10,8)); i++) {
            last_ret = ret;
            ret = (char*)malloc(10);
            check(ret != last_ret, X("ret should not equal last_ret"));
            check((size_t)ret%8 == 0, X("ret should be divisible by 8"));
            check(ret == align(last_ret+10, 8), X("alloc(10) not as expected"));
         }
      }

      ACTION mallocpass() {
         malloc(max_heap-(64*1024));
         size_t diff = (64*1024)-(size_t)get_heap_base();
         malloc(diff-align(1,8));
      }

      ACTION mallocpass2() {
         char* ptr = (char*)malloc(max_heap-(64*1024));
         size_t diff = (64*1024)-(size_t)get_heap_base();
         malloc(diff-align(1,8));
         for (size_t i=0; i < (max_heap-diff); i++) {
            ptr[i] = 'a';
         }
         for (size_t i=0; i < (max_heap-diff); i++) {
            eosio::check(ptr[i] == 'a', "should have written to memory");
         }

      }
    
      ACTION mallocfail() {
         mallocpass();
         malloc(1);
      }

      ACTION malloctest() {
          char* ptr1 = (char*)malloc(0);
          check(ptr1 == nullptr, "should not have allocated a 0 char buf");

          // 20 chars - 20 + 4(header) which is divisible by 8
          ptr1 = (char*)malloc(20);
          check(ptr1 != nullptr, "should have allocated a 20 char buf");

          // 20 chars allocated
          char* ptr2 = (char*)malloc(20);
          check(ptr2 != nullptr, "should have allocated another 20 char buf");
          check(ptr1 + 20 < ptr2, "20 char buf should have been created after ptr1"); // test specific to implementation (can remove for refactor)

          // 20 chars allocated
          char* ptr3 = (char*)malloc(20);
          check(ptr3 != nullptr, "should have allocated another 20 char buf");
          check(ptr2 + 20 < ptr3, "20 char buf should have been created after ptr1"); // test specific to implementation (can remove for refactor)

          // 20 chars allocated
          char* ptr4 = (char*)malloc(20);
          check(ptr4 != nullptr, "should have allocated another 20 char buf");
          check(ptr3 + 20 < ptr4, "20 char buf should have been created after ptr1"); // test specific to implementation (can remove for refactor)
      }
};
