#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

// #define NATIVE_NAME
#define NATIVE_SYMBOL

using namespace eosio;
using namespace eosio::native;

// Defined in `eosio.cdt/libraries/eosiolib/symbol.hpp`
EOSIO_TEST_BEGIN(symbol_code_type_test)
   // ----------------
   // constructors/raw
   REQUIRE_EQUAL( symbol_code{}.raw(), 0ULL );

   REQUIRE_EQUAL( symbol_code{0ULL}.raw(), 0ULL );
   REQUIRE_EQUAL( symbol_code{1ULL}.raw(), 1ULL );
   REQUIRE_EQUAL( symbol_code{18446744073709551615ULL}.raw(), 18446744073709551615ULL );

   REQUIRE_EQUAL( symbol_code{"A"}.raw(), 65ULL );
   REQUIRE_EQUAL( symbol_code{"Z"}.raw(), 90ULL );
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"}.raw(), 18367622009667905ULL );
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"}.raw(), 25432092013386330ULL );

   REQUIRE_ASSERT( "string is too long to be a valid symbol_code", ([]() {symbol_code{"ABCDEFGH"};}) );
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"a"};}) );
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"z"};}) );
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"@"};}) );
   REQUIRE_ASSERT( "only uppercase letters allowed in symbol_code string", ([]() {symbol_code{"["};}) );

   // --------
   // is_valid
   REQUIRE_EQUAL( symbol_code{65ULL}.is_valid(), true ); // "A"
   REQUIRE_EQUAL( symbol_code{90ULL}.is_valid(), true ); // "Z"
   REQUIRE_EQUAL( symbol_code{18367622009667905ULL}.is_valid(), true ); // "AAAAAAA"
   REQUIRE_EQUAL( symbol_code{25432092013386330ULL}.is_valid(), true ); // "ZZZZZZZ"

   REQUIRE_EQUAL( symbol_code{64ULL}.is_valid(), false );
   REQUIRE_EQUAL( symbol_code{25432092013386331ULL}.is_valid(), false );

   // ------
   // length
   REQUIRE_EQUAL( symbol_code{""}.length(), 0 );
   REQUIRE_EQUAL( symbol_code{"S"}.length(), 1 );
   REQUIRE_EQUAL( symbol_code{"SY"}.length(), 2 );
   REQUIRE_EQUAL( symbol_code{"SYM"}.length(), 3 );
   REQUIRE_EQUAL( symbol_code{"SYMB"}.length(), 4 );
   REQUIRE_EQUAL( symbol_code{"SYMBO"}.length(), 5 );
   REQUIRE_EQUAL( symbol_code{"SYMBOL"}.length(), 6 );
   REQUIRE_EQUAL( symbol_code{"SYMBOLL"}.length(), 7 );

   // -------------
   // operator bool
   // Note that I must be explicit about calling the operator because it is defined as `explicit`
   REQUIRE_EQUAL( symbol_code{0}.operator bool(), false );
   REQUIRE_EQUAL( symbol_code{1}.operator bool(), true );
   REQUIRE_EQUAL( !symbol_code{0}, true );
   REQUIRE_EQUAL( !symbol_code{1}, false );

   REQUIRE_EQUAL( symbol_code{""}.operator bool(), false );
   REQUIRE_EQUAL( symbol_code{"SYMBOL"}.operator bool(), true );
   REQUIRE_EQUAL( !symbol_code{""}, true );
   REQUIRE_EQUAL( !symbol_code{"SYMBOL"}, false );

   // ---------------
   // write_as_string
   // TODO: rename when moved
   char buffer1[7]{};

   // TODO: rename when moved
   std::string test_str1{"A"};
   symbol_code{test_str1}.write_as_string( buffer1, buffer1 + sizeof(buffer1) );
print_f("%\n",test_str1);
print_f("%\n",buffer1);
   REQUIRE_EQUAL( memcmp(test_str1.c_str(), buffer1, strlen(test_str1.c_str())), 0 );
   symbol_code{test_str1 = "Z"}.write_as_string( buffer1, buffer1 + sizeof(buffer1) );
print_f("%\n",test_str1);
print_f("%\n",buffer1);
   REQUIRE_EQUAL( memcmp(test_str1.c_str(), buffer1, strlen(test_str1.c_str())), 0 );
   symbol_code{test_str1 = "AAAAAAA"}.write_as_string( buffer1, buffer1 + sizeof(buffer1) );
print_f("%\n",test_str1);
print_f("%\n",buffer1);
   REQUIRE_EQUAL( memcmp(test_str1.c_str(), buffer1, strlen(test_str1.c_str())), 0 );
   symbol_code{test_str1 = "ZZZZZZZ"}.write_as_string( buffer1, buffer1 + sizeof(buffer1) );
print_f("%\n",test_str1);
print_f("%\n",buffer1);
   REQUIRE_EQUAL( memcmp(test_str1.c_str(), buffer1, strlen(test_str1.c_str())), 0 );

   // ---------
   // to_string
   REQUIRE_EQUAL( symbol_code{"A"}.to_string(), "A" );
   REQUIRE_EQUAL( symbol_code{"Z"}.to_string(), "Z" );
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"}.to_string(), "AAAAAAA" );
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"}.to_string(), "ZZZZZZZ" );

   // ----------
   // operator==
   REQUIRE_EQUAL( symbol_code{"A"} == symbol_code{"A"}, true );
   REQUIRE_EQUAL( symbol_code{"Z"} == symbol_code{"Z"}, true );
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"} == symbol_code{"AAAAAAA"}, true );
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"} == symbol_code{"ZZZZZZZ"}, true );

   // ----------
   // operator!=
   REQUIRE_EQUAL( symbol_code{"A"} != symbol_code{0}, true );
   REQUIRE_EQUAL( symbol_code{"Z"} != symbol_code{0}, true );
   REQUIRE_EQUAL( symbol_code{"AAAAAAA"} != symbol_code{0}, true );
   REQUIRE_EQUAL( symbol_code{"ZZZZZZZ"} != symbol_code{0}, true );

   // ---------
   // operator<
   REQUIRE_EQUAL( symbol_code{0} < symbol_code{"A"}, true );
   REQUIRE_EQUAL( symbol_code{0} < symbol_code{"Z"}, true );
   REQUIRE_EQUAL( symbol_code{0} < symbol_code{"AAAAAAA"}, true );
   REQUIRE_EQUAL( symbol_code{0} < symbol_code{"ZZZZZZZ"}, true );
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/symbol.hpp`
EOSIO_TEST_BEGIN(symbol_type_test)
   // ------------
   // constructors/raw
   REQUIRE_EQUAL( symbol{}.raw(), 0ULL );

   REQUIRE_EQUAL( symbol{0ULL}.raw(), 0ULL );
   REQUIRE_EQUAL( symbol{1ULL}.raw(), 1ULL );
   REQUIRE_EQUAL( symbol{18446744073709551615ULL}.raw(), 18446744073709551615ULL );

   // Note that unless constructed with `initializer_list`, precision does not check for wrap-around
   REQUIRE_EQUAL( (symbol{"A", 0}.raw()), 16640ULL );
   REQUIRE_EQUAL( (symbol{"Z", 0}.raw()), 23040ULL );
   REQUIRE_EQUAL( (symbol{"AAAAAAA", 0}.raw()), 4702111234474983680ULL );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ", 0}.raw()), 6510615555426900480ULL );

   REQUIRE_EQUAL( (symbol{symbol_code{"A"}, 0}.raw()), 16640ULL );
   REQUIRE_EQUAL( (symbol{symbol_code{"Z"}, 0}.raw()), 23040ULL );
   REQUIRE_EQUAL( (symbol{symbol_code{"AAAAAAA"}, 0}.raw()), 4702111234474983680ULL );
   REQUIRE_EQUAL( (symbol{symbol_code{"ZZZZZZZ"}, 0}.raw()), 6510615555426900480ULL );

print_f("%",symbol{"A", 0}.raw());
print_f("%",symbol{"Z", 0}.raw());
print_f("%",symbol{"AAAAAAA", 0}.raw());
print_f("%",symbol{"ZZZZZZZ", 0}.raw());
print_f("%",symbol{"ZZZZZZZ", 255}.raw());

   // --------
   // is_valid
   REQUIRE_EQUAL( symbol{16640ULL}.is_valid(), true ); // "A", precision: 0
   REQUIRE_EQUAL( symbol{23040ULL}.is_valid(), true ); // "Z", precision: 0
   REQUIRE_EQUAL( symbol{4702111234474983680ULL}.is_valid(), true ); // "AAAAAAA", precision: 0
   REQUIRE_EQUAL( symbol{6510615555426900480ULL}.is_valid(), true ); // "ZZZZZZZ", precision: 0

   REQUIRE_EQUAL( symbol{16639ULL}.is_valid(), false );
   REQUIRE_EQUAL( symbol{6510615555426900736ULL}.is_valid(), false );

   // ---------
   // precision
   REQUIRE_EQUAL( (symbol{"A", 0}.precision()), 0 );
   REQUIRE_EQUAL( (symbol{"Z", 0}.precision()), 0 );
   REQUIRE_EQUAL( (symbol{"AAAAAAA", 0}.precision()), 0 );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ", 0}.precision()), 0 );

   REQUIRE_EQUAL( (symbol{"A", 255}.precision()), 255 );
   REQUIRE_EQUAL( (symbol{"Z", 255}.precision()), 255 );
   REQUIRE_EQUAL( (symbol{"AAAAAAA", 255}.precision()), 255 );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ", 255}.precision()), 255 );

   // ----
   // code
   REQUIRE_EQUAL( (symbol{"A", 0}.code()), symbol_code{"A"} );
   REQUIRE_EQUAL( (symbol{"Z", 0}.code()), symbol_code{"Z"} );
   REQUIRE_EQUAL( (symbol{"AAAAAAA", 0}.code()), symbol_code{"AAAAAAA"} );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ", 0}.code()), symbol_code{"ZZZZZZZ"} );

   // -------------
   // operator bool
   // Note that I must be explicit about calling the operator because it is defined as `explicit`
   REQUIRE_EQUAL( symbol{0}.operator bool(), false );
   REQUIRE_EQUAL( symbol{1}.operator bool(), true );
   REQUIRE_EQUAL( !symbol{0}, true );
   REQUIRE_EQUAL( !symbol{1}, false );

   REQUIRE_EQUAL( (symbol{"", 0}.operator bool()), false );
   REQUIRE_EQUAL( (symbol{"SYMBOL", 0}.operator bool()), true );
   REQUIRE_EQUAL( (!symbol{"", 0}), true );
   REQUIRE_EQUAL( (!symbol{"SYMBOL", 0}), false );

   // -----
   // print
   

   // ----------
   // operator==
   REQUIRE_EQUAL( (symbol{"A", 0} == symbol{"A", 0}), true );
   REQUIRE_EQUAL( (symbol{"Z", 0} == symbol{"Z", 0}), true );
   REQUIRE_EQUAL( (symbol{"AAAAAAA", 0} == symbol{"AAAAAAA", 0}), true );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ", 0} == symbol{"ZZZZZZZ", 0}), true );

   // ----------
   // operator!=
   REQUIRE_EQUAL( (symbol{"A",0} != symbol{0}), true );
   REQUIRE_EQUAL( (symbol{"Z",0} != symbol{0}), true );
   REQUIRE_EQUAL( (symbol{"AAAAAAA",0} != symbol{0}), true );
   REQUIRE_EQUAL( (symbol{"ZZZZZZZ",0} != symbol{0}), true );

   // ---------
   // operator<
   REQUIRE_EQUAL( (symbol{0} < symbol{"A",0}), true );
   REQUIRE_EQUAL( (symbol{0} < symbol{"Z",0}), true );
   REQUIRE_EQUAL( (symbol{0} < symbol{"AAAAAAA",0}), true );
   REQUIRE_EQUAL( (symbol{0} < symbol{"ZZZZZZZ",0}), true );
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/symbol.hpp`
EOSIO_TEST_BEGIN(extended_symbol_type_test)
   // ------------
   // constructors
   
   
   // ----------
   // get_symbol
   

   // ------------
   // get_contract
   

   // -----
   // print
   

   // ----------
   // operator==
   

   // ----------
   // operator!=
   

   // ---------
   // operator<
   
EOSIO_TEST_END

int main(int argc, char** argv) {
   // EOSIO_TEST(symbol_code_type_test);
   EOSIO_TEST(symbol_type_test);
   // EOSIO_TEST(extended_symbol_type_test);
   return has_failed();
}
