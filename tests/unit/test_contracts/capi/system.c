#include <eosio/system.h>
#include <stddef.h>

void test_system( void ) {
   eosio_assert(0, NULL);
   eosio_assert_message(0, NULL, 0);
   eosio_assert_code(0, 0);
   eosio_exit(0);
   current_time();
   is_feature_activated(NULL);
   get_sender();
}
