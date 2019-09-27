/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <string>

#include <eosio/tester.hpp>
#include <eosio/asset.hpp>

using std::string;

using eosio::name;
using eosio::symbol;
using eosio::extended_symbol;
using eosio::asset;
using eosio::extended_asset;

static constexpr int64_t asset_mask{(1LL << 62) - 1};
static constexpr int64_t asset_min{-asset_mask}; // -4611686018427387903
static constexpr int64_t asset_max{ asset_mask}; //  4611686018427387903

// Definitions in `eosio.cdt/libraries/eosio/asset.hpp`
EOSIO_TEST_BEGIN(asset_type_test)
   static constexpr symbol s0{"A", 0};
   static constexpr symbol s1{"Z", 0};
   static constexpr symbol s2{"AAAAAAA", 0};
   static constexpr symbol s3{"ZZZZZZZ", 0};
   static constexpr symbol sym_no_prec{"SYMBOLL", 0}; // Symbol with no precision
   static constexpr symbol sym_prec{"SYMBOLL", 63};   // Symbol with precision

   //// constexpr asset()
   CHECK_EQUAL( asset{}.amount, 0ULL )
   CHECK_EQUAL( asset{}.symbol.raw(), 0ULL )

   //// constexpr asset(int64_t, symbol)
   CHECK_EQUAL( (asset{0LL, s0}.amount), 0LL )
   CHECK_EQUAL( (asset{asset_min, s0}.amount), asset_min )
   CHECK_EQUAL( (asset{asset_max, s0}.amount), asset_max )

   CHECK_EQUAL( (asset{0LL, s0}.symbol.raw()), 16640ULL ) // "A", precision: 0
   CHECK_EQUAL( (asset{0LL, s1}.symbol.raw()), 23040ULL ) // "Z", precision: 0
   CHECK_EQUAL( (asset{0LL, s2}.symbol.raw()), 4702111234474983680ULL ) // "AAAAAAA", precision: 0
   CHECK_EQUAL( (asset{0LL, s3}.symbol.raw()), 6510615555426900480ULL ) // "ZZZZZZZ", precision: 0

   // Note: there is an invariant established for `asset` that is not enforced for `symbol`
   // For example:
   // `symbol{};` // valid code
   // `asset{{}, symbol{}};` // throws "invalid symbol name"

   CHECK_ASSERT( "invalid symbol name", ([]() {asset{0LL, symbol{0LL}};}) )
   CHECK_ASSERT( "invalid symbol name", ([]() {asset{0LL, symbol{1LL}};}) )
   CHECK_ASSERT( "invalid symbol name", ([]() {asset{0LL, symbol{16639ULL}};}) )
   CHECK_ASSERT( "invalid symbol name", ([]() {asset{0LL, symbol{6510615555426900736ULL}};}) )

   CHECK_ASSERT( "magnitude of asset amount must be less than 2^62", ([]() {asset{ asset_min - 1, {}};}) )
   CHECK_ASSERT( "magnitude of asset amount must be less than 2^62", ([]() {asset{ asset_max + 1, {}};}) )

   // ----------------------------------
   // bool is_amount_within_range()const
   asset asset_check_amount{};
   asset_check_amount.amount = asset_min;
   CHECK_EQUAL( asset_check_amount.is_amount_within_range(), true )
   asset_check_amount.amount = asset_max;
   CHECK_EQUAL( asset_check_amount.is_amount_within_range(), true )

   asset_check_amount.amount = asset_min-1;
   CHECK_EQUAL( asset_check_amount.is_amount_within_range(), false )
   asset_check_amount.amount = asset_max+1;
   CHECK_EQUAL( asset_check_amount.is_amount_within_range(), false )

   // --------------------
   // bool is_valid()const
   asset asset_valid{};
   asset_valid.symbol = symbol{16640ULL}; // "A", precision: 0
   CHECK_EQUAL( asset_valid.is_valid(), true )
   asset_valid.symbol = symbol{23040ULL}; // "Z", precision: 0
   CHECK_EQUAL( asset_valid.is_valid(), true )
   asset_valid.symbol = symbol{4702111234474983680ULL};
   CHECK_EQUAL( asset_valid.is_valid(), true ) // "AAAAAAA", precision: 0
   asset_valid.symbol = symbol{6510615555426900480ULL};
   CHECK_EQUAL( asset_valid.is_valid(), true ) // "ZZZZZZZ", precision: 0

   asset_valid.symbol = symbol{16639ULL};
   CHECK_EQUAL( asset_valid.is_valid(), false )
   asset_valid.symbol = symbol{6510615555426900736ULL};
   CHECK_EQUAL( asset_valid.is_valid(), false )

   // ------------------------
   // void set_amount(int64_t)
   asset asset_set_amount{0LL, sym_no_prec};
   CHECK_EQUAL( (asset_set_amount.set_amount(0LL), asset_set_amount.amount), 0LL )
   CHECK_EQUAL( (asset_set_amount.set_amount(1LL), asset_set_amount.amount), 1LL )
   CHECK_EQUAL( (asset_set_amount.set_amount(asset_min), asset_set_amount.amount), asset_min )
   CHECK_EQUAL( (asset_set_amount.set_amount(asset_max), asset_set_amount.amount), asset_max )

   CHECK_ASSERT( "magnitude of asset amount must be less than 2^62", (
      [&]() {
         asset_set_amount.set_amount(asset_min - 1);
      })
   )

   CHECK_ASSERT( "magnitude of asset amount must be less than 2^62", (
      [&]() {
         asset_set_amount.set_amount(asset_max + 1);
      })
   )

   // ---------------------
   // std::to_string()const
   // Note:
   // Printing an `asset` is limited to a precision of 63
   // This will trigger an error:
   // `asset{int64_t{1LL}, symbol{"SYMBOLL", 64}}.print();` // output: "Floating point exception: ..."

   CHECK_EQUAL( (asset{ 0LL, sym_no_prec}.to_string()), "0 SYMBOLL" )
   CHECK_EQUAL( (asset{-0LL, sym_no_prec}.to_string()), "0 SYMBOLL" )
   CHECK_EQUAL( (asset{ 0LL, sym_prec}.to_string()),
                "0.000000000000000000000000000000000000000000000000000000000000000 SYMBOLL" )
   CHECK_EQUAL( (asset{-0LL, sym_prec}.to_string()),
                "0.000000000000000000000000000000000000000000000000000000000000000 SYMBOLL" )

   CHECK_EQUAL( (asset{ 1LL, sym_no_prec}.to_string()),  "1 SYMBOLL" )
   CHECK_EQUAL( (asset{-1LL, sym_no_prec}.to_string()), "-1 SYMBOLL" )
   CHECK_EQUAL( (asset{-1LL, symbol{"SYMBOLL", 1}}.to_string()), "-0.1 SYMBOLL" )
   CHECK_EQUAL( (asset{ 1LL, symbol{"SYMBOLL", 1}}.to_string()),  "0.1 SYMBOLL" )
   CHECK_EQUAL( (asset{-12LL, sym_no_prec}.to_string()), "-12 SYMBOLL" )
   CHECK_EQUAL( (asset{ 12LL, sym_no_prec}.to_string()),  "12 SYMBOLL" )
   CHECK_EQUAL( (asset{-123LL, sym_no_prec}.to_string()), "-123 SYMBOLL" )
   CHECK_EQUAL( (asset{ 123LL, sym_no_prec}.to_string()),  "123 SYMBOLL" )
   CHECK_EQUAL( (asset{-12LL, symbol{"SYMBOLL", 2}}.to_string()), "-0.12 SYMBOLL" )
   CHECK_EQUAL( (asset{ 12LL, symbol{"SYMBOLL", 2}}.to_string()),  "0.12 SYMBOLL" )
   CHECK_EQUAL( (asset{-12LL, symbol{"SYMBOLL", 1}}.to_string()), "-1.2 SYMBOLL" )
   CHECK_EQUAL( (asset{ 12LL, symbol{"SYMBOLL", 1}}.to_string()),  "1.2 SYMBOLL" )
   CHECK_EQUAL( (asset{-123LL, symbol{"SYMBOLL", 2}}.to_string()), "-1.23 SYMBOLL" )
   CHECK_EQUAL( (asset{ 123LL, symbol{"SYMBOLL", 2}}.to_string()),  "1.23 SYMBOLL" )
   CHECK_EQUAL( (asset{ 1LL, sym_prec}.to_string()),
                "0.000000000000000000000000000000000000000000000000000000000000001 SYMBOLL" )
   CHECK_EQUAL( (asset{-1LL, sym_prec}.to_string()),
                "-0.000000000000000000000000000000000000000000000000000000000000001 SYMBOLL" )

   CHECK_EQUAL( (asset{asset_min, sym_no_prec}.to_string()), "-4611686018427387903 SYMBOLL" )
   CHECK_EQUAL( (asset{asset_max, sym_no_prec}.to_string()),  "4611686018427387903 SYMBOLL" )
   CHECK_EQUAL( (asset{asset_min, symbol{"SYMBOLL", 2}}.to_string()), "-46116860184273879.03 SYMBOLL" )
   CHECK_EQUAL( (asset{asset_max, symbol{"SYMBOLL", 2}}.to_string()),  "46116860184273879.03 SYMBOLL" )
   CHECK_EQUAL( (asset{asset_min, sym_prec}.to_string()),
                "-0.000000000000000000000000000000000000000000004611686018427387903 SYMBOLL" )
   CHECK_EQUAL( (asset{asset_max, sym_prec}.to_string()),
                "0.000000000000000000000000000000000000000000004611686018427387903 SYMBOLL" )

   // -----------------
   // void print()const
    CHECK_PRINT( "0 SYMBOLL", [&](){asset{0LL, sym_no_prec}.print();} );
    CHECK_PRINT( "0 SYMBOLL", [&](){asset{-0LL, sym_no_prec}.print();} );
    CHECK_PRINT( "0.000000000000000000000000000000000000000000000000000000000000000 SYMBOLL", (
       [&]() {
          asset{0LL, sym_prec}.print();
       })
    )
    CHECK_PRINT( "0.000000000000000000000000000000000000000000000000000000000000000 SYMBOLL", (
       [&]() {
          asset{-0LL, sym_prec}.print();
       })
    )

    CHECK_PRINT(  "1 SYMBOLL", [&](){asset{1LL, sym_no_prec}.print();} );
    CHECK_PRINT( "-1 SYMBOLL", [&](){asset{-1LL, sym_no_prec}.print();} );
    CHECK_PRINT(  "0.000000000000000000000000000000000000000000000000000000000000001 SYMBOLL", (
       [&]() {
          asset{1LL, sym_prec}.print();
       })
    )
    CHECK_PRINT( "-0.000000000000000000000000000000000000000000000000000000000000001 SYMBOLL", (
       [&]() {
          asset{-1LL, sym_prec}.print();
       })
    )

    CHECK_PRINT( "-4611686018427387903 SYMBOLL", [&](){asset{asset_min, sym_no_prec}.print();} );
    CHECK_PRINT(  "4611686018427387903 SYMBOLL", [&](){asset{asset_max, sym_no_prec}.print();} );
    CHECK_PRINT(  "-0.000000000000000000000000000000000000000000004611686018427387903 SYMBOLL", (
       [&]() {
          asset{asset_min, sym_prec}.print();
       })
    )
    CHECK_PRINT( "0.000000000000000000000000000000000000000000004611686018427387903 SYMBOLL", (
       [&]() {
           asset{asset_max, sym_prec}.print();
       })
    )

   // Printing symbols at every level of precision, starting at a precision of `1`
   for( uint8_t precision{1}; precision < 64; ++precision ) {
      CHECK_EQUAL( (asset{0LL, symbol{"SYMBOLL", precision}}.to_string()),
                   (std::string(std::string("0.") + std::string(precision, '0') + std::string(" SYMBOLL"))) )
   }

   // ----------------------
   // asset operator-()const
   CHECK_EQUAL( (-asset{ 0LL, sym_no_prec}.amount), (asset{0LL, sym_no_prec}.amount) )
   CHECK_EQUAL( (-asset{-0LL, sym_no_prec}.amount), (asset{0LL, sym_no_prec}.amount) )
   CHECK_EQUAL( (-asset{ 0LL, sym_prec}.amount), (asset{0LL, sym_prec}.amount) )
   CHECK_EQUAL( (-asset{-0LL, sym_prec}.amount), (asset{0LL, sym_prec}.amount) )

   CHECK_EQUAL( (-asset{ 1LL, sym_no_prec}.amount), (asset{-1LL, sym_no_prec}.amount) )
   CHECK_EQUAL( (-asset{-1LL, sym_no_prec}.amount), (asset{ 1LL, sym_no_prec}.amount) )
   CHECK_EQUAL( (-asset{ 1LL, sym_prec}.amount), (asset{-1LL, sym_prec}.amount) )
   CHECK_EQUAL( (-asset{-1LL, sym_prec}.amount), (asset{ 1LL, sym_prec}.amount) )

   CHECK_EQUAL( (-asset{asset_min, sym_no_prec}.amount), (asset{asset_max, sym_no_prec}.amount) )
   CHECK_EQUAL( (-asset{asset_max, sym_no_prec}.amount), (asset{asset_min, sym_no_prec}.amount) )
   CHECK_EQUAL( (-asset{asset_min, sym_prec}.amount), (asset{asset_max, sym_prec}.amount) )
   CHECK_EQUAL( (-asset{asset_max, sym_prec}.amount), (asset{asset_min, sym_prec}.amount) )

   // ------------------------------------------------------------------------------------------
   // inline friend asset& operator+(const asset&, const asset&)/asset& operator+=(const asset&)
   CHECK_EQUAL( (asset{0LL, sym_no_prec} += asset{ 0LL, sym_no_prec}), (asset{0LL, sym_no_prec}) )
   CHECK_EQUAL( (asset{1LL, sym_no_prec} += asset{-1LL, sym_no_prec}), (asset{0LL, sym_no_prec}) )

   CHECK_ASSERT( "attempt to add asset with different symbol", (
      [&]() {
         asset{1LL, sym_no_prec} += asset{1LL, symbol{"LLOBMYS", 0}};
      })
   )

   CHECK_ASSERT( "addition underflow", (
      [&]() {
         asset{asset_min, sym_no_prec} += -asset{1LL, sym_no_prec};
      })
   )

   CHECK_ASSERT( "addition overflow", (
      [&]() {
         asset{asset_max, sym_no_prec} +=  asset{1LL, sym_no_prec};
      })
   )

   // ------------------------------------------------------------------------------------------
   // inline friend asset& operator-(const asset&, const asset&)/asset& operator-=(const asset&)
   CHECK_EQUAL( (asset{0LL, sym_no_prec} -= asset{0LL, sym_no_prec} ), (asset{0LL, sym_no_prec}) )
   CHECK_EQUAL( (asset{1LL, sym_no_prec} -= asset{1LL, sym_no_prec} ), (asset{0LL, sym_no_prec}) )

   CHECK_ASSERT( "attempt to subtract asset with different symbol", (
      [&]() {
         asset{1LL, sym_no_prec} -= asset{1LL, symbol{"LLOBMYS", 0}};
      })
   )

   CHECK_ASSERT( "subtraction underflow", (
      [&]() {
         asset{asset_min, sym_no_prec} -=  asset{1LL, sym_no_prec};
      })
   )

   CHECK_ASSERT( "subtraction overflow", (
      [&]() {
         asset{asset_max, sym_no_prec} -= -asset{1LL, sym_no_prec};
      })
   )

   // -----------------------------------------------------------------------
   // friend asset operator*(const asset&, int64_t)/asset operator*=(int64_t)
   CHECK_EQUAL( (asset{ 0LL, sym_no_prec} *=  0LL ), (asset{ 0LL, sym_no_prec}) );
   CHECK_EQUAL( (asset{ 2LL, sym_no_prec} *=  1LL ), (asset{ 2LL, sym_no_prec}) );
   CHECK_EQUAL( (asset{ 2LL, sym_no_prec} *= -1LL ), (asset{-2LL, sym_no_prec}) );

   CHECK_ASSERT( "multiplication underflow", (
      [&]() {
         asset{asset_min, sym_no_prec} *= 2LL;
      })
   )

   CHECK_ASSERT( "multiplication overflow", (
      [&]() {
         asset{ asset_max, sym_no_prec} *= 2LL;
      })
   )

   // ---------------------------------------------
   // friend asset operator/(const asset&, int64_t)
   CHECK_EQUAL( (asset{ 0LL, sym_no_prec} / asset{ 1LL, sym_no_prec}.amount), (asset{ 0LL, sym_no_prec}) )
   CHECK_EQUAL( (asset{ 1LL, sym_no_prec} / asset{ 1LL, sym_no_prec}.amount), (asset{ 1LL, sym_no_prec}) )
   CHECK_EQUAL( (asset{ 4LL, sym_no_prec} / asset{ 2LL, sym_no_prec}.amount), (asset{ 2LL, sym_no_prec}) )
   CHECK_EQUAL( (asset{-4LL, sym_no_prec} / asset{ 2LL, sym_no_prec}.amount), (asset{-2LL, sym_no_prec}) )
   CHECK_EQUAL( (asset{-4LL, sym_no_prec} / asset{-2LL, sym_no_prec}.amount), (asset{ 2LL, sym_no_prec}) )

   // ----------------------------------------------------
   // friend int64_t operator/(const asset&, const asset&)
   CHECK_EQUAL( (asset{ 0LL, sym_no_prec} / asset{ 1LL, sym_no_prec} ), (asset{ 0LL, sym_no_prec}.amount) )
   CHECK_EQUAL( (asset{ 1LL, sym_no_prec} / asset{ 1LL, sym_no_prec} ), (asset{ 1LL, sym_no_prec}.amount) )
   CHECK_EQUAL( (asset{ 4LL, sym_no_prec} / asset{ 2LL, sym_no_prec} ), (asset{ 2LL, sym_no_prec}.amount) )
   CHECK_EQUAL( (asset{-4LL, sym_no_prec} / asset{ 2LL, sym_no_prec} ), (asset{-2LL, sym_no_prec}.amount) )
   CHECK_EQUAL( (asset{-4LL, sym_no_prec} / asset{-2LL, sym_no_prec} ), (asset{ 2LL, sym_no_prec}.amount) )

   // ---------------------------------
   // friend asset& operator/=(int64_t)
   CHECK_EQUAL( (asset{ 0LL, sym_no_prec} /= asset{ 1LL, sym_no_prec}.amount), (asset{ 0LL, sym_no_prec}) )
   CHECK_EQUAL( (asset{ 1LL, sym_no_prec} /= asset{ 1LL, sym_no_prec}.amount), (asset{ 1LL, sym_no_prec}) )
   CHECK_EQUAL( (asset{ 4LL, sym_no_prec} /= asset{ 2LL, sym_no_prec}.amount), (asset{ 2LL, sym_no_prec}) )
   CHECK_EQUAL( (asset{-4LL, sym_no_prec} /= asset{ 2LL, sym_no_prec}.amount), (asset{-2LL, sym_no_prec}) )
   CHECK_EQUAL( (asset{-4LL, sym_no_prec} /= asset{-2LL, sym_no_prec}.amount), (asset{ 2LL, sym_no_prec}) )

   CHECK_ASSERT( "divide by zero", ([&]() {asset{1LL, sym_no_prec} /= 0;}) )

   // Note:
   // There is no invariant established here when adding or setting the `amount`
   CHECK_ASSERT( "signed division overflow", (
      []() {
         asset a{};
         a.amount = std::numeric_limits<int64_t>::min();
         a /= -1LL;
      })
   )

   CHECK_ASSERT( "comparison of assets with different symbols is not allowed", (
      [&]() {
         asset{1LL, s0} / asset{1LL, s1};
      })
   )

   // --------------------------------------------------
   // friend bool operator==(const asset&, const asset&)
   CHECK_EQUAL( (asset{0LL, sym_no_prec} == asset{0LL, sym_no_prec} ), true )
   CHECK_EQUAL( (asset{asset_min, sym_no_prec} == asset{asset_min, sym_no_prec} ), true )
   CHECK_EQUAL( (asset{asset_max, sym_no_prec} == asset{asset_max, sym_no_prec} ), true )

   CHECK_ASSERT( "comparison of assets with different symbols is not allowed", (
      []() {
         bool b{asset{{}, symbol{"SYMBOLL", 0}} == asset{{}, symbol{"LLOBMYS", 0}}};
         return b;
      })
   )

   // ---------------------------------------------------
   // friend bool operator!=( const asset&, const asset&)
   CHECK_EQUAL( (asset{asset_min, sym_no_prec} != -asset{asset_min, sym_no_prec} ), true )
   CHECK_EQUAL( (asset{asset_max, sym_no_prec} != -asset{asset_max, sym_no_prec} ), true )

   // -------------------------------------------------
   // friend bool operator<(const asset&, const asset&)
   CHECK_EQUAL( (asset{0LL, sym_no_prec} < asset{1LL, sym_no_prec} ), true )

   // --------------------------------------------------
   // friend bool operator<=(const asset&, const asset&)
   CHECK_EQUAL( ( asset{0LL, sym_no_prec} <= asset{1LL, sym_no_prec} ), true )
   CHECK_EQUAL( ( asset{1LL, sym_no_prec} <= asset{1LL, sym_no_prec} ), true )

   // -------------------------------------------------
   // friend bool operator>(const asset&, const asset&)
   CHECK_EQUAL( ( asset{1LL, sym_no_prec} > asset{0LL, sym_no_prec} ),  true  )
   CHECK_EQUAL( ( asset{0LL, sym_no_prec}  > asset{1LL, sym_no_prec} ), false )

   // --------------------------------------------------
   // friend bool operator>=( const asset&, const asset&)
   CHECK_EQUAL( ( asset{1LL, sym_no_prec} >= asset{0LL, sym_no_prec} ), true )
   CHECK_EQUAL( ( asset{1LL, sym_no_prec} >= asset{1LL, sym_no_prec} ), true )
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosio/asset.hpp`
EOSIO_TEST_BEGIN(extended_asset_type_test)
   static constexpr symbol sym_no_prec{"SYMBOLL",0};
   static constexpr symbol sym_prec{"SYMBOLL",63};

   static constexpr extended_symbol ext_sym_no_prec{sym_no_prec, name{"eosioaccountj"}};
   static constexpr extended_symbol ext_sym_prec{sym_prec, name{"eosioaccountj"}};

   static const asset asset_no_prec{0LL, sym_no_prec};
   static const asset asset_prec{0LL, sym_prec};

   //// extended_asset()
   CHECK_EQUAL( extended_asset{}.quantity, asset{} )
   CHECK_EQUAL( extended_asset{}.contract, name{}  )

   //// extended_asset(int64_t, extended_symbol)
   CHECK_EQUAL( (extended_asset{{},ext_sym_no_prec}.quantity), (asset{0LL, sym_no_prec}) )
   CHECK_EQUAL( (extended_asset{{},ext_sym_no_prec}.contract), (name{"eosioaccountj"}) )

   //// extended_asset(asset, name)
   CHECK_EQUAL( (extended_asset{asset_no_prec, name{"eosioaccountj"}}.quantity), (asset{ 0LL, sym_no_prec}) )
   CHECK_EQUAL( (extended_asset{asset_no_prec, name{"eosioaccountj"}}.contract), (name{"eosioaccountj"}) )

   // ------------------------------------------
   // extended_symbol get_extended_symbol()const
   CHECK_EQUAL( (extended_asset{{},ext_sym_no_prec}.get_extended_symbol()), (ext_sym_no_prec) )
   CHECK_EQUAL( (extended_asset{{},ext_sym_prec}.get_extended_symbol()), (ext_sym_prec) )

   // -----------------
   // void print()const
   CHECK_PRINT( "0 A@1", [](){extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"1"}}.print();} )
   CHECK_PRINT( "0 A@5", [](){extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"5"}}.print();} )
   CHECK_PRINT( "0 Z@a", [](){extended_asset{asset{int64_t{0}, symbol{"Z", 0}}, name{"a"}}.print();} )
   CHECK_PRINT( "0 Z@z", [](){extended_asset{asset{int64_t{0}, symbol{"Z", 0}}, name{"z"}}.print();} )

   CHECK_PRINT( "1.1 A@1", [](){extended_asset{asset{int64_t{11}, symbol{"A", 1}}, name{"1"}}.print();} )
   CHECK_PRINT( "1.1 A@5", [](){extended_asset{asset{int64_t{11}, symbol{"A", 1}}, name{"5"}}.print();} )
   CHECK_PRINT( "1.1 Z@a", [](){extended_asset{asset{int64_t{11}, symbol{"Z", 1}}, name{"a"}}.print();} )
   CHECK_PRINT( "1.1 Z@z", [](){extended_asset{asset{int64_t{11}, symbol{"Z", 1}}, name{"z"}}.print();} )

   CHECK_PRINT( "0.000000000000000000000000000000000000000000000000000000000000011 A@1",
      [](){extended_asset{asset{int64_t{11}, symbol{"A", 63}}, name{"1"}}.print();} )
   CHECK_PRINT( "0.000000000000000000000000000000000000000000000000000000000000011 A@5",
      [](){extended_asset{asset{int64_t{11}, symbol{"A", 63}}, name{"5"}}.print();} )
   CHECK_PRINT( "0.000000000000000000000000000000000000000000000000000000000000011 Z@a",
      [](){extended_asset{asset{int64_t{11}, symbol{"Z", 63}}, name{"a"}}.print();} )
   CHECK_PRINT( "0.000000000000000000000000000000000000000000000000000000000000011 Z@z",
      [](){extended_asset{asset{int64_t{11}, symbol{"Z", 63}}, name{"z"}}.print();} )

   CHECK_PRINT( "0 AAAAAAA@111111111111j", [](){extended_asset{asset{int64_t{0}, symbol{"AAAAAAA", 0}}, name{"111111111111j"}}.print();} )
   CHECK_PRINT( "0 AAAAAAA@555555555555j", [](){extended_asset{asset{int64_t{0}, symbol{"AAAAAAA", 0}}, name{"555555555555j"}}.print();} )
   CHECK_PRINT( "0 ZZZZZZZ@aaaaaaaaaaaaj", [](){extended_asset{asset{int64_t{0}, symbol{"ZZZZZZZ", 0}}, name{"aaaaaaaaaaaaj"}}.print();} )
   CHECK_PRINT( "0 ZZZZZZZ@zzzzzzzzzzzzj", [](){extended_asset{asset{int64_t{0}, symbol{"ZZZZZZZ", 0}}, name{"zzzzzzzzzzzzj"}}.print();} )

   CHECK_PRINT( "11 AAAAAAA@111111111111j", [](){extended_asset{asset{int64_t{11}, symbol{"AAAAAAA", 0}}, name{"111111111111j"}}.print();} )
   CHECK_PRINT( "11 AAAAAAA@555555555555j", [](){extended_asset{asset{int64_t{11}, symbol{"AAAAAAA", 0}}, name{"555555555555j"}}.print();} )
   CHECK_PRINT( "11 ZZZZZZZ@aaaaaaaaaaaaj", [](){extended_asset{asset{int64_t{11}, symbol{"ZZZZZZZ", 0}}, name{"aaaaaaaaaaaaj"}}.print();} )
   CHECK_PRINT( "11 ZZZZZZZ@zzzzzzzzzzzzj", [](){extended_asset{asset{int64_t{11}, symbol{"ZZZZZZZ", 0}}, name{"zzzzzzzzzzzzj"}}.print();} )

   CHECK_PRINT( "0.000000000000000000000000000000000000000000000000000000000000011 AAAAAAA@111111111111j",
     [](){extended_asset{asset{int64_t{11}, symbol{"AAAAAAA", 63}}, name{"111111111111j"}}.print();} )
   CHECK_PRINT( "0.000000000000000000000000000000000000000000000000000000000000011 AAAAAAA@555555555555j",
     [](){extended_asset{asset{int64_t{11}, symbol{"AAAAAAA", 63}}, name{"555555555555j"}}.print();} )
   CHECK_PRINT( "0.000000000000000000000000000000000000000000000000000000000000011 ZZZZZZZ@aaaaaaaaaaaaj",
     [](){extended_asset{asset{int64_t{11}, symbol{"ZZZZZZZ", 63}}, name{"aaaaaaaaaaaaj"}}.print();} )
   CHECK_PRINT( "0.000000000000000000000000000000000000000000000000000000000000011 ZZZZZZZ@zzzzzzzzzzzzj",
     [](){extended_asset{asset{int64_t{11}, symbol{"ZZZZZZZ", 63}}, name{"zzzzzzzzzzzzj"}}.print();} )

   // -------------------------------
   // extended_asset operator-()const
   CHECK_EQUAL( (-extended_asset{asset{ 0, sym_no_prec}, {}}.quantity), (extended_asset{asset_no_prec, {}}.quantity) )
   CHECK_EQUAL( (-extended_asset{asset{-0, sym_no_prec}, {}}.quantity), (extended_asset{asset_no_prec, {}}.quantity) )
   CHECK_EQUAL( (-extended_asset{asset{ 0, sym_prec}, {}}.quantity), (extended_asset{asset_prec, {}}.quantity) )
   CHECK_EQUAL( (-extended_asset{asset{-0, sym_prec}, {}}.quantity), (extended_asset{asset_prec, {}}.quantity) )

   CHECK_EQUAL( (-extended_asset{asset{1LL, sym_no_prec}, {}}.quantity), (extended_asset{asset{-1LL, sym_no_prec}, {}}.quantity) )
   CHECK_EQUAL( (-extended_asset{asset{1LL, sym_no_prec}, {}}.quantity), (extended_asset{asset{-1LL, sym_no_prec}, {}}.quantity) )
   CHECK_EQUAL( (-extended_asset{asset{1LL, sym_prec}, {}}.quantity), (extended_asset{asset{-1LL, sym_prec}, {}}.quantity) )
   CHECK_EQUAL( (-extended_asset{asset{1LL, sym_prec}, {}}.quantity), (extended_asset{asset{-1LL, sym_prec}, {}}.quantity) )

   CHECK_EQUAL( (-extended_asset{asset{asset_max, sym_no_prec}, {}}.quantity), (extended_asset{asset{asset_min, sym_no_prec}, {}}.quantity) )
   CHECK_EQUAL( (-extended_asset{asset{asset_max, sym_no_prec}, {}}.quantity), (extended_asset{asset{asset_min, sym_no_prec}, {}}.quantity) )
   CHECK_EQUAL( (-extended_asset{asset{asset_max, sym_prec}, {}}.quantity), (extended_asset{asset{asset_min, sym_prec}, {}}.quantity) )
   CHECK_EQUAL( (-extended_asset{asset{asset_max, sym_prec}, {}}.quantity), (extended_asset{asset{asset_min, sym_prec}, {}}.quantity) )

   // -----------------------------------------------------------------------------
   // friend extended_asset operator+(const extended_asset&, const extended_asset&)
   CHECK_EQUAL( (extended_asset{asset{0LL, sym_no_prec}, {}} + extended_asset{asset{ 0LL, sym_no_prec}, {}}), (extended_asset{asset_no_prec, {}}) )
   CHECK_EQUAL( (extended_asset{asset{1LL, sym_no_prec}, {}} + extended_asset{asset{-1LL, sym_no_prec}, {}}), (extended_asset{asset_no_prec, {}}) )

   CHECK_ASSERT( "type mismatch", (
      [&]() {
         extended_asset{asset_no_prec, name{"eosioaccountj"}} + extended_asset{asset_no_prec, name{"jtnuoccaoisoe"}};
      })
   )

   // -------------------------------------------------------------------------
   // friend extended_asset& operator+=(extended_asset&, const extended_asset&)
   extended_asset temp{asset_no_prec, {}};
   CHECK_EQUAL( (temp += temp), (extended_asset{asset_no_prec, {}}) )
   temp = extended_asset{asset{1LL, sym_no_prec}, {}};
   CHECK_EQUAL( (temp += extended_asset{asset{-1LL, sym_no_prec}, {}}), (extended_asset{asset_no_prec, {}}) )

   CHECK_ASSERT( "type mismatch", (
      [&]() {
         temp += extended_asset{asset_no_prec, name{"eosioaccountj"}};
      })
   )

   // -----------------------------------------------------------------------------
   // friend extended_asset operator-(const extended_asset&, const extended_asset&)
   CHECK_EQUAL( (extended_asset{asset_no_prec, {}} - extended_asset{asset_no_prec, {}}),
                  (extended_asset{asset_no_prec, {}}) )
   CHECK_EQUAL( (extended_asset{asset{1LL, sym_no_prec}, {}} - extended_asset{asset{1LL, sym_no_prec}, {}}),
                  (extended_asset{asset{asset_no_prec}, {}}) )

   CHECK_ASSERT( "type mismatch", (
      [&]() {
         extended_asset{asset_no_prec, name{"eosioaccountj"}} - extended_asset{asset_no_prec, name{"jtnuoccaoisoe"}};
      })
   )

   // --------------------------------------------------------------------------
   // friend extended_asset& operator-=(extended_asset&, const extended_asset&)
   temp = extended_asset{asset_no_prec, {}};
   CHECK_EQUAL( (temp -= temp), (extended_asset{asset_no_prec, {}}) )
   temp = extended_asset{asset{1LL, sym_no_prec}, {}};
   CHECK_EQUAL( (temp -= temp), (extended_asset{asset_no_prec, {}}) )

   CHECK_ASSERT( "type mismatch", (
      [&]() {
         temp -= extended_asset{asset_no_prec, name{"jtnuoccaoisoe"}};
      })
   )

   // --------------------------------------------------------------------
   // friend bool operator==(const extended_asset&, const extended_asset&)
   CHECK_EQUAL( (extended_asset{asset_no_prec, {}} == extended_asset{asset_no_prec, {}}), true )
   CHECK_EQUAL( (extended_asset{asset{1LL, sym_no_prec}, {}} == extended_asset{asset{1LL, sym_no_prec}, {}}), true )

   // --------------------------------------------------------------------
   // friend bool operator!=(const extended_asset&, const extended_asset&)
   CHECK_EQUAL( (extended_asset{asset_no_prec, name{"eosioaccountj"}} != extended_asset{asset_no_prec, name{"jtnuoccaoisoe"}}), true )
   CHECK_EQUAL( (extended_asset{asset{1LL, sym_no_prec}, {}} != extended_asset{asset{-1LL, sym_no_prec}, {}}), true )
   CHECK_EQUAL( (extended_asset{asset{1LL, sym_no_prec}, {}} != extended_asset{asset{ 0LL, sym_no_prec}, name{"eosioaccountj"}}), true )

   // -------------------------------------------------------------------
   // friend bool operator<(const extended_asset&, const extended_asset&)
   CHECK_EQUAL( (extended_asset{asset_no_prec, name{}} < extended_asset{asset{ 1LL, sym_no_prec}, {}}), true )
   CHECK_ASSERT( "type mismatch", (
      [&]() {
         bool b{extended_asset{{}, name{}} < extended_asset{{}, name{"eosioaccountj"}}};
         return b;
      })
   )

   // --------------------------------------------------------------------
   // friend bool operator<=(const extended_asset&, const extended_asset&)
   CHECK_EQUAL( (extended_asset{asset_no_prec, name{}} <= extended_asset{asset{ 1LL, sym_no_prec}, {}}), true );
   CHECK_ASSERT( "type mismatch", (
      [&]() {
         bool b{extended_asset{{}, name{}} <= extended_asset{{}, name{"eosioaccountj"}}};
         return b;
      })
   )

   // --------------------------------------------------------------------
   // friend bool operator>=(const extended_asset&, const extended_asset&)
   CHECK_EQUAL( (extended_asset{asset{ 1LL, sym_no_prec}, {}} >= extended_asset{asset_no_prec, name{}}), true );
   CHECK_ASSERT( "type mismatch", (
      [&]() {
         bool b{extended_asset{{}, name{}} >= extended_asset{{}, name{"eosioaccountj"}}};
         return b;
      })
   )
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   bool verbose = false;
   if( argc >= 2 && std::strcmp( argv[1], "-v" ) == 0 ) {
      verbose = true;
   }
   silence_output(!verbose);

   EOSIO_TEST(asset_type_test);
   EOSIO_TEST(extended_asset_type_test);
   return has_failed();
}
