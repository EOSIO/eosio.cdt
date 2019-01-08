#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

#define NATIVE_NAME
#define NATIVE_SYMBOL
#define NATIVE_ASSET

using namespace eosio;
using namespace eosio::native;

// Defined in `eosio.cdt/libraries/eosiolib/name.hpp`
EOSIO_TEST_BEGIN(name_type_test)
   // ------------
   // constructors
   REQUIRE_EQUAL( name{}.value, 0ULL );

   REQUIRE_EQUAL( name{0ULL}.value, 0ULL );
   REQUIRE_EQUAL( name{1ULL}.value, 1ULL );
   REQUIRE_EQUAL( name{18446744073709551615ULL}.value, 18446744073709551615ULL );

   REQUIRE_EQUAL( name{name::raw{0ULL}}.value, 0ULL );
   REQUIRE_EQUAL( name{name::raw{1ULL}}.value, 1ULL );
   REQUIRE_EQUAL( name{name::raw{18446744073709551615ULL}}.value, 18446744073709551615ULL );

   REQUIRE_EQUAL( name{"1"}.value, 576460752303423488ULL );
   REQUIRE_EQUAL( name{"5"}.value, 2882303761517117440ULL );
   REQUIRE_EQUAL( name{"a"}.value, 3458764513820540928ULL );
   REQUIRE_EQUAL( name{"z"}.value, 17870283321406128128ULL );

   REQUIRE_EQUAL( name{"abc"}.value, 3589368903014285312ULL );
   REQUIRE_EQUAL( name{"123"}.value, 614178399182651392ULL );

   REQUIRE_EQUAL( name{".abc"}.value, 112167778219196416ULL );
   REQUIRE_EQUAL( name{".........abc"}.value, 102016ULL );
   REQUIRE_EQUAL( name{"123."}.value, 614178399182651392ULL );
   REQUIRE_EQUAL( name{"123........."}.value, 614178399182651392ULL );
   REQUIRE_EQUAL( name{".a.b.c.1.2.3."}.value, 108209673814966320ULL );
   
   REQUIRE_EQUAL( name{"abc.123"}.value, 3589369488740450304ULL );
   REQUIRE_EQUAL( name{"123.abc"}.value, 614181822271586304ULL );

   REQUIRE_EQUAL( name{"12345abcdefgj"}.value, 614251623682315983ULL );
   REQUIRE_EQUAL( name{"hijklmnopqrsj"}.value, 7754926748989239183ULL );
   REQUIRE_EQUAL( name{"tuvwxyz.1234j"}.value, 14895601873741973071ULL );

   REQUIRE_EQUAL( name{"111111111111j"}.value, 595056260442243615ULL );
   REQUIRE_EQUAL( name{"555555555555j"}.value, 2975281302211218015ULL );
   REQUIRE_EQUAL( name{"aaaaaaaaaaaaj"}.value, 3570337562653461615ULL );
   REQUIRE_EQUAL( name{"zzzzzzzzzzzzj"}.value, 18446744073709551615ULL );

   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"-1"};}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"0"};}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"6"};}) );
   REQUIRE_ASSERT( "thirteenth character in name cannot be a letter that comes after j", ([]() {name{"111111111111k"};}) );
   REQUIRE_ASSERT( "thirteenth character in name cannot be a letter that comes after j", ([]() {name{"zzzzzzzzzzzzk"};}) );
   REQUIRE_ASSERT( "string is too long to be a valid name", ([]() {name{"12345abcdefghj"};}) );

   // ---------------
   // `char_to_value`
   char c{'.'};
   uint8_t expected_value{0}; // Will increment to the expected correct value in the set [0,32)
#ifdef NATIVE_NAME
   print_f("Value of name::char_to_value(%): %", c, static_cast<uint64_t>(name::char_to_value(c)) );
   print_f("Value of expected_value:         %", static_cast<uint64_t>(expected_value) );
#endif
   REQUIRE_EQUAL( name::char_to_value(c), expected_value );
   ++expected_value;

   for(c = '1'; c <= '5'; ++c ) {
#ifdef NATIVE_NAME
      print_f("Value of name::char_to_value(%): %", c, static_cast<uint64_t>(name::char_to_value(c)) );
      print_f("Value of expected_value:         %", static_cast<uint64_t>(expected_value) );
#endif
      REQUIRE_EQUAL( name::char_to_value(c), expected_value );
      ++expected_value;
   }

   for(c = 'a'; c <= 'z'; ++c ) {
#ifdef NATIVE_NAME
      print_f("Value of name::char_to_value(%): %", c, static_cast<uint64_t>(name::char_to_value(c)) );
      print_f("Value of expected_value:         %", static_cast<uint64_t>(expected_value) );
#endif
      REQUIRE_EQUAL( name::char_to_value(c), expected_value );
      ++expected_value;
   }
   
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'-'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'/'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'6'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'A'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'Z'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'`'});}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'{'});}) );
   

   // ------
   // length
   REQUIRE_EQUAL( name{""}.length(), 0 );
   REQUIRE_EQUAL( name{"e"}.length(), 1 );
   REQUIRE_EQUAL( name{"eo"}.length(), 2 );
   REQUIRE_EQUAL( name{"eos"}.length(), 3 );
   REQUIRE_EQUAL( name{"eosi"}.length(), 4 );
   REQUIRE_EQUAL( name{"eosio"}.length(), 5 );
   REQUIRE_EQUAL( name{"eosioa"}.length(), 6 );
   REQUIRE_EQUAL( name{"eosioac"}.length(), 7 );
   REQUIRE_EQUAL( name{"eosioacc"}.length(), 8 );
   REQUIRE_EQUAL( name{"eosioacco"}.length(), 9 );
   REQUIRE_EQUAL( name{"eosioaccou"}.length(), 10 );
   REQUIRE_EQUAL( name{"eosioaccoun"}.length(), 11 );
   REQUIRE_EQUAL( name{"eosioaccount"}.length(), 12 );
   REQUIRE_EQUAL( name{"eosioaccountj"}.length(), 13 );

   REQUIRE_ASSERT( "string is too long to be a valid name", ([]() {name{"12345abcdefghj"}.length();}) );
   
   // ------
   // suffix
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{".eosioaccounj"}.suffix() ); // TODO: interesting print behavior; starts newline sometimes
   print_f("Value of expected suffix: eosioaccounj\n" );
#endif
   
   REQUIRE_EQUAL( name{".eosioaccounj"}.suffix(), name{"eosioaccounj"} );
   
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"e.osioaccounj"}.suffix() );
   print_f("Value of expected suffix: osioaccounj\n" );
#endif
   
   REQUIRE_EQUAL( name{"e.osioaccounj"}.suffix(), name{"osioaccounj"} );
   
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"eo.sioaccounj"}.suffix() );
   print_f("Value of expected suffix: sioaccounj\n" );
#endif
   
   REQUIRE_EQUAL( name{"eo.sioaccounj"}.suffix(), name{"sioaccounj"} );
   
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"eos.ioaccounj"}.suffix() );
   print_f("Value of expected suffix: ioaccounj\n" );
#endif
   
   REQUIRE_EQUAL( name{"eos.ioaccounj"}.suffix(), name{"ioaccounj"} );
   
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"eosi.oaccounj"}.suffix() );
   print_f("Value of expected suffix: oaccounj\n" );
#endif
   
   REQUIRE_EQUAL( name{"eosi.oaccounj"}.suffix(), name{"oaccounj"} );
   
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"eosio.accounj"}.suffix() );
   print_f("Value of expected suffix: accounj\n" );
#endif
   
   REQUIRE_EQUAL( name{"eosio.accounj"}.suffix(), name{"accounj"} );
   
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"eosioa.ccounj"}.suffix() );
   print_f("Value of expected suffix: ccounj\n" );
#endif
   
   REQUIRE_EQUAL( name{"eosioa.ccounj"}.suffix(), name{"ccounj"} );
   
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"eosioac.counj"}.suffix() );
   print_f("Value of expected suffix: counj\n" );
#endif
   
   REQUIRE_EQUAL( name{"eosioac.counj"}.suffix(), name{"counj"} );
   
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"eosioacc.ounj"}.suffix() );
   print_f("Value of expected suffix: ounj\n" );
#endif
   
   REQUIRE_EQUAL( name{"eosioacc.ounj"}.suffix(), name{"ounj"} );
   
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"eosioacco.unj"}.suffix() );
   print_f("Value of expected suffix: unj\n" );
#endif
   
   REQUIRE_EQUAL( name{"eosioacco.unj"}.suffix(), name{"unj"} );
   
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"eosioaccou.nj"}.suffix() );
   print_f("Value of expected suffix: nj\n" );
#endif
   
   REQUIRE_EQUAL( name{"eosioaccou.nj"}.suffix(), name{"nj"} );
   
#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"eosioaccoun.j"}.suffix() );
   print_f("Value of expected suffix: j\n" );
#endif
   
   REQUIRE_EQUAL( name{"eosioaccoun.j"}.suffix(), name{"j"} );

// Note that this case is ignored because '.' characters at the end of a name are ignored
// Although, it is my goal to make the functions perfect mirrors of eachother evenutally (01/07/2019)
// #ifdef NATIVE_NAME
//    print_f("Value of suffix:          %\n", name{"eosioaccounj."}.suffix() );
//    print_f("Value of expected suffix: \n" );
// #endif
//    eosio_assert( name{"eosioaccounj."}.suffix() == name{""}, "name.suffix() != \"\"" );

#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"e.o.s.i.o.a.c"}.suffix() );
   print_f("Value of expected suffix: c\n" );
#endif
   
   REQUIRE_EQUAL( name{"e.o.s.i.o.a.c"}.suffix(), name{"c"} );

#ifdef NATIVE_NAME
   print_f("Value of suffix:          %\n", name{"eos.ioa.cco"}.suffix() );
   print_f("Value of expected suffix: cco\n" );
#endif
   
   REQUIRE_EQUAL( name{"eos.ioa.cco"}.suffix(), name{"cco"} );

   // ------------
   // operator raw
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"1"}}), 576460752303423488ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"5"}}), 2882303761517117440ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"a"}}), 3458764513820540928ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"z"}}), 17870283321406128128ULL );

   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"abc"}}), 3589368903014285312ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"123"}}), 614178399182651392ULL );

   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{".abc"}}), 112167778219196416ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{".........abc"}}), 102016ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"123."}}), 614178399182651392ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"123........."}}), 614178399182651392ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{".a.b.c.1.2.3."}}), 108209673814966320ULL );
   
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"abc.123"}}), 3589369488740450304ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"123.abc"}}), 614181822271586304ULL );

   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"12345abcdefgj"}}), 614251623682315983ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"hijklmnopqrsj"}}), 7754926748989239183ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"tuvwxyz.1234j"}}), 14895601873741973071ULL );

   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"111111111111j"}}), 595056260442243615ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"555555555555j"}}), 2975281302211218015ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"aaaaaaaaaaaaj"}}), 3570337562653461615ULL );
   REQUIRE_EQUAL( static_cast<uint64_t>(name::raw{name{"zzzzzzzzzzzzj"}}), 18446744073709551615ULL );
     
   // -------------
   // operator bool
   // Note that I must be explicit about calling the operator because it is defined as `explicit`
   REQUIRE_EQUAL( name{0}.operator bool(), false );
   REQUIRE_EQUAL( name{1}.operator bool(), true );
   REQUIRE_EQUAL( !name{0}, true );
   REQUIRE_EQUAL( !name{1}, false );

   REQUIRE_EQUAL( name{""}.operator bool(), false );
   REQUIRE_EQUAL( name{"1"}.operator bool(), true );
   REQUIRE_EQUAL( !name{""}, true );
   REQUIRE_EQUAL( !name{"1"}, false );

   // ---------------
   // write_as_string
   char buffer[13]{};
   
   std::string test_str{"1"};
   name{test_str}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = "5"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = "a"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = "z"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );

   name{test_str = "abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = "123"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );

   // Note that any '.' characters at the end of a name are ignored
   name{test_str = ".abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = ".........abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = "123."}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp("123", buffer, 3), 0 );
   name{test_str = "123........."}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp("123", buffer, 3), 0 );
   name{test_str = ".a.b.c.1.2.3."}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(".a.b.c.1.2.3", buffer, 12), 0 );

   name{test_str = "abc.123"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = "123.abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );

   name{test_str = "12345abcdefgj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = "hijklmnopqrsj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = "tuvwxyz.1234j"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );

   name{test_str = "111111111111j"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = "555555555555j"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = "aaaaaaaaaaaaj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );
   name{test_str = "zzzzzzzzzzzzj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 );

   // ---------
   // to_string
   REQUIRE_EQUAL( name{"1"}.to_string(), "1" );
   REQUIRE_EQUAL( name{"5"}.to_string(), "5" );
   REQUIRE_EQUAL( name{"a"}.to_string(), "a" );
   REQUIRE_EQUAL( name{"z"}.to_string(), "z" );

   REQUIRE_EQUAL( name{"abc"}.to_string(), "abc" );
   REQUIRE_EQUAL( name{"123"}.to_string(), "123" );

   REQUIRE_EQUAL( name{".abc"}.to_string(), ".abc" );
   REQUIRE_EQUAL( name{".........abc"}.to_string(), ".........abc" );
   REQUIRE_EQUAL( name{"123."}.to_string(), "123" );
   REQUIRE_EQUAL( name{"123........."}.to_string(), "123" );
   REQUIRE_EQUAL( name{".a.b.c.1.2.3."}.to_string(), ".a.b.c.1.2.3" );
   
   REQUIRE_EQUAL( name{"abc.123"}.to_string(), "abc.123" );
   REQUIRE_EQUAL( name{"123.abc"}.to_string(), "123.abc" );

   REQUIRE_EQUAL( name{"12345abcdefgj"}.to_string(), "12345abcdefgj" );
   REQUIRE_EQUAL( name{"hijklmnopqrsj"}.to_string(), "hijklmnopqrsj" );
   REQUIRE_EQUAL( name{"tuvwxyz.1234j"}.to_string(), "tuvwxyz.1234j" );

   REQUIRE_EQUAL( name{"111111111111j"}.to_string(), "111111111111j" );
   REQUIRE_EQUAL( name{"555555555555j"}.to_string(), "555555555555j" );
   REQUIRE_EQUAL( name{"aaaaaaaaaaaaj"}.to_string(), "aaaaaaaaaaaaj" );
   REQUIRE_EQUAL( name{"zzzzzzzzzzzzj"}.to_string(), "zzzzzzzzzzzzj" );

   // ----------
   // operator==
   REQUIRE_EQUAL( name{"1"} == name{"1"}, true );
   REQUIRE_EQUAL( name{"5"} == name{"5"}, true );
   REQUIRE_EQUAL( name{"a"} == name{"a"}, true );
   REQUIRE_EQUAL( name{"z"} == name{"z"}, true );

   REQUIRE_EQUAL( name{"abc"} == name{"abc"}, true );
   REQUIRE_EQUAL( name{"123"} == name{"123"}, true );

   REQUIRE_EQUAL( name{".abc"} == name{".abc"}, true );
   REQUIRE_EQUAL( name{".........abc"} == name{".........abc"}, true );
   REQUIRE_EQUAL( name{"123."} == name{"123"}, true );
   REQUIRE_EQUAL( name{"123........."} == name{"123"}, true );
   REQUIRE_EQUAL( name{".a.b.c.1.2.3."} == name{".a.b.c.1.2.3"}, true );
   
   REQUIRE_EQUAL( name{"abc.123"} == name{"abc.123"}, true );
   REQUIRE_EQUAL( name{"123.abc"} == name{"123.abc"}, true );

   REQUIRE_EQUAL( name{"12345abcdefgj"} == name{"12345abcdefgj"}, true );
   REQUIRE_EQUAL( name{"hijklmnopqrsj"} == name{"hijklmnopqrsj"}, true );
   REQUIRE_EQUAL( name{"tuvwxyz.1234j"} == name{"tuvwxyz.1234j"}, true );

   REQUIRE_EQUAL( name{"111111111111j"} == name{"111111111111j"}, true );
   REQUIRE_EQUAL( name{"555555555555j"} == name{"555555555555j"}, true );
   REQUIRE_EQUAL( name{"aaaaaaaaaaaaj"} == name{"aaaaaaaaaaaaj"}, true );
   REQUIRE_EQUAL( name{"zzzzzzzzzzzzj"} == name{"zzzzzzzzzzzzj"}, true );

   // ----------
   // operator!=
   REQUIRE_EQUAL( name{"1"} != name{0}, true );
   REQUIRE_EQUAL( name{"5"} != name{0}, true );
   REQUIRE_EQUAL( name{"a"} != name{0}, true );
   REQUIRE_EQUAL( name{"z"} != name{0}, true );

   REQUIRE_EQUAL( name{"abc"} != name{0}, true );
   REQUIRE_EQUAL( name{"123"} != name{0}, true );

   REQUIRE_EQUAL( name{".abc"} != name{0}, true );
   REQUIRE_EQUAL( name{".........abc"} != name{0}, true );
   REQUIRE_EQUAL( name{"123."} != name{0}, true );
   REQUIRE_EQUAL( name{"123........."} != name{0}, true );
   REQUIRE_EQUAL( name{".a.b.c.1.2.3."} != name{0}, true );
   
   REQUIRE_EQUAL( name{"abc.123"} != name{0}, true );
   REQUIRE_EQUAL( name{"123.abc"} != name{0}, true );

   REQUIRE_EQUAL( name{"12345abcdefgj"} != name{0}, true );
   REQUIRE_EQUAL( name{"hijklmnopqrsj"} != name{0}, true );
   REQUIRE_EQUAL( name{"tuvwxyz.1234j"} != name{0}, true );

   REQUIRE_EQUAL( name{"111111111111j"} != name{0}, true );
   REQUIRE_EQUAL( name{"555555555555j"} != name{0}, true );
   REQUIRE_EQUAL( name{"aaaaaaaaaaaaj"} != name{0}, true );
   REQUIRE_EQUAL( name{"zzzzzzzzzzzzj"} != name{0}, true );

   // ---------
   // operator<
   REQUIRE_EQUAL( name{0} < name{"1"}, true );
   REQUIRE_EQUAL( name{0} < name{"5"}, true );
   REQUIRE_EQUAL( name{0} < name{"a"}, true );
   REQUIRE_EQUAL( name{0} < name{"z"}, true );

   REQUIRE_EQUAL( name{0} < name{"abc"}, true );
   REQUIRE_EQUAL( name{0} < name{"123"}, true );

   REQUIRE_EQUAL( name{0} < name{".abc"}, true );
   REQUIRE_EQUAL( name{0} < name{".........abc"}, true );
   REQUIRE_EQUAL( name{0} < name{"123."}, true );
   REQUIRE_EQUAL( name{0} < name{"123........."}, true );
   REQUIRE_EQUAL( name{0} < name{".a.b.c.1.2.3."}, true );
   
   REQUIRE_EQUAL( name{0} < name{"abc.123"}, true );
   REQUIRE_EQUAL( name{0} < name{"123.abc"}, true );

   REQUIRE_EQUAL( name{0} < name{"12345abcdefgj"}, true );
   REQUIRE_EQUAL( name{0} < name{"hijklmnopqrsj"}, true );
   REQUIRE_EQUAL( name{0} < name{"tuvwxyz.1234j"}, true );

   REQUIRE_EQUAL( name{0} < name{"111111111111j"}, true );
   REQUIRE_EQUAL( name{0} < name{"555555555555j"}, true );
   REQUIRE_EQUAL( name{0} < name{"aaaaaaaaaaaaj"}, true );
   REQUIRE_EQUAL( name{0} < name{"zzzzzzzzzzzzj"}, true );

   // ------------
   // operator""_n
   REQUIRE_EQUAL( name{0} == ""_n, true );
   
   REQUIRE_EQUAL( name{"1"} == "1"_n, true );
   REQUIRE_EQUAL( name{"5"} == "5"_n, true );
   REQUIRE_EQUAL( name{"a"} == "a"_n, true );
   REQUIRE_EQUAL( name{"z"} == "z"_n, true );

   REQUIRE_EQUAL( name{"abc"} == "abc"_n, true );
   REQUIRE_EQUAL( name{"123"} == "123"_n, true );

   REQUIRE_EQUAL( name{".abc"} == ".abc"_n, true );
   REQUIRE_EQUAL( name{".........abc"} == ".........abc"_n, true );
   REQUIRE_EQUAL( name{"123."} == "123."_n, true );
   REQUIRE_EQUAL( name{"123........."} == "123........."_n, true );
   REQUIRE_EQUAL( name{".a.b.c.1.2.3."} == ".a.b.c.1.2.3."_n, true );
   
   REQUIRE_EQUAL( name{"abc.123"} == "abc.123"_n, true );
   REQUIRE_EQUAL( name{"123.abc"} == "123.abc"_n, true );

   REQUIRE_EQUAL( name{"12345abcdefgj"} == "12345abcdefgj"_n, true );
   REQUIRE_EQUAL( name{"hijklmnopqrsj"} == "hijklmnopqrsj"_n, true );
   REQUIRE_EQUAL( name{"tuvwxyz.1234j"} == "tuvwxyz.1234j"_n, true );

   REQUIRE_EQUAL( name{"111111111111j"} == "111111111111j"_n, true );
   REQUIRE_EQUAL( name{"555555555555j"} == "555555555555j"_n, true );
   REQUIRE_EQUAL( name{"aaaaaaaaaaaaj"} == "aaaaaaaaaaaaj"_n, true );
   REQUIRE_EQUAL( name{"zzzzzzzzzzzzj"} == "zzzzzzzzzzzzj"_n, true );

   silence_output(false);
EOSIO_TEST_END

// // Intrinsics defined in `eos/libraries/chain/wasm_interface.cpp` that involve the `name` type
// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(set_resource_limits_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::set_resource_limits>([](name account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(get_resource_limits_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::get_resource_limits>([](name account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(set_proposed_producers_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::set_proposed_producers>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(is_privileged_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::is_privileged>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(set_privileged_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::set_privileged>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(get_active_producers_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::get_active_producers>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(check_permission_authorization_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::check_permission_authorization>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(get_permission_last_used_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::get_permission_last_used>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(get_account_creation_time_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::get_account_creation_time>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(get_permission_last_used_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::get_permission_last_used>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(require_authorization_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::require_authorization>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(has_authorization_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::has_authorization>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(require_recipient_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::require_recipient>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(is_account_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::is_account>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(send_deferred_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::send_deferred>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// EOSIO_TEST_BEGIN(is_account_test)
// intrinsics::set_intrinsic<intrinsics::is_account>([](uint64_t a) { 
//       if (a == 3)
//          return true;
//       if (a == 4)
//          return true;
//       return false;
//       });

//    CHECK_ASSERT("is not an account", ([]() {
//       eosio_assert(is_account(5), "is not an account");
//       }));
//    CHECK_EQUAL(is_account(3), true);
//    CHECK_EQUAL(is_account(4), true);
//    CHECK_EQUAL(is_account(6), true);
//    REQUIRE_EQUAL(is_account(7), true);
// EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(name_type_test);
   return has_failed();
}
