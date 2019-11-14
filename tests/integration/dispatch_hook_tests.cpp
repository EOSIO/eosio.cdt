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

BOOST_AUTO_TEST_SUITE(dispatch_hook_tests)

enum handler { notify_any_handler, notify_handler, post_dispatch, none };

BOOST_FIXTURE_TEST_CASE( dispatch_hook_tests, tester ) try {
   auto test1 = [&](handler expected, bool has_predispatch) {
      const char * message = nullptr;
      std::set<int> exceptions;
      switch (expected) {
       case notify_any_handler:
         message = "in notify any handler";
         exceptions = { 0, 2, 4, 6 };
         break;
       case notify_handler:
         message = "in notify handler";
         exceptions = { 0, 1, 4, 5 };
         break;
       case post_dispatch:
         message = "in post_dispatch";
         exceptions = { 0, 1, 2, 3 };
         break;
       case none: break;
      }
      for(int i = 0; i < (has_predispatch?16:8); ++i) {
         if(exceptions.count(i)) {
            BOOST_CHECK_EXCEPTION(push_action(N(test), N(forward), N(test), mvo()("dest", "notify")("res", i)), eosio_assert_message_exception, eosio_assert_message_is(message));
         } else {
            push_action(N(test), N(forward), N(test), mvo()("dest", "notify")("res", i));
         }
      }
   };
   create_accounts( { N(test), N(notify) } );
   produce_block();
   set_code( N(test), contracts::dispatch_notify_wasm() );
   set_abi( N(test), contracts::dispatch_notify_abi().data() );
   set_code( N(notify), contracts::dispatch_hook0_wasm() );
   produce_blocks();
   test1(post_dispatch, true);
   produce_block();

   set_code( N(notify), contracts::dispatch_hook1_wasm() );
   produce_blocks();
   test1(notify_handler, true);
   produce_block();

   set_code( N(notify), contracts::dispatch_hook2_wasm() );
   produce_blocks();
   test1(notify_any_handler, true);
   produce_block();

   set_code( N(notify), contracts::dispatch_hook3_wasm() );
   produce_blocks();
   test1(post_dispatch, true);
   produce_block();

   set_code( N(notify), contracts::dispatch_hook4_wasm() );
   produce_blocks();
   test1(post_dispatch, true);
   produce_block();

   set_code( N(notify), contracts::dispatch_hook5_wasm() );
   produce_blocks();
   test1(notify_handler, true);
   produce_block();

} FC_LOG_AND_RETHROW() }
