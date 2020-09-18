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

BOOST_AUTO_TEST_SUITE(capi_tests)

BOOST_FIXTURE_TEST_CASE( capi_tests, tester ) try {
   create_accounts( { "test"_n } );
   produce_block();

   set_code( "test"_n, contracts::capi_tests_wasm() );
   set_abi( "test"_n, contracts::capi_tests_abi().data() );
   produce_blocks();

   push_action("test"_n, "act"_n, "test"_n, {});
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
