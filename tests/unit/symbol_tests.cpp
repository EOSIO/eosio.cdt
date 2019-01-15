#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

using namespace eosio;
using namespace eosio::native;

// Defined in `eosio.cdt/libraries/eosiolib/symbol.hpp`
EOSIO_TEST_BEGIN(symbol_code_type_test)
   silence_output(true);

   // ------------------------------------------
   // constructors/constexpr uint64_t raw()const

   /// constexpr symbol_code()
   REQUIRE_EQUAL( symbol_code{}.raw(), 0ULL );

   /// constexpr explicit symbol_code(uint64_t raw)
   REQUIRE_EQUAL( symbol_code{0ULL}.raw(), 0ULL );
   REQUIRE_EQUAL( symbol_code{1ULL}.raw(), 1ULL );
   REQUIRE_EQUAL( symbol_code{18446744073709551615ULL}.raw(), 18446744073709551615ULL );

   /// constexpr explicit symbol_code(std::string_view str)
   REQUIRE_EQUAL( symbol_code{"A"}.raw(), 65ULL );
   REQUIRE_EQUAL( symbol_code{"Z"}.raw(), 90ULL );
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"}.raw(), 18367622009667905ULL );
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"}.raw(), 25432092013386330ULL );

   REQUIRE_ASSERT( "string is too long to be a valid symbol_code", ([]() {symbol_code{"ABCDEFGH"};}) );
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"a"};}) );
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"z"};}) );
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"@"};}) );
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"["};}) );

   // ------------------------------
   // constexpr bool is_valid()const
   REQUIRE_EQUAL( symbol_code{65ULL}.is_valid(), true ); // "A"
   REQUIRE_EQUAL( symbol_code{90ULL}.is_valid(), true ); // "Z"
   REQUIRE_EQUAL( symbol_code{18367622009667905ULL}.is_valid(), true ); // "AAAAAAA"
   REQUIRE_EQUAL( symbol_code{25432092013386330ULL}.is_valid(), true ); // "ZZZZZZZ"

   REQUIRE_EQUAL( symbol_code{64ULL}.is_valid(), false );
   REQUIRE_EQUAL( symbol_code{25432092013386331ULL}.is_valid(), false );

   // --------------------------------
   // constexpr uint32_t length()const
   REQUIRE_EQUAL( symbol_code{""}.length(), 0 );
   REQUIRE_EQUAL( symbol_code{"S"}.length(), 1 );
   REQUIRE_EQUAL( symbol_code{"SY"}.length(), 2 );
   REQUIRE_EQUAL( symbol_code{"SYM"}.length(), 3 );
   REQUIRE_EQUAL( symbol_code{"SYMB"}.length(), 4 );
   REQUIRE_EQUAL( symbol_code{"SYMBO"}.length(), 5 );
   REQUIRE_EQUAL( symbol_code{"SYMBOL"}.length(), 6 );
   REQUIRE_EQUAL( symbol_code{"SYMBOLL"}.length(), 7 );

   // ---------------------------------------
   // constexpr explicit operator bool()const
   // Note that I must be explicit about calling the operator because it is defined as `explicit`
   REQUIRE_EQUAL( symbol_code{0}.operator bool(), false );
   REQUIRE_EQUAL( symbol_code{1}.operator bool(), true );
   REQUIRE_EQUAL( !symbol_code{0}, true );
   REQUIRE_EQUAL( !symbol_code{1}, false );

   REQUIRE_EQUAL( symbol_code{""}.operator bool(), false );
   REQUIRE_EQUAL( symbol_code{"SYMBOL"}.operator bool(), true );
   REQUIRE_EQUAL( !symbol_code{""}, true );
   REQUIRE_EQUAL( !symbol_code{"SYMBOL"}, false );

   // ----------------------------------------
   // char* write_as_string(char*, char*)const
   char buffer[7]{};

   std::string test_str{"A"};
   symbol_code{test_str}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   symbol_code{test_str = "Z"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   symbol_code{test_str = "AAAAAAA"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   symbol_code{test_str = "ZZZZZZZ"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );

   // ----------------------------
   // std::string to_string()const
   REQUIRE_EQUAL( symbol_code{"A"}.to_string(), "A" );
   REQUIRE_EQUAL( symbol_code{"Z"}.to_string(), "Z" );
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"}.to_string(), "AAAAAAA" );
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"}.to_string(), "ZZZZZZZ" );

   // --------------------------------------------------------------
   // friend bool operator==(const symbol_code&, const symbol_code&)
   REQUIRE_EQUAL( symbol_code{"A"} == symbol_code{"A"}, true );
   REQUIRE_EQUAL( symbol_code{"Z"} == symbol_code{"Z"}, true );
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"} == symbol_code{"AAAAAAA"}, true );
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"} == symbol_code{"ZZZZZZZ"}, true );

   // --------------------------------------------------------------
   // friend bool operator!=(const symbol_code&, const symbol_code&)
   REQUIRE_EQUAL( symbol_code{"A"} != symbol_code{0}, true );
   REQUIRE_EQUAL( symbol_code{"Z"} != symbol_code{0}, true );
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"} != symbol_code{0}, true );
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"} != symbol_code{0}, true );

   // -------------------------------------------------------------
   // friend bool operator<(const symbol_code&, const symbol_code&)
   REQUIRE_EQUAL( symbol_code{0} < symbol_code{"A"}, true );
   REQUIRE_EQUAL( symbol_code{0} < symbol_code{"Z"}, true );
   REQUIRE_EQUAL( symbol_code{0} < symbol_code{"AAAAAAA"}, true );
   REQUIRE_EQUAL( symbol_code{0} < symbol_code{"ZZZZZZZ"}, true );

   silence_output(false);
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/symbol.hpp`
EOSIO_TEST_BEGIN(symbol_type_test)
   silence_output(true);

   // ------------------------------------------
   // constructors/constexpr uint64_t raw()const

   /// constexpr symbol()
   REQUIRE_EQUAL( symbol{}.raw(), 0ULL );

   /// constexpr explicit symbol(uint64_t)
   REQUIRE_EQUAL( symbol{0ULL}.raw(), 0ULL );
   REQUIRE_EQUAL( symbol{1ULL}.raw(), 1ULL );
   REQUIRE_EQUAL( symbol{18446744073709551615ULL}.raw(), 18446744073709551615ULL );

   /// constexpr symbol(std::string_view, uint8_t)
   // Note that unless constructed with `initializer_list`, precision does not check for wrap-around
   REQUIRE_EQUAL( (symbol{"A", 0}.raw()), 16640ULL );
   REQUIRE_EQUAL( (symbol{"Z", 0}.raw()), 23040ULL );
   REQUIRE_EQUAL( (symbol{"AAAAAAA", 0}.raw()), 4702111234474983680ULL );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ", 0}.raw()), 6510615555426900480ULL );

   /// constexpr symbol(symbol_code, uint8_t)
   REQUIRE_EQUAL( (symbol{symbol_code{"A"}, 0}.raw()), 16640ULL );
   REQUIRE_EQUAL( (symbol{symbol_code{"Z"}, 0}.raw()), 23040ULL );
   REQUIRE_EQUAL( (symbol{symbol_code{"AAAAAAA"}, 0}.raw()), 4702111234474983680ULL );
   REQUIRE_EQUAL( (symbol{symbol_code{"ZZZZZZZ"}, 0}.raw()), 6510615555426900480ULL );

   // --------------------
   // bool is_valid()const
   REQUIRE_EQUAL( symbol{16640ULL}.is_valid(), true ); // "A", precision: 0
   REQUIRE_EQUAL( symbol{23040ULL}.is_valid(), true ); // "Z", precision: 0
   REQUIRE_EQUAL( symbol{4702111234474983680ULL}.is_valid(), true ); // "AAAAAAA", precision: 0
   REQUIRE_EQUAL( symbol{6510615555426900480ULL}.is_valid(), true ); // "ZZZZZZZ", precision: 0

   REQUIRE_EQUAL( symbol{16639ULL}.is_valid(), false );
   REQUIRE_EQUAL( symbol{6510615555426900736ULL}.is_valid(), false );

   // -------------------------
   // uint64_t precision()const
   REQUIRE_EQUAL( (symbol{"A", 0}.precision()), 0 );
   REQUIRE_EQUAL( (symbol{"Z", 0}.precision()), 0 );
   REQUIRE_EQUAL( (symbol{"AAAAAAA", 0}.precision()), 0 );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ", 0}.precision()), 0 );

   REQUIRE_EQUAL( (symbol{"A", 255}.precision()), 255 );
   REQUIRE_EQUAL( (symbol{"Z", 255}.precision()), 255 );
   REQUIRE_EQUAL( (symbol{"AAAAAAA", 255}.precision()), 255 );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ", 255}.precision()), 255 );

   // -----------------------
   // symbol_code code()const
   REQUIRE_EQUAL( (symbol{"A", 0}.code()), symbol_code{"A"} );
   REQUIRE_EQUAL( (symbol{"Z", 0}.code()), symbol_code{"Z"} );
   REQUIRE_EQUAL( (symbol{"AAAAAAA", 0}.code()), symbol_code{"AAAAAAA"} );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ", 0}.code()), symbol_code{"ZZZZZZZ"} );

   // ---------------------------------------
   // constexpr explicit operator bool()const
   // Note that I must be explicit about calling the operator because it is defined as `explicit`
   REQUIRE_EQUAL( symbol{0}.operator bool(), false );
   REQUIRE_EQUAL( symbol{1}.operator bool(), true );
   REQUIRE_EQUAL( !symbol{0}, true );
   REQUIRE_EQUAL( !symbol{1}, false );

   REQUIRE_EQUAL( (symbol{"", 0}.operator bool()), false );
   REQUIRE_EQUAL( (symbol{"SYMBOL", 0}.operator bool()), true );
   REQUIRE_EQUAL( (!symbol{"", 0}), true );
   REQUIRE_EQUAL( (!symbol{"SYMBOL", 0}), false );

   // ---------------------
   // void print(bool)const
// symbol{"A", 0}.print(true);
// symbol{"AAAAAAA", 255}.print(true);
   // REQUIRE_PRINT( "0,A", [](){symbol{"A", 0}.print(true);} );
   // REQUIRE_PRINT( "0,Z", [](){symbol{"Z", 0}.print(true);} );
   // REQUIRE_PRINT( "255,AAAAAAA", [](){symbol{"AAAAAAA", 255}.print(true);} );
   // REQUIRE_PRINT( "255,ZZZZZZZ", [](){symbol{"ZZZZZZZ", 255}.print(true);} );

   // REQUIRE_PRINT( ",A", [](){symbol{"A", 0}.print(false);} );
   // REQUIRE_PRINT( ",Z", [](){symbol{"Z", 0}.print(false);} );
   // REQUIRE_PRINT( ",AAAAAAA", [](){symbol{"AAAAAAA", 255}.print(false);} );
   // REQUIRE_PRINT( ",ZZZZZZZ", [](){symbol{"ZZZZZZZ", 255}.print(false);} );

   // --------------------------------------------------------------
   // friend constexpr bool operator==(const symbol&, const symbol&)
   REQUIRE_EQUAL( (symbol{"A", 0} == symbol{"A", 0}), true );
   REQUIRE_EQUAL( (symbol{"Z", 0} == symbol{"Z", 0}), true );
   REQUIRE_EQUAL( (symbol{"AAAAAAA", 0} == symbol{"AAAAAAA", 0}), true );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ", 0} == symbol{"ZZZZZZZ", 0}), true );

   // --------------------------------------------------------------
   // friend constexpr bool operator!=(const symbol&, const symbol&)
   REQUIRE_EQUAL( (symbol{"A",0} != symbol{0}), true );
   REQUIRE_EQUAL( (symbol{"Z",0} != symbol{0}), true );
   REQUIRE_EQUAL( (symbol{"AAAAAAA",0} != symbol{0}), true );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ",0} != symbol{0}), true );

   // --------------------------------------------------------------
   // friebnd constexpr bool operator<(const symbol&, const symbol&)
   REQUIRE_EQUAL( (symbol{0} < symbol{"A",0}), true );
   REQUIRE_EQUAL( (symbol{0} < symbol{"Z",0}), true );
   REQUIRE_EQUAL( (symbol{0} < symbol{"AAAAAAA",0}), true );
   REQUIRE_EQUAL( (symbol{0} < symbol{"ZZZZZZZ",0}), true );

   silence_output(false);
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/symbol.hpp`
EOSIO_TEST_BEGIN(extended_symbol_type_test)
   silence_output(true);

   // ------------------------------------
   // constructors/get_symbol/get_contract

   /// constexpr extended_symbol()
   REQUIRE_EQUAL( (extended_symbol{symbol{}, name{}}.get_symbol().raw()), 0ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{}, name{}}.get_contract().value), 0ULL );
   
   /// constexpr extended_symbol(symbol, name)/constexpr symbol get_symbol()const/constexpr name get_contract()const
   REQUIRE_EQUAL( (extended_symbol{symbol{"A", 0}, name{"1"}}.get_symbol().raw()), 16640ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"A", 0}, name{"5"}}.get_symbol().code().raw()), 65ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"Z", 0}, name{"a"}}.get_symbol().raw()), 23040ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"Z", 0}, name{"z"}}.get_symbol().code().raw()), 90ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"A", 0}, name{"1"}}.get_contract().value), 576460752303423488ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"A", 0}, name{"5"}}.get_contract().value), 2882303761517117440ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"Z", 0}, name{"a"}}.get_contract().value), 3458764513820540928ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"Z", 0}, name{"z"}}.get_contract().value), 17870283321406128128ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"AAAAAAA", 0}, name{"111111111111j"}}.get_symbol().raw()), 4702111234474983680ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"AAAAAAA", 0}, name{"555555555555j"}}.get_symbol().code().raw()), 18367622009667905ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"ZZZZZZZ", 0}, name{"aaaaaaaaaaaaj"}}.get_symbol().raw()), 6510615555426900480ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"ZZZZZZZ", 0}, name{"zzzzzzzzzzzzj"}}.get_symbol().code().raw()), 25432092013386330ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"AAAAAAA", 0}, name{"111111111111j"}}.get_contract().value), 595056260442243615ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"AAAAAAA", 0}, name{"555555555555j"}}.get_contract().value), 2975281302211218015ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"ZZZZZZZ", 0}, name{"aaaaaaaaaaaaj"}}.get_contract().value), 3570337562653461615ULL );
   REQUIRE_EQUAL( (extended_symbol{symbol{"ZZZZZZZ", 0}, name{"zzzzzzzzzzzzj"}}.get_contract().value), 18446744073709551615ULL );

   // ---------------------
   // void print(bool)const
// extended_symbol{symbol{"A", 0}, name{"1"}}.print(true);
// extended_symbol{symbol{"AAAAAAA", 255}, name{"111111111111j"}}.print(true);
   // REQUIRE_PRINT( "0@576460752303423488", [](){extended_symbol{symbol{"A", 0}, name{"1"}}.print(true);} );
   // REQUIRE_PRINT( "0@2882303761517117440", [](){extended_symbol{symbol{"Z", 0}, name{"5"}}.print(true);} );
   // REQUIRE_PRINT( "255@595056260442243615", [](){extended_symbol{symbol{"AAAAAAA", 255}, name{"111111111111j"}}.print(true);} );
   // REQUIRE_PRINT( "255@2975281302211218015", [](){extended_symbol{symbol{"ZZZZZZZ", 255}, name{"555555555555j"}}.print(true);} );

   // REQUIRE_PRINT( "@576460752303423488", [](){extended_symbol{symbol{"A", 0}, name{"1"}}.print(false);} );
   // REQUIRE_PRINT( "@2882303761517117440", [](){extended_symbol{symbol{"Z", 0}, name{"5"}}.print(false);} );
   // REQUIRE_PRINT( "@595056260442243615", [](){extended_symbol{symbol{"AAAAAAA", 255}, name{"111111111111j"}}.print(false);} );
   // REQUIRE_PRINT( "@2975281302211218015", [](){extended_symbol{symbol{"ZZZZZZZ", 255}, name{"555555555555j"}}.print(false);} );

   // -------------------------------------------------------------------------------
   // friend constexpr bool operator==(const extended_symbol&, const extended_symbol&)
   REQUIRE_EQUAL( (extended_symbol{symbol{"A", 0}, name{0}} == extended_symbol{symbol{"A", 0}, name{}}), true );
   REQUIRE_EQUAL( (extended_symbol{symbol{"Z", 0}, name{0}} == extended_symbol{symbol{"Z", 0}, name{}}), true );
   REQUIRE_EQUAL( (extended_symbol{symbol{"AAAAAAA", 0}, name{0}} == extended_symbol{symbol{"AAAAAAA", 0}, name{}}), true );
   REQUIRE_EQUAL( (extended_symbol{symbol{"ZZZZZZZ", 0}, name{0}} == extended_symbol{symbol{"ZZZZZZZ", 0}, name{}}), true );

   // -------------------------------------------------------------------------------
   // friend constexpr bool operator!=(const extended_symbol&, const extended_symbol&)
   REQUIRE_EQUAL( (extended_symbol{symbol{"A", 0}, name{0}} != extended_symbol{symbol{0}, name{0}}), true );
   REQUIRE_EQUAL( (extended_symbol{symbol{"Z", 0}, name{0}} != extended_symbol{symbol{0}, name{0}}), true );
   REQUIRE_EQUAL( (extended_symbol{symbol{"AAAAAAA", 0}, name{0}} != extended_symbol{symbol{0}, name{0}}), true );
   REQUIRE_EQUAL( (extended_symbol{symbol{"ZZZZZZZ", 0}, name{0}} != extended_symbol{symbol{0}, name{0}}), true );

   // -------------------------------------------------------------------------------
   // friend constexpr bool operator<(const extended_symbol&, const extended_symbol&)
   REQUIRE_EQUAL( (extended_symbol{symbol{0}, name{0}} < extended_symbol{symbol{"A", 0}, name{0}}), true );
   REQUIRE_EQUAL( (extended_symbol{symbol{0}, name{0}} < extended_symbol{symbol{"Z", 0}, name{0}}), true );
   REQUIRE_EQUAL( (extended_symbol{symbol{0}, name{0}} < extended_symbol{symbol{"AAAAAAA", 0}, name{0}}), true );
   REQUIRE_EQUAL( (extended_symbol{symbol{0}, name{0}} < extended_symbol{symbol{"ZZZZZZZ", 0}, name{0}}), true );

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(symbol_code_type_test);
   EOSIO_TEST(symbol_type_test);
   EOSIO_TEST(extended_symbol_type_test);
   return has_failed();
}
