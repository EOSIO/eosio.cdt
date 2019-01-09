#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

#include <eosiolib/asset.hpp>

#define NATIVE_NAME
#define NATIVE_SYMBOL
#define NATIVE_ASSET

using namespace eosio;
using namespace eosio::native;

// For incremented the `precision` in any test relating to `precision`
// The `precision` field in the `symbol` type must be constructed via constexpr,
// therefore a constexpr function is needed to dynamically test `precision`
constexpr uint8_t increment_precision( uint8_t precision ) { return ++precision; }

// Defined in `eosio.cdt/libraries/eosiolib/asset.hpp`
EOSIO_TEST_BEGIN(asset_type_test)
   // --------------------------------------------
   // constructors/is_amount_within_range/is_valid
   REQUIRE_EQUAL( asset{}.amount, 0ULL );
   REQUIRE_EQUAL( asset{}.symbol.raw(), 0ULL );

   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"SYMBOLL",0}}.amount), 0 );
   REQUIRE_EQUAL( (asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL",0}}.amount), 4611686018427387903LL );
   REQUIRE_EQUAL( (asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL",0}}.amount), -4611686018427387903LL );

   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"A",0}}.symbol.raw()), 16640ULL ); // "A", precision: 0
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"Z",0}}.symbol.raw()), 23040ULL ); // "Z", precision: 0
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"AAAAAAA",0}}.symbol.raw()), 4702111234474983680ULL ); // "AAAAAAA", precision: 0
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"ZZZZZZZ",0}}.symbol.raw()), 6510615555426900480ULL ); // "ZZZZZZZ", precision: 0

   REQUIRE_ASSERT( "invalid symbol name", ([]() {asset{int64_t{}, symbol{16639ULL}};}) );
   REQUIRE_ASSERT( "invalid symbol name", ([]() {asset{int64_t{}, symbol{6510615555426900736ULL}};}) );

   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ([]() {asset{int64_t{4611686018427387904LL}, symbol{}};}) );
   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ([]() {asset{int64_t{-4611686018427387904LL}, symbol{}};}) );

   // ----------
   // set_amount
   asset a{int64_t{0}, symbol{"SYMBOLL", 1}};
   REQUIRE_EQUAL( (a.set_amount(0), a.amount), 0);
   REQUIRE_EQUAL( (a.set_amount(1LL), a.amount), 1LL);
   REQUIRE_EQUAL( (a.set_amount(4611686018427387903LL), a.amount), 4611686018427387903LL);
   REQUIRE_EQUAL( (a.set_amount(-4611686018427387903LL), a.amount), -4611686018427387903LL);

   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", (
      []() {asset{int64_t{0}, symbol{"SYMBOLL",0}}.set_amount(4611686018427387904LL);}) );
   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", (
      []() {asset{int64_t{0}, symbol{"SYMBOLL",0}}.set_amount(-4611686018427387904LL);}) );

   // ---------
   // to_string
   // Note that printing an `asset` is limited to a precision of 63
   // This will trigger an error:
   // `asset{int64_t{1LL}, symbol{"SYMBOLL", 64}}.print();`
   // output: "Floating point exception: ..."

   // This produces weird behavior when the `precision` is 0:
   // `asset{int64_t{1LL}, symbol{"SYMBOLL", precision}}.print();`
   // output: "1. SYMBOLL"

   // Note that `assets` with negative amounts with 0 `precision` print have the odd behavior of printing two '-' characters:
   REQUIRE_EQUAL( (asset{int64_t{0LL}, symbol{"SYMBOLL", 0}}.to_string()),
                  "0. SYMBOLL" );
   REQUIRE_EQUAL( (asset{int64_t{0LL}, symbol{"SYMBOLL", 63}}.to_string()),
                  "0.000000000000000000000000000000000000000000000000000000000000000 SYMBOLL" );

   REQUIRE_EQUAL( (asset{int64_t{1LL}, symbol{"SYMBOLL", 0}}.to_string()),
                  "1. SYMBOLL" );
   REQUIRE_EQUAL( (asset{int64_t{-1LL}, symbol{"SYMBOLL", 0}}.to_string()),
                  "--1. SYMBOLL" );
   REQUIRE_EQUAL( (asset{int64_t{1LL}, symbol{"SYMBOLL", 63}}.to_string()),
                  "0.000000000000000000000000000000000000000000000000000000000000001 SYMBOLL" );
   REQUIRE_EQUAL( (asset{int64_t{-1LL}, symbol{"SYMBOLL", 63}}.to_string()),
                  "-0.000000000000000000000000000000000000000000000000000000000000001 SYMBOLL" );

   REQUIRE_EQUAL( (asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}}.to_string()),
                  "4611686018427387903. SYMBOLL" );
   REQUIRE_EQUAL( (asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}}.to_string()),
                  "--4611686018427387903. SYMBOLL" );
   REQUIRE_EQUAL( (asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 63}}.to_string()),
                  "0.000000000000000000000000000000000000000000004611686018427387903 SYMBOLL" );
   REQUIRE_EQUAL( (asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 63}}.to_string()),
                  "-0.000000000000000000000000000000000000000000004611686018427387903 SYMBOLL" );

   // How are these `assets` constructed? Isn't `uint8_t precision` supposed to be constexpr when constructing a symbol?
   for( uint8_t precision{0}; precision < 64; ++precision ) {
      REQUIRE_EQUAL( (asset{int64_t{0LL}, symbol{"SYMBOLL", precision}}.to_string()),
                     (std::string(std::string("0.") + std::string(precision, '0') + std::string(" SYMBOLL"))) );

   // -----
   // print
// extended_symbol{symbol{"A", 0}, name{"1"}}.print(true);
// extended_symbol{symbol{"AAAAAAA", 255}, name{"111111111111j"}}.print(true);
   // REQUIRE_PRINT( "0@576460752303423488", [](){extended_symbol{symbol{"A", 0}, name{"1"}}.print(true);} );
   // REQUIRE_PRINT( "0@2882303761517117440", [](){extended_symbol{symbol{"Z", 0}, name{"5"}}.print(true);} );
   // REQUIRE_PRINT( "255@595056260442243615", [](){extended_symbol{symbol{"AAAAAAA", 255}, name{"111111111111j"}}.print(true);} );
   // REQUIRE_PRINT( "255@2975281302211218015", [](){extended_symbol{symbol{"ZZZZZZZ", 255}, name{"555555555555j"}}.print(true);} );
   

   // ---------
   // operator-

   // ----------
   // operator-=

   // ----------
   // operator+=

   // ----------
   // operator*=

   // ----------
   // operator/=

   // ---------
   // operator-

   // ---------
   // operator+

   // ---------
   // operator*

   // ---------
   // operator*

   // ---------
   // operator/

   // ---------
   // operator/

   // ----------
   // operator==

   // ----------
   // operator!=

   // ---------
   // operator<

   // ----------
   // operator<=

   // ---------
   // operator>

   // ----------
   // operator>=
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/asset.hpp`
EOSIO_TEST_BEGIN(extended_asset_type_test)
   // constructors

   // get_extended_symbol

   // print

   // operator-

   // operator-

   // operator+

   // operator+=

   // operator-=

   // operator<

   // operator==

   // operator!=

   // operator<=

   // operator>=

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(asset_type_test);
   return has_failed();
}
