#include <eosio/transaction.h>
#include <stddef.h>

void test_transaction( void ) {
   send_deferred(NULL, 0, NULL, 0, 0);
   cancel_deferred(NULL);
   read_transaction(NULL, 0);
   transaction_size();
   tapos_block_num();
   tapos_block_prefix();
   expiration();
   get_action(0, 0, NULL, 0);
   get_context_free_data(0, NULL, 0);
}
