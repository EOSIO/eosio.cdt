#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] malloc_tests : public contract{
   public:
      using contract::contract;

      static constexpr size_t max_heap = 33*1024*1024;

      [[eosio::action]]
      void mallocpass() {
         // make sure that malloc allocates non-overlapping writable memory
         volatile char * ptr0 = (char*)malloc(1);
         *ptr0 = 0x11;
         volatile short * ptr1 = (short*)malloc(sizeof(short));
         *ptr1 = 0x2222;
         volatile int * ptr2 = (int*)malloc(sizeof(int));
         *ptr2 = 0x33333333;
         volatile long long *ptr3 = (long long*)malloc(sizeof(long long));
         *ptr3 = 0x4444444444444444;
         volatile __int128_t *ptr4 = (__int128_t*)malloc(sizeof(__int128_t));
         *ptr4 = ((__int128_t(0x5555555555555555) << 64) | 0x5555555555555555);
         volatile __int128_t *ptr5 = (__int128_t*)malloc(sizeof(__int128_t));
         *ptr5 = ((__int128_t(0x6666666666666666) << 64) | 0x6666666666666666);
         volatile __int128_t *ptr6 = (__int128_t*)malloc(sizeof(__int128_t) * 2);
         ptr6[0] = ptr6[1] = ((__int128_t(0x7777777777777777) << 64) | 0x7777777777777777);
         volatile __int128_t *ptr7 = (__int128_t*)malloc(sizeof(__int128_t) * 2);
         ptr7[0] = ptr7[1] = ((__int128_t(0x8888888888888888) << 64) | 0x8888888888888888);
         volatile long long *ptr8 = (long long*)malloc(sizeof(long long) * 3);
         ptr8[0] = ptr8[1] = ptr8[2] = 0x9999999999999999;
         volatile long long *ptr9 = (long long*)malloc(sizeof(long long) * 3);
         ptr9[0] = ptr9[1] = ptr9[2] = 0xAAAAAAAAAAAAAAAA;
         eosio::check(*ptr0 == 0x11, "wrong value for char");
         eosio::check(*ptr1 == 0x2222, "wrong value for short");
         eosio::check(*ptr2 == 0x33333333, "wrong value for int");
         eosio::check(*ptr3 == 0x4444444444444444, "wrong value for long long");
         eosio::check(*ptr4 == ((__int128_t(0x5555555555555555) << 64) | 0x5555555555555555), "wrong value for __int128 #1");
         eosio::check(*ptr5 == ((__int128_t(0x6666666666666666) << 64) | 0x6666666666666666), "wrong value for __int128 #2");
         eosio::check(ptr6[0] == ((__int128_t(0x7777777777777777) << 64) | 0x7777777777777777), "wrong value for __int128[2] #1");
         eosio::check(ptr6[1] == ((__int128_t(0x7777777777777777) << 64) | 0x7777777777777777), "wrong value for __int128[2] #1");
         eosio::check(ptr7[0] == ((__int128_t(0x8888888888888888) << 64) | 0x8888888888888888), "wrong value for __int128[2] #2");
         eosio::check(ptr7[1] == ((__int128_t(0x8888888888888888) << 64) | 0x8888888888888888), "wrong value for __int128[2] #2");
         eosio::check(ptr8[0] == 0x9999999999999999, "wrong value for long long[3] #1");
         eosio::check(ptr8[1] == 0x9999999999999999, "wrong value for long long[3] #1");
         eosio::check(ptr8[2] == 0x9999999999999999, "wrong value for long long[3] #1");
         eosio::check(ptr9[0] == 0xAAAAAAAAAAAAAAAA, "wrong value for long long[3] #2");
         eosio::check(ptr9[1] == 0xAAAAAAAAAAAAAAAA, "wrong value for long long[3] #2");
         eosio::check(ptr9[2] == 0xAAAAAAAAAAAAAAAA, "wrong value for long long[3] #2");
      }

      template<typename T>
      void malloc_align_test() {
         eosio::check((uintptr_t)malloc(sizeof(T)) % alignof(T) == 0, "insufficient alignment");
         malloc(1);
         eosio::check((uintptr_t)malloc(sizeof(T)) % alignof(T) == 0, "insufficient alignment");
      }
      [[eosio::action]]
      void mallocalign() {
         malloc_align_test<short>();
         malloc_align_test<int>();
         malloc_align_test<long>();
         malloc_align_test<long long>();
         malloc_align_test<void*>();
         malloc_align_test<float>();
         malloc_align_test<double>();
         malloc_align_test<long double>();
         malloc_align_test<__int128_t>();
      }

      [[eosio::action]]
      void mallocfail() {
         malloc(max_heap);
      }
};
