#include <eosio/native/tester.hpp>
#include <eosiolib/asset.hpp>

using eosio::name;
using eosio::symbol;
using eosio::extended_symbol;
using eosio::asset;
using eosio::extended_asset;

const int64_t asset_mask{(1LL << 62) - 1};
const int64_t asset_min{-asset_mask}; // -4611686018427387903
const int64_t asset_max{ asset_mask}; //  4611686018427387903

// For incrementing the `precision` in any test relating to `precision`
// The `precision` field in the `symbol` type must be constructed via constexpr,
// therefore a constexpr function is needed to dynamically test `precision`
constexpr uint8_t increment_precision( uint8_t precision ) { return ++precision; }

// Defined in `eosio.cdt/libraries/eosiolib/asset.hpp`
EOSIO_TEST_BEGIN(asset_type_test)
   silence_output(true);

   symbol s0{"A", 0};
   symbol s1{"Z", 0};
   symbol s2{"AAAAAAA", 0};
   symbol s3{"ZZZZZZZ", 0};
   symbol sym_no_prec{"SYMBOLL", 0}; // Symbol with no precision
   symbol sym_prec{"SYMBOLL", 63};   // Symbol with precision

   // --------------------------------------------------------------------
   // constructors/bool is_amount_within_range()const/bool is_valid()const

   /// constexpr asset()
   REQUIRE_EQUAL( asset{}.amount, 0ULL )
   REQUIRE_EQUAL( asset{}.symbol.raw(), 0ULL )
      
   /// constexpr asset(int64_t, symbol)
   REQUIRE_EQUAL( (asset{0LL, s0}.amount), 0LL )
   REQUIRE_EQUAL( (asset{asset_min, s0}.amount), asset_min )
   REQUIRE_EQUAL( (asset{asset_max, s0}.amount), asset_max )

   REQUIRE_EQUAL( (asset{0LL, s0}.symbol.raw()), 16640ULL ) // "A", precision: 0
   REQUIRE_EQUAL( (asset{0LL, s1}.symbol.raw()), 23040ULL ) // "Z", precision: 0
   REQUIRE_EQUAL( (asset{0LL, s2}.symbol.raw()), 4702111234474983680ULL ) // "AAAAAAA", precision: 0
   REQUIRE_EQUAL( (asset{0LL, s3}.symbol.raw()), 6510615555426900480ULL ) // "ZZZZZZZ", precision: 0
   
   // There is an invariant established for `asset` that isn't enforced with `symbol`
   // For example:
   // valid code:
   // `symbol{};`
   // throws "invalid symbol name":
   // `asset{{}, symbol{}};`
   REQUIRE_ASSERT( "invalid symbol name", ([]() {asset{0LL, symbol{0LL}};}) )
   REQUIRE_ASSERT( "invalid symbol name", ([]() {asset{0LL, symbol{1LL}};}) )
   REQUIRE_ASSERT( "invalid symbol name", ([]() {asset{0LL, symbol{16639ULL}};}) )
   REQUIRE_ASSERT( "invalid symbol name", ([]() {asset{0LL, symbol{6510615555426900736ULL}};}) )

   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ([]() {asset{ asset_min - 1, {}};}) )
   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ([]() {asset{ asset_max + 1, {}};}) )

   // ------------------------
   // void set_amount(int64_t)
   asset a{0LL, sym_no_prec};
   REQUIRE_EQUAL( (a.set_amount(0LL), a.amount), 0LL )
   REQUIRE_EQUAL( (a.set_amount(1LL), a.amount), 1LL )
   REQUIRE_EQUAL( (a.set_amount(asset_min), a.amount), asset_min )
   REQUIRE_EQUAL( (a.set_amount(asset_max), a.amount), asset_max )
   
   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ([&]() {
         a.set_amount(asset_min - 1);
      })
   )

   REQUIRE_ASSERT( "magnitude of asset amount must be less than 2^62", ([&]() {
         a.set_amount(asset_max + 1);
      })
   )

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
   REQUIRE_EQUAL( (asset{0LL, sym_no_prec}.to_string()),
                  "0. SYMBOLL" )
   REQUIRE_EQUAL( (asset{-0LL, sym_no_prec}.to_string()),
                  "0. SYMBOLL" )
   REQUIRE_EQUAL( (asset{0LL, sym_prec}.to_string()),
                  "0.000000000000000000000000000000000000000000000000000000000000000 SYMBOLL" )
   REQUIRE_EQUAL( (asset{-0LL, sym_prec}.to_string()),
                  "0.000000000000000000000000000000000000000000000000000000000000000 SYMBOLL" )

   REQUIRE_EQUAL( (asset{1LL, sym_no_prec}.to_string()),
                  "1. SYMBOLL" )
   REQUIRE_EQUAL( (asset{-1LL, sym_no_prec}.to_string()),
                  "--1. SYMBOLL" )
   REQUIRE_EQUAL( (asset{1LL, sym_prec}.to_string()),
                  "0.000000000000000000000000000000000000000000000000000000000000001 SYMBOLL" )
   REQUIRE_EQUAL( (asset{-1LL, sym_prec}.to_string()),
                  "-0.000000000000000000000000000000000000000000000000000000000000001 SYMBOLL" )

   REQUIRE_EQUAL( (asset{asset_min, sym_no_prec}.to_string()),
                  "--4611686018427387903. SYMBOLL" )
   REQUIRE_EQUAL( (asset{asset_max, sym_no_prec}.to_string()),
                  "4611686018427387903. SYMBOLL" )
   REQUIRE_EQUAL( (asset{asset_min, sym_prec}.to_string()),
                  "-0.000000000000000000000000000000000000000000004611686018427387903 SYMBOLL" )
   REQUIRE_EQUAL( (asset{asset_max, sym_prec}.to_string()),
                  "0.000000000000000000000000000000000000000000004611686018427387903 SYMBOLL" )
   
   // How are these `assets` constructed? Isn't `uint8_t precision` supposed to be constexpr when constructing a symbol?
   for( uint8_t precision{0}; precision < 64; ++precision ) {
      REQUIRE_EQUAL( (asset{0LL, symbol{"SYMBOLL", precision}}.to_string()),
                     (std::string(std::string("0.") + std::string(precision, '0') + std::string(" SYMBOLL"))) )
   }

   // ----------------------
   // asset operator-()const
   REQUIRE_EQUAL( (-asset{ 0LL, sym_no_prec}.amount), (asset{0LL, sym_no_prec}.amount) )
   REQUIRE_EQUAL( (-asset{-0LL, sym_no_prec}.amount), (asset{0LL, sym_no_prec}.amount) )
   REQUIRE_EQUAL( (-asset{ 0LL, sym_prec}.amount), (asset{0LL, sym_prec}.amount) )
   REQUIRE_EQUAL( (-asset{-0LL, sym_prec}.amount), (asset{0LL, sym_prec}.amount) )

   REQUIRE_EQUAL( (-asset{ 1LL, sym_no_prec}.amount), (asset{-1LL, sym_no_prec}.amount) )
   REQUIRE_EQUAL( (-asset{-1LL, sym_no_prec}.amount), (asset{ 1LL, sym_no_prec}.amount) )
   REQUIRE_EQUAL( (-asset{ 1LL, sym_prec}.amount), (asset{-1LL, sym_prec}.amount) )
   REQUIRE_EQUAL( (-asset{-1LL, sym_prec}.amount), (asset{ 1LL, sym_prec}.amount) )

   REQUIRE_EQUAL( (-asset{asset_min, sym_no_prec}.amount), (asset{asset_max, sym_no_prec}.amount) )
   REQUIRE_EQUAL( (-asset{asset_max, sym_no_prec}.amount), (asset{asset_min, sym_no_prec}.amount) )
   REQUIRE_EQUAL( (-asset{asset_min, sym_prec}.amount), (asset{asset_max, sym_prec}.amount) )
   REQUIRE_EQUAL( (-asset{asset_max, sym_prec}.amount), (asset{asset_min, sym_prec}.amount) )

   // ------------------------------------------------------------------------------------------
   // inline friend asset& operator+(const asset&, const asset&)/asset& operator+=(const asset&)
   REQUIRE_EQUAL( (asset{0LL, sym_no_prec} += asset{ 0LL, sym_no_prec} ), (asset{0LL, sym_no_prec}) )
   REQUIRE_EQUAL( (asset{1LL, sym_no_prec} += asset{-1LL, sym_no_prec} ), (asset{0LL, sym_no_prec}) )

   REQUIRE_ASSERT( "attempt to add asset with different symbol", ([&]() {
         asset{1LL, sym_no_prec} += asset{1LL, symbol{"LLOBMYS", 0}};
      })
   )

   REQUIRE_ASSERT( "addition underflow", ([&]() {
         asset{asset_min, sym_no_prec} += -asset{1LL, sym_no_prec};
      })
   )

   REQUIRE_ASSERT( "addition overflow", ([&]() {
         asset{asset_max, sym_no_prec} +=  asset{1LL, sym_no_prec};
      })
   )
           
   // ------------------------------------------------------------------------------------------
   // inline friend asset& operator-(const asset&, const asset&)/asset& operator-=(const asset&)
   REQUIRE_EQUAL( (asset{0LL, sym_no_prec} -= asset{0LL, sym_no_prec} ), (asset{0LL, sym_no_prec}) )
   REQUIRE_EQUAL( (asset{1LL, sym_no_prec} -= asset{1LL, sym_no_prec} ), (asset{0LL, sym_no_prec}) )

   REQUIRE_ASSERT( "attempt to subtract asset with different symbol", ([&]() {
         asset{1LL, sym_no_prec} -= asset{1LL, symbol{"LLOBMYS", 0}};
      })
   )

   REQUIRE_ASSERT( "subtraction underflow", ([&]() {
         asset{asset_min, sym_no_prec} -=  asset{1LL, sym_no_prec};
      })
   )

   REQUIRE_ASSERT( "subtraction overflow", ([&]() {
         asset{asset_max, sym_no_prec} -= -asset{1LL, sym_no_prec};
      })
   )

   // -----------------------------------------------------------------------
   // friend asset operator*(const asset&, int64_t)/asset operator*=(int64_t)
   REQUIRE_EQUAL( (asset{ 0LL, sym_no_prec} *=  0LL ), (asset{ 0LL, sym_no_prec}) );
   REQUIRE_EQUAL( (asset{ 2LL, sym_no_prec} *=  1LL ), (asset{ 2LL, sym_no_prec}) );
   REQUIRE_EQUAL( (asset{ 2LL, sym_no_prec} *= -1LL ), (asset{-2LL, sym_no_prec}) );

   REQUIRE_ASSERT( "multiplication underflow", ([&]() {
         asset{asset_min, sym_no_prec} *= 2LL;
      })
   )

   REQUIRE_ASSERT( "multiplication overflow", ([&]() {
         asset{ asset_max, sym_no_prec} *= 2LL;
      })
   )

   // -----------------------------------------------------------------------------------------------------------------------------
   // friend asset operator/(const asset&, int64_t)/asset& operator/=(int64_t)/friend int64_t operator/(const asset&, const asset&)
   REQUIRE_EQUAL( (asset{ 0LL, sym_no_prec} / asset{ 1LL, sym_no_prec}.amount ), (asset{ 0LL, sym_no_prec}) )
   REQUIRE_EQUAL( (asset{ 1LL, sym_no_prec} / asset{ 1LL, sym_no_prec}.amount ), (asset{ 1LL, sym_no_prec}) )
   REQUIRE_EQUAL( (asset{ 4LL, sym_no_prec} / asset{ 2LL, sym_no_prec}.amount ), (asset{ 2LL, sym_no_prec}) )
   REQUIRE_EQUAL( (asset{-4LL, sym_no_prec} / asset{ 2LL, sym_no_prec}.amount ), (asset{-2LL, sym_no_prec}) )
   REQUIRE_EQUAL( (asset{-4LL, sym_no_prec} / asset{-2LL, sym_no_prec}.amount ), (asset{ 2LL, sym_no_prec}) )

   REQUIRE_EQUAL( (asset{ 0LL, sym_no_prec} /= asset{ 1LL, sym_no_prec}.amount ), (asset{ 0LL, sym_no_prec}) )
   REQUIRE_EQUAL( (asset{ 1LL, sym_no_prec} /= asset{ 1LL, sym_no_prec}.amount ), (asset{ 1LL, sym_no_prec}) )
   REQUIRE_EQUAL( (asset{ 4LL, sym_no_prec} /= asset{ 2LL, sym_no_prec}.amount ), (asset{ 2LL, sym_no_prec}) )
   REQUIRE_EQUAL( (asset{-4LL, sym_no_prec} /= asset{ 2LL, sym_no_prec}.amount ), (asset{-2LL, sym_no_prec}) )
   REQUIRE_EQUAL( (asset{-4LL, sym_no_prec} /= asset{-2LL, sym_no_prec}.amount ), (asset{ 2LL, sym_no_prec}) )

   REQUIRE_EQUAL( (asset{ 0LL, sym_no_prec} / asset{ 1LL, sym_no_prec} ), (asset{ 0LL, sym_no_prec}.amount) )
   REQUIRE_EQUAL( (asset{ 1LL, sym_no_prec} / asset{ 1LL, sym_no_prec} ), (asset{ 1LL, sym_no_prec}.amount) )
   REQUIRE_EQUAL( (asset{ 4LL, sym_no_prec} / asset{ 2LL, sym_no_prec} ), (asset{ 2LL, sym_no_prec}.amount) )
   REQUIRE_EQUAL( (asset{-4LL, sym_no_prec} / asset{ 2LL, sym_no_prec} ), (asset{-2LL, sym_no_prec}.amount) )
   REQUIRE_EQUAL( (asset{-4LL, sym_no_prec} / asset{-2LL, sym_no_prec} ), (asset{ 2LL, sym_no_prec}.amount) )

   REQUIRE_ASSERT( "divide by zero", ([&]() {asset{1LL, sym_no_prec} /= 0;}) )

   // Note that there is no invariant established here when adding or setting the `amount`
   REQUIRE_ASSERT( "signed division overflow", ([&]() {
         asset a{};
         a.amount = std::numeric_limits<int64_t>::min();
         a /= -1LL;
      })
   )

   REQUIRE_ASSERT( "comparison of assets with different symbols is not allowed", ([&]() {
         asset{{1LL}, s0} / asset{{1LL}, s1};
      })
   )

   // --------------------------------------------------
   // friend bool operator==(const asset&, const asset&)
   REQUIRE_EQUAL( (asset{{}, sym_no_prec} == asset{{}, sym_no_prec} ), true )
   REQUIRE_EQUAL( (asset{asset_min, sym_no_prec} == asset{asset_min, sym_no_prec} ), true )
   REQUIRE_EQUAL( (asset{asset_max, sym_no_prec} == asset{asset_max, sym_no_prec} ), true )

   REQUIRE_ASSERT( "comparison of assets with different symbols is not allowed", ( []() {
      asset{{}, symbol{"SYMBOLL", 0}} == asset{{}, symbol{"LLOBMYS", 0}};
      })
   )

   // ---------------------------------------------------
   // friend bool operator!=( const asset&, const asset&)
   REQUIRE_EQUAL( (asset{asset_min, sym_no_prec} != -asset{asset_min, sym_no_prec} ), true )
   REQUIRE_EQUAL( (asset{asset_max, sym_no_prec} != -asset{asset_max, sym_no_prec} ), true )

   // -------------------------------------------------
   // friend bool operator<(const asset&, const asset&)
   REQUIRE_EQUAL( (asset{{}, sym_no_prec} < asset{1LL, sym_no_prec} ), true )

   // --------------------------------------------------
   // friend bool operator<=(const asset&, const asset&)
   REQUIRE_EQUAL( ( asset{{},  sym_no_prec} <= asset{1LL, sym_no_prec} ), true )
   REQUIRE_EQUAL( ( asset{1LL, sym_no_prec} <= asset{1LL, sym_no_prec} ), true )

   // -------------------------------------------------
   // friend bool operator>(const asset&, const asset&)
   REQUIRE_EQUAL( ( asset{1LL, sym_no_prec} > asset{{}, sym_no_prec} ), true )
   REQUIRE_EQUAL( ( asset{{}, sym_no_prec}  > asset{1LL, sym_no_prec} ), false )

   // --------------------------------------------------
   // friend bool operator>=( const asset&, const asset&)
   REQUIRE_EQUAL( ( asset{1LL, sym_no_prec} >= asset{{},  sym_no_prec} ), true )
   REQUIRE_EQUAL( ( asset{1LL, sym_no_prec} >= asset{1LL, sym_no_prec} ), true )

   silence_output(false);
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/asset.hpp`
EOSIO_TEST_BEGIN(extended_asset_type_test)
   silence_output(true);

   symbol sym_no_prec{"SYMBOLL", 0};
   symbol sym_prec{"SYMBOLL", 63};

   extended_symbol ex_sym_no_prec{sym_no_prec, name{"eosioaccountj"}};
   extended_symbol ex_sym_prec{sym_prec, name{"eosioaccountj"}};

   asset asset_no_prec{0LL, sym_no_prec};
   asset asset_prec{0LL, sym_prec};

   // ------------
   // constructors

   /// extended_asset()
   REQUIRE_EQUAL( extended_asset{}.quantity, asset{} )
   REQUIRE_EQUAL( extended_asset{}.contract, name{} )

   /// extended_asset(int64_t, extended_symbol)
   REQUIRE_EQUAL( (extended_asset{{}, ex_sym_no_prec}.quantity), (asset{0LL, sym_no_prec}) )
   REQUIRE_EQUAL( (extended_asset{{}, ex_sym_no_prec}.contract), (name{"eosioaccountj"}) )

   /// extended_asset(asset, name)
   REQUIRE_EQUAL( (extended_asset{asset_no_prec, name{"eosioaccountj"}}.quantity), (asset{ 0LL, sym_no_prec}) )
   REQUIRE_EQUAL( (extended_asset{asset_no_prec, name{"eosioaccountj"}}.contract), (name{"eosioaccountj"}) )

   // ------------------------------------------
   // extended_symbol get_extended_symbol()const
   REQUIRE_EQUAL( (extended_asset{{}, ex_sym_no_prec}.get_extended_symbol()), (ex_sym_no_prec) )
   REQUIRE_EQUAL( (extended_asset{{}, ex_sym_prec}.get_extended_symbol()), (ex_sym_prec) )

   // Note: uncomment once the appended '\n' is removed from the `print` function
   // Fix this printing functionality
   // Note that if there is no precision, there will be odd output:
   // `extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"1"}}.print();`
   // output: "0. A@1"

   // REQUIRE_PRINT( "0. A@1", [](){extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"1"}}.print();} )
   // REQUIRE_PRINT( "0. A@5", [](){extended_asset{asset{int64_t{0}, symbol{"A", 0}}, name{"5"}}.print();} )
   // REQUIRE_PRINT( "0. Z@a", [](){extended_asset{asset{int64_t{0}, symbol{"Z", 0}}, name{"a"}}.print();} )
   // REQUIRE_PRINT( "0. Z@z", [](){extended_asset{asset{int64_t{0}, symbol{"Z", 0}}, name{"z"}}.print();} )

   // REQUIRE_PRINT( "0. AAAAAAA@111111111111j", [](){extended_asset{asset{int64_t{0}, symbol{"AAAAAAA", 0}}, name{"111111111111j"}}.print();} )
   // REQUIRE_PRINT( "0. AAAAAAA@555555555555j", [](){extended_asset{asset{int64_t{0}, symbol{"AAAAAAA", 0}}, name{"555555555555j"}}.print();} )
   // REQUIRE_PRINT( "0. ZZZZZZZ@aaaaaaaaaaaaj", [](){extended_asset{asset{int64_t{0}, symbol{"ZZZZZZZ", 0}}, name{"aaaaaaaaaaaaj"}}.print();} )
   // REQUIRE_PRINT( "0. ZZZZZZZ@zzzzzzzzzzzzj", [](){extended_asset{asset{int64_t{0}, symbol{"ZZZZZZZ", 0}}, name{"zzzzzzzzzzzzj"}}.print();} )

   // -------------------------------
   // extended_asset operator-()const
   REQUIRE_EQUAL( (-extended_asset{asset{ 0, sym_no_prec}, {}}.quantity), (extended_asset{asset_no_prec, {}}.quantity) )
   REQUIRE_EQUAL( (-extended_asset{asset{-0, sym_no_prec}, {}}.quantity), (extended_asset{asset_no_prec, {}}.quantity) )
   REQUIRE_EQUAL( (-extended_asset{asset{ 0, sym_prec}, {}}.quantity),    (extended_asset{asset_prec, {}}.quantity) )
   REQUIRE_EQUAL( (-extended_asset{asset{-0, sym_prec}, {}}.quantity),    (extended_asset{asset_prec, {}}.quantity) )

   REQUIRE_EQUAL( (-extended_asset{asset{1LL, sym_no_prec}, {}}.quantity), (extended_asset{asset{-1LL, sym_no_prec}, {}}.quantity) )
   REQUIRE_EQUAL( (-extended_asset{asset{1LL, sym_no_prec}, {}}.quantity), (extended_asset{asset{-1LL, sym_no_prec}, {}}.quantity) )
   REQUIRE_EQUAL( (-extended_asset{asset{1LL, sym_prec}, {}}.quantity),    (extended_asset{asset{-1LL, sym_prec}, {}}.quantity) )
   REQUIRE_EQUAL( (-extended_asset{asset{1LL, sym_prec}, {}}.quantity),    (extended_asset{asset{-1LL, sym_prec}, {}}.quantity) )

   REQUIRE_EQUAL( (-extended_asset{asset{asset_max, sym_no_prec}, {}}.quantity), (extended_asset{asset{asset_min, sym_no_prec}, {}}.quantity) )
   REQUIRE_EQUAL( (-extended_asset{asset{asset_max, sym_no_prec}, {}}.quantity), (extended_asset{asset{asset_min, sym_no_prec}, {}}.quantity) )
   REQUIRE_EQUAL( (-extended_asset{asset{asset_max, sym_prec}, {}}.quantity),    (extended_asset{asset{asset_min, sym_prec}, {}}.quantity) )
   REQUIRE_EQUAL( (-extended_asset{asset{asset_max, sym_prec}, {}}.quantity),    (extended_asset{asset{asset_min, sym_prec}, {}}.quantity) )

   // -----------------------------------------------------------------------------
   // friend extended_asset operator+(const extended_asset&, const extended_asset&)
   REQUIRE_EQUAL( (extended_asset{asset{0LL, sym_no_prec}, {}} + extended_asset{asset{ 0LL, sym_no_prec}, {}}), (extended_asset{asset_no_prec, {}}) )
   REQUIRE_EQUAL( (extended_asset{asset{1LL, sym_no_prec}, {}} + extended_asset{asset{-1LL, sym_no_prec}, {}}), (extended_asset{asset_no_prec, {}}) )

   REQUIRE_ASSERT( "type mismatch", ([&]() {
         extended_asset{asset_no_prec, name{"eosioaccountj"}} + extended_asset{asset_no_prec, name{"jtnuoccaoisoe"}};
      })
   )

   // -------------------------------------------------------------------------
   // friend extended_asset& operator+=(extended_asset&, const extended_asset&)
   extended_asset temp{asset_no_prec, {}};
   REQUIRE_EQUAL( (temp += temp), (extended_asset{asset_no_prec, {}}) )
   temp = extended_asset{asset{1LL, sym_no_prec}, {}};
   REQUIRE_EQUAL( (temp += extended_asset{asset{-1LL, sym_no_prec}, {}}), (extended_asset{asset_no_prec, {}}) )

   REQUIRE_ASSERT( "type mismatch", ([&]() {
         temp += extended_asset{asset_no_prec, name{"eosioaccountj"}};
      })
   )

   // -----------------------------------------------------------------------------
   // friend extended_asset operator-(const extended_asset&, const extended_asset&)
   REQUIRE_EQUAL( (extended_asset{asset_no_prec, {}} - extended_asset{asset_no_prec, {}}),
                  (extended_asset{asset_no_prec, {}}) )
   REQUIRE_EQUAL( (extended_asset{asset{1LL, sym_no_prec}, {}} - extended_asset{asset{1LL, sym_no_prec}, {}}),
                  (extended_asset{asset{asset_no_prec}, {}}) )

   REQUIRE_ASSERT( "type mismatch", ([&]() {
         extended_asset{asset_no_prec, name{"eosioaccountj"}} - extended_asset{asset_no_prec, name{"jtnuoccaoisoe"}};
      })
   )

   // --------------------------------------------------------------------------
   // friend extended_asset& operator-=(extended_asset&, const extended_asset&)
   temp = extended_asset{asset_no_prec, {}};
   REQUIRE_EQUAL( (temp -= temp), (extended_asset{asset_no_prec, {}}) )
   temp = extended_asset{asset{1LL, sym_no_prec}, {}};
   REQUIRE_EQUAL( (temp -= temp), (extended_asset{asset_no_prec, {}}) )

   REQUIRE_ASSERT( "type mismatch", ([&]() {
         temp -= extended_asset{asset_no_prec, name{"jtnuoccaoisoe"}};
      })
   )

   // --------------------------------------------------------------------
   // friend bool operator==(const extended_asset&, const extended_asset&)
   REQUIRE_EQUAL( (extended_asset{asset_no_prec, {}} == extended_asset{asset_no_prec, {}}), true )
   REQUIRE_EQUAL( (extended_asset{asset{1LL, sym_no_prec}, {}} == extended_asset{asset{1LL, sym_no_prec}, {}}), true )

   // // Why is this not throwing??
   // REQUIRE_ASSERT( "type mismatch", ([&]() {asset1 == asset2;}) );

   // --------------------------------------------------------------------
   // friend bool operator!=(const extended_asset&, const extended_asset&)
   REQUIRE_EQUAL( (extended_asset{asset_no_prec, name{"eosioaccountj"}} != extended_asset{asset_no_prec, name{"jtnuoccaoisoe"}}), true )
   REQUIRE_EQUAL( (extended_asset{asset{1LL, sym_no_prec}, {}} != extended_asset{asset{-1LL, sym_no_prec}, {}}), true )
   REQUIRE_EQUAL( (extended_asset{asset{1LL, sym_no_prec}, {}} != extended_asset{asset{ 0LL, sym_no_prec}, name{"eosioaccountj"}}), true )

   // -------------------------------------------------------------------
   // friend bool operator<(const extended_asset&, const extended_asset&)
   REQUIRE_EQUAL( (extended_asset{asset_no_prec, name{}} < extended_asset{asset{ 1LL, sym_no_prec}, {}}), true )
   REQUIRE_ASSERT( "type mismatch", ([&]() {extended_asset{{}, name{}} < extended_asset{{}, name{"eosioaccountj"}};}) )

   // --------------------------------------------------------------------
   // friend bool operator<=(const extended_asset&, const extended_asset&)
   REQUIRE_EQUAL( (extended_asset{asset_no_prec, name{}} <= extended_asset{asset{ 1LL, sym_no_prec}, {}}), true );
   REQUIRE_ASSERT( "type mismatch", ([&]() {extended_asset{{}, name{}} <= extended_asset{{}, name{"eosioaccountj"}};}) )

   // --------------------------------------------------------------------
   // friend bool operator>=(const extended_asset&, const extended_asset&)
   REQUIRE_EQUAL( (extended_asset{asset{ 1LL, sym_no_prec}, {}} >= extended_asset{asset_no_prec, name{}}), true );
   REQUIRE_ASSERT( "type mismatch", ([&]() {extended_asset{{}, name{}} >= extended_asset{{}, name{"eosioaccountj"}};}) )

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(asset_type_test);
   EOSIO_TEST(extended_asset_type_test);
   return has_failed();
}
