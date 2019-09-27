#include <eosio/name.hpp>
#include <memory>

extern "C" __attribute__((eosio_wasm_import)) void print_range(const char*, const char*);

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
   extern "C" {

   void eosio_assert(uint32_t test, const char* msg) {
      if (!test)
         eosio_assert_message(test, msg, strlen(msg));
   }

   void eosio_assert_code(uint32_t test, uint64_t code) {
      eosio_assert(0, "eosio_assert_code is not available");
      [[unreachable]];
   }

   uint64_t current_time() {
      eosio_assert(0, "current_time is not available");
      [[unreachable]];
   }

   bool is_privileged(uint64_t account) {
      eosio_assert(0, "is_privileged is not available");
      [[unreachable]];
   }

   void get_resource_limits(uint64_t account, int64_t* ram_bytes, int64_t* net_weight, int64_t* cpu_weight) {
      eosio_assert(0, "get_resource_limits is not available");
      [[unreachable]];
   }

   void set_resource_limits(uint64_t account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight) {
      eosio_assert(0, "set_resource_limits is not available");
      [[unreachable]];
   }

   void set_privileged(uint64_t account, bool is_priv) {
      eosio_assert(0, "set_privileged is not available");
      [[unreachable]];
   }

   void set_blockchain_parameters_packed(char* data, uint32_t datalen) {
      eosio_assert(0, "set_blockchain_parameters_packed is not available");
      [[unreachable]];
   }

   uint32_t get_blockchain_parameters_packed(char* data, uint32_t datalen) {
      eosio_assert(0, "get_blockchain_parameters_packed is not available");
      [[unreachable]];
   }

   int64_t set_proposed_producers(char*, uint32_t) {
      eosio_assert(0, "set_proposed_producers is not available");
      [[unreachable]];
   }

   uint32_t get_active_producers(uint64_t*, uint32_t) {
      eosio_assert(0, "get_active_producers is not available");
      [[unreachable]];
   }

   bool is_feature_activated(void* feature_digest) {
      eosio_assert(0, "is_feature_activated is not available");
      [[unreachable]];
   }

   void preactivate_feature(const void* feature_digest) {
      eosio_assert(0, "preactivate_feature is not available");
      [[unreachable]];
   }

   int64_t set_proposed_producers_ex(uint64_t producer_data_format, char* producer_data, uint32_t producer_data_size) {
      eosio_assert(0, "set_proposed_producers_ex is not available");
      [[unreachable]];
   }

   } // extern "C"
} // namespace internal_use_do_not_use
} // namespace eosio

namespace std {
bool uncaught_exception() noexcept { return false; }
} // namespace std
