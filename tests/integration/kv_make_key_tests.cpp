#include <boost/test/data/test_case.hpp>
#include <boost/test/unit_test.hpp>
#include <eosio/testing/tester.hpp>
#include <eosio/chain/abi_serializer.hpp>
#include <fc/variant_object.hpp>

#include <contracts.hpp>

using namespace eosio;
using namespace eosio::testing;

using mvo = fc::mutable_variant_object;

BOOST_AUTO_TEST_SUITE(key_value_tests)

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

std::vector<name> data_set{
   N(makekeyname), N(makekeystr), N(makekeyistr), N(makekeyuill), N(makekeyil), N(makekeyuilll),
   N(makekeyflt), N(makekeydbl), N(makekeystct), N(makekeytup)
};

BOOST_DATA_TEST_CASE(makekeytests, data_set) { try {
   make_key_test(sample);
} FC_LOG_AND_RETHROW() }

BOOST_AUTO_TEST_SUITE_END()
