#include <cstring>

extern "C" {
   void* memset( void* ptr, int c, size_t n ) {
      uint8_t* p = (uint8_t*)ptr;
      for ( size_t i=0; i < n; i++ )
         p[i] = (uint8_t)c;
      return ptr;
   }
   void* memcpy( void* ptr1, const void* ptr2, size_t n ) {
      uint8_t* p1 = (uint8_t*)ptr1;
      const uint8_t* p2 = (const uint8_t*)ptr2;
      for ( size_t i=0; i < n; i++ )
         p1[i] = p2[i];
      return ptr1;
   }
   void* memmove( void* ptr1, const void* ptr2, size_t n ) {
      uint8_t* p1 = (uint8_t*)ptr1;
      const uint8_t* p2 = (const uint8_t*)ptr2;
      uint8_t* p3 = new uint8_t[n];
      memcpy( p3, p2, n );
      memcpy( p1, p3, n );
      delete[] p3;
      return ptr1;
   }
   int memcmp( const void* ptr1, const void* ptr2, size_t n ) {
      const uint8_t* p1 = (uint8_t*)ptr1;
      const uint8_t* p2 = (uint8_t*)ptr2;
      for ( size_t i=0; i < n; i++ ) {
         if ( p1[i] < p2[i] )
            return -1;
         else if ( p1[i] > p2[i] )
            return 1;
      }
      return 0;
   }
}
