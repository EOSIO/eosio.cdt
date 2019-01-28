#include <eosio/native/tester.hpp>

#include <eosiolib/symbol.hpp>

using eosio::name;
using eosio::symbol_code;
using eosio::symbol;
using eosio::extended_symbol;

const uint64_t u64min = std::numeric_limits<uint64_t>::min(); // 0ULL
const uint64_t u64max = std::numeric_limits<uint64_t>::max(); // 18446744073709551615ULL

// Definitions in `eosio.cdt/libraries/eosiolib/symbol.hpp`
EOSIO_TEST_BEGIN(symbol_code_type_test)
   silence_output(true);

   // ------------------------------------------
   // constructors/constexpr uint64_t raw()const

   /// constexpr symbol_code()
   REQUIRE_EQUAL( symbol_code{}.raw(), 0ULL )

   /// constexpr explicit symbol_code(uint64_t raw)
   REQUIRE_EQUAL( symbol_code{0ULL}.raw(), 0ULL )
   REQUIRE_EQUAL( symbol_code{1ULL}.raw(), 1ULL )
   REQUIRE_EQUAL( symbol_code{u64max}.raw(), u64max )

   /// constexpr explicit symbol_code(std::string_view str)
   REQUIRE_EQUAL( symbol_code{"A"}.raw(), 65ULL )
   REQUIRE_EQUAL( symbol_code{"Z"}.raw(), 90ULL )
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"}.raw(), 18367622009667905ULL )
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"}.raw(), 25432092013386330ULL )

   REQUIRE_ASSERT( "string is too long to be a valid symbol_code", ([]() {symbol_code{"ABCDEFGH"};}) )
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"a"};}) )
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"z"};}) )
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"@"};}) )
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"["};}) )

   // ------------------------------
   // constexpr bool is_valid()const
   REQUIRE_EQUAL( symbol_code{65ULL}.is_valid(), true ) // "A"
   REQUIRE_EQUAL( symbol_code{90ULL}.is_valid(), true ) // "Z"
   REQUIRE_EQUAL( symbol_code{18367622009667905ULL}.is_valid(), true ) // "AAAAAAA"
   REQUIRE_EQUAL( symbol_code{25432092013386330ULL}.is_valid(), true ) // "ZZZZZZZ"

   REQUIRE_EQUAL( symbol_code{64ULL}.is_valid(), false )
   REQUIRE_EQUAL( symbol_code{25432092013386331ULL}.is_valid(), false )

   // --------------------------------
   // constexpr uint32_t length()const
   REQUIRE_EQUAL( symbol_code{""}.length(), 0 )
   REQUIRE_EQUAL( symbol_code{"S"}.length(), 1 )
   REQUIRE_EQUAL( symbol_code{"SY"}.length(), 2 )
   REQUIRE_EQUAL( symbol_code{"SYM"}.length(), 3 )
   REQUIRE_EQUAL( symbol_code{"SYMB"}.length(), 4 )
   REQUIRE_EQUAL( symbol_code{"SYMBO"}.length(), 5 )
   REQUIRE_EQUAL( symbol_code{"SYMBOL"}.length(), 6 )
   REQUIRE_EQUAL( symbol_code{"SYMBOLL"}.length(), 7 )

   // ---------------------------------------
   // constexpr explicit operator bool()const
   REQUIRE_EQUAL( symbol_code{0ULL}.operator bool(), false )
   REQUIRE_EQUAL( symbol_code{1ULL}.operator bool(), true )
   REQUIRE_EQUAL( !symbol_code{0ULL}.operator bool(), true )
   REQUIRE_EQUAL( !symbol_code{1ULL}.operator bool(), false )

   REQUIRE_EQUAL( symbol_code{""}.operator bool(), false )
   REQUIRE_EQUAL( symbol_code{"SYMBOL"}.operator bool(), true )
   REQUIRE_EQUAL( !symbol_code{""}.operator bool(), true )
   REQUIRE_EQUAL( !symbol_code{"SYMBOL"}.operator bool(), false )

   // ----------------------------------------
   // char* write_as_string(char*, char*)const
   char buffer[7]{};

   std::string test_str{"A"};
   symbol_code{test_str}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   symbol_code{test_str = "Z"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   symbol_code{test_str = "AAAAAAA"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   symbol_code{test_str = "ZZZZZZZ"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )

   // ----------------------------
   // std::string to_string()const
   REQUIRE_EQUAL( symbol_code{"A"}.to_string(), "A" )
   REQUIRE_EQUAL( symbol_code{"Z"}.to_string(), "Z" )
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"}.to_string(), "AAAAAAA" )
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"}.to_string(), "ZZZZZZZ" )

   // --------------------------------------------------------------
   // friend bool operator==(const symbol_code&, const symbol_code&)
   REQUIRE_EQUAL( symbol_code{"A"} == symbol_code{"A"}, true )
   REQUIRE_EQUAL( symbol_code{"Z"} == symbol_code{"Z"}, true )
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"} == symbol_code{"AAAAAAA"}, true )
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"} == symbol_code{"ZZZZZZZ"}, true )

   // --------------------------------------------------------------
   // friend bool operator!=(const symbol_code&, const symbol_code&)
   REQUIRE_EQUAL( symbol_code{"A"} != symbol_code{}, true )
   REQUIRE_EQUAL( symbol_code{"Z"} != symbol_code{}, true )
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"} != symbol_code{}, true )
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"} != symbol_code{}, true )

   // -------------------------------------------------------------
   // friend bool operator<(const symbol_code&, const symbol_code&)
   REQUIRE_EQUAL( symbol_code{} < symbol_code{"A"}, true )
   REQUIRE_EQUAL( symbol_code{} < symbol_code{"Z"}, true )
   REQUIRE_EQUAL( symbol_code{} < symbol_code{"AAAAAAA"}, true )
   REQUIRE_EQUAL( symbol_code{} < symbol_code{"ZZZZZZZ"}, true )

   silence_output(false);
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosiolib/symbol.hpp`
EOSIO_TEST_BEGIN(symbol_type_test)
   silence_output(true);

   symbol_code sc0{"A"};
   symbol_code sc1{"Z"};
   symbol_code sc2{"AAAAAAA"};
   symbol_code sc3{"ZZZZZZZ"};

   // ------------------------------------------
   // constructors/constexpr uint64_t raw()const

   /// constexpr symbol()
   REQUIRE_EQUAL( symbol{}.raw(), 0ULL )

   /// constexpr explicit symbol(uint64_t)
   REQUIRE_EQUAL( symbol{0ULL}.raw(), 0ULL )
   REQUIRE_EQUAL( symbol{1ULL}.raw(), 1ULL )
   REQUIRE_EQUAL( symbol{u64max}.raw(), u64max )

   /// constexpr symbol(std::string_view, uint8_t)
   // Note that unless constructed with `initializer_list`, precision does not check for wrap-around
   REQUIRE_EQUAL( (symbol{sc0, 0}.raw()), 16640ULL )
   REQUIRE_EQUAL( (symbol{sc1, 0}.raw()), 23040ULL )
   REQUIRE_EQUAL( (symbol{sc2, 0}.raw()), 4702111234474983680ULL )
   REQUIRE_EQUAL( (symbol{sc3, 0}.raw()), 6510615555426900480ULL )

   /// constexpr symbol(symbol_code, uint8_t)
   REQUIRE_EQUAL( (symbol{sc0, 0}.raw()), 16640ULL )
   REQUIRE_EQUAL( (symbol{sc1, 0}.raw()), 23040ULL )
   REQUIRE_EQUAL( (symbol{sc2, 0}.raw()), 4702111234474983680ULL )
   REQUIRE_EQUAL( (symbol{sc3, 0}.raw()), 6510615555426900480ULL )

   // --------------------
   // bool is_valid()const
   REQUIRE_EQUAL( symbol{16640ULL}.is_valid(), true ) // "A", precision: 0
   REQUIRE_EQUAL( symbol{23040ULL}.is_valid(), true ) // "Z", precision: 0
   REQUIRE_EQUAL( symbol{4702111234474983680ULL}.is_valid(), true ) // "AAAAAAA", precision: 0
   REQUIRE_EQUAL( symbol{6510615555426900480ULL}.is_valid(), true ) // "ZZZZZZZ", precision: 0

   REQUIRE_EQUAL( symbol{16639ULL}.is_valid(), false )
   REQUIRE_EQUAL( symbol{6510615555426900736ULL}.is_valid(), false )

   // -------------------------
   // uint8_t precision()const
   REQUIRE_EQUAL( (symbol{sc0, 0}.precision()), 0 )
   REQUIRE_EQUAL( (symbol{sc1, 0}.precision()), 0 )
   REQUIRE_EQUAL( (symbol{sc2, 0}.precision()), 0 )
   REQUIRE_EQUAL( (symbol{sc3, 0}.precision()), 0 )

   REQUIRE_EQUAL( (symbol{sc0, 255}.precision()), 255 )
   REQUIRE_EQUAL( (symbol{sc1, 255}.precision()), 255 )
   REQUIRE_EQUAL( (symbol{sc2, 255}.precision()), 255 )
   REQUIRE_EQUAL( (symbol{sc3, 255}.precision()), 255 )

   // -----------------------
   // symbol_code code()const
   REQUIRE_EQUAL( (symbol{sc0, 0}.code()), sc0 )
   REQUIRE_EQUAL( (symbol{sc1, 0}.code()), sc1 )
   REQUIRE_EQUAL( (symbol{sc2, 0}.code()), sc2 )
   REQUIRE_EQUAL( (symbol{sc3, 0}.code()), sc3 )

   // ---------------------------------------
   // constexpr explicit operator bool()const
   REQUIRE_EQUAL( symbol{0}.operator bool(), false )
   REQUIRE_EQUAL( symbol{1}.operator bool(), true )
   REQUIRE_EQUAL( !symbol{0}.operator bool(), true )
   REQUIRE_EQUAL( !symbol{1}.operator bool(), false )

   REQUIRE_EQUAL( (symbol{"", 0}.operator bool()), false )
   REQUIRE_EQUAL( (symbol{"SYMBOLL", 0}.operator bool()), true )
   REQUIRE_EQUAL( (!symbol{"", 0}.operator bool()), true )
   REQUIRE_EQUAL( (!symbol{"SYMBOLL", 0}.operator bool()), false )

   // Note: uncomment once the appended '\n' is removed from the `print` function
   // ---------------------
   // void print(bool)const
   // symbol{"A", 0}.print(true);
   // symbol{"AAAAAAA", 255}.print(true);
   // REQUIRE_PRINT( "0,A", [&](){symbol{"A", 0}.print(true);} );
   // REQUIRE_PRINT( "0,Z", [&](){symbol{"Z", 0}.print(true);} );
   // REQUIRE_PRINT( "255,AAAAAAA", [&](){symbol{"AAAAAAA", 255}.print(true);} );
   // REQUIRE_PRINT( "255,ZZZZZZZ", [&](){symbol{"ZZZZZZZ", 255}.print(true);} );

   // REQUIRE_PRINT( ",A", [&](){symbol{"A", 0}.print(false);} );
   // REQUIRE_PRINT( ",Z", [&](){symbol{"Z", 0}.print(false);} );
   // REQUIRE_PRINT( ",AAAAAAA", [&](){symbol{"AAAAAAA", 255}.print(false);} );
   // REQUIRE_PRINT( ",ZZZZZZZ", [&](){symbol{"ZZZZZZZ", 255}.print(false);} );

   // --------------------------------------------------------------
   // friend constexpr bool operator==(const symbol&, const symbol&)
   REQUIRE_EQUAL( (symbol{sc0, 0} == symbol{sc0, 0}), true )
   REQUIRE_EQUAL( (symbol{sc1, 0} == symbol{sc1, 0}), true )
   REQUIRE_EQUAL( (symbol{sc2, 0} == symbol{sc2, 0}), true )
   REQUIRE_EQUAL( (symbol{sc3, 0} == symbol{sc3, 0}), true )

   // --------------------------------------------------------------
   // friend constexpr bool operator!=(const symbol&, const symbol&)
   REQUIRE_EQUAL( (symbol{sc0, 0} != symbol{}), true )
   REQUIRE_EQUAL( (symbol{sc1, 0} != symbol{}), true )
   REQUIRE_EQUAL( (symbol{sc2, 0} != symbol{}), true )
   REQUIRE_EQUAL( (symbol{sc3, 0} != symbol{}), true )

   // --------------------------------------------------------------
   // friend constexpr bool operator<(const symbol&, const symbol&)
   REQUIRE_EQUAL( (symbol{} < symbol{sc0, 0}), true )
   REQUIRE_EQUAL( (symbol{} < symbol{sc1, 0}), true )
   REQUIRE_EQUAL( (symbol{} < symbol{sc2, 0}), true )
   REQUIRE_EQUAL( (symbol{} < symbol{sc3, 0}), true )

   silence_output(false);
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosiolib/symbol.hpp`
EOSIO_TEST_BEGIN(extended_symbol_type_test)
   silence_output(true);

   name n0{"1"};
   name n1{"5"};
   name n2{"a"};
   name n3{"z"};
   name n4{"111111111111j"};
   name n5{"555555555555j"};
   name n6{"aaaaaaaaaaaaj"};
   name n7{"zzzzzzzzzzzzj"};

   symbol s0{"A", 0};
   symbol s1{"Z", 0};
   symbol s2{"AAAAAAA", 0};
   symbol s3{"ZZZZZZZ", 0};

   // ------------------------------------
   // constructors/get_symbol/get_contract

   /// constexpr extended_symbol()
   REQUIRE_EQUAL( (extended_symbol{{}, {}}.get_symbol().raw()), 0ULL )
   REQUIRE_EQUAL( (extended_symbol{{}, {}}.get_contract().value), 0ULL )
   
   /// constexpr extended_symbol(symbol, name)/constexpr symbol get_symbol()const/constexpr name get_contract()const
   REQUIRE_EQUAL( (extended_symbol{s0, n0}.get_symbol().raw()), 16640ULL )
   REQUIRE_EQUAL( (extended_symbol{s0, n1}.get_symbol().code().raw()), 65ULL )
   REQUIRE_EQUAL( (extended_symbol{s1, n2}.get_symbol().raw()), 23040ULL )
   REQUIRE_EQUAL( (extended_symbol{s1, n3}.get_symbol().code().raw()), 90ULL )
   REQUIRE_EQUAL( (extended_symbol{s0, n0}.get_contract().value), 576460752303423488ULL )
   REQUIRE_EQUAL( (extended_symbol{s0, n1}.get_contract().value), 2882303761517117440ULL )
   REQUIRE_EQUAL( (extended_symbol{s1, n2}.get_contract().value), 3458764513820540928ULL )
   REQUIRE_EQUAL( (extended_symbol{s1, n3}.get_contract().value), 17870283321406128128ULL )
   REQUIRE_EQUAL( (extended_symbol{s2, n4}.get_symbol().raw()), 4702111234474983680ULL )
   REQUIRE_EQUAL( (extended_symbol{s2, n5}.get_symbol().code().raw()), 18367622009667905ULL )
   REQUIRE_EQUAL( (extended_symbol{s3, n6}.get_symbol().raw()), 6510615555426900480ULL )
   REQUIRE_EQUAL( (extended_symbol{s3, n7}.get_symbol().code().raw()), 25432092013386330ULL )
   REQUIRE_EQUAL( (extended_symbol{s2, n4}.get_contract().value), 595056260442243615ULL )
   REQUIRE_EQUAL( (extended_symbol{s2, n5}.get_contract().value), 2975281302211218015ULL )
   REQUIRE_EQUAL( (extended_symbol{s3, n6}.get_contract().value), 3570337562653461615ULL )
   REQUIRE_EQUAL( (extended_symbol{s3, n7}.get_contract().value), u64max )

   // // Note: uncomment once the appended '\n' is removed from the `print` function
   // // ---------------------
   // // void print(bool)const
   // extended_symbol{s0, n0}.print(true);
   // extended_symbol{s0, n0}.print(true);
   // REQUIRE_PRINT( "0@576460752303423488", [&](){extended_symbol{s0, n0}.print(true);} );
   // REQUIRE_PRINT( "0@2882303761517117440", [&](){extended_symbol{s1, n1}.print(true);} );
   // REQUIRE_PRINT( "255@595056260442243615", [&](){extended_symbol{symbol{"AAAAAAA", 255}, name{"111111111111j"}}.print(true);} );
   // REQUIRE_PRINT( "255@2975281302211218015", [&](){extended_symbol{symbol{"ZZZZZZZ", 255}, name{"555555555555j"}}.print(true);} );

   // REQUIRE_PRINT( "@576460752303423488", [&](){extended_symbol{symbol{"A", 0}, name{"1"}}.print(false);} );
   // REQUIRE_PRINT( "@2882303761517117440", [&](){extended_symbol{symbol{"Z", 0}, name{"5"}}.print(false);} );
   // REQUIRE_PRINT( "@595056260442243615", [&](){extended_symbol{symbol{"AAAAAAA", 255}, name{"111111111111j"}}.print(false);} );
   // REQUIRE_PRINT( "@2975281302211218015", [&](){extended_symbol{symbol{"ZZZZZZZ", 255}, name{"555555555555j"}}.print(false);} );

   // -------------------------------------------------------------------------------
   // friend constexpr bool operator==(const extended_symbol&, const extended_symbol&)
   REQUIRE_EQUAL( (extended_symbol{s0, n0} == extended_symbol{s0, n0}), true )
   REQUIRE_EQUAL( (extended_symbol{s1, n3} == extended_symbol{s1, n3}), true )
   REQUIRE_EQUAL( (extended_symbol{s2, n4} == extended_symbol{s2, n4}), true )
   REQUIRE_EQUAL( (extended_symbol{s3, n7} == extended_symbol{s3, n7}), true )

   // -------------------------------------------------------------------------------
   // friend constexpr bool operator!=(const extended_symbol&, const extended_symbol&)
   REQUIRE_EQUAL( (extended_symbol{} != extended_symbol{s0, {}}), true )
   REQUIRE_EQUAL( (extended_symbol{s0, {}} != extended_symbol{s1, {}}), true )
   REQUIRE_EQUAL( (extended_symbol{s1, {}} != extended_symbol{s2, {}}), true )
   REQUIRE_EQUAL( (extended_symbol{s2, {}} != extended_symbol{s3, {}}), true )

   // -------------------------------------------------------------------------------
   // friend constexpr bool operator<(const extended_symbol&, const extended_symbol&)
   REQUIRE_EQUAL( (extended_symbol{} < extended_symbol{s0, {}}), true )
   REQUIRE_EQUAL( (extended_symbol{} < extended_symbol{s1, {}}), true )
   REQUIRE_EQUAL( (extended_symbol{} < extended_symbol{s2, {}}), true )
   REQUIRE_EQUAL( (extended_symbol{} < extended_symbol{s3, {}}), true )

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(symbol_code_type_test);
   EOSIO_TEST(symbol_type_test);
   EOSIO_TEST(extended_symbol_type_test);
   return has_failed();
}
