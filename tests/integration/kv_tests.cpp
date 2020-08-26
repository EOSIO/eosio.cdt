#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>

#include <eosio/chain/abi_serializer.hpp>
#include <eosio/testing/tester.hpp>

#include <fc/variant_object.hpp>

#include <contracts.hpp>

using namespace eosio;
using namespace eosio::testing;

using mvo = fc::mutable_variant_object;

#ifdef NON_VALIDATING_TEST
#define TESTER tester
#else
#define TESTER validating_tester
#endif

void setup(TESTER& tester, std::vector<uint8_t> wasm, std::vector<char> abi) {
   tester.create_accounts( { N(kvtest) } );
   tester.produce_block();
   tester.set_code( N(kvtest), wasm );
   tester.set_abi( N(kvtest), abi.data() );
   tester.produce_blocks();

   tester.set_code(config::system_account_name, contracts::kv_bios_wasm());
   tester.set_abi(config::system_account_name, contracts::kv_bios_abi().data());

   auto data = mvo()("k", 1024)("v", 1024*1024)("i", 256);
   tester.push_action(config::system_account_name, N(ramkvlimits), config::system_account_name, data);
   tester.push_action(N(kvtest), N(setup), N(kvtest), {});

   tester.produce_blocks();
}

BOOST_AUTO_TEST_SUITE(key_value_tests)

BOOST_AUTO_TEST_CASE(single_tests_find) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action(N(kvtest), N(find), N(kvtest), {});

   BOOST_CHECK_EXCEPTION(tester.push_action(N(kvtest), N(finderror), N(kvtest), {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("Cannot read end iterator"));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_CASE(single_tests_get) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action(N(kvtest), N(get), N(kvtest), {});

   BOOST_CHECK_EXCEPTION(tester.push_action(N(kvtest), N(geterror), N(kvtest), {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("Key not found in `[]`"));
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_bounds, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action(N(kvtest), N(bounds), N(kvtest), {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_iteration, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action(N(kvtest), N(iteration), N(kvtest), {});

   BOOST_CHECK_EXCEPTION(tester.push_action(N(kvtest), N(itrerror1), N(kvtest), {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("cannot increment end iterator"));

   BOOST_CHECK_EXCEPTION(tester.push_action(N(kvtest), N(itrerror2), N(kvtest), {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("decremented past the beginning"));
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_reverse_iteration, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action(N(kvtest), N(riteration), N(kvtest), {});

   BOOST_CHECK_EXCEPTION(tester.push_action(N(kvtest), N(ritrerror1), N(kvtest), {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("incremented past the end"));

   BOOST_CHECK_EXCEPTION(tester.push_action(N(kvtest), N(ritrerror2), N(kvtest), {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("decremented past the beginning"));
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_range, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action(N(kvtest), N(range), N(kvtest), {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_erase, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action(N(kvtest), N(erase), N(kvtest), {});
} FC_LOG_AND_RETHROW()

// Multi
// -----
BOOST_FIXTURE_TEST_CASE(multi_tests_get, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_multi_tests_wasm(), contracts::kv_multi_tests_abi());
   tester.push_action(N(kvtest), N(get), N(kvtest), {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(multi_tests_iteration, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_multi_tests_wasm(), contracts::kv_multi_tests_abi());
   tester.push_action(N(kvtest), N(iteration), N(kvtest), {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(multi_tests_non_unique, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_multi_tests_wasm(), contracts::kv_multi_tests_abi());
   tester.push_action(N(kvtest), N(nonunique), N(kvtest), {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(multi_tests_update, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_multi_tests_wasm(), contracts::kv_multi_tests_abi());
   tester.push_action(N(kvtest), N(update), N(kvtest), {});

   BOOST_CHECK_EXCEPTION(tester.push_action(N(kvtest), N(updateerr1), N(kvtest), {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("Attempted to update an existing secondary index."));
   BOOST_CHECK_EXCEPTION(tester.push_action(N(kvtest), N(updateerr2), N(kvtest), {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("Attempted to store an existing secondary index."));
} FC_LOG_AND_RETHROW()

// Variant
// -------
BOOST_FIXTURE_TEST_CASE(variant_tests, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_variant_tests_wasm(), contracts::kv_variant_tests_abi());
   tester.push_action(N(kvtest), N(vriant), N(kvtest), {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(variant_upgrade_tests, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_variant_tests_wasm(), contracts::kv_variant_tests_abi());
   tester.push_action(N(kvtest), N(vriantupgrd), N(kvtest), {});
} FC_LOG_AND_RETHROW()

// Make Key
// --------
std::vector<name> data_set{
   N(makekeyname), N(makekeystr), N(makekeyuill), N(makekeyil), N(makekeyuilll),
   N(makekeyflt), N(makekeydbl), N(makekeystct), N(makekeytup)
};

BOOST_DATA_TEST_CASE(makekeytests, data_set) { try {
   TESTER tester;
   setup(tester, contracts::kv_make_key_tests_wasm(), contracts::kv_make_key_tests_abi());
   tester.push_action(N(kvtest), sample, N(kvtest), {});
} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_SUITE_END()
