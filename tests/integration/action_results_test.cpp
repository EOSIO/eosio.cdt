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
   create_accounts( { N(test) } );
   produce_block();

   set_code( N(test), contracts::action_results_test_wasm() );
   set_abi( N(test),  contracts::action_results_test_abi().data() );

   produce_blocks();
   auto trace = push_action(N(test), N(action1), N(test), mvo());
   // need to fix this test after Kevin fixes action_return
   wdump((trace));

   trace = push_action(N(test), N(action2), N(test), mvo());
   wdump((trace));

   trace = push_action(N(test), N(action3), N(test), mvo());
   wdump((trace));

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
