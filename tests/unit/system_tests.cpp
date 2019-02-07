/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/native/tester.hpp>
#include <eosiolib/system.hpp>

#include <string>

using eosio::check;
using std::string;

// Definitions in `eosio.cdt/libraries/eosiolib/system.hpp`
EOSIO_TEST_BEGIN(system_test)
   silence_output(true);

   // ------------------------------------
   // inline void check(bool, const char*)
   CHECK_ASSERT( "asserted", []() { const char* str{"asserted"}; check(false, str);} );

   // -------------------------------------------
   // inline void check(bool, const std::string&)
   CHECK_ASSERT( "asserted", []() { const string str{"asserted"}; check(false, str);} );

   // --------------------------------------------
   // inline void check(bool, const std::string&&)
   CHECK_ASSERT( "asserted", []() { const string str{"asserted"}; check(false, std::move(str));} );

   // --------------------------------------------
   // inline void check(bool, const char*, size_t)
   CHECK_ASSERT( "assert", []() { const char* str{"asserted"}; check(false, str, 6);} );

   // ---------------------------------------------------
   // inline void check(bool, const std::string&, size_t)
   CHECK_ASSERT( "assert", []() { const string str{"asserted"}; check(false, str, 6);} );

   // Note: this function will not work when given `0` as the `uint64_t` value.
   // Maybe some invariants should be established as to what is acceptable and what isn't.
   // For example:
   // CHECK_ASSERT("0", []() { check(false, 0);} );
   // ---------------------------------
   // inline void check(bool, uint64_t)
   CHECK_ASSERT("1", []() { check(false, 1);} );
   CHECK_ASSERT("100", []() { check(false, 100);} );
   CHECK_ASSERT("18446744073709551615", []() { check(false, 18446744073709551615ULL);} );
   CHECK_ASSERT("18446744073709551615", []() { check(false, -1ULL);} );

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(system_test);
   return has_failed();
}
