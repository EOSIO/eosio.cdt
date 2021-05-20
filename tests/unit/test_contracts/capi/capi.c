#include <stdint.h>

void test_action( void );
void test_chain( void );
void test_crypto( void );
void test_db( void );
void test_permission( void );
void test_print( void );
void test_privileged( void );
void test_system( void );
void test_transaction( void );

void apply( uint64_t a, uint64_t b, uint64_t c ) {
   if(a != 0xed234054a367196eull) return; // random value that should not be used
   test_action();
   test_chain();
   test_crypto();
   test_db();
   test_permission();
   test_print();
   test_privileged();
   test_system();
   test_transaction();
}
