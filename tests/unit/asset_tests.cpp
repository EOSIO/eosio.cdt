#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

#include <eosiolib/asset.hpp>

using namespace eosio;
using namespace eosio::native;

// For incrementing the `precision` in any test relating to `precision`
// The `precision` field in the `symbol` type must be constructed via constexpr,
// therefore a constexpr function is needed to dynamically test `precision`
constexpr uint8_t increment_precision( uint8_t precision ) { return ++precision; }

// Defined in `eosio.cdt/libraries/eosiolib/asset.hpp`
EOSIO_TEST_BEGIN(asset_type_test)
   silence_output(true);

   // --------------------------------------------------------------------
   // constructors/bool is_amount_within_range()const/bool is_valid()const

   /// constexpr asset()
   REQUIRE_EQUAL( asset{}.amount, 0ULL );
   REQUIRE_EQUAL( asset{}.symbol.raw(), 0ULL );

   /// constexpr asset(int64_t, symbol)
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"SYMBOLL",0}}.amount), 0 );
   REQUIRE_EQUAL( (asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}}.amount), 4611686018427387903LL );
   REQUIRE_EQUAL( (asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}}.amount), -4611686018427387903LL );

   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"A",0}}.symbol.raw()), 16640ULL ); // "A", precision: 0
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"Z",0}}.symbol.raw()), 23040ULL ); // "Z", precision: 0
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"AAAAAAA",0}}.symbol.raw()), 4702111234474983680ULL ); // "AAAAAAA", precision: 0
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"ZZZZZZZ",0}}.symbol.raw()), 6510615555426900480ULL ); // "ZZZZZZZ", precision: 0

   REQUIRE_ASSERT( "invalid symbol name", ([]() {asset{int64_t{}, symbol{16639ULL}};}) );
   REQUIRE_ASSERT( "invalid symbol name", ([]() {asset{int64_t{}, symbol{6510615555426900736ULL}};}) );

   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ([]() {asset{int64_t{4611686018427387904LL}, symbol{}};}) );
   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ([]() {asset{int64_t{-4611686018427387904LL}, symbol{}};}) );

   // ------------------------
   // void set_amount(int64_t)
   asset a{int64_t{0}, symbol{"SYMBOLL", 1}};
   REQUIRE_EQUAL( (a.set_amount(0), a.amount), 0);
   REQUIRE_EQUAL( (a.set_amount(1LL), a.amount), 1LL);
   REQUIRE_EQUAL( (a.set_amount(4611686018427387903LL), a.amount), 4611686018427387903LL);
   REQUIRE_EQUAL( (a.set_amount(-4611686018427387903LL), a.amount), -4611686018427387903LL);

   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", (
      []() {asset{int64_t{0}, symbol{"SYMBOLL",0}}.set_amount(4611686018427387904LL);}) );
   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", (
      []() {asset{int64_t{0}, symbol{"SYMBOLL",0}}.set_amount(-4611686018427387904LL);}) );

   // ---------------------------------------
   // std::to_string()const/void print()const
   // Note that printing an `asset` is limited to a precision of 63
   // This will trigger an error:
   // `asset{int64_t{1LL}, symbol{"SYMBOLL", 64}}.print();`
   // output: "Floating point exception: ..."

   // This produces weird behavior when the `precision` is 0:
   // `asset{int64_t{1LL}, symbol{"SYMBOLL", precision}}.print();`
   // output: "1. SYMBOLL"

   // Note that `assets` with negative amounts with 0 `precision` print have the odd behavior of printing two '-' characters:
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"SYMBOLL", 0}}.to_string()),
                  "0. SYMBOLL" );
   REQUIRE_EQUAL( (asset{int64_t{-0}, symbol{"SYMBOLL", 0}}.to_string()),
                  "0. SYMBOLL" );
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"SYMBOLL", 63}}.to_string()),
                  "0.000000000000000000000000000000000000000000000000000000000000000 SYMBOLL" );
   REQUIRE_EQUAL( (asset{int64_t{-0}, symbol{"SYMBOLL", 63}}.to_string()),
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
   }

   // ----------------------
   // asset operator-()const
   REQUIRE_EQUAL( (-asset{int64_t{0}, symbol{"SYMBOLL", 0}}.amount), (asset{int64_t{0}, symbol{"SYMBOLL", 0}}.amount) );
   REQUIRE_EQUAL( (-asset{int64_t{-0}, symbol{"SYMBOLL", 0}}.amount), (asset{int64_t{0}, symbol{"SYMBOLL", 0}}.amount) );
   REQUIRE_EQUAL( (-asset{int64_t{0}, symbol{"SYMBOLL", 63}}.amount), (asset{int64_t{0}, symbol{"SYMBOLL", 63}}.amount) );
   REQUIRE_EQUAL( (-asset{int64_t{-0}, symbol{"SYMBOLL", 63}}.amount), (asset{int64_t{0}, symbol{"SYMBOLL", 63}}.amount) );

   REQUIRE_EQUAL( (-asset{int64_t{1LL}, symbol{"SYMBOLL", 0}}.amount), (asset{int64_t{-1LL}, symbol{"SYMBOLL", 0}}.amount) );
   REQUIRE_EQUAL( (-asset{int64_t{-1LL}, symbol{"SYMBOLL", 0}}.amount), (asset{int64_t{1LL}, symbol{"SYMBOLL", 0}}.amount) );
   REQUIRE_EQUAL( (-asset{int64_t{1LL}, symbol{"SYMBOLL", 63}}.amount), (asset{int64_t{-1LL}, symbol{"SYMBOLL", 63}}.amount) );
   REQUIRE_EQUAL( (-asset{int64_t{-1LL}, symbol{"SYMBOLL", 63}}.amount), (asset{int64_t{1LL}, symbol{"SYMBOLL", 63}}.amount) );

   REQUIRE_EQUAL( (-asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}}.amount),
                  (asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}}.amount) );
   REQUIRE_EQUAL( (-asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}}.amount),
                  (asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}}.amount) );
   REQUIRE_EQUAL( (-asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 63}}.amount),
                  (asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 63}}.amount) );
   REQUIRE_EQUAL( (-asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 63}}.amount),
                  (asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 63}}.amount) );

   // ------------------------------------------------------------------------------------------
   // inline friend asset& operator+(const asset&, const asset&)/asset& operator+=(const asset&)
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"SYMBOLL", 0}} += asset{int64_t{0}, symbol{"SYMBOLL", 0}} ),
                  (asset{int64_t{0}, symbol{"SYMBOLL", 0}}) );

   REQUIRE_EQUAL( (asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} += asset{int64_t{-1LL}, symbol{"SYMBOLL", 0}} ),
                  (asset{int64_t{0}, symbol{"SYMBOLL", 0}}) );

   REQUIRE_ASSERT( "attempt to add asset with different symbol", (
      []() {asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} += asset{int64_t{1LL}, symbol{"LLOBMYS", 0}};}) );
   REQUIRE_ASSERT( "addition underflow", (
      []() {asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}} += -asset{int64_t{1LL}, symbol{"SYMBOLL", 0}};}) );
   REQUIRE_ASSERT( "addition overflow", (
      []() {asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}} += asset{int64_t{1LL}, symbol{"SYMBOLL", 0}};}) );
           
   // ------------------------------------------------------------------------------------------
   // inline friend asset& operator-(const asset&, const asset&)/asset& operator-=(const asset&)
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"SYMBOLL", 0}} -= asset{int64_t{0}, symbol{"SYMBOLL", 0}} ),
                  (asset{int64_t{0}, symbol{"SYMBOLL", 0}}) );

   REQUIRE_EQUAL( (asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} -= asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} ),
                  (asset{int64_t{0}, symbol{"SYMBOLL", 0}}) );

   REQUIRE_ASSERT( "attempt to subtract asset with different symbol", (
      []() {asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} -= asset{int64_t{1LL}, symbol{"LLOBMYS", 0}};}) );
   REQUIRE_ASSERT( "subtraction underflow", (
      []() {asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}} -= asset{int64_t{1LL}, symbol{"SYMBOLL", 0}};}) );
   REQUIRE_ASSERT( "subtraction overflow", (
      []() {asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}} -= -asset{int64_t{1LL}, symbol{"SYMBOLL", 0}};}) );

   // -----------------------------------------------------------------------
   // friend asset operator*(const asset&, int64_t)/asset operator*=(int64_t)
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"SYMBOLL", 0}} *= 0 ),
                  (asset{int64_t{0}, symbol{"SYMBOLL", 0}}) );

   REQUIRE_EQUAL( (asset{int64_t{2}, symbol{"SYMBOLL", 0}} *= 1LL ),
                  (asset{int64_t{2}, symbol{"SYMBOLL", 0}}) );
   REQUIRE_EQUAL( (asset{int64_t{2}, symbol{"SYMBOLL", 0}} *= -1LL ),
                  (asset{int64_t{-2}, symbol{"SYMBOLL", 0}}) );

   REQUIRE_ASSERT( "multiplication overflow", (
      []() {asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}} *= 2LL;}) );
   REQUIRE_ASSERT( "multiplication underflow", (
      []() {asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}} *= 2LL;}) );

   // // ------------------------------------------------------------------------
   // // friend asset operator/(const asset&, int64_t)/asset& operator/=(int64_t)/friend int64_t operator/(const asset&, const asset&)
   // REQUIRE_EQUAL( (asset{int64_t{}, symbol{"SYMBOLL", 0}} /= asset{int64_t{}, symbol{"SYMBOLL", 0}} ),
   //                (asset{int64_t{}, symbol{"SYMBOLL", 0}}) );
   // REQUIRE_EQUAL( (asset{int64_t{}, symbol{"SYMBOLL", 0}} /= asset{int64_t{}, symbol{"SYMBOLL", 0}} ),
   //                (asset{int64_t{}, symbol{"SYMBOLL", 0}}) );
   // REQUIRE_EQUAL( (asset{int64_t{}, symbol{"SYMBOLL", 0}} /= asset{int64_t{}, symbol{"SYMBOLL", 0}} ),
   //                (asset{int64_t{}, symbol{"SYMBOLL", 0}}) );
   // REQUIRE_EQUAL( (asset{int64_t{}, symbol{"SYMBOLL", 0}} /= asset{int64_t{}, symbol{"SYMBOLL", 0}} ),
   //                (asset{int64_t{}, symbol{"SYMBOLL", 0}}) );

   REQUIRE_ASSERT( "divide by zero", ( []() {asset{int64_t{1}, symbol{"SYMBOLL", 0}} /= 0;}) );
///////////////////////////////////////////////////////////////////////////////////////////////////
// Why is this not throwing??
// And why is my addition test throwing??
   // REQUIRE_ASSERT( "signed division overflow", (
   //    []() {asset{int64_t{std::numeric_limits<int64_t>::min()}, symbol{"SYMBOLL", 0}} /= -1LL;}) );

   // REQUIRE_ASSERT( "divide by zero", (
   //    []() {asset{int64_t{std::numeric_limits<int64_t>::min()}, symbol{"SYMBOLL", 0}} /
   //          asset{int64_t{std::numeric_limits<int64_t>::min()}, symbol{"SYMBOLL", 0}};}) );
   // REQUIRE_ASSERT( "comparison of assets with different symbols is not allowed", (
   //    []() {asset{int64_t{std::numeric_limits<int64_t>::min()}, symbol{"SYMBOLL", 0}} /
   //          asset{int64_t{std::numeric_limits<int64_t>::min()}, symbol{"SYMBOLL", 0}}}) );
///////////////////////////////////////////////////////////////////////////////////////////////////

   // --------------------------------------------------
   // friend bool operator==(const asset&, const asset&)
   REQUIRE_EQUAL( (asset{int64_t{0}, symbol{"SYMBOLL", 0}} == asset{int64_t{0}, symbol{"SYMBOLL", 0}} ), true );

   REQUIRE_EQUAL( (asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}} ==
                   asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}} ), true );

   REQUIRE_EQUAL( (asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}} ==
                   asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}} ), true );

    REQUIRE_ASSERT( "comparison of assets with different symbols is not allowed", (
       []() {asset{int64_t{0}, symbol{"SYMBOLL", 0}} == asset{int64_t{0}, symbol{"LLOBMYS", 0}};}) );

   // ---------------------------------------------------
   // friend bool operator!=( const asset&, const asset&)
   REQUIRE_EQUAL( (asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}} !=
                   -asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}} ), true );

   REQUIRE_EQUAL( (asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}} !=
                   -asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}} ), true );

   // -------------------------------------------------
   // friend bool operator<(const asset&, const asset&)
   REQUIRE_EQUAL( ( asset{int64_t{0}, symbol{"SYMBOLL", 0}} < asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} ), true );

   // --------------------------------------------------
   // friend bool operator<=(const asset&, const asset&)
   REQUIRE_EQUAL( ( asset{int64_t{0}, symbol{"SYMBOLL", 0}} <= asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} ), true );
   REQUIRE_EQUAL( ( asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} <= asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} ), true );

   // -------------------------------------------------
   // friend bool operator>(const asset&, const asset&)
   REQUIRE_EQUAL( ( asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} > asset{int64_t{0}, symbol{"SYMBOLL", 0}} ), true );

   // --------------------------------------------------
   // friend bool operator>=( const asset&, const asset&)
   REQUIRE_EQUAL( ( asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} >= asset{int64_t{0}, symbol{"SYMBOLL", 0}} ), true );
   REQUIRE_EQUAL( ( asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} >= asset{int64_t{1LL}, symbol{"SYMBOLL", 0}} ), true );

   silence_output(false);
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/asset.hpp`
EOSIO_TEST_BEGIN(extended_asset_type_test)
   silence_output(true);

   // ------------
   // constructors

   /// extended_asset()
   REQUIRE_EQUAL( extended_asset{}.quantity, asset{} );
   REQUIRE_EQUAL( extended_asset{}.contract, name{} );

   /// extended_asset(asset, name)
   REQUIRE_EQUAL( (extended_asset{int64_t{0}, extended_symbol{symbol{"SYMBOLL", 0}, name{"eosioaccountj"}}}.quantity),
                  (asset{int64_t{0}, symbol{"SYMBOLL", 0}}) );
   REQUIRE_EQUAL( (extended_asset{int64_t{0}, extended_symbol{symbol{"SYMBOLL", 0}, name{"eosioaccountj"}}}.contract),
                   name{"eosioaccountj"});

   REQUIRE_EQUAL( (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{"eosioaccountj"}}.quantity),
                  (asset{int64_t{0}, symbol{"SYMBOLL", 0}}) );
   REQUIRE_EQUAL( (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{"eosioaccountj"}}.contract),
                  (name{"eosioaccountj"}) );

   // ------------------------------------------
   // extended_symbol get_extended_symbol()const
   REQUIRE_EQUAL( (extended_asset{asset{}, name{}}.get_extended_symbol()), extended_symbol{} );

   REQUIRE_EQUAL( (extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"1"}}.get_extended_symbol()),
                  (extended_symbol{symbol{"A", 0}, name{"1"}}) );
   REQUIRE_EQUAL( (extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"5"}}.get_extended_symbol()),
                  (extended_symbol{symbol{"A", 0}, name{"5"}}) );
   REQUIRE_EQUAL( (extended_asset{asset{int64_t{0}, symbol{"Z", 0}}, name{"a"}}.get_extended_symbol()),
                  (extended_symbol{symbol{"Z", 0}, name{"a"}}) );
   REQUIRE_EQUAL( (extended_asset{asset{int64_t{0}, symbol{"Z", 0}}, name{"z"}}.get_extended_symbol()),
                  (extended_symbol{symbol{"Z", 0}, name{"z"}}) );

   REQUIRE_EQUAL( (extended_asset{asset{int64_t{0}, symbol{"AAAAAAA", 0}}, name{"111111111111j"}}.get_extended_symbol()),
                  (extended_symbol{symbol{"AAAAAAA", 0}, name{"111111111111j"}}) );
   REQUIRE_EQUAL( (extended_asset{asset{int64_t{0}, symbol{"AAAAAAA", 0}}, name{"555555555555j"}}.get_extended_symbol()),
                  (extended_symbol{symbol{"AAAAAAA", 0}, name{"555555555555j"}}) );
   REQUIRE_EQUAL( (extended_asset{asset{int64_t{0}, symbol{"ZZZZZZZ", 0}}, name{"aaaaaaaaaaaaj"}}.get_extended_symbol()),
                  (extended_symbol{symbol{"ZZZZZZZ", 0}, name{"aaaaaaaaaaaaj"}}) );
   REQUIRE_EQUAL( (extended_asset{asset{int64_t{0}, symbol{"ZZZZZZZ", 0}}, name{"zzzzzzzzzzzzj"}}.get_extended_symbol()),
                  (extended_symbol{symbol{"ZZZZZZZ", 0}, name{"zzzzzzzzzzzzj"}}) );


   // Fix this printing functionality
   // Note that if there is no precision, there will be odd output:
   // `extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"1"}}.print();`
   // output: "0. A@1"
   // -----------------
   // void print()const
///////////////////////////////////////////////////////////////////////////////////////////////////
// How is this not the correct print result??
// print_f("0. A@1");
// print_f("\n");;
// extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"1"}}.print();
// print_f("\n");;
//    REQUIRE_PRINT( "0. A@1", [](){extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"1"}}.print();} );
//    REQUIRE_PRINT( "0. A@5", [](){extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"5"}}.print();} );
//    REQUIRE_PRINT( "0. Z@a", [](){extended_asset{asset{int64_t{0}, symbol{"Z", 0}}, name{"a"}}.print();} );
//    REQUIRE_PRINT( "0. Z@z", [](){extended_asset{asset{int64_t{0}, symbol{"Z", 0}}, name{"z"}}.print();} );

//    REQUIRE_PRINT( "0. AAAAAAA@111111111111j", [](){extended_asset{asset{int64_t{0}, symbol{"AAAAAAA", 0}}, name{"111111111111j"}}.print();} );
//    REQUIRE_PRINT( "0. AAAAAAA@555555555555j", [](){extended_asset{asset{int64_t{0}, symbol{"AAAAAAA", 0}}, name{"555555555555j"}}.print();} );
//    REQUIRE_PRINT( "0. ZZZZZZZ@aaaaaaaaaaaaj", [](){extended_asset{asset{int64_t{0}, symbol{"ZZZZZZZ", 0}}, name{"aaaaaaaaaaaaj"}}.print();} );
//    REQUIRE_PRINT( "0. ZZZZZZZ@zzzzzzzzzzzzj", [](){extended_asset{asset{int64_t{0}, symbol{"ZZZZZZZ", 0}}, name{"zzzzzzzzzzzzj"}}.print();} );
///////////////////////////////////////////////////////////////////////////////////////////////////

   // -------------------------------
   // extended_asset operator-()const
   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{-0}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );
   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{-0}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );

   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{1LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{-1LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{1LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{-1LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{1LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{-1LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );
   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{1LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{-1LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );

   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );
   REQUIRE_EQUAL( (-extended_asset{asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
                  (extended_asset{asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );
   

   // -----------------------------------------------------------------------------
   // friend extended_asset operator+(const extended_asset&, const extended_asset&)
   extended_asset asset0{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}};
   extended_asset asset1{asset{int64_t{1LL}, symbol{"SYMBOLL", 0}}, name{}};
   extended_asset asset2{asset{int64_t{-1LL}, symbol{"SYMBOLL", 0}}, name{}};

   extended_asset asset3{asset{int64_t{}, symbol{"SYMBOLL", 0}}, name{"eosioaccountj"}};
   extended_asset asset4{asset{int64_t{}, symbol{"SYMBOLL", 0}}, name{"jtnuoccaoisoe"}};

   REQUIRE_EQUAL( (asset0 + asset0), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );
   REQUIRE_EQUAL( (asset1 + asset2), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );

   REQUIRE_ASSERT( "type mismatch", (
      [&]() {asset3 + asset4;}) );

   // -------------------------------------------------------------------------
   // friend extended_asset& operator+=(extended_asset&, const extended_asset&)
   REQUIRE_EQUAL( (asset0 += asset0), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );
   REQUIRE_EQUAL( (asset1 += asset2), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );

   REQUIRE_ASSERT( "type mismatch", (
      [&]() {asset3 += asset4;}) );

   // --------------------------------------------------------------------------
   // friend extended_asset& operator-=( extended_asset&, const extended_asset&)
   REQUIRE_EQUAL( (asset0 - asset0), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );
   REQUIRE_EQUAL( (asset1 - asset1), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );

   REQUIRE_ASSERT( "type mismatch", (
      [&]() {asset3 - asset4;}) );

   // ----------------------------------------------------------------------------
   //friend extended_asset operator-(const extended_asset&, const extended_asset&)
   REQUIRE_EQUAL( (asset0 -= asset0), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );
   REQUIRE_EQUAL( (asset1 -= asset1), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );

   REQUIRE_ASSERT( "type mismatch", (
      [&]() {asset3 -= asset4;}) );

   // --------------------------------------------------------------------
   // friend bool operator==(const extended_asset&, const extended_asset&)
   REQUIRE_EQUAL( (asset0 == asset0), true );
   REQUIRE_EQUAL( (asset1 == asset1), true );

///////////////////////////////////////////////////////////////////////////////////////////////////
// Why is this not throwing??
   // REQUIRE_ASSERT( "type mismatch", (
   //    [&]() {asset3 == asset4;}) );
///////////////////////////////////////////////////////////////////////////////////////////////////

   // --------------------------------------------------------------------
   // friend bool operator!=(const extended_asset&, const extended_asset&)
// print_f("%", asset3 != asset4);
// print_f("\n");
   REQUIRE_EQUAL( asset3 != asset4, true );
   // REQUIRE_EQUAL( (asset1 != asset2), false );
   // REQUIRE_EQUAL( (asset2 != asset3), false );

   // REQUIRE_ASSERT( "type mismatch", (
   //    [&]() {asset0 != asset1;}) );

   // --------------------------------------------------------------------
   // friend bool operator<( const extended_asset&, const extended_asset&)
   // ???
   REQUIRE_EQUAL( asset0 < asset1, true );

   // REQUIRE_ASSERT( "type mismatch", (
   //    []() {extended_asset{asset{}, name{}};}) );

   // --------------------------------------------------------------------
   // friend bool operator<=(const extended_asset&, const extended_asset&)
   // REQUIRE_EQUAL( asset0 <= asset1, true );

   // --------------------------------------------------------------------
   // friend bool operator>=(const extended_asset&, const extended_asset&)
   // REQUIRE_EQUAL( asset1 > asset0, true );

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(asset_type_test);
   EOSIO_TEST(extended_asset_type_test);
   return has_failed();
}
