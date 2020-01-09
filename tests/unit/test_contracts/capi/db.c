#include <eosio/db.h>
#include <stddef.h>

void test_db( void ) {
   db_store_i64(0, 0, 0, 0, NULL, 0);
   db_update_i64(0, 0, NULL, 0);
   db_remove_i64(0);
   db_get_i64(0, NULL, 0);
   db_next_i64(0, NULL);
   db_previous_i64(0, NULL);
   db_find_i64(0, 0, 0, 0);
   db_lowerbound_i64(0, 0, 0, 0);
   db_upperbound_i64(0, 0, 0, 0);
   db_end_i64(0, 0, 0);

   db_idx64_store(0, 0, 0, 0, NULL);
   db_idx64_update(0, 0, NULL);
   db_idx64_remove(0);
   db_idx64_next(0, NULL);
   db_idx64_previous(0, NULL);
   db_idx64_find_primary(0, 0, 0, NULL, 0);
   db_idx64_find_secondary(0, 0, 0, NULL, NULL);
   db_idx64_lowerbound(0, 0, 0, NULL, NULL);
   db_idx64_upperbound(0, 0, 0, NULL, NULL);
   db_idx64_end(0, 0, 0);

   db_idx128_store(0, 0, 0, 0, NULL);
   db_idx128_update(0, 0, NULL);
   db_idx128_remove(0);
   db_idx128_next(0, NULL);
   db_idx128_previous(0, NULL);
   db_idx128_find_primary(0, 0, 0, NULL, 0);
   db_idx128_find_secondary(0, 0, 0, NULL, NULL);
   db_idx128_lowerbound(0, 0, 0, NULL, NULL);
   db_idx128_upperbound(0, 0, 0, NULL, NULL);
   db_idx128_end(0, 0, 0);

   db_idx256_store(0, 0, 0, 0, NULL, 0);
   db_idx256_update(0, 0, NULL, 0);
   db_idx256_remove(0);
   db_idx256_next(0, NULL);
   db_idx256_previous(0, NULL);
   db_idx256_find_primary(0, 0, 0, NULL, 0, 0);
   db_idx256_find_secondary(0, 0, 0, NULL,0,  NULL);
   db_idx256_lowerbound(0, 0, 0, NULL, 0, NULL);
   db_idx256_upperbound(0, 0, 0, NULL, 0, NULL);
   db_idx256_end(0, 0, 0);

   db_idx_double_store(0, 0, 0, 0, NULL);
   db_idx_double_update(0, 0, NULL);
   db_idx_double_remove(0);
   db_idx_double_next(0, NULL);
   db_idx_double_previous(0, NULL);
   db_idx_double_find_primary(0, 0, 0, NULL, 0);
   db_idx_double_find_secondary(0, 0, 0, NULL, NULL);
   db_idx_double_lowerbound(0, 0, 0, NULL, NULL);
   db_idx_double_upperbound(0, 0, 0, NULL, NULL);
   db_idx_double_end(0, 0, 0);

   db_idx_long_double_store(0, 0, 0, 0, NULL);
   db_idx_long_double_update(0, 0, NULL);
   db_idx_long_double_remove(0);
   db_idx_long_double_next(0, NULL);
   db_idx_long_double_previous(0, NULL);
   db_idx_long_double_find_primary(0, 0, 0, NULL, 0);
   db_idx_long_double_find_secondary(0, 0, 0, NULL, NULL);
   db_idx_long_double_lowerbound(0, 0, 0, NULL, NULL);
   db_idx_long_double_upperbound(0, 0, 0, NULL, NULL);
   db_idx_long_double_end(0, 0, 0);
}
