#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

#include <eosiolib/asset.hpp>

#define NATIVE_NAME
#define NATIVE_SYMBOL
#define NATIVE_ASSET

using namespace eosio;
using namespace eosio::native;

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
   // REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"SYMBOLL",0}}.set_amount(0LL)), 0LL);
   // REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"SYMBOLL",0}}.set_amount(4611686018427387903LL)), 4611686018427387903LL);
   // REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"SYMBOLL",0}}.set_amount(-4611686018427387903LL)), -4611686018427387903LL);

   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", (
      []() {asset{int64_t{0}, symbol{"SYMBOLL",0}}.set_amount(4611686018427387904LL);}) );
   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", (
      []() {asset{int64_t{0}, symbol{"SYMBOLL",0}}.set_amount(-4611686018427387904LL);}) );

   // ---------
   // to_string
   asset{int64_t{0}, symbol{"SYMBOLL",0}}.print();
print_f("\n");
   asset{int64_t{10}, symbol{"A",1}}.print();
print_f("\n");
   asset{int64_t{10}, symbol{"Z",1}}.print();
print_f("\n");
   asset{int64_t{10}, symbol{"AAAAAAA",1}}.print();
print_f("\n");
   asset{int64_t{10}, symbol{"ZZZZZZZ",1}}.print();
print_f("\n");

   // REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"A",0}}.symbol.raw()), 16640ULL ); // "A", precision: 0
   // REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"Z",0}}.symbol.raw()), 23040ULL ); // "Z", precision: 0
   // REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"AAAAAAA",0}}.symbol.raw()), 4702111234474983680ULL ); // "AAAAAAA", precision: 0
   // REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"ZZZZZZZ",0}}.symbol.raw()), 6510615555426900480ULL ); // "ZZZZZZZ", precision: 0

   // -----
   // print

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
