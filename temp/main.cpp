// eosio-cpp -c main.cpp -fnative; ./a.out

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
   eosio_assert( name{"123."}.value == 614178399182651392ULL, "name != 123." );
   
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
   print_f("Value of expected suffix: eosioaccounj" );
#endif
   eosio_assert( name{".eosioaccounj"}.suffix() == name{"eosioaccounj"}, "name.suffix() != eosioaccounj" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"e.osioaccounj"}.suffix() );
   print_f("Value of expected suffix: osioaccounj" );
#endif
   eosio_assert( name{"e.osioaccounj"}.suffix() == name{"osioaccounj"}, "name.suffix() != osioaccounj" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eo.sioaccounj"}.suffix() );
   print_f("Value of expected suffix: sioaccounj" );
#endif
   eosio_assert( name{"eo.sioaccounj"}.suffix() == name{"sioaccounj"}, "name.suffix() != sioaccounj" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eos.ioaccounj"}.suffix() );
   print_f("Value of expected suffix: ioaccounj" );
#endif
   eosio_assert( name{"eos.ioaccounj"}.suffix() == name{"ioaccounj"}, "name.suffix() != ioaccounj" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosi.oaccounj"}.suffix() );
   print_f("Value of expected suffix: oaccounj" );
#endif
   eosio_assert( name{"eosi.oaccounj"}.suffix() == name{"oaccounj"}, "name.suffix() != oaccounj" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosio.accounj"}.suffix() );
   print_f("Value of expected suffix: accounj" );
#endif
   eosio_assert( name{"eosio.accounj"}.suffix() == name{"accounj"}, "name.suffix() != accounj" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioa.ccounj"}.suffix() );
   print_f("Value of expected suffix: ccounj" );
#endif
   eosio_assert( name{"eosioa.ccounj"}.suffix() == name{"ccounj"}, "name.suffix() != ccounj" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioac.counj"}.suffix() );
   print_f("Value of expected suffix: counj" );
#endif
   eosio_assert( name{"eosioac.counj"}.suffix() == name{"counj"}, "name.suffix() != counj" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioacc.ounj"}.suffix() );
   print_f("Value of expected suffix: ounj" );
#endif
   eosio_assert( name{"eosioacc.ounj"}.suffix() == name{"ounj"}, "name.suffix() != ounj" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioacco.unj"}.suffix() );
   print_f("Value of expected suffix: unj" );
#endif
   eosio_assert( name{"eosioacco.unj"}.suffix() == name{"unj"}, "name.suffix() != unj" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioaccou.nj"}.suffix() );
   print_f("Value of expected suffix: nj" );
#endif
   eosio_assert( name{"eosioaccou.nj"}.suffix() == name{"nj"}, "name.suffix() != nj" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioaccoun.j"}.suffix() );
   print_f("Value of expected suffix: j" );
#endif
   eosio_assert( name{"eosioaccoun.j"}.suffix() == name{"j"}, "name.suffix() != j" );
#ifdef NATIVE
   print_f("Value of suffix:          %\n", name{"eosioaccounj."}.suffix() );
   print_f("Value of expected suffix: " );
#endif
   eosio_assert( name{"eosioaccounj."}.suffix() == name{""}, "name.suffix() != \"\"" );

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
