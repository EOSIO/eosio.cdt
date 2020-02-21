#include <boost/test/included/unit_test.hpp>
#include <eosio/tester.hpp>

#include <contracts.hpp>

using namespace eosio;

BOOST_AUTO_TEST_SUITE(action_results_tests_suite)

BOOST_FIXTURE_TEST_CASE( action_results_tests, test_chain ) {
   create_account( "test"_n );
   finish_block();

   set_code( N(test), contracts::action_results_test_wasm() );
#if 0
   produce_blocks();
   auto trace = push_action(N(test), N(action1), N(test), mvo());
   // need to fix this test after Kevin fixes action_return
   wdump((trace));

   trace = push_action(N(test), N(action2), N(test), mvo());
   wdump((trace));

   trace = push_action(N(test), N(action3), N(test), mvo());
   wdump((trace));
#endif
}

BOOST_AUTO_TEST_SUITE_END()
