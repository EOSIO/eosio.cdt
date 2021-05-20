#include <eosio/permission.h>
#include <stddef.h>

void test_permission( void ) {
   check_transaction_authorization(NULL, 0, NULL, 0, NULL, 0);
   check_permission_authorization(0, 0, NULL, 0, NULL, 0, 0);
   get_permission_last_used(0, 0);
   get_account_creation_time(0);
}
