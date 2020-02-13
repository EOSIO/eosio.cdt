#include <eosio/name.hpp>
#include <memory>

extern "C" __attribute__((eosio_wasm_import)) void print_range(const char*, const char*);

extern "C" void prints(const char* cstr) { print_range(cstr, cstr + strlen(cstr)); }
extern "C" void prints_l(const char* cstr, uint32_t len) { print_range(cstr, cstr + len); }

extern "C" void printn(uint64_t n) {
   std::string s = eosio::name_to_string(n);
   print_range(s.data(), s.data() + s.size());
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

extern "C" {
   struct __attribute__((aligned (16))) capi_checksum160 { uint8_t hash[20]; };
   struct __attribute__((aligned (16))) capi_checksum256 { uint8_t hash[32]; };
   struct __attribute__((aligned (16))) capi_checksum512 { uint8_t hash[64]; };

   int recover_key( capi_checksum256* digest, const char* sig, uint32_t sig_len, char* pub, uint32_t pub_len) {
      eosio::check(false, "recover_key is not available");
      [[unreachable]];
   }
   void assert_recover_key( capi_checksum256* digest, const char* sig, uint32_t sig_len, const char* pub, uint32_t pub_len) {
      eosio::check(false, "assert_recover_key is not available");
      [[unreachable]];
   }

   __attribute__((eosio_wasm_import))
   void sha256( const char* data, uint32_t length, capi_checksum256* hash );

   __attribute__((eosio_wasm_import))
   void sha1( const char* data, uint32_t length, capi_checksum160* hash );

   __attribute__((eosio_wasm_import))
   void sha512( const char* data, uint32_t length, capi_checksum512* hash );

   __attribute__((eosio_wasm_import))
   void ripemd160( const char* data, uint32_t length, capi_checksum160* hash );

   void assert_sha1(const char* data, uint32_t len, const capi_checksum160* expected) {
      capi_checksum160 actual;
      sha1(data, len, &actual);
      eosio::check(memcmp(actual.hash, expected->hash, sizeof(actual.hash)) == 0, "hash mismatch");
   }
   void assert_sha256(const char* data, uint32_t len, const capi_checksum256* expected) {
      capi_checksum256 actual;
      sha256(data, len, &actual);
      eosio::check(memcmp(actual.hash, expected->hash, sizeof(actual.hash)) == 0, "hash mismatch");
   }
   void assert_sha512(const char* data, uint32_t len, const capi_checksum512* expected) {
      capi_checksum512 actual;
      sha512(data, len, &actual);
      eosio::check(memcmp(actual.hash, expected->hash, sizeof(actual.hash)) == 0, "hash mismatch");
   }
   void assert_ripemd160(const char* data, uint32_t len, const capi_checksum160* expected) {
      capi_checksum160 actual;
      ripemd160(data, len, &actual);
      eosio::check(memcmp(actual.hash, expected->hash, sizeof(actual.hash)) == 0, "hash mismatch");
   }
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
