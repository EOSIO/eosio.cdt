#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

using eosio::check;
using std::string;

using namespace eosio::native;

// Defined in `eosio.cdt/libraries/eosiolib/system.hpp`
EOSIO_TEST_BEGIN(system_test)
   silence_output(false);

   // inline void check(bool, const char*)
   REQUIRE_ASSERT( "asserted", []() { const char* str{"asserted"}; check(false, str);} );

   // inline void check(bool, const std::string&)
   REQUIRE_ASSERT( "asserted", []() { const string str{"asserted"}; check(false, str);} );

   // inline void check(bool, const std::string&&)
   REQUIRE_ASSERT( "asserted", []() { const string str{"asserted"}; check(false, std::move(str));} );

   // inline void check(bool, const char*, size_t)
   REQUIRE_ASSERT( "assert", []() { const char* str{"asserted"}; check(false, str, 6);} );

   // inline void check(bool, const std::string&, size_t)
   REQUIRE_ASSERT( "assert", []() { const string str{"asserted"}; check(false, str, 6);} );

   // Note: this function will not work when give `0` as the `uint64_t` value.
   // Maybe some invariants should be established as to what is acceptable and what isn't.
   // REQUIRE_ASSERT("0", []() { check(false, 0);} );
   // inline void check(bool, uint64_t)
   REQUIRE_ASSERT("1", []() { check(false, 1);} );
   REQUIRE_ASSERT("100", []() { check(false, 100);} );
   REQUIRE_ASSERT("18446744073709551615", []() { check(false, 18446744073709551615ULL);} );
   REQUIRE_ASSERT("18446744073709551615", []() { check(false, -1ULL);} );

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(system_test);
   return has_failed();
}
