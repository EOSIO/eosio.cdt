#include <eosio/native/tester.hpp>
#include <eosiolib/asset.hpp>

using eosio::name;
using eosio::symbol;
using eosio::extended_symbol;
using eosio::asset;
using eosio::extended_asset;

const int64_t i64_min{std::numeric_limits<int64_t>::min()}; // -9223372036854775808LL
const int64_t i64_max{std::numeric_limits<int64_t>::max()}; //  9223372036854775807LL

const uint64_t u64_min{std::numeric_limits<uint64_t>::min()}; // 0ULL
const uint64_t u64_max{std::numeric_limits<uint64_t>::max()}; // 18446744073709551615ULL

const int64_t asset_mask{(1LL << 62) - 1};
const int64_t asset_min{-asset_mask}; // -4611686018427387903
const int64_t asset_max{ asset_mask}; //  4611686018427387903

// For incrementing the `precision` in any test relating to `precision`
// The `precision` field in the `symbol` type must be constructed via constexpr,
// therefore a constexpr function is needed to dynamically test `precision`
constexpr uint8_t increment_precision( uint8_t precision ) { return ++precision; }

// Defined in `eosio.cdt/libraries/eosiolib/asset.hpp`
EOSIO_TEST_BEGIN(asset_type_test)
   silence_output(false);

   symbol s0{"A", 0};
   symbol s1{"Z", 0};
   symbol s2{"AAAAAAA", 0};
   symbol s3{"ZZZZZZZ", 0};
   symbol s4{"SYMBOLL", 0};
   symbol s5{"SYMBOLL", 63};

   // --------------------------------------------------------------------
   // constructors/bool is_amount_within_range()const/bool is_valid()const

   /// constexpr asset()
   REQUIRE_EQUAL( asset{}.amount, 0ULL );
   REQUIRE_EQUAL( asset{}.symbol.raw(), 0ULL );

   /// constexpr asset(int64_t, symbol)
   REQUIRE_EQUAL( (asset{0LL, s4}.amount), 0LL );
   REQUIRE_EQUAL( (asset{asset_min, s4}.amount), asset_min );
   REQUIRE_EQUAL( (asset{asset_max, s4}.amount), asset_max );

   REQUIRE_EQUAL( (asset{0LL, s0}.symbol.raw()), 16640ULL ); // "A", precision: 0
   REQUIRE_EQUAL( (asset{0LL, s1}.symbol.raw()), 23040ULL ); // "Z", precision: 0
   REQUIRE_EQUAL( (asset{0LL, s2}.symbol.raw()), 4702111234474983680ULL ); // "AAAAAAA", precision: 0
   REQUIRE_EQUAL( (asset{0LL, s3}.symbol.raw()), 6510615555426900480ULL ); // "ZZZZZZZ", precision: 0

   REQUIRE_ASSERT( "invalid symbol name", ([]() {asset{0LL, symbol{16639ULL}};}) );
   REQUIRE_ASSERT( "invalid symbol name", ([]() {asset{0LL, symbol{6510615555426900736ULL}};}) );

   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ([]() {asset{ asset_min - 1, {}};}) );
   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ([]() {asset{ asset_max + 1, {}};}) );

   // ------------------------
   // void set_amount(int64_t)
   asset a{0LL, s4};
   REQUIRE_EQUAL( (a.set_amount(0LL), a.amount), 0LL);
   REQUIRE_EQUAL( (a.set_amount(1LL), a.amount), 1LL);
   REQUIRE_EQUAL( (a.set_amount(asset_min), a.amount), asset_min);
   REQUIRE_EQUAL( (a.set_amount(asset_max), a.amount), asset_max);

   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ( [&]() {
         a.set_amount(asset_min - 1);
      })
   );

   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ( [&]() {
         a.set_amount(asset_max + 1);
      })
   );

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
   REQUIRE_EQUAL( (asset{0LL, s4}.to_string()),
                  "0. SYMBOLL" );
   REQUIRE_EQUAL( (asset{-0LL, s4}.to_string()),
                  "0. SYMBOLL" );
   REQUIRE_EQUAL( (asset{0LL, s5}.to_string()),
                  "0.000000000000000000000000000000000000000000000000000000000000000 SYMBOLL" );
   REQUIRE_EQUAL( (asset{-0LL, s5}.to_string()),
                  "0.000000000000000000000000000000000000000000000000000000000000000 SYMBOLL" );

   REQUIRE_EQUAL( (asset{1LL, s4}.to_string()),
                  "1. SYMBOLL" );
   REQUIRE_EQUAL( (asset{-1LL, s4}.to_string()),
                  "--1. SYMBOLL" );
   REQUIRE_EQUAL( (asset{1LL, s5}.to_string()),
                  "0.000000000000000000000000000000000000000000000000000000000000001 SYMBOLL" );
   REQUIRE_EQUAL( (asset{-1LL, s5}.to_string()),
                  "-0.000000000000000000000000000000000000000000000000000000000000001 SYMBOLL" );

   REQUIRE_EQUAL( (asset{asset_min, s4}.to_string()),
                  "--4611686018427387903. SYMBOLL" );
   REQUIRE_EQUAL( (asset{asset_max, s4}.to_string()),
                  "4611686018427387903. SYMBOLL" );
   REQUIRE_EQUAL( (asset{asset_min, s5}.to_string()),
                  "-0.000000000000000000000000000000000000000000004611686018427387903 SYMBOLL" );
   REQUIRE_EQUAL( (asset{asset_max, s5}.to_string()),
                  "0.000000000000000000000000000000000000000000004611686018427387903 SYMBOLL" );
   
   // How are these `assets` constructed? Isn't `uint8_t precision` supposed to be constexpr when constructing a symbol?
   for( uint8_t precision{0}; precision < 64; ++precision ) {
      REQUIRE_EQUAL( (asset{0LL, symbol{"SYMBOLL", precision}}.to_string()),
                     (std::string(std::string("0.") + std::string(precision, '0') + std::string(" SYMBOLL"))) );
   }

   // ----------------------
   // asset operator-()const
   REQUIRE_EQUAL( (-asset{ 0LL, s4}.amount), (asset{0LL, s4}.amount) );
   REQUIRE_EQUAL( (-asset{-0LL, s4}.amount), (asset{0LL, s4}.amount) );
   REQUIRE_EQUAL( (-asset{ 0LL, s5}.amount), (asset{0LL, s5}.amount) );
   REQUIRE_EQUAL( (-asset{-0LL, s5}.amount), (asset{0LL, s5}.amount) );

   REQUIRE_EQUAL( (-asset{ 1LL, s4}.amount), (asset{-1LL, s4}.amount) );
   REQUIRE_EQUAL( (-asset{-1LL, s4}.amount), (asset{ 1LL, s4}.amount) );
   REQUIRE_EQUAL( (-asset{ 1LL, s5}.amount), (asset{-1LL, s5}.amount) );
   REQUIRE_EQUAL( (-asset{-1LL, s5}.amount), (asset{ 1LL, s5}.amount) );

   REQUIRE_EQUAL( (-asset{asset_min, symbol{"SYMBOLL",  0}}.amount),
                  ( asset{asset_max, symbol{"SYMBOLL",  0}}.amount) );

   REQUIRE_EQUAL( (-asset{asset_max, symbol{"SYMBOLL",  0}}.amount),
                  ( asset{asset_min, symbol{"SYMBOLL",  0}}.amount) );

   REQUIRE_EQUAL( (-asset{asset_min, symbol{"SYMBOLL", 63}}.amount),
                  ( asset{asset_max, symbol{"SYMBOLL", 63}}.amount) );

   REQUIRE_EQUAL( (-asset{asset_max, symbol{"SYMBOLL", 63}}.amount),
                  ( asset{asset_min, symbol{"SYMBOLL", 63}}.amount) );

   // ------------------------------------------------------------------------------------------
   // inline friend asset& operator+(const asset&, const asset&)/asset& operator+=(const asset&)
   REQUIRE_EQUAL( (asset{0LL, s4} += asset{0LL, s4} ),
                  (asset{0LL, s4}) );

   REQUIRE_EQUAL( (asset{1LL, s4} += asset{-1LL, s4} ),
                  (asset{0LL, s4}) );

   REQUIRE_ASSERT( "attempt to add asset with different symbol", ( [&]() {
         asset{1LL, s4} += asset{1LL, symbol{"LLOBMYS", 0}};
      })
   );

   REQUIRE_ASSERT( "addition underflow", ( [&]() {
         asset{asset_min, s4} += -asset{1LL, s4};
      })
   );

   REQUIRE_ASSERT( "addition overflow", ( [&]() {
         asset{asset_max, s4} +=  asset{1LL, s4};
      })
   );
           
   // ------------------------------------------------------------------------------------------
   // inline friend asset& operator-(const asset&, const asset&)/asset& operator-=(const asset&)
   REQUIRE_EQUAL( (asset{0LL, s4} -= asset{0LL, s4} ), (asset{0LL, s4}) );
   REQUIRE_EQUAL( (asset{1LL, s4} -= asset{1LL, s4} ), (asset{0LL, s4}) );

   REQUIRE_ASSERT( "attempt to subtract asset with different symbol", ( [&]() {
         asset{1LL, s4} -= asset{1LL, symbol{"LLOBMYS", 0}};
      })
   );

   REQUIRE_ASSERT( "subtraction underflow", ( [&]() {
         asset{asset_min, s4} -=  asset{1LL, s4};
      })
   );

   REQUIRE_ASSERT( "subtraction overflow", ( [&]() {
         asset{asset_max, s4} -= -asset{1LL, s4};
      })
   );

   // -----------------------------------------------------------------------
   // friend asset operator*(const asset&, int64_t)/asset operator*=(int64_t)
   REQUIRE_EQUAL( (asset{ 0LL, s4} *= 0 ), (asset{ 0LL, s4}) );
   REQUIRE_EQUAL( (asset{ 2LL, s4} *=  1LL ), (asset{ 2LL, s4}) );
   REQUIRE_EQUAL( (asset{ 2LL, s4} *= -1LL ), (asset{-2LL, s4}) );

   REQUIRE_ASSERT( "multiplication underflow", ( [&]() {
         asset{asset_min, s4} *= 2LL;
      })
   );

   REQUIRE_ASSERT( "multiplication overflow", ( [&]() {
         asset{ asset_max, s4} *= 2LL;
      })
   );

   // -----------------------------------------------------------------------------------------------------------------------------
   // friend asset operator/(const asset&, int64_t)/asset& operator/=(int64_t)/friend int64_t operator/(const asset&, const asset&)
   REQUIRE_EQUAL( (asset{ 0LL, s4} / asset{ 1LL, s4}.amount ), (asset{ 0LL, s4}) );
   REQUIRE_EQUAL( (asset{ 1LL, s4} / asset{ 1LL, s4}.amount ), (asset{ 1LL, s4}) );
   REQUIRE_EQUAL( (asset{ 4LL, s4} / asset{ 2LL, s4}.amount ), (asset{ 2LL, s4}) );
   REQUIRE_EQUAL( (asset{-4LL, s4} / asset{ 2LL, s4}.amount ), (asset{-2LL, s4}) );
   REQUIRE_EQUAL( (asset{-4LL, s4} / asset{-2LL, s4}.amount ), (asset{ 2LL, s4}) );

   REQUIRE_EQUAL( (asset{ 0LL, s4} /= asset{ 1LL, s4}.amount ), (asset{ 0LL, s4}) );
   REQUIRE_EQUAL( (asset{ 1LL, s4} /= asset{ 1LL, s4}.amount ), (asset{ 1LL, s4}) );
   REQUIRE_EQUAL( (asset{ 4LL, s4} /= asset{ 2LL, s4}.amount ), (asset{ 2LL, s4}) );
   REQUIRE_EQUAL( (asset{-4LL, s4} /= asset{ 2LL, s4}.amount ), (asset{-2LL, s4}) );
   REQUIRE_EQUAL( (asset{-4LL, s4} /= asset{-2LL, s4}.amount ), (asset{ 2LL, s4}) );

   REQUIRE_EQUAL( (asset{ 0LL, s4} / asset{ 1LL, s4} ), (asset{ 0LL, s4}.amount) );
   REQUIRE_EQUAL( (asset{ 1LL, s4} / asset{ 1LL, s4} ), (asset{ 1LL, s4}.amount) );
   REQUIRE_EQUAL( (asset{ 4LL, s4} / asset{ 2LL, s4} ), (asset{ 2LL, s4}.amount) );
   REQUIRE_EQUAL( (asset{-4LL, s4} / asset{ 2LL, s4} ), (asset{-2LL, s4}.amount) );
   REQUIRE_EQUAL( (asset{-4LL, s4} / asset{-2LL, s4} ), (asset{ 2LL, s4}.amount) );

   REQUIRE_ASSERT( "divide by zero", ( [&]() {asset{1LL, s4} /= 0;}) );

   // Note that there should be an invariant established here when adding or setting the `amount`
   REQUIRE_ASSERT( "signed division overflow", ( [&]() {
         asset a{};
         a.amount = std::numeric_limits<int64_t>::min();
         a /= -1LL;
      })
   );

   REQUIRE_ASSERT( "comparison of assets with different symbols is not allowed", ( [&]() {
         asset{{1LL}, s0} / asset{{1LL}, s1};
      })
   );

   // --------------------------------------------------
   // friend bool operator==(const asset&, const asset&)
   REQUIRE_EQUAL( (asset{{}, s4} == asset{{}, s4} ), true );
   REQUIRE_EQUAL( (asset{asset_min, s4} == asset{asset_min, s4} ), true );
   REQUIRE_EQUAL( (asset{asset_max, s4} == asset{asset_max, s4} ), true );

   REQUIRE_ASSERT( "comparison of assets with different symbols is not allowed", ( []() {
      asset{{}, symbol{"SYMBOLL", 0}} == asset{{}, symbol{"LLOBMYS", 0}};
      })
   );

   // ---------------------------------------------------
   // friend bool operator!=( const asset&, const asset&)
   REQUIRE_EQUAL( (asset{asset_min, s4} != -asset{asset_min, s4} ), true );
   REQUIRE_EQUAL( (asset{asset_max, s4} != -asset{asset_max, s4} ), true );

   // -------------------------------------------------
   // friend bool operator<(const asset&, const asset&)
   REQUIRE_EQUAL( (asset{{}, s4} < asset{1LL, s4} ), true );

   // --------------------------------------------------
   // friend bool operator<=(const asset&, const asset&)
   REQUIRE_EQUAL( ( asset{{},  s4} <= asset{1LL, s4} ), true );
   REQUIRE_EQUAL( ( asset{1LL, s4} <= asset{1LL, s4} ), true );

   // -------------------------------------------------
   // friend bool operator>(const asset&, const asset&)
   REQUIRE_EQUAL( ( asset{1LL, s4} > asset{{}, s4} ), true );
   REQUIRE_EQUAL( ( asset{{}, s4}  > asset{1LL, s4} ), false );

   // --------------------------------------------------
   // friend bool operator>=( const asset&, const asset&)
   REQUIRE_EQUAL( ( asset{1LL, s4} >= asset{{},  s4} ), true );
   REQUIRE_EQUAL( ( asset{1LL, s4} >= asset{1LL, s4} ), true );

   silence_output(false);
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/asset.hpp`
EOSIO_TEST_BEGIN(extended_asset_type_test)
   silence_output(false);

   name n0{"1"};
   name n1{"5"};
   name n2{"a"};
   name n3{"z"};
   name n4{"111111111111j"};
   name n5{"555555555555j"};
   name n6{"aaaaaaaaaaaaj"};
   name n7{"zzzzzzzzzzzzj"};
   name n8{"eosioaccountj"};

   symbol s0{"A", 0};
   symbol s1{"Z", 0};
   symbol s2{"AAAAAAA", 0};
   symbol s3{"ZZZZZZZ", 0};
   symbol s4{"SYMBOLL", 0};
   symbol s5{"SYMBOLL", 63};

   extended_symbol es0{s4, n8};

   asset a0{0LL, s0};
   asset a1{0LL, s0};
   asset a2{0LL, s1};
   asset a3{0LL, s1};
   asset a4{0LL, s2};
   asset a5{0LL, s2};
   asset a6{0LL, s3};
   asset a7{0LL, s3};
   asset a8{0LL, s4};

   // ------------
   // constructors

   /// extended_asset()
   REQUIRE_EQUAL( extended_asset{}.quantity, asset{} );
   REQUIRE_EQUAL( extended_asset{}.contract, name{} );

    /// extended_asset(int64_t, extended_symbol)
   REQUIRE_EQUAL( (extended_asset{{}, es0}.quantity), (asset{{}, s4}) );
   REQUIRE_EQUAL( (extended_asset{{}, es0}.contract), n8);

   /// extended_asset(asset, name)
   REQUIRE_EQUAL( (extended_asset{a8, n8}.quantity), (asset{{}, s4}) );
   REQUIRE_EQUAL( (extended_asset{a8, n8}.contract), (n8) );

   // // ------------------------------------------
   // // extended_symbol get_extended_symbol()const
   // REQUIRE_EQUAL( (extended_asset{{}, {}}.get_extended_symbol()), extended_symbol{} );

   // REQUIRE_EQUAL( (extended_asset{asset{{}, s0}, n0}.get_extended_symbol()),
   //                (extended_symbol{symbol{"A", 0}, name{"1"}}) );
   // REQUIRE_EQUAL( (extended_asset{asset{{}, s0}, n1}.get_extended_symbol()),
   //                (extended_symbol{symbol{"A", 0}, name{"5"}}) );
   // REQUIRE_EQUAL( (extended_asset{asset{{}, s1}, n2}.get_extended_symbol()),
   //                (extended_symbol{symbol{"Z", 0}, name{"a"}}) );
   // REQUIRE_EQUAL( (extended_asset{asset{{}, s1}, n3}.get_extended_symbol()),
   //                (extended_symbol{symbol{"Z", 0}, name{"z"}}) );

   // REQUIRE_EQUAL( (extended_asset{asset{{}, s2}, n4}.get_extended_symbol()),
   //                (extended_symbol{s2, n4}) );
   // REQUIRE_EQUAL( (extended_asset{asset{{}, s2}, n5}.get_extended_symbol()),
   //                (extended_symbol{s2, n5}) );
   // REQUIRE_EQUAL( (extended_asset{asset{{}, symbol{"ZZZZZZZ", 0}}, n6}.get_extended_symbol()),
   //                (extended_symbol{symbol{"ZZZZZZZ", 0}, n6}) );
   // REQUIRE_EQUAL( (extended_asset{asset{{}, symbol{"ZZZZZZZ", 0}}, n7}.get_extended_symbol()),
   //                (extended_symbol{symbol{"ZZZZZZZ", 0}, n7}) );


//    // Fix this printing functionality
//    // Note that if there is no precision, there will be odd output:
//    // `extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"1"}}.print();`
//    // output: "0. A@1"
//    // -----------------
//    // void print()const
// ///////////////////////////////////////////////////////////////////////////////////////////////////
// // How is this not the correct print result??
// // print_f("0. A@1");
// // print_f("\n");;
// // extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"1"}}.print();
// // print_f("\n");;
// //    REQUIRE_PRINT( "0. A@1", [](){extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"1"}}.print();} );
// //    REQUIRE_PRINT( "0. A@5", [](){extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"5"}}.print();} );
// //    REQUIRE_PRINT( "0. Z@a", [](){extended_asset{asset{int64_t{0}, symbol{"Z", 0}}, name{"a"}}.print();} );
// //    REQUIRE_PRINT( "0. Z@z", [](){extended_asset{asset{int64_t{0}, symbol{"Z", 0}}, name{"z"}}.print();} );

// //    REQUIRE_PRINT( "0. AAAAAAA@111111111111j", [](){extended_asset{asset{int64_t{0}, symbol{"AAAAAAA", 0}}, name{"111111111111j"}}.print();} );
// //    REQUIRE_PRINT( "0. AAAAAAA@555555555555j", [](){extended_asset{asset{int64_t{0}, symbol{"AAAAAAA", 0}}, name{"555555555555j"}}.print();} );
// //    REQUIRE_PRINT( "0. ZZZZZZZ@aaaaaaaaaaaaj", [](){extended_asset{asset{int64_t{0}, symbol{"ZZZZZZZ", 0}}, name{"aaaaaaaaaaaaj"}}.print();} );
// //    REQUIRE_PRINT( "0. ZZZZZZZ@zzzzzzzzzzzzj", [](){extended_asset{asset{int64_t{0}, symbol{"ZZZZZZZ", 0}}, name{"zzzzzzzzzzzzj"}}.print();} );
// ///////////////////////////////////////////////////////////////////////////////////////////////////

//    // -------------------------------
//    // extended_asset operator-()const
//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{-0}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );
//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{-0}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );

//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{1LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{-1LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{1LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{-1LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{1LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{-1LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );
//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{1LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{-1LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );

//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 0}}, name{}}.quantity) );
//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );
//    REQUIRE_EQUAL( (-extended_asset{asset{int64_t{4611686018427387903LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity),
//                   (extended_asset{asset{int64_t{-4611686018427387903LL}, symbol{"SYMBOLL", 63}}, name{}}.quantity) );
   

//    // -----------------------------------------------------------------------------
//    // friend extended_asset operator+(const extended_asset&, const extended_asset&)
//    extended_asset asset0{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}};
//    extended_asset asset1{asset{int64_t{1LL}, symbol{"SYMBOLL", 0}}, name{}};
//    extended_asset asset2{asset{int64_t{-1LL}, symbol{"SYMBOLL", 0}}, name{}};

//    extended_asset asset3{asset{int64_t{}, symbol{"SYMBOLL", 0}}, name{"eosioaccountj"}};
//    extended_asset asset4{asset{int64_t{}, symbol{"SYMBOLL", 0}}, name{"jtnuoccaoisoe"}};

//    REQUIRE_EQUAL( (asset0 + asset0), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );
//    REQUIRE_EQUAL( (asset1 + asset2), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );

//    REQUIRE_ASSERT( "type mismatch", (
//       [&]() {asset3 + asset4;}) );

//    // -------------------------------------------------------------------------
//    // friend extended_asset& operator+=(extended_asset&, const extended_asset&)
//    REQUIRE_EQUAL( (asset0 += asset0), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );
//    REQUIRE_EQUAL( (asset1 += asset2), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );

//    REQUIRE_ASSERT( "type mismatch", (
//       [&]() {asset3 += asset4;}) );

//    // --------------------------------------------------------------------------
//    // friend extended_asset& operator-=( extended_asset&, const extended_asset&)
//    REQUIRE_EQUAL( (asset0 - asset0), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );
//    REQUIRE_EQUAL( (asset1 - asset1), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );

//    REQUIRE_ASSERT( "type mismatch", (
//       [&]() {asset3 - asset4;}) );

//    // ----------------------------------------------------------------------------
//    //friend extended_asset operator-(const extended_asset&, const extended_asset&)
//    REQUIRE_EQUAL( (asset0 -= asset0), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );
//    REQUIRE_EQUAL( (asset1 -= asset1), (extended_asset{asset{int64_t{0}, symbol{"SYMBOLL", 0}}, name{}}) );

//    REQUIRE_ASSERT( "type mismatch", (
//       [&]() {asset3 -= asset4;}) );

//    // --------------------------------------------------------------------
//    // friend bool operator==(const extended_asset&, const extended_asset&)
//    REQUIRE_EQUAL( (asset0 == asset0), true );
//    REQUIRE_EQUAL( (asset1 == asset1), true );

// ///////////////////////////////////////////////////////////////////////////////////////////////////
// // Why is this not throwing??
//    // REQUIRE_ASSERT( "type mismatch", (
//    //    [&]() {asset3 == asset4;}) );
// ///////////////////////////////////////////////////////////////////////////////////////////////////

//    // --------------------------------------------------------------------
//    // friend bool operator!=(const extended_asset&, const extended_asset&)
// // print_f("%", asset3 != asset4);
// // print_f("\n");
//    REQUIRE_EQUAL( asset3 != asset4, true );
//    // REQUIRE_EQUAL( (asset1 != asset2), false );
//    // REQUIRE_EQUAL( (asset2 != asset3), false );

//    // REQUIRE_ASSERT( "type mismatch", (
//    //    [&]() {asset0 != asset1;}) );

//    // --------------------------------------------------------------------
//    // friend bool operator<( const extended_asset&, const extended_asset&)
//    // ???
//    REQUIRE_EQUAL( asset0 < asset1, true );

//    // REQUIRE_ASSERT( "type mismatch", (
//    //    []() {extended_asset{asset{}, name{}};}) );

//    // --------------------------------------------------------------------
//    // friend bool operator<=(const extended_asset&, const extended_asset&)
//    // REQUIRE_EQUAL( asset0 <= asset1, true );

//    // --------------------------------------------------------------------
//    // friend bool operator>=(const extended_asset&, const extended_asset&)
//    // REQUIRE_EQUAL( asset1 > asset0, true );

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   // EOSIO_TEST(asset_type_test);
   EOSIO_TEST(extended_asset_type_test);
   return has_failed();
}
