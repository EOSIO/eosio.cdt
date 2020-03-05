#include <boost/test/unit_test.hpp>
#include <eosio/testing/tester.hpp>
#include <eosio/chain/abi_serializer.hpp>
#include <fc/variant_object.hpp>

#include <contracts.hpp>

using namespace eosio;
using namespace eosio::testing;

using mvo = fc::mutable_variant_object;

BOOST_AUTO_TEST_SUITE(key_value_make_key_tests)

void make_key_test(name test_name) {
   tester t;

   t.create_accounts( { N(kvtest) } );
   t.produce_block();
   t.set_code( N(kvtest), contracts::kv_make_key_tests_wasm() );
   t.set_abi( N(kvtest), contracts::kv_make_key_tests_abi().data() );
   t.produce_blocks();

   t.set_code(config::system_account_name, contracts::kv_bios_wasm());
   t.set_abi(config::system_account_name, contracts::kv_bios_abi().data());

   auto data = mvo()("k", 1024)("v", 1024*1024)("i", 256);
   t.push_action(config::system_account_name, N(ramkvlimits), config::system_account_name, data);
   t.push_action(N(kvtest), N(setup), N(kvtest), {});
   t.push_action(N(kvtest), test_name, N(kvtest), {});
}

BOOST_AUTO_TEST_CASE(makekeyname) try {
   make_key_test(N(makekeyname));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_CASE(makekeystr) try {
   make_key_test(N(makekeystr));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_CASE(makekeyistr) try {
   make_key_test(N(makekeyistr));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_CASE(makekeyuill) try {
   make_key_test(N(makekeyuill));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_CASE(makekeyil) try {
   make_key_test(N(makekeyil));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_CASE(makekeyuilll) try {
   make_key_test(N(makekeyuilll));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_CASE(makekeyflt) try {
   make_key_test(N(makekeyflt));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_CASE(makekeydbl) try {
   make_key_test(N(makekeydbl));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_CASE(makekeystct) try {
   make_key_test(N(makekeystct));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_CASE(makekeytup) try {
   make_key_test(N(makekeytup));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
