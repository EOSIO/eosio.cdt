#include <catch2/catch.hpp>
#include <eosio/tester.hpp>

#include <contracts.hpp>

using namespace eosio;
using eosio::testing::contracts;
using std::tuple;

TEST_CASE_METHOD( test_chain, "Tests for malloc", "[malloc]" ) {
   create_code_account( "test"_n );
   finish_block();
   set_code( "test"_n, contracts::malloc_tests_wasm() );
   finish_block();

   transact({{{"test"_n, "active"_n}, "test"_n, "mallocpass"_n, tuple()}});
   transact({{{"test"_n, "active"_n}, "test"_n, "mallocalign"_n, tuple()}});
   transact({{{"test"_n, "active"_n}, "test"_n, "mallocfail"_n, tuple()}}, "failed to allocate pages");
}
