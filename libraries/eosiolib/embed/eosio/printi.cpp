#include <eosio/print.hpp>
#include <string.h>

namespace eosio { namespace internal_use_do_not_use {

extern "C" void printi(int64_t value) {
   if (value < 0) {
      prints("-");
      printui(-value);
   } else
      printui(value);
}

}} // namespace eosio::internal_use_do_not_use
