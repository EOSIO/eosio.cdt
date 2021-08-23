#include <eosio/print.hpp>
#include <string.h>

namespace eosio { namespace internal_use_do_not_use {

extern "C" void prints(const char* cstr) { prints_l(cstr, strlen(cstr)); }

}} // namespace eosio::internal_use_do_not_use
