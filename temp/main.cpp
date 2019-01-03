// eosio-cpp -c main.cpp -fnative; ./a.out
// TODO: better error messages

#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

#define NATIVE

using namespace eosio;
using namespace eosio::native;

int main()
{
   eosio_assert( name{}.value == 0ULL, "name != 0ULL" );

   eosio_assert( name{0ULL}.value == 0ULL, "name != 0ULL" );
   eosio_assert( name{1ULL}.value == 1ULL, "name != 1ULL" );
   eosio_assert( name{18446744073709551615ULL}.value == 18446744073709551615ULL, "name != 18446744073709551615ULL" );

   eosio_assert( name{name::raw{0ULL}}.value == 0ULL, "name != 0ULL" );
   eosio_assert( name{name::raw{1ULL}}.value == 1ULL, "name != 1ULL" );
   eosio_assert( name{name::raw{18446744073709551615ULL}}.value == 18446744073709551615ULL, "name != 18446744073709551615ULL" );

   eosio_assert( name{"1"}.value == 576460752303423488ULL, "name != 1" );
   eosio_assert( name{"5"}.value == 2882303761517117440ULL, "name != 5" );
   eosio_assert( name{"a"}.value == 3458764513820540928ULL, "name != a" );
   eosio_assert( name{"z"}.value == 17870283321406128128ULL, "name != z" );

   eosio_assert( name{"abc"}.value == 3589368903014285312ULL, "name != abc" );
   eosio_assert( name{"123"}.value == 614178399182651392ULL, "name != 123" );

   eosio_assert( name{".abc"}.value == 112167778219196416ULL, "name != .abc" );
   eosio_assert( name{".........abc"}.value == 102016ULL, "name != .........abc" );
   eosio_assert( name{"123."}.value == 614178399182651392ULL, "name != 123." );
   eosio_assert( name{"123........."}.value == 614178399182651392ULL, "name != 123........." );
   eosio_assert( name{".a.b.c.1.2.3."}.value == 108209673814966320ULL, "name != .a.b.c.1.2.3." );
   
   eosio_assert( name{"abc.123"}.value == 3589369488740450304ULL, "name != abc.123" );
   eosio_assert( name{"123.abc"}.value == 614181822271586304ULL, "name != 123.abc" );

   eosio_assert( name{"12345abcdefgj"}.value == 614251623682315983ULL, "name != 12345abcdefgj" );
   eosio_assert( name{"hijklmnopqrsj"}.value == 7754926748989239183ULL, "name != hijklmnopqrsj" );
   eosio_assert( name{"tuvwxyz.1234j"}.value == 14895601873741973071ULL, "name != tuvwxyz.1234j" );

   eosio_assert( name{"111111111111j"}.value == 595056260442243615ULL, "name != 111111111111j" );
   eosio_assert( name{"555555555555j"}.value == 2975281302211218015ULL, "name != 555555555555j" );
   eosio_assert( name{"aaaaaaaaaaaaj"}.value == 3570337562653461615ULL, "name != aaaaaaaaaaaaj" );
   eosio_assert( name{"zzzzzzzzzzzzj"}.value == 18446744073709551615ULL, "name != zzzzzzzzzzzzj" );

   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"-1"};}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"0"};}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"6"};}) );
   REQUIRE_ASSERT( "thirteenth character in name cannot be a letter that comes after j", ([]() {name{"111111111111k"};}) );
   REQUIRE_ASSERT( "thirteenth character in name cannot be a letter that comes after j", ([]() {name{"zzzzzzzzzzzzk"};}) );
   REQUIRE_ASSERT( "string is too long to be a valid name", ([]() {name{"12345abcdefghj"};}) );

   // ------------------------------------------------------------------------------------------------------------------------------

   char c{'.'};
   uint8_t expected_value{0}; // Will increment to the expected correct value in the set [0,32)
#ifdef NATIVE
   print_f("Value of name::char_to_value(%): %", c, static_cast<uint64_t>(name::char_to_value(c)) );
   print_f("Value of expected_value:         %", static_cast<uint64_t>(expected_value) );
#endif
   eosio_assert( name::char_to_value(c) == expected_value, "name::char_to_value(c) != expected_value" );
   ++expected_value;

   for(c = '1'; c <= '5'; ++c ) {
#ifdef NATIVE
      print_f("Value of name::char_to_value(%): %", c, static_cast<uint64_t>(name::char_to_value(c)) );
      print_f("Value of expected_value:         %", static_cast<uint64_t>(expected_value) );
#endif
      eosio_assert( name::char_to_value(c) == expected_value, "name::char_to_value(c) != expected_value" );
      ++expected_value;
   }

   for(c = 'a'; c <= 'z'; ++c ) {
#ifdef NATIVE
      print_f("Value of name::char_to_value(%): %", c, static_cast<uint64_t>(name::char_to_value(c)) );
      print_f("Value of expected_value:         %", static_cast<uint64_t>(expected_value) );
#endif
      eosio_assert( name::char_to_value(c) == expected_value, "name::char_to_value(c) != expected_value" );
      ++expected_value;
   }
   
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'-'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'/'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'6'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'A'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'Z'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'`'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'{'});}) );
   

   // ------------------------------------------------------------------------------------------------------------------------------

   eosio_assert( name{""}.length() == 0, "name.length() != 0" );
   eosio_assert( name{"e"}.length() == 1, "name.length() != 1" );
   eosio_assert( name{"eo"}.length() == 2, "name.length() != 2" );
   eosio_assert( name{"eos"}.length() == 3, "name.length() != 3" );
   eosio_assert( name{"eosi"}.length() == 4, "name.length() != 4" );
   eosio_assert( name{"eosio"}.length() == 5, "name.length() != 5" );
   eosio_assert( name{"eosioa"}.length() == 6, "name.length() != 6" );
   eosio_assert( name{"eosioac"}.length() == 7, "name.length() != 7" );
   eosio_assert( name{"eosioacc"}.length() == 8, "name.length() != 8" );
   eosio_assert( name{"eosioacco"}.length() == 9, "name.length() != 9" );
   eosio_assert( name{"eosioaccou"}.length() == 10, "name.length() != 10" );
   eosio_assert( name{"eosioaccoun"}.length() == 11, "name.length() != 11" );
   eosio_assert( name{"eosioaccount"}.length() == 12, "name.length() != 12" );
   eosio_assert( name{"eosioaccountj"}.length() == 13, "name.length() != 13" );

   REQUIRE_ASSERT( "string is too long to be a valid name", ([]() {name{"12345abcdefghj"}.length();}) );
   
   // ------------------------------------------------------------------------------------------------------------------------------

#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{".eosioaccounj"}.suffix() ); // TODO: interesting print behavior; starts newline sometimes
   print_f("Value of expected suffix: eosioaccounj\n" );
#endif
   
   eosio_assert( name{".eosioaccounj"}.suffix() == name{"eosioaccounj"}, "name.suffix() != eosioaccounj" );
   
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"e.osioaccounj"}.suffix() );
   print_f("Value of expected suffix: osioaccounj\n" );
#endif
   
   eosio_assert( name{"e.osioaccounj"}.suffix() == name{"osioaccounj"}, "name.suffix() != osioaccounj" );
   
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eo.sioaccounj"}.suffix() );
   print_f("Value of expected suffix: sioaccounj\n" );
#endif
   
   eosio_assert( name{"eo.sioaccounj"}.suffix() == name{"sioaccounj"}, "name.suffix() != sioaccounj" );
   
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eos.ioaccounj"}.suffix() );
   print_f("Value of expected suffix: ioaccounj\n" );
#endif
   
   eosio_assert( name{"eos.ioaccounj"}.suffix() == name{"ioaccounj"}, "name.suffix() != ioaccounj" );
   
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosi.oaccounj"}.suffix() );
   print_f("Value of expected suffix: oaccounj\n" );
#endif
   
   eosio_assert( name{"eosi.oaccounj"}.suffix() == name{"oaccounj"}, "name.suffix() != oaccounj" );
   
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosio.accounj"}.suffix() );
   print_f("Value of expected suffix: accounj\n" );
#endif
   
   eosio_assert( name{"eosio.accounj"}.suffix() == name{"accounj"}, "name.suffix() != accounj" );
   
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioa.ccounj"}.suffix() );
   print_f("Value of expected suffix: ccounj\n" );
#endif
   
   eosio_assert( name{"eosioa.ccounj"}.suffix() == name{"ccounj"}, "name.suffix() != ccounj" );
   
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioac.counj"}.suffix() );
   print_f("Value of expected suffix: counj\n" );
#endif
   
   eosio_assert( name{"eosioac.counj"}.suffix() == name{"counj"}, "name.suffix() != counj" );
   
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioacc.ounj"}.suffix() );
   print_f("Value of expected suffix: ounj\n" );
#endif
   
   eosio_assert( name{"eosioacc.ounj"}.suffix() == name{"ounj"}, "name.suffix() != ounj" );
   
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioacco.unj"}.suffix() );
   print_f("Value of expected suffix: unj\n" );
#endif
   
   eosio_assert( name{"eosioacco.unj"}.suffix() == name{"unj"}, "name.suffix() != unj" );
   
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioaccou.nj"}.suffix() );
   print_f("Value of expected suffix: nj\n" );
#endif
   
   eosio_assert( name{"eosioaccou.nj"}.suffix() == name{"nj"}, "name.suffix() != nj" );
   
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioaccoun.j"}.suffix() );
   print_f("Value of expected suffix: j\n" );
#endif
   
   eosio_assert( name{"eosioaccoun.j"}.suffix() == name{"j"}, "name.suffix() != j" );
   
// #ifdef NATIVE
//    print_f("Value of suffix:          %\n", name{"eosioaccounj."}.suffix() );
//    print_f("Value of expected suffix: \n" );
// #endif
//    eosio_assert( name{"eosioaccounj."}.suffix() == name{""}, "name.suffix() != \"\"" );

   // ------------------------------------------------------------------------------------------------------------------------------

   eosio_assert( static_cast<uint64_t>(name::raw{name{"1"}}) == 576460752303423488ULL, "name != 1" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"5"}}) == 2882303761517117440ULL, "name != 5" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"a"}}) == 3458764513820540928ULL, "name != a" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"z"}}) == 17870283321406128128ULL, "name != z" );

   eosio_assert( static_cast<uint64_t>(name::raw{name{"abc"}}) == 3589368903014285312ULL, "name != abc" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"123"}}) == 614178399182651392ULL, "name != 123" );

   eosio_assert( static_cast<uint64_t>(name::raw{name{".abc"}}) == 112167778219196416ULL, "name != .abc" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{".........abc"}}) == 102016ULL, "name != .........abc" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"123."}}) == 614178399182651392ULL, "name != 123." );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"123........."}}) == 614178399182651392ULL, "name != 123........." );
   eosio_assert( static_cast<uint64_t>(name::raw{name{".a.b.c.1.2.3."}}) == 108209673814966320ULL, "name != .a.b.c.1.2.3." );
   
   eosio_assert( static_cast<uint64_t>(name::raw{name{"abc.123"}}) == 3589369488740450304ULL, "name != abc.123" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"123.abc"}}) == 614181822271586304ULL, "name != 123.abc" );

   eosio_assert( static_cast<uint64_t>(name::raw{name{"12345abcdefgj"}}) == 614251623682315983ULL, "name != 12345abcdefgj" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"hijklmnopqrsj"}}) == 7754926748989239183ULL, "name != hijklmnopqrsj" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"tuvwxyz.1234j"}}) == 14895601873741973071ULL, "name != tuvwxyz.1234j" );

   eosio_assert( static_cast<uint64_t>(name::raw{name{"111111111111j"}}) == 595056260442243615ULL, "name != 111111111111j" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"555555555555j"}}) == 2975281302211218015ULL, "name != 555555555555j" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"aaaaaaaaaaaaj"}}) == 3570337562653461615ULL, "name != aaaaaaaaaaaaj" );
   eosio_assert( static_cast<uint64_t>(name::raw{name{"zzzzzzzzzzzzj"}}) == 18446744073709551615ULL, "name != zzzzzzzzzzzzj" );
     
   // ------------------------------------------------------------------------------------------------------------------------------

   eosio_assert( static_cast<bool>(name{"1"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"5"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"a"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"z"}) == true, "name != true" );

   eosio_assert( static_cast<bool>(name{"abc"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"123"}) == true, "name != true" );

   eosio_assert( static_cast<bool>(name{".abc"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{".........abc"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"123."}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"123........."}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{".a.b.c.1.2.3."}) == true, "name != true" );
   
   eosio_assert( static_cast<bool>(name{"abc.123"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"123.abc"}) == true, "name != true" );

   eosio_assert( static_cast<bool>(name{"12345abcdefgj"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"hijklmnopqrsj"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"tuvwxyz.1234j"}) == true, "name != true" );

   eosio_assert( static_cast<bool>(name{"111111111111j"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"555555555555j"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"aaaaaaaaaaaaj"}) == true, "name != true" );
   eosio_assert( static_cast<bool>(name{"zzzzzzzzzzzzj"}) == true, "name != true" );

   eosio_assert( static_cast<bool>(name{0}) == false, "name != false" );

   // ------------------------------------------------------------------------------------------------------------------------------
   // print_f("%", );

   char buffer[13]{};
   
   std::string test_str{"1"};
   name{test_str}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != 1" );
   name{test_str = "5"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != 5" );
   name{test_str = "a"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != a" );
   name{test_str = "z"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != z" );

   name{test_str = "abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != abc" );
   name{test_str = "123"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != 123" );

   name{test_str = ".abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != .abc" );
   name{test_str = ".........abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != .........abc" );
   name{test_str = "123."}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != 123." );
   name{test_str = "123........."}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != 123........." );
   name{test_str = ".a.b.c.1.2.3."}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != .a.b.c.1.2.3." );

   name{test_str = "abc.123"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != abc.123" );
   name{test_str = "123.abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != 123.abc" );

   name{test_str = "12345abcdefgj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != 12345abcdefgj" );
   name{test_str = "hijklmnopqrsj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != hijklmnopqrsj" );
   name{test_str = "tuvwxyz.1234j"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != tuvwxyz.1234j" );

   name{test_str = "111111111111j"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != 111111111111j" );
   name{test_str = "555555555555j"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != 555555555555j" );
   name{test_str = "aaaaaaaaaaaaj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != aaaaaaaaaaaaj" );
   name{test_str = "zzzzzzzzzzzzj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   eosio_assert( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())) == 0, "name != zzzzzzzzzzzzj" );

   // char buffer[13]{};

   // name{"1"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "1") == 0, "name != 1" );
   // name{"5"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "5") == 0, "name != 5" );
   // name{"a"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "a") == 0, "name != a" );
   // name{"z"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "z") == 0, "name != z" );

   // name{"abc"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "abc") == 0, "name != abc" );
   // name{"123"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "123") == 0, "name != 123" );

   // name{".abc"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, ".abc") == 0, "name != .abc" );
   // name{"123."}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "123.") == 0, "name != 123." );

   // name{"abc.123"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "abc.123") == 0, "name != abc.123" );
   // name{"123.abc"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "123.abc") == 0, "name != 123.abc" );

   // name{"12345abcdefgj"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "12345abcdefgj") == 0, "name != 12345abcdefgj" );
   // name{"hijklmnopqrsj"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "hijklmnopqrsj") == 0, "name != hijklmnopqrsj" );
   // name{"tuvwxyz.1234j"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "tuvwxyz.1234j") == 0, "name != tuvwxyz.1234j" );

   // name{"111111111111j"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "111111111111j") == 0, "name != 111111111111j" );
   // name{"555555555555j"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "555555555555j") == 0, "name != 555555555555j" );
   // name{"aaaaaaaaaaaaj"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "aaaaaaaaaaaaj") == 0, "name != aaaaaaaaaaaaj" );
   // name{"zzzzzzzzzzzzj"}.write_as_string(buffer, buffer + sizeof(buffer));
   // print_f( "%\n", buffer );
   // eosio_assert( strcmp(buffer, "zzzzzzzzzzzzj") == 0, "name != zzzzzzzzzzzzj" );

   // ------------------------------------------------------------------------------------------------------------------------------
   
   // eosio_assert( symbol_code{}.raw() == 0ULL, "symbol_code != 0ULL" );
   // eosio_assert( symbol_code{0ULL}.raw() == 0ULL, "symbol_code != 0ULL" );
   // eosio_assert( symbol_code{1ULL}.raw() == 1ULL, "symbol_code != 1ULL" );
   // eosio_assert( symbol_code{18446744073709551615ULL}.raw() == 18446744073709551615ULL, "symbol_code != 18446744073709551615ULL" );

   // eosio_assert( symbol_code{"A"}.raw() == 65ULL, "symbol_code != 65ULL" );
   // eosio_assert( symbol_code{"Z"}.raw() == 90ULL, "symbol_code != 90ULL" );
   // eosio_assert( symbol_code{"AAAAAAA"}.raw() == 18367622009667905ULL, "symbol_code != 18367622009667905ULL" );
   // eosio_assert( symbol_code{"ZZZZZZZ"}.raw() == 25432092013386330ULL, "symbol_code != 25432092013386330ULL" );

   // symbol_code s{"ZZZZZZZ"};
   // eosio::print_f("%\n",s.raw());
   
   // eosio_assert( symbol{}.raw() == 0ULL, "symbol != 0ULL" );
   // eosio_assert( symbol{0ULL}.raw() == 0ULL, "symbol != 0ULL" );
   // eosio_assert( symbol{1ULL}.raw() == 1ULL, "symbol != 1ULL" );
   // eosio_assert( symbol{18446744073709551615ULL}.raw() == 18446744073709551615ULL, "symbol != 18446744073709551615ULL" );

   // eosio_assert( symbol{1}.raw() == 576460752303423488ULL, "symbol != 1" );
   // eosio_assert( symbol{5}.raw() == 2882303761517117440ULL, "symbol != 5" );
   // eosio_assert( symbol{"a"}.raw() == 3458764513820540928ULL, "symbol != a" );
   // eosio_assert( symbol{"z"}.raw() == 17870283321406128128ULL, "symbol != z" );

   // eosio_assert( symbol{"abc"}.raw() == 3589368903014285312ULL, "symbol != abc" );
   // eosio_assert( symbol{"123"}.raw() == 614178399182651392ULL, "symbol != 123" );

   // eosio_assert( symbol{".abc"}.raw() == 112167778219196416ULL, "symbol != .abc" );
   // eosio_assert( symbol{"123."}.raw() == 614178399182651392ULL, "symbol != 123." );
   
   // eosio_assert( symbol{"abc.123"}.raw() == 3589369488740450304ULL, "symbol != abc.123" );
   // eosio_assert( symbol{"123.abc"}.raw() == 614181822271586304ULL, "symbol != 123.abc" );

   // eosio_assert( symbol{"12345abcdefgj"}.raw() == 614251623682315983ULL, "symbol != 12345abcdefgj" );
   // eosio_assert( symbol{"hijklmnopqrsj"}.raw() == 7754926748989239183ULL, "symbol != hijklmnopqrsj" );
   // eosio_assert( symbol{"tuvwxyz.1234j"}.raw() == 14895601873741973071ULL, "symbol != tuvwxyz.1234j" );

   // eosio_assert( symbol{"111111111111j"}.raw() == 595056260442243615ULL, "symbol != 111111111111j" );
   // eosio_assert( symbol{"555555555555j"}.raw() == 2975281302211218015ULL, "symbol != 555555555555j" );
   // eosio_assert( symbol{"aaaaaaaaaaaaj"}.raw() == 3570337562653461615ULL, "symbol != aaaaaaaaaaaaj" );
   // eosio_assert( symbol{"zzzzzzzzzzzzj"}.raw() == 18446744073709551615ULL, "symbol != zzzzzzzzzzzzj" );
   // eosio::symbol n(18446744073709551615ULL);
   // eosio::print_f("%\n",n.raw());
   
   // return 0;
}
