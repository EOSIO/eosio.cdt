#include <catch2/catch.hpp>
#include <eosio/tester.hpp>

#include <contracts.hpp>

using namespace eosio;
using eosio::testing::contracts;

TEST_CASE_METHOD( test_chain, "Tests for action results", "[action_results]" ) {
   create_account( "test"_n );
   finish_block();

#if 0
   set_code( "test"_n, contracts::action_results_test_wasm() );
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
