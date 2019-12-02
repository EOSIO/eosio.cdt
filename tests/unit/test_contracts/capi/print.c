#include <eosio/print.h>
#include <stddef.h>

void test_print( void ) {
   prints(NULL);
   prints_l(NULL, 0);
   printi(0);
   printui(0);
   printi128(NULL);
   printui128(NULL);
   printsf(0.0f);
   printdf(0.0);
   printqf(NULL);
   printn(0);
   printhex(NULL, 0);
}
