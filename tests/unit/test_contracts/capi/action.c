#include <eosio/action.h>
#include <stddef.h>

void test_action( void ) {
   read_action_data(NULL, 0);
   action_data_size();
   require_recipient(0);
   require_auth(0);
   has_auth(0);
   require_auth2(0, 0);
   is_account(0);
   send_inline(NULL, 0);
   send_context_free_inline(NULL, 0);
   publication_time();
   current_receiver();
   set_action_return_value(NULL, 0);
}
