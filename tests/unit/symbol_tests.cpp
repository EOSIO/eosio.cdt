#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

using namespace eosio::native;

// Defined in `eosio.cdt/libraries/eosiolib/symbol.hpp`
EOSIO_TEST_BEGIN(symbol_type_test)
   eosio_assert( symbol_code{}.raw() == 0ULL, "symbol_code != 0ULL" );
   eosio_assert( symbol_code{0ULL}.raw() == 0ULL, "symbol_code != 0ULL" );
   eosio_assert( symbol_code{1ULL}.raw() == 1ULL, "symbol_code != 1ULL" );
   eosio_assert( symbol_code{18446744073709551615ULL}.raw() == 18446744073709551615ULL, "symbol_code != 18446744073709551615ULL" );

   eosio_assert( symbol_code{"A"}.raw() == 65ULL, "symbol_code != 65ULL" );
   eosio_assert( symbol_code{"Z"}.raw() == 90ULL, "symbol_code != 90ULL" );
   eosio_assert( symbol_code{"AAAAAAA"}.raw() == 18367622009667905ULL, "symbol_code != 18367622009667905ULL" );
   eosio_assert( symbol_code{"ZZZZZZZ"}.raw() == 25432092013386330ULL, "symbol_code != 25432092013386330ULL" );

   eosio_assert( symbol{0ULL}.value == 0ULL, "symbol != 0ULL" );
   eosio_assert( symbol{1ULL}.value == 1ULL, "symbol != 1ULL" );
   eosio_assert( symbol{18446744073709551615ULL}.value == 18446744073709551615ULL, "symbol != 18446744073709551615ULL" );

   eosio_assert( symbol{"1"}.value == 576460752303423488ULL, "symbol != 1" );
   eosio_assert( symbol{"5"}.value == 2882303761517117440ULL, "symbol != 5" );
   eosio_assert( symbol{"a"}.value == 3458764513820540928ULL, "symbol != a" );
   eosio_assert( symbol{"z"}.value == 17870283321406128128ULL, "symbol != z" );

   eosio_assert( symbol{"abc"}.value == 3589368903014285312ULL, "symbol != abc" );
   eosio_assert( symbol{"123"}.value == 614178399182651392ULL, "symbol != 123" );

   eosio_assert( symbol{".abc"}.value == 112167778219196416ULL, "symbol != .abc" );
   eosio_assert( symbol{"123."}.value == 614178399182651392ULL, "symbol != 123." );
   
   eosio_assert( symbol{"abc.123"}.value == 3589369488740450304ULL, "symbol != abc.123" );
   eosio_assert( symbol{"123.abc"}.value == 614181822271586304ULL, "symbol != 123.abc" );

   eosio_assert( symbol{"12345abcdefgj"}.value == 614251623682315983ULL, "symbol != 12345abcdefgj" );
   eosio_assert( symbol{"hijklmnopqrsj"}.value == 7754926748989239183ULL, "symbol != hijklmnopqrsj" );
   eosio_assert( symbol{"tuvwxyz.1234j"}.value == 14895601873741973071ULL, "symbol != tuvwxyz.1234j" );

   eosio_assert( symbol{"111111111111j"}.value == 595056260442243615ULL, "symbol != 111111111111j" );
   eosio_assert( symbol{"555555555555j"}.value == 2975281302211218015ULL, "symbol != 555555555555j" );
   eosio_assert( symbol{"aaaaaaaaaaaaj"}.value == 3570337562653461615ULL, "symbol != aaaaaaaaaaaaj" );
   eosio_assert( symbol{"zzzzzzzzzzzzj"}.value == 18446744073709551615ULL, "symbol != zzzzzzzzzzzzj" );

   REQUIRE_ASSERT( "character is not in allowed character set for symbols", ([]() {symbol{"-1"}.value;}) );

   REQUIRE_ASSERT( "character is not in allowed character set for symbols", ([]() {symbol{"0"}.value;}) );

   REQUIRE_ASSERT( "character is not in allowed character set for symbols", ([]() {symbol{"6"}.value;}) );

   REQUIRE_ASSERT( "thirteenth character in symbol cannot be a letter that comes after j", ([]() {symbol{"111111111111k"}.value;}) );

   REQUIRE_ASSERT( "thirteenth character in symbol cannot be a letter that comes after j", ([]() {symbol{"zzzzzzzzzzzzk"}.value;}) );

   REQUIRE_ASSERT( "the length must not be longer than 13 characters in an `eosio::symbol`", ([]() {symbol{"12345abcdefghj"}.value;}) );

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(symbol_type_test);
   return has_failed();
}
