#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void __ashlti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
   unsigned __int128 i = high;
   i <<= 64;
   i |= low;
   i <<= shift;
   ret = i;
}

void __ashrti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
   // retain the signedness
   ret = high;
   ret <<= 64;
   ret |= low;
   ret >>= shift;
}

void __lshlti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
   unsigned __int128 i = high;
   i <<= 64;
   i |= low;
   i <<= shift;
   ret = (unsigned __int128)i;
}

void __lshrti3(__int128& ret, uint64_t low, uint64_t high, uint32_t shift) {
   unsigned __int128 i = high;
   i <<= 64;
   i |= low;
   i >>= shift;
   ret = (unsigned __int128)i;
}

void __divti3(__int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
   __int128 lhs = ha;
   __int128 rhs = hb;

   lhs <<= 64;
   lhs |=  la;

   rhs <<= 64;
   rhs |=  lb;
   //if ( rhs == 0 )
   //   eosio_assert(false, "divide by zero");

   lhs /= rhs;

   ret = lhs;
}

void __udivti3(unsigned __int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
   unsigned __int128 lhs = ha;
   unsigned __int128 rhs = hb;

   lhs <<= 64;
   lhs |=  la;

   rhs <<= 64;
   rhs |=  lb;
   //if ( rhs == 0 )
   //   eosio_assert(false, "divide by zero");

   lhs /= rhs;
   ret = lhs;
}

void __multi3(__int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
   __int128 lhs = ha;
   __int128 rhs = hb;

   lhs <<= 64;
   lhs |=  la;

   rhs <<= 64;
   rhs |=  lb;

   lhs *= rhs;
   ret = lhs;
}

void __modti3(__int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
   __int128 lhs = ha;
   __int128 rhs = hb;

   lhs <<= 64;
   lhs |=  la;

   rhs <<= 64;
   rhs |=  lb;
   //if ( rhs == 0 )
   //   eosio_assert(false, "divide by zero");

   lhs %= rhs;
   ret = lhs;
}

void __umodti3(unsigned __int128& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb) {
   unsigned __int128 lhs = ha;
   unsigned __int128 rhs = hb;

   lhs <<= 64;
   lhs |=  la;

   rhs <<= 64;
   rhs |=  lb;
   //if ( rhs == 0 )
   //   eosio_assert(false, "divide by zero");

   lhs %= rhs;
   ret = lhs;
}

#ifdef __cplusplus
}
#endif

