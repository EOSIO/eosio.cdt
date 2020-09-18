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

BOOST_AUTO_TEST_SUITE(memory_tests)

BOOST_FIXTURE_TEST_CASE( malloc_tests, tester ) try {
   create_accounts( { "test"_n } );
   produce_block();
   set_code( "test"_n, contracts::malloc_tests_wasm() );
   set_abi( "test"_n, contracts::malloc_tests_abi().data() );
   produce_blocks();

   push_action("test"_n, "mallocpass"_n, "test"_n, {});
   push_action("test"_n, "mallocalign"_n, "test"_n, {});
   BOOST_CHECK_EXCEPTION( push_action("test"_n, "mallocfail"_n, "test"_n, {}),
                          eosio_assert_message_exception,
                          eosio_assert_message_is("failed to allocate pages") );
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
