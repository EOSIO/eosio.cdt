#include <softfloat.hpp>

extern "C" {
   float _eosio_f32_add(float a, float b) {
      float32_t ret = f32_add( to_softfloat32(a), to_softfloat32(b) );
      return *reinterpret_cast<float*>(&ret);
   }   
   float _eosio_f32_sub( float a, float b ) {
      float32_t ret = f32_sub( to_softfloat32(a), to_softfloat32(b) );
      return *reinterpret_cast<float*>(&ret);
   }
   float _eosio_f32_div( float a, float b ) {
      float32_t ret = f32_div( to_softfloat32(a), to_softfloat32(b) );
      return *reinterpret_cast<float*>(&ret);
   }
   float _eosio_f32_mul( float a, float b ) {
      float32_t ret = f32_mul( to_softfloat32(a), to_softfloat32(b) );
      return *reinterpret_cast<float*>(&ret);
   }

   double _eosio_f64_add( double a, double b ) {
      float64_t ret = f64_add( to_softfloat64(a), to_softfloat64(b) );
      return from_softfloat64(ret);
   }
   double _eosio_f64_sub( double a, double b ) {
      float64_t ret = f64_sub( to_softfloat64(a), to_softfloat64(b) );
      return from_softfloat64(ret);
   }
   double _eosio_f64_div( double a, double b ) {
      float64_t ret = f64_div( to_softfloat64(a), to_softfloat64(b) );
      return from_softfloat64(ret);
   }
   double _eosio_f64_mul( double a, double b ) {
      float64_t ret = f64_mul( to_softfloat64(a), to_softfloat64(b) );
      return from_softfloat64(ret);
   }
}
