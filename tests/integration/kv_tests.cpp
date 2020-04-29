#include <catch2/catch.hpp>
#include <eosio/tester.hpp>

#include <contracts.hpp>

using namespace eosio;
using eosio::testing::contracts;

// FIXME: Put this in a header
// Manages resources used by the kv-store
class [[eosio::contract]] kv_bios : eosio::contract {
 public:
   using contract::contract;
   [[eosio::action]] void setdisklimit(name account, int64_t limit);
   [[eosio::action]] void setramlimit(name account, int64_t limit);
   [[eosio::action]] void ramkvlimits(uint32_t k, uint32_t v, uint32_t i);
   [[eosio::action]] void diskkvlimits(uint32_t k, uint32_t v, uint32_t i);
   void kvlimits_impl(name db, uint32_t k, uint32_t v, uint32_t i);
   using ramkvlimits_action = action_wrapper<"ramkvlimits"_n, &kv_bios::ramkvlimits, "eosio"_n>;
};

eosio::checksum256 make_checksum256(std::string_view src) {
   std::array<uint8_t, 32> buf;
   eosio::unhex(buf.begin(), src.begin(), src.end());
   return eosio::checksum256(buf);
}

void setup(test_chain& tester, const char* wasm_file) {
   tester.set_code( "eosio"_n, contracts::boot_wasm() );
   tester.transact({action({"eosio"_n, "active"_n}, "eosio"_n, "activate"_n,
                           make_checksum256("825ee6288fb1373eab1b5187ec2f04f6eacb39cb3a97f356a07c91622dd61d16"))});
   tester.finish_block();

   tester.create_code_account( "kvtest"_n );
   tester.finish_block();
   tester.set_code( "kvtest"_n, wasm_file );
   tester.finish_block();

   tester.set_code("eosio"_n, contracts::kv_bios_wasm());

   tester.as("kvtest"_n).act<kv_bios::ramkvlimits_action>(1024, 1024*1024, 256);
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "setup"_n, std::tuple())});

   tester.finish_block();
}

TEST_CASE("single_tests_find", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_single_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "find"_n, std::tuple())});
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "finderror"_n, std::tuple())}, "Cannot read end iterator");
}

TEST_CASE("single_tests_get", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_single_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "get"_n, std::tuple())});
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "geterror"_n, std::tuple())}, "Key not found in `[]`");
}

TEST_CASE("single_tests_bounds", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_single_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "bounds"_n, std::tuple())});
}

TEST_CASE("single_tests_iteration", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_single_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "iteration"_n, std::tuple())});
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "itrerror1"_n, std::tuple())}, "cannot increment end iterator");
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "itrerror2"_n, std::tuple())}, "decremented past the beginning");
}

TEST_CASE("single_tests_reverse_iteration", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_single_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "riteration"_n, std::tuple())});
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "ritrerror1"_n, std::tuple())}, "incremented past the end");
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "ritrerror2"_n, std::tuple())}, "decremented past the beginning");
}

TEST_CASE("single_tests_range", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_single_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "range"_n, std::tuple())});
}

TEST_CASE("single_tests_erase", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_single_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "erase"_n, std::tuple())});
}

// Multi
// -----
TEST_CASE("multi_tests_get", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_multi_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "get"_n, std::tuple())});
}

TEST_CASE("multi_tests_iteration", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_multi_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "iteration"_n, std::tuple())});
}

TEST_CASE("multi_tests_non_unique", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_multi_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "nonunique"_n, std::tuple())});
}

TEST_CASE("multi_tests_non_update", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_multi_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "update"_n, std::tuple())});
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "updateerr1"_n, std::tuple())}, "Attempted to update an existing secondary index.");
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "updateerr2"_n, std::tuple())}, "Attempted to store an existing secondary index.");
}

// Variant
// -------
TEST_CASE("variant_tests", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_variant_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "vriant"_n, std::tuple())});
}
TEST_CASE("variant_upgrade_tests", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_variant_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "vriantupgrd"_n, std::tuple())});
}

// Make Key
// --------
std::vector<name> data_set{
   "makekeyname"_n, "makekeystr"_n, "makekeyuill"_n, "makekeyil"_n, "makekeyuilll"_n,
   "makekeyflt"_n, "makekeydbl"_n, "makekeystct"_n, "makekeytup"_n
};

TEST_CASE("makekeytests", "[kv_tests]") {
   for(auto sample: data_set) {
      test_chain tester;
      setup(tester, contracts::kv_make_key_tests_wasm());
      tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, sample, std::tuple())});
   }
}

TEST_CASE("singleton_tests", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_singleton_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "test"_n, std::tuple())});
}

TEST_CASE("singleton_tests_error", "[kv_tests]") {
   test_chain tester;
   setup(tester, contracts::kv_singleton_tests_wasm());
   tester.transact({action({"kvtest"_n, "active"_n}, "kvtest"_n, "erase"_n, std::tuple())}, "tried to get the singleton 'kvtest'/'count' that does not exist");
}
