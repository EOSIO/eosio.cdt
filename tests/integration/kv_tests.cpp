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

struct kv_tester {
   kv_tester(std::vector<uint8_t> wasm, std::vector<char> abi) {
      /*
      chain.close();
      auto cfg = chain.get_config();
      cfg.backing_store = eosio::chain::backing_store_type::ROCKSDB;
      chain.init(cfg);
      */

      chain.create_accounts({"kvtest"_n});
      chain.produce_block();
      chain.set_code("kvtest"_n, wasm);
      chain.set_abi("kvtest"_n, abi.data());
      chain.produce_blocks();

      chain.set_code(config::system_account_name, contracts::kv_bios_wasm());
      chain.set_abi(config::system_account_name, contracts::kv_bios_abi().data());

      chain.push_action(config::system_account_name, "ramkvlimits"_n, config::system_account_name,
            mvo()("k", 1024)("v", 1024*1024)("i", 256));
      chain.produce_blocks();
   }

   void push_action(name act, std::string exception_msg="") {
      if (exception_msg.empty()) {
         chain.push_action("kvtest"_n, act, "kvtest"_n, {});
      } else {
         BOOST_CHECK_EXCEPTION(chain.push_action("kvtest"_n, act, "kvtest"_n, {}),
                               eosio_assert_message_exception,
                               eosio_assert_message_is(exception_msg));
      }
   }

   tester chain;
};

// TODO rework after release for these to use new table type
void setup(TESTER& tester, std::vector<uint8_t> wasm, std::vector<char> abi) {
   tester.create_accounts( { "kvtest"_n } );
   tester.produce_block();
   tester.set_code( "kvtest"_n, wasm );
   tester.set_abi( "kvtest"_n, abi.data() );
   tester.produce_blocks();

   tester.set_code(config::system_account_name, contracts::kv_bios_wasm());
   tester.set_abi(config::system_account_name, contracts::kv_bios_abi().data());

   auto data = mvo()("k", 1024)("v", 1024*1024)("i", 256);
   tester.push_action(config::system_account_name, "ramkvlimits"_n, config::system_account_name, data);
   tester.push_action("kvtest"_n, "setup"_n, "kvtest"_n, {});

   tester.produce_blocks();
}

BOOST_AUTO_TEST_SUITE(key_value_tests)

BOOST_AUTO_TEST_CASE(map_tests) try {
   kv_tester t = {contracts::kv_map_tests_wasm(), contracts::kv_map_tests_abi()};
   t.push_action("test"_n);
   t.push_action("iter"_n);
   t.push_action("erase"_n);
   t.push_action("eraseexcp"_n, "key not found");
   t.push_action("bounds"_n);
   t.push_action("ranges"_n);
   t.push_action("empty"_n);
   t.push_action("gettmpbuf"_n);
   t.push_action("constrct"_n);
   t.push_action("keys"_n);
} FC_LOG_AND_RETHROW()

// TODO replace these tests with new table tests after this release
BOOST_AUTO_TEST_CASE(single_tests_find) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action("kvtest"_n, "find"_n, "kvtest"_n, {});

   BOOST_CHECK_EXCEPTION(tester.push_action("kvtest"_n, "finderror"_n, "kvtest"_n, {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("Cannot read end iterator"));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_CASE(single_tests_get) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action("kvtest"_n, "get"_n, "kvtest"_n, {});

   BOOST_CHECK_EXCEPTION(tester.push_action("kvtest"_n, "geterror"_n, "kvtest"_n, {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("Key not found in `[]`"));
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_bounds, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action("kvtest"_n, "bounds"_n, "kvtest"_n, {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_iteration, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action("kvtest"_n, "iteration"_n, "kvtest"_n, {});

   BOOST_CHECK_EXCEPTION(tester.push_action("kvtest"_n, "itrerror1"_n, "kvtest"_n, {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("cannot increment end iterator"));

   BOOST_CHECK_EXCEPTION(tester.push_action("kvtest"_n, "itrerror2"_n, "kvtest"_n, {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("decremented past the beginning"));
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_reverse_iteration, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action("kvtest"_n, "riteration"_n, "kvtest"_n, {});

   BOOST_CHECK_EXCEPTION(tester.push_action("kvtest"_n, "ritrerror1"_n, "kvtest"_n, {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("incremented past the end"));

   BOOST_CHECK_EXCEPTION(tester.push_action("kvtest"_n, "ritrerror2"_n, "kvtest"_n, {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("decremented past the beginning"));
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_range, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action("kvtest"_n, "range"_n, "kvtest"_n, {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_erase, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_single_tests_wasm(), contracts::kv_single_tests_abi());
   tester.push_action("kvtest"_n, "erase"_n, "kvtest"_n, {});
} FC_LOG_AND_RETHROW()

// Multi
// -----
BOOST_FIXTURE_TEST_CASE(multi_tests_get, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_multi_tests_wasm(), contracts::kv_multi_tests_abi());
   tester.push_action("kvtest"_n, "get"_n, "kvtest"_n, {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(multi_tests_iteration, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_multi_tests_wasm(), contracts::kv_multi_tests_abi());
   tester.push_action("kvtest"_n, "iteration"_n, "kvtest"_n, {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(multi_tests_non_unique, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_multi_tests_wasm(), contracts::kv_multi_tests_abi());
   tester.push_action("kvtest"_n, "nonunique"_n, "kvtest"_n, {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(multi_tests_update, tester) try {
   TESTER tester;
   setup(tester, contracts::kv_multi_tests_wasm(), contracts::kv_multi_tests_abi());
   tester.push_action("kvtest"_n, "update"_n, "kvtest"_n, {});

   BOOST_CHECK_EXCEPTION(tester.push_action("kvtest"_n, "updateerr1"_n, "kvtest"_n, {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("Attempted to update an existing secondary index."));
   BOOST_CHECK_EXCEPTION(tester.push_action("kvtest"_n, "updateerr2"_n, "kvtest"_n, {}),
                         eosio_assert_message_exception,
                         eosio_assert_message_is("Attempted to store an existing secondary index."));
} FC_LOG_AND_RETHROW()

// Make Key
// --------
std::vector<name> data_set{
   "makekeyname"_n, "makekeystr"_n, "makekeyuill"_n, "makekeyil"_n, "makekeyuilll"_n,
   "makekeyflt"_n, "makekeydbl"_n, "makekeystct"_n, "makekeytup"_n
};

BOOST_DATA_TEST_CASE(makekeytests, data_set) { try {
   TESTER tester;
   setup(tester, contracts::kv_make_key_tests_wasm(), contracts::kv_make_key_tests_abi());
   tester.push_action("kvtest"_n, sample, "kvtest"_n, {});
} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_SUITE_END()
