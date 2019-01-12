#include <eosiolib/eosio.hpp>

using namespace eosio;

CONTRACT malloc_tests : public contract{
   public:
      using contract::contract;

      inline char* align(char* ptr, uint8_t align_amt) {
         return (char*)((((size_t)ptr) + align_amt-1) & ~(align_amt-1));
      }

      inline size_t align(size_t ptr, uint8_t align_amt) {
         return (ptr + align_amt-1) & ~(align_amt-1);
      }
     
      ACTION alloctests( bool should_fail ) {
         char* ret = (char*)malloc(0);
         char* last_ret = ret;

         check(ret == NULL, "malloc(0) should return null");

         ret = (char*)malloc(10);
         check(ret != last_ret, "ret should not equal last_ret");
         check(ret == (char*)(64*1024), "malloc(10) should return 64K");
         
         last_ret = ret;
         ret = (char*)malloc(10);
         check(ret != last_ret, "ret should not equal last_ret");
         check((size_t)ret%8 == 0, "ret should be divisible by 8");
         check(ret == align(last_ret+10, 8), "malloc(10) not as expected");

         for (int i=0; i < 20; i++) {
            (ret+10)[i] = 'a';
         }

         last_ret = ret;
         ret = (char*)calloc(10, 2);
         check(ret != last_ret, "ret should not equal last_ret");
         check((size_t)ret%2 == 0, "ret should be divisible by 2");
         check(ret == align(last_ret+10, 8), "calloc(10, 2) not as expected");
         
         for (int i=0; i < 20; i++) {
            check(ret[i] == 0, "ret[i] should equal 0");
         }

         last_ret = ret;
         ret = (char*)malloc(3);
         check(ret != last_ret, "ret should not equal last_ret");
         check((size_t)ret%8 == 0, "ret should be divisible by 8");
         check(ret == align(last_ret+20, 8), "malloc(3) not as expected"); 

         last_ret = ret;
         ret = (char*)malloc(30);
         check(ret != last_ret, "ret should not equal last_ret");
         check((size_t)ret%8 == 0, "ret should be divisible by 8");
         check(ret == align(last_ret+3, 8), "malloc(30) not as expected"); 

         last_ret = ret;
         ret = (char*)realloc(ret, 30);
         check(ret != last_ret, "ret should not equal last_ret");
         check((size_t)ret%8 == 0, "ret should be divisible by 8");
         check(ret == align(last_ret+30, 8), "realloc(30) not as expected"); 

         last_ret = ret;
         ret = (char*)realloc(ret, 64*1024);
         check(ret != last_ret, "ret should not equal last_ret");
         check((size_t)ret%8 == 0, "ret should be divisible by 8");
         check(ret == align(last_ret+30, 8), "realloc(64*1024) not as expected"); 

         last_ret = ret;
         ret = (char*)realloc(ret, 1);
         check(ret != last_ret, "ret should not equal last_ret");
         check((size_t)ret%8 == 0, "ret should be divisible by 8");
         check(ret == align(last_ret+64*1024, 8), "realloc(1) not as expected"); 

         last_ret = ret;
         ret = (char*)malloc(31*1024*1024);
         check(ret != last_ret, "ret should not equal last_ret");
         check((size_t)ret%8 == 0, "ret should be divisible by 8");
         check(ret == align(last_ret+1, 8), "malloc(1) not as expected"); 

         last_ret = ret;
         ret = (char*)malloc(1);
         check(ret != last_ret, "ret should not equal last_ret");
         check((size_t)ret%8 == 0, "ret should be divisible by 8");
         check(ret == align(last_ret+31*1024*1024, 8), "malloc(32*1024*1024) not as expected"); 

         last_ret = ret;
         ret = (char*)malloc(917352);
         check(ret != last_ret, "ret should not equal last_ret");
         check((size_t)ret%8 == 0, "ret should be divisible by 8");
         check(ret == align(last_ret+1, 8), "malloc(1) not as expected"); 
   
         last_ret = ret;
         ret = (char*)malloc(1*1024*1024);
         check(ret != last_ret, "ret should not equal last_ret");
         check((size_t)ret%8 == 0, "ret should be divisible by 8");
         check(ret == align(last_ret+917352, 8), "malloc(1*1024*1024) not as expected"); 
         
         // should assert
         if (should_fail)
            ret = (char*)malloc(1);
      }
};

EOSIO_DISPATCH(malloc_tests, (alloctests))
