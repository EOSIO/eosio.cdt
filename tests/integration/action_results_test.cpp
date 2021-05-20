#include <boost/test/unit_test.hpp>
#include <eosio/testing/tester.hpp>
#include <eosio/chain/abi_serializer.hpp>

#include <Runtime/Runtime.h>

#include <fc/variant_object.hpp>

#include <contracts.hpp>

using namespace eosio;
using namespace eosio::testing;
using namespace eosio::chain;
using namespace eosio::testing;
using namespace fc;

using mvo = fc::mutable_variant_object;

BOOST_AUTO_TEST_SUITE(action_results_tests_suite)

BOOST_FIXTURE_TEST_CASE( action_results_tests, tester ) try {
   create_accounts( { "test"_n } );
   produce_block();

   set_code( "test"_n, contracts::action_results_test_wasm() );
   set_abi( "test"_n,  contracts::action_results_test_abi().data() );

   produce_blocks();
   auto trace = push_action("test"_n, "action1"_n, "test"_n, mvo());
   // need to fix this test after Kevin fixes action_return
   wdump((trace));

   trace = push_action("test"_n, "action2"_n, "test"_n, mvo());
   wdump((trace));

   trace = push_action("test"_n, "action3"_n, "test"_n, mvo());
   wdump((trace));

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
