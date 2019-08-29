/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <limits>
#include <string>

#include <eosio/tester.hpp>
#include <eosio/symbol.hpp>

using std::numeric_limits;
using std::string;

using eosio::name;
using eosio::symbol_code;
using eosio::symbol;
using eosio::extended_symbol;

static constexpr uint64_t u64min = numeric_limits<uint64_t>::min(); // 0ULL
static constexpr uint64_t u64max = numeric_limits<uint64_t>::max(); // 18446744073709551615ULL

// Definitions in `eosio.cdt/libraries/eosio/symbol.hpp`
EOSIO_TEST_BEGIN(symbol_code_type_test)
   //// constexpr symbol_code()
   // constexpr uint64_t raw()const
   CHECK_EQUAL( symbol_code{}.raw(), 0ULL )

   //// constexpr explicit symbol_code(uint64_t raw)
   CHECK_EQUAL( symbol_code{0ULL}.raw(), 0ULL )
   CHECK_EQUAL( symbol_code{1ULL}.raw(), 1ULL )
   CHECK_EQUAL( symbol_code{u64max}.raw(), u64max )

   //// constexpr explicit symbol_code(string_view str)
   CHECK_EQUAL( symbol_code{"A"}.raw(), 65ULL )
   CHECK_EQUAL( symbol_code{"Z"}.raw(), 90ULL )
   CHECK_EQUAL( symbol_code{"AAAAAAA"}.raw(), 18367622009667905ULL )
   CHECK_EQUAL( symbol_code{"ZZZZZZZ"}.raw(), 25432092013386330ULL )

   CHECK_ASSERT( "string is too long to be a valid symbol_code", ([]() {symbol_code{"ABCDEFGH"};}) )
   CHECK_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"a"};}) )
   CHECK_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"z"};}) )
   CHECK_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"@"};}) )
   CHECK_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"["};}) )

   // ------------------------------
   // constexpr bool is_valid()const
   CHECK_EQUAL( symbol_code{65ULL}.is_valid(), true ) // "A"
   CHECK_EQUAL( symbol_code{90ULL}.is_valid(), true ) // "Z"
   CHECK_EQUAL( symbol_code{18367622009667905ULL}.is_valid(), true ) // "AAAAAAA"
   CHECK_EQUAL( symbol_code{25432092013386330ULL}.is_valid(), true ) // "ZZZZZZZ"

   CHECK_EQUAL( symbol_code{64ULL}.is_valid(), false )
   CHECK_EQUAL( symbol_code{25432092013386331ULL}.is_valid(), false )

   // --------------------------------
   // constexpr uint32_t length()const
   CHECK_EQUAL( symbol_code{""}.length(), 0 )
   CHECK_EQUAL( symbol_code{"S"}.length(), 1 )
   CHECK_EQUAL( symbol_code{"SY"}.length(), 2 )
   CHECK_EQUAL( symbol_code{"SYM"}.length(), 3 )
   CHECK_EQUAL( symbol_code{"SYMB"}.length(), 4 )
   CHECK_EQUAL( symbol_code{"SYMBO"}.length(), 5 )
   CHECK_EQUAL( symbol_code{"SYMBOL"}.length(), 6 )
   CHECK_EQUAL( symbol_code{"SYMBOLL"}.length(), 7 )

   // ---------------------------------------
   // constexpr explicit operator bool()const
   CHECK_EQUAL( symbol_code{0ULL}.operator bool(), false )
   CHECK_EQUAL( symbol_code{1ULL}.operator bool(), true )
   CHECK_EQUAL( !symbol_code{0ULL}.operator bool(), true )
   CHECK_EQUAL( !symbol_code{1ULL}.operator bool(), false )

   CHECK_EQUAL( symbol_code{""}.operator bool(), false )
   CHECK_EQUAL( symbol_code{"SYMBOL"}.operator bool(), true )
   CHECK_EQUAL( !symbol_code{""}.operator bool(), true )
   CHECK_EQUAL( !symbol_code{"SYMBOL"}.operator bool(), false )

   // ----------------------------------------
   // char* write_as_string(char*, char*)const
   char buffer[7]{};

   string test_str{"A"};
   symbol_code{test_str}.write_as_string( buffer, buffer + sizeof(buffer) );
   CHECK_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   symbol_code{test_str = "Z"}.write_as_string( buffer, buffer + sizeof(buffer) );
   CHECK_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   symbol_code{test_str = "AAAAAAA"}.write_as_string( buffer, buffer + sizeof(buffer) );
   CHECK_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   symbol_code{test_str = "ZZZZZZZ"}.write_as_string( buffer, buffer + sizeof(buffer) );
   CHECK_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )

   // -----------------------
   // string to_string()const
   CHECK_EQUAL( symbol_code{"A"}.to_string(), "A" )
   CHECK_EQUAL( symbol_code{"Z"}.to_string(), "Z" )
   CHECK_EQUAL( symbol_code{"AAAAAAA"}.to_string(), "AAAAAAA" )
   CHECK_EQUAL( symbol_code{"ZZZZZZZ"}.to_string(), "ZZZZZZZ" )

   // --------------------------------------------------------------
   // friend bool operator==(const symbol_code&, const symbol_code&)
   CHECK_EQUAL( symbol_code{"A"} == symbol_code{"A"}, true )
   CHECK_EQUAL( symbol_code{"Z"} == symbol_code{"Z"}, true )
   CHECK_EQUAL( symbol_code{"AAAAAAA"} == symbol_code{"AAAAAAA"}, true )
   CHECK_EQUAL( symbol_code{"ZZZZZZZ"} == symbol_code{"ZZZZZZZ"}, true )

   // --------------------------------------------------------------
   // friend bool operator!=(const symbol_code&, const symbol_code&)
   CHECK_EQUAL( symbol_code{"A"} != symbol_code{}, true )
   CHECK_EQUAL( symbol_code{"Z"} != symbol_code{}, true )
   CHECK_EQUAL( symbol_code{"AAAAAAA"} != symbol_code{}, true )
   CHECK_EQUAL( symbol_code{"ZZZZZZZ"} != symbol_code{}, true )

   // -------------------------------------------------------------
   // friend bool operator<(const symbol_code&, const symbol_code&)
   CHECK_EQUAL( symbol_code{} < symbol_code{"A"}, true )
   CHECK_EQUAL( symbol_code{} < symbol_code{"Z"}, true )
   CHECK_EQUAL( symbol_code{} < symbol_code{"AAAAAAA"}, true )
   CHECK_EQUAL( symbol_code{} < symbol_code{"ZZZZZZZ"}, true )
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosio/symbol.hpp`
EOSIO_TEST_BEGIN(symbol_type_test)
   static constexpr symbol_code sc0{"A"};
   static constexpr symbol_code sc1{"Z"};
   static constexpr symbol_code sc2{"AAAAAAA"};
   static constexpr symbol_code sc3{"ZZZZZZZ"};

   //// constexpr symbol()
   // constexpr uint64_t raw()const
   CHECK_EQUAL( symbol{}.raw(), 0ULL )

   //// constexpr explicit symbol(uint64_t)
   CHECK_EQUAL( symbol{0ULL}.raw(), 0ULL )
   CHECK_EQUAL( symbol{1ULL}.raw(), 1ULL )
   CHECK_EQUAL( symbol{u64max}.raw(), u64max )

   //// constexpr symbol(string_view, uint8_t)
   // Note:
   // Unless constructed with `initializer_list`, precision does not check for wrap-around
   CHECK_EQUAL( (symbol{sc0, 0}.raw()), 16640ULL )
   CHECK_EQUAL( (symbol{sc1, 0}.raw()), 23040ULL )
   CHECK_EQUAL( (symbol{sc2, 0}.raw()), 4702111234474983680ULL )
   CHECK_EQUAL( (symbol{sc3, 0}.raw()), 6510615555426900480ULL )

   //// constexpr symbol(symbol_code, uint8_t)
   CHECK_EQUAL( (symbol{sc0, 0}.raw()), 16640ULL )
   CHECK_EQUAL( (symbol{sc1, 0}.raw()), 23040ULL )
   CHECK_EQUAL( (symbol{sc2, 0}.raw()), 4702111234474983680ULL )
   CHECK_EQUAL( (symbol{sc3, 0}.raw()), 6510615555426900480ULL )

   // --------------------
   // bool is_valid()const
   CHECK_EQUAL( symbol{16640ULL}.is_valid(), true ) // "A", precision: 0
   CHECK_EQUAL( symbol{23040ULL}.is_valid(), true ) // "Z", precision: 0
   CHECK_EQUAL( symbol{4702111234474983680ULL}.is_valid(), true ) // "AAAAAAA", precision: 0
   CHECK_EQUAL( symbol{6510615555426900480ULL}.is_valid(), true ) // "ZZZZZZZ", precision: 0

   CHECK_EQUAL( symbol{16639ULL}.is_valid(), false )
   CHECK_EQUAL( symbol{6510615555426900736ULL}.is_valid(), false )

   // -------------------------
   // uint8_t precision()const
   CHECK_EQUAL( (symbol{sc0,0}.precision()), 0 )
   CHECK_EQUAL( (symbol{sc1,0}.precision()), 0 )
   CHECK_EQUAL( (symbol{sc2,0}.precision()), 0 )
   CHECK_EQUAL( (symbol{sc3,0}.precision()), 0 )

   CHECK_EQUAL( (symbol{sc0,255}.precision()), 255 )
   CHECK_EQUAL( (symbol{sc1,255}.precision()), 255 )
   CHECK_EQUAL( (symbol{sc2,255}.precision()), 255 )
   CHECK_EQUAL( (symbol{sc3,255}.precision()), 255 )

   // -----------------------
   // symbol_code code()const
   CHECK_EQUAL( (symbol{sc0,0}.code()), sc0 )
   CHECK_EQUAL( (symbol{sc1,0}.code()), sc1 )
   CHECK_EQUAL( (symbol{sc2,0}.code()), sc2 )
   CHECK_EQUAL( (symbol{sc3,0}.code()), sc3 )

   // ---------------------------------------
   // constexpr explicit operator bool()const
   CHECK_EQUAL( symbol{0}.operator bool(), false )
   CHECK_EQUAL( symbol{1}.operator bool(), true )
   CHECK_EQUAL( !symbol{0}.operator bool(), true )
   CHECK_EQUAL( !symbol{1}.operator bool(), false )

   CHECK_EQUAL( (symbol{"", 0}.operator bool()), false )
   CHECK_EQUAL( (symbol{"SYMBOLL", 0}.operator bool()), true )
   CHECK_EQUAL( (!symbol{"", 0}.operator bool()), true )
   CHECK_EQUAL( (!symbol{"SYMBOLL", 0}.operator bool()), false )

   // ---------------------
   // void print(bool)const
   CHECK_PRINT( "0,A", [&](){symbol{"A", 0}.print(true);} );
   CHECK_PRINT( "0,Z", [&](){symbol{"Z", 0}.print(true);} );
   CHECK_PRINT( "255,AAAAAAA", [&](){symbol{"AAAAAAA", 255}.print(true);} );
   CHECK_PRINT( "255,ZZZZZZZ", [&](){symbol{"ZZZZZZZ", 255}.print(true);} );

   // --------------------------------------------------------------
   // friend constexpr bool operator==(const symbol&, const symbol&)
   CHECK_EQUAL( (symbol{sc0, 0} == symbol{sc0, 0}), true )
   CHECK_EQUAL( (symbol{sc1, 0} == symbol{sc1, 0}), true )
   CHECK_EQUAL( (symbol{sc2, 0} == symbol{sc2, 0}), true )
   CHECK_EQUAL( (symbol{sc3, 0} == symbol{sc3, 0}), true )

   // --------------------------------------------------------------
   // friend constexpr bool operator!=(const symbol&, const symbol&)
   CHECK_EQUAL( (symbol{sc0, 0} != symbol{}), true )
   CHECK_EQUAL( (symbol{sc1, 0} != symbol{}), true )
   CHECK_EQUAL( (symbol{sc2, 0} != symbol{}), true )
   CHECK_EQUAL( (symbol{sc3, 0} != symbol{}), true )

   // --------------------------------------------------------------
   // friend constexpr bool operator<(const symbol&, const symbol&)
   CHECK_EQUAL( (symbol{} < symbol{sc0, 0}), true )
   CHECK_EQUAL( (symbol{} < symbol{sc1, 0}), true )
   CHECK_EQUAL( (symbol{} < symbol{sc2, 0}), true )
   CHECK_EQUAL( (symbol{} < symbol{sc3, 0}), true )
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosio/symbol.hpp`
EOSIO_TEST_BEGIN(extended_symbol_type_test)
   static constexpr name n0{"1"};
   static constexpr name n1{"5"};
   static constexpr name n2{"a"};
   static constexpr name n3{"z"};
   static constexpr name n4{"111111111111j"};
   static constexpr name n5{"555555555555j"};
   static constexpr name n6{"aaaaaaaaaaaaj"};
   static constexpr name n7{"zzzzzzzzzzzzj"};

   static constexpr symbol s0{"A",0};
   static constexpr symbol s1{"Z",0};
   static constexpr symbol s2{"AAAAAAA",255};
   static constexpr symbol s3{"ZZZZZZZ",255};

   //// constexpr extended_symbol()
   // constexpr name get_symbol()
   // constexpr name get_contract()
   CHECK_EQUAL( (extended_symbol{{}, {}}.get_symbol().raw()), 0ULL )
   CHECK_EQUAL( (extended_symbol{{}, {}}.get_contract().value), 0ULL )

   //// constexpr extended_symbol(symbol, name)
   CHECK_EQUAL( (extended_symbol{s0, n0}.get_symbol().raw()), 16640ULL )
   CHECK_EQUAL( (extended_symbol{s0, n1}.get_symbol().code().raw()), 65ULL )
   CHECK_EQUAL( (extended_symbol{s1, n2}.get_symbol().raw()), 23040ULL )
   CHECK_EQUAL( (extended_symbol{s1, n3}.get_symbol().code().raw()), 90ULL )
   CHECK_EQUAL( (extended_symbol{s0, n0}.get_contract().value), 576460752303423488ULL )
   CHECK_EQUAL( (extended_symbol{s0, n1}.get_contract().value), 2882303761517117440ULL )
   CHECK_EQUAL( (extended_symbol{s1, n2}.get_contract().value), 3458764513820540928ULL )
   CHECK_EQUAL( (extended_symbol{s1, n3}.get_contract().value), 17870283321406128128ULL )
   CHECK_EQUAL( (extended_symbol{s2, n4}.get_symbol().raw()), 4702111234474983935ULL )
   CHECK_EQUAL( (extended_symbol{s2, n5}.get_symbol().code().raw()), 18367622009667905ULL )
   CHECK_EQUAL( (extended_symbol{s3, n6}.get_symbol().raw()), 6510615555426900735ULL )
   CHECK_EQUAL( (extended_symbol{s3, n7}.get_symbol().code().raw()), 25432092013386330ULL )
   CHECK_EQUAL( (extended_symbol{s2, n4}.get_contract().value), 595056260442243615ULL )
   CHECK_EQUAL( (extended_symbol{s2, n5}.get_contract().value), 2975281302211218015ULL )
   CHECK_EQUAL( (extended_symbol{s3, n6}.get_contract().value), 3570337562653461615ULL )
   CHECK_EQUAL( (extended_symbol{s3, n7}.get_contract().value), u64max )

   // ---------------------
   // void print(bool)const
   // Note:
   // Uncomment once print checking has been resolved
   // CHECK_PRINT( "0,A@1", [&](){extended_symbol{s0, n0}.print(true);} );
   // CHECK_PRINT( "0,A@5", [&](){extended_symbol{s0, n1}.print(true);} );
   // CHECK_PRINT( "0,Z@a", [&](){extended_symbol{s1, n2}.print(true);} );
   // CHECK_PRINT( "0,Z@z", [&](){extended_symbol{s1, n3}.print(true);} );
   // CHECK_PRINT( "255,AAAAAAA@111111111111j", [&](){extended_symbol{s2, n4}.print(true);} );
   // CHECK_PRINT( "255,AAAAAAA@555555555555j", [&](){extended_symbol{s2, n5}.print(true);} );
   // CHECK_PRINT( "255,ZZZZZZZ@aaaaaaaaaaaaj", [&](){extended_symbol{s3, n6}.print(true);} );
   // CHECK_PRINT( "255,ZZZZZZZ@zzzzzzzzzzzzj", [&](){extended_symbol{s3, n7}.print(true);} );

   // -------------------------------------------------------------------------------
   // friend constexpr bool operator==(const extended_symbol&, const extended_symbol&)
   CHECK_EQUAL( (extended_symbol{s0, n0} == extended_symbol{s0, n0}), true )
   CHECK_EQUAL( (extended_symbol{s1, n3} == extended_symbol{s1, n3}), true )
   CHECK_EQUAL( (extended_symbol{s2, n4} == extended_symbol{s2, n4}), true )
   CHECK_EQUAL( (extended_symbol{s3, n7} == extended_symbol{s3, n7}), true )

   // -------------------------------------------------------------------------------
   // friend constexpr bool operator!=(const extended_symbol&, const extended_symbol&)
   CHECK_EQUAL( (extended_symbol{} != extended_symbol{s0, {}}), true )
   CHECK_EQUAL( (extended_symbol{s0, {}} != extended_symbol{s1, {}}), true )
   CHECK_EQUAL( (extended_symbol{s1, {}} != extended_symbol{s2, {}}), true )
   CHECK_EQUAL( (extended_symbol{s2, {}} != extended_symbol{s3, {}}), true )

   // -------------------------------------------------------------------------------
   // friend constexpr bool operator<(const extended_symbol&, const extended_symbol&)
   CHECK_EQUAL( (extended_symbol{} < extended_symbol{s0, {}}), true )
   CHECK_EQUAL( (extended_symbol{} < extended_symbol{s1, {}}), true )
   CHECK_EQUAL( (extended_symbol{} < extended_symbol{s2, {}}), true )
   CHECK_EQUAL( (extended_symbol{} < extended_symbol{s3, {}}), true )
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   bool verbose = false;
   if( argc >= 2 && std::strcmp( argv[1], "-v" ) == 0 ) {
      verbose = true;
   }
   silence_output(!verbose);

   EOSIO_TEST(symbol_code_type_test);
   EOSIO_TEST(symbol_type_test);
   EOSIO_TEST(extended_symbol_type_test);
   return has_failed();
}
