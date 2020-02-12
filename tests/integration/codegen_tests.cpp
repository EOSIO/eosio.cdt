#include <boost/test/unit_test.hpp>
#include <eosio/tester.hpp>
#include <tuple>
#include <string>

#include <contracts.hpp>

using namespace eosio;
using eosio::testing::contracts;
using namespace std::literals;
using std::tuple;

BOOST_AUTO_TEST_SUITE(codegen_tests)

BOOST_FIXTURE_TEST_CASE( simple_tests, test_chain ) {
   create_code_account( "test"_n );
   create_code_account( "eosio.token"_n );
   create_code_account( "someone"_n );
   create_code_account( "other"_n );
   finish_block();

   set_code( "eosio.token"_n, contracts::transfer_wasm() );
   set_code( "someone"_n, contracts::transfer_wasm() );
   set_code( "test"_n, contracts::simple_wasm() );
   set_code( "other"_n, contracts::simple_wasm() );

   finish_block();
   transact({action({"test"_n, "active"_n}, "test"_n, "test1"_n, tuple("bucky"_n))});

   transact({action({"test"_n, "active"_n}, "test"_n, "test1"_n, tuple("notbucky"_n))}, "not bucky");

   transact({action({"test"_n, "active"_n}, "test"_n, "test2"_n, tuple(33, "some string"s))});
   transact({action({"test"_n, "active"_n}, "test"_n, "test2"_n, tuple(30, "some string"s))}, "33 does not match");
   transact({action({"test"_n, "active"_n}, "test"_n, "test2"_n, tuple(33, "not some string"s))}, "some string does not match");

   transact({action({"test"_n, "active"_n}, "test"_n, "test3"_n, tuple(33, "some string"s))}, "8000000000000000000");

   finish_block();

   transact({{{"test"_n, "active"_n}, "test"_n, "test4"_n, tuple("someone"_n)}});
   transact({{{"test"_n, "active"_n}, "test"_n, "test5"_n, tuple("someone"_n)}});
   transact({{{"test"_n, "active"_n}, "test"_n, "testa"_n, tuple("someone"_n)}});
   transact({{{"test"_n, "active"_n}, "test"_n, "testb"_n, tuple("someone"_n)}}, "should only be eosio for action failure");

   // test that the pre_dispatch will short circuit dispatching if false
   transact({{{"test"_n, "active"_n}, "test"_n, "testc"_n, tuple("bucky"_n)}});
   transact({{{"test"_n, "active"_n}, "test"_n, "testc"_n, tuple("someone"_n)}}, "should be bucky");
   transact({{{"test"_n, "active"_n}, "test"_n, "testc"_n, tuple("quit"_n)}});

}

BOOST_FIXTURE_TEST_CASE( simple_eosio_tests, test_chain ) {
   set_code( "eosio"_n, contracts::simple_wasm() );
   finish_block();
   transact({{{"eosio"_n, "active"_n}, "eosio"_n, "test1"_n, tuple("bucky"_n)}});

   transact({{{"eosio"_n, "active"_n}, "eosio"_n, "test1"_n, tuple("notbucky"_n)}}, "not bucky");

   transact({{{"eosio"_n, "active"_n}, "eosio"_n, "test2"_n, tuple(33, "some string"s)}});
   transact({{{"eosio"_n, "active"_n}, "eosio"_n, "test2"_n, tuple(30, "some string"s)}}, "33 does not match");
   transact({{{"eosio"_n, "active"_n}, "eosio"_n, "test2"_n, tuple(33, "not some string"s)}}, "some string does not match");

   transact({{{"eosio"_n, "active"_n}, "eosio"_n, "test3"_n, tuple(33, "some string"s)}});
}

BOOST_AUTO_TEST_SUITE_END()
