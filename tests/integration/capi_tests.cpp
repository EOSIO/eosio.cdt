#include <catch2/catch.hpp>
#include <eosio/tester.hpp>
#include <tuple>

#include <contracts.hpp>

using namespace eosio;
using eosio::testing::contracts;
using std::tuple;

TEST_CASE_METHOD( test_chain, "C API tests", "[capi]" ) {
   create_code_account( "test"_n );
   finish_block();
   set_code( "test"_n, contracts::capi_tests_wasm() );
   finish_block();

   transact({action({"test"_n, "active"_n}, "test"_n, "act"_n, std::tuple())});
}
