#include <boost/test/unit_test.hpp>
#include <eosio/tester.hpp>

#include <contracts.hpp>

using namespace eosio;
using eosio::testing::contracts;
using std::tuple;

BOOST_AUTO_TEST_SUITE(memory_tests)

BOOST_FIXTURE_TEST_CASE( malloc_tests, test_chain ) {
   create_code_account( "test"_n );
   finish_block();
   set_code( "test"_n, contracts::malloc_tests_wasm() );
   finish_block();

   transact({{{"test"_n, "active"_n}, "test"_n, "mallocpass"_n, tuple()}});
   transact({{{"test"_n, "active"_n}, "test"_n, "mallocalign"_n, tuple()}});
   transact({{{"test"_n, "active"_n}, "test"_n, "mallocfail"_n, tuple()}}, "failed to allocate pages");
}

BOOST_AUTO_TEST_SUITE_END()
