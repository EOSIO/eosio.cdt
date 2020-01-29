#include <boost/test/unit_test.hpp>
#include <eosio/testing/tester.hpp>
#include <eosio/chain/abi_serializer.hpp>

#include <contracts.hpp>

using namespace eosio;
using namespace eosio::testing;

BOOST_AUTO_TEST_SUITE(key_value_tests)

BOOST_FIXTURE_TEST_CASE(single_tests_find, tester) try {
    create_accounts( { N(kvtest) } );
    produce_block();
    set_code( N(kvtest), contracts::kv_single_tests_wasm() );
    set_abi( N(kvtest), contracts::kv_single_tests_abi().data() );
    produce_blocks();

    push_action(N(kvtest), N(setup), N(kvtest), {});
    push_action(N(kvtest), N(find), N(kvtest), {});

    BOOST_CHECK_EXCEPTION(push_action(N(kvtest), N(finderror), N(kvtest), {}),
                          eosio_assert_message_exception,
                          eosio_assert_message_is("Cannot read end iterator"));
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_get, tester) try {
    create_accounts( { N(kvtest) } );
    produce_block();
    set_code( N(kvtest), contracts::kv_single_tests_wasm() );
    set_abi( N(kvtest), contracts::kv_single_tests_abi().data() );
    produce_blocks();

    push_action(N(kvtest), N(setup), N(kvtest), {});
    push_action(N(kvtest), N(get), N(kvtest), {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_bounds, tester) try {
    create_accounts( { N(kvtest) } );
    produce_block();
    set_code( N(kvtest), contracts::kv_single_tests_wasm() );
    set_abi( N(kvtest), contracts::kv_single_tests_abi().data() );
    produce_blocks();

    push_action(N(kvtest), N(setup), N(kvtest), {});
    push_action(N(kvtest), N(bounds), N(kvtest), {});
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_iteration, tester) try {
    create_accounts( { N(kvtest) } );
    produce_block();
    set_code( N(kvtest), contracts::kv_single_tests_wasm() );
    set_abi( N(kvtest), contracts::kv_single_tests_abi().data() );
    produce_blocks();

    push_action(N(kvtest), N(setup), N(kvtest), {});
    push_action(N(kvtest), N(iteration), N(kvtest), {});

    BOOST_CHECK_EXCEPTION(push_action(N(kvtest), N(itrerror1), N(kvtest), {}),
                          eosio_assert_message_exception,
                          eosio_assert_message_is("cannot increment end iterator"));

    BOOST_CHECK_EXCEPTION(push_action(N(kvtest), N(itrerror2), N(kvtest), {}),
                          eosio_assert_message_exception,
                          eosio_assert_message_is("incremented past the beginning"));
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_range, tester) try {
    create_accounts( { N(kvtest) } );
    produce_block();
    set_code( N(kvtest), contracts::kv_single_tests_wasm() );
    set_abi( N(kvtest), contracts::kv_single_tests_abi().data() );
    produce_blocks();

    push_action(N(kvtest), N(setup), N(kvtest), {});
    push_action(N(kvtest), N(range), N(kvtest), {});

    BOOST_CHECK_EXCEPTION(push_action(N(kvtest), N(rangeerror1), N(kvtest), {}),
                          eosio_assert_message_exception,
                          eosio_assert_message_is("Beginning of range should be less than or equal to end"));
    BOOST_CHECK_EXCEPTION(push_action(N(kvtest), N(rangeerror2), N(kvtest), {}),
                          eosio_assert_message_exception,
                          eosio_assert_message_is("beginning of range is not in table"));
    BOOST_CHECK_EXCEPTION(push_action(N(kvtest), N(rangeerror3), N(kvtest), {}),
                          eosio_assert_message_exception,
                          eosio_assert_message_is("end of range is not in table"));
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(single_tests_erase, tester) try {
    create_accounts( { N(kvtest) } );
    produce_block();
    set_code( N(kvtest), contracts::kv_single_tests_wasm() );
    set_abi( N(kvtest), contracts::kv_single_tests_abi().data() );
    produce_blocks();

    push_action(N(kvtest), N(setup), N(kvtest), {});
    push_action(N(kvtest), N(erase), N(kvtest), {});

    BOOST_CHECK_EXCEPTION(push_action(N(kvtest), N(eraseerror), N(kvtest), {}),
                          eosio_assert_message_exception,
                          eosio_assert_message_is("Attempted to erase non-existent key"));
} FC_LOG_AND_RETHROW()

// Multi
// -----
BOOST_FIXTURE_TEST_CASE(multi_tests_find, tester) try {
    create_accounts( { N(kvtest) } );
    produce_block();
    set_code( N(kvtest), contracts::kv_multi_tests_wasm() );
    set_abi( N(kvtest), contracts::kv_multi_tests_abi().data() );
    produce_blocks();

    push_action(N(kvtest), N(setup), N(kvtest), {});
    push_action(N(kvtest), N(find), N(kvtest), {});

    BOOST_CHECK_EXCEPTION(push_action(N(kvtest), N(finderror), N(kvtest), {}),
                          eosio_assert_message_exception,
                          eosio_assert_message_is("Cannot read end iterator"));
} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(multi_tests_iteration, tester) try {
    create_accounts( { N(kvtest) } );
    produce_block();
    set_code( N(kvtest), contracts::kv_multi_tests_wasm() );
    set_abi( N(kvtest), contracts::kv_multi_tests_abi().data() );
    produce_blocks();

    push_action(N(kvtest), N(setup), N(kvtest), {});
    push_action(N(kvtest), N(iteration), N(kvtest), {});
} FC_LOG_AND_RETHROW()
BOOST_AUTO_TEST_SUITE_END()
