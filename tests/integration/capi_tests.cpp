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
   create_accounts( { N(test) } );
   produce_block();

   const auto& pfm = control->get_protocol_feature_manager();

   auto d = pfm.get_builtin_digest(builtin_protocol_feature_t::action_return_value)
   schedule_protocol_features( {*d} );

   set_code( N(test), contracts::capi_tests_wasm() );
   set_abi( N(test), contracts::capi_tests_abi().data() );
   produce_blocks();

   push_action(N(test), N(act), N(test), {});
} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
