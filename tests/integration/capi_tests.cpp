#include <boost/test/included/unit_test.hpp>
#include <eosio/tester.hpp>
#include <tuple>

#include <contracts.hpp>

using namespace eosio;
using eosio::testing::contracts;
using std::tuple;

BOOST_AUTO_TEST_SUITE(capi_tests)

BOOST_FIXTURE_TEST_CASE( capi_tests, test_chain ) {
   create_code_account( "test"_n );
   finish_block();
   set_code( "test"_n, contracts::capi_tests_wasm() );
   finish_block();

   transact({action({"test"_n, "active"_n}, "test"_n, "act"_n, std::tuple())});
}

BOOST_AUTO_TEST_SUITE_END()
