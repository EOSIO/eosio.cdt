#include <eosio/name.hpp>
#include <memory>

extern "C" void prints(const char* cstr) { print_range(cstr, cstr + strlen(cstr)); }
extern "C" void prints_l(const char* cstr, uint32_t len) { print_range(cstr, cstr + len); }

extern "C" void printn(uint64_t n) {
   char buffer[13];
   auto end = eosio::name{ n }.write_as_string(buffer, buffer + sizeof(buffer));
   print_range(buffer, end);
}

extern "C" void printui(uint64_t value) {
   char  s[21];
   char* ch = s;
   do {
      *ch++ = '0' + (value % 10);
      value /= 10;
   } while (value);
   std::reverse(s, ch);
   print_range(s, ch);
}

extern "C" void printi(int64_t value) {
   if (value < 0) {
      prints("-");
      printui(-value);
   } else
      printui(value);
}

namespace eosio {
void print(std::string_view sv) { print_range(sv.data(), sv.data() + sv.size()); }

namespace internal_use_do_not_use {

   extern "C" void eosio_assert(uint32_t test, const char* msg) {
      if (!test)
         eosio_assert_message(test, msg, strlen(msg));
   }

   extern "C" bool is_feature_activated(void* feature_digest) {
      eosio_assert(0, "is_feature_activated is not available");
      return false;
   }

   extern "C" void preactivate_feature(void* feature_digest) {
      eosio_assert(0, "preactivate_feature is not available");
   }

} // namespace internal_use_do_not_use
} // namespace eosio

namespace std {
bool uncaught_exception() noexcept { return false; }
} // namespace std