#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>

#include <eosio/chain/abi_serializer.hpp>
#include <eosio/testing/tester.hpp>

#include <fc/variant_object.hpp>

#include <contracts.hpp>

using namespace eosio;
using namespace eosio::testing;

using mvo = fc::mutable_variant_object;

struct kv_tester {
   kv_tester(std::vector<uint8_t> wasm, std::vector<char> abi) {
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

   void push_action(name act) {
      chain.push_action("kvtest"_n, act, "kvtest"_n, {});
   }
   tester chain;
};

BOOST_AUTO_TEST_SUITE(key_value_tests)

BOOST_AUTO_TEST_CASE(map_tests) try {
   kv_tester t = {contracts::kv_map_tests_wasm(), contracts::kv_map_tests_abi()};
   t.push_action("test"_n);
   t.push_action("iter"_n);
   t.push_action("erase"_n);

   //BOOST_CHECK_EXCEPTION(tester.push_action("kvtest"_n, "finderror"_n, "kvtest"_n, {}),
   //                      eosio_assert_message_exception,
   //                      eosio_assert_message_is("Cannot read end iterator"));
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
