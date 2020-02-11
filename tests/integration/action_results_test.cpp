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

   const auto& pfm = control->get_protocol_feature_manager();

   set_code( N(test), contracts::action_results_test_wasm() );
   set_abi( N(test),  contracts::action_results_test_abi().data() );
   auto d = pfm.get_builtin_digest(builtin_protocol_feature_t::action_return_value)
   schedule_protocol_features( {*d} );

   produce_blocks();
   const auto& trace = push_action(N(test), N(action1), N(test), mvo());

   /*
   BOOST_CHECK_THROW(push_action(N(test), N(test1), N(test), mvo()("nm", "notbucky")),
         fc::exception);

   push_action(N(test), N(test2), N(test),
         mvo()
         ("arg0", 33)
         ("arg1", "some string"));
   BOOST_CHECK_THROW(push_action(N(test), N(test2), N(test), mvo() ("arg0", 30)("arg1", "some string")), fc::exception);
   BOOST_CHECK_THROW(push_action(N(test), N(test2), N(test), mvo() ("arg0", 33)("arg1", "not some string")), fc::exception);

   set_abi( N(test),  contracts::simple_wrong_abi().data() );
   produce_blocks();

   BOOST_CHECK_THROW(push_action(N(test), N(test3), N(test), mvo() ("arg0", 33) ("arg1", "some string")), fc::exception);

   set_abi( N(test),  contracts::simple_abi().data() );
   produce_blocks();

   push_action(N(test), N(test4), N(test), mvo() ("to", "someone"));
   push_action(N(test), N(test5), N(test), mvo() ("to", "someone"));
   push_action(N(test), N(testa), N(test), mvo() ("to", "someone"));
   BOOST_CHECK_THROW(push_action(N(test), N(testb), N(test), mvo() ("to", "someone")), fc::exception);

   // test that the pre_dispatch will short circuit dispatching if false
   push_action(N(test), N(testc), N(test), mvo() ("nm", "bucky"));
   BOOST_CHECK_THROW(push_action(N(test), N(testc), N(test), mvo() ("nm", "someone")), fc::exception);
   push_action(N(test), N(testc), N(test), mvo() ("nm", "quit"));
   */

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
