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

BOOST_AUTO_TEST_SUITE(codegen_tests)

BOOST_FIXTURE_TEST_CASE( simple_tests, tester ) try {
   create_accounts( { "test"_n, "eosio.token"_n, "someone"_n, "other"_n } );
   produce_block();

   set_code( "eosio.token"_n,  contracts::transfer_wasm() );
   set_abi(  "eosio.token"_n,  contracts::transfer_abi().data() );

   set_code( "someone"_n, contracts::transfer_wasm() );
   set_abi(  "someone"_n,  contracts::transfer_abi().data() );

   set_code( "test"_n, contracts::simple_wasm() );
   set_abi( "test"_n,  contracts::simple_abi().data() );

   set_code( "other"_n, contracts::simple_wasm() );
   set_abi( "other"_n,  contracts::simple_abi().data() );

   produce_blocks();
   push_action("test"_n, "test1"_n, "test"_n,
         mvo()
         ("nm", "bucky"));

   BOOST_CHECK_THROW(push_action("test"_n, "test1"_n, "test"_n, mvo()("nm", "notbucky")),
         fc::exception);

   push_action("test"_n, "test2"_n, "test"_n,
         mvo()
         ("arg0", 33)
         ("arg1", "some string"));
   BOOST_CHECK_THROW(push_action("test"_n, "test2"_n, "test"_n, mvo() ("arg0", 30)("arg1", "some string")), fc::exception);
   BOOST_CHECK_THROW(push_action("test"_n, "test2"_n, "test"_n, mvo() ("arg0", 33)("arg1", "not some string")), fc::exception);

   set_abi( "test"_n,  contracts::simple_wrong_abi().data() );
   produce_blocks();

   BOOST_CHECK_THROW(push_action("test"_n, "test3"_n, "test"_n, mvo() ("arg0", 33) ("arg1", "some string")), fc::exception);

   set_abi( "test"_n,  contracts::simple_abi().data() );
   produce_blocks();

   push_action("test"_n, "test4"_n, "test"_n, mvo() ("to", "someone"));
   push_action("test"_n, "test5"_n, "test"_n, mvo() ("to", "someone"));
   push_action("test"_n, "testa"_n, "test"_n, mvo() ("to", "someone"));
   BOOST_CHECK_THROW(push_action("test"_n, "testb"_n, "test"_n, mvo() ("to", "someone")), fc::exception);

   // test that the pre_dispatch will short circuit dispatching if false
   push_action("test"_n, "testc"_n, "test"_n, mvo() ("nm", "bucky"));
   BOOST_CHECK_THROW(push_action("test"_n, "testc"_n, "test"_n, mvo() ("nm", "someone")), fc::exception);
   push_action("test"_n, "testc"_n, "test"_n, mvo() ("nm", "quit"));

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( simple_eosio_tests, tester ) try {
   set_code( "eosio"_n, contracts::simple_wasm() );
   set_abi( "eosio"_n,  contracts::simple_wrong_abi().data() );
   produce_blocks();
   push_action("eosio"_n, "test1"_n, "eosio"_n,
         mvo()
         ("nm", "bucky"));

   BOOST_CHECK_THROW(push_action("eosio"_n, "test1"_n, "eosio"_n, mvo()("nm", "notbucky")),
         fc::exception);

   push_action("eosio"_n, "test2"_n, "eosio"_n,
         mvo()
         ("arg0", 33)
         ("arg1", "some string"));
   BOOST_CHECK_THROW(push_action("eosio"_n, "test2"_n, "eosio"_n, mvo() ("arg0", 30)("arg1", "some string")), fc::exception);
   BOOST_CHECK_THROW(push_action("eosio"_n, "test2"_n, "eosio"_n, mvo() ("arg0", 33)("arg1", "not some string")), fc::exception);

   push_action("eosio"_n, "test3"_n, "eosio"_n,
         mvo()
         ("arg0", 33)
         ("arg1", "some string"));

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
