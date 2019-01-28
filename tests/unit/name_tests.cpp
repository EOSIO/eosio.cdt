#include <eosio/native/tester.hpp>
#include <eosiolib/name.hpp>

using eosio::name;

const uint64_t u64min = std::numeric_limits<uint64_t>::min(); // 0ULL
const uint64_t u64max = std::numeric_limits<uint64_t>::max(); // 18446744073709551615ULL

// Definitions in `eosio.cdt/libraries/eosiolib/name.hpp`
EOSIO_TEST_BEGIN(name_type_test)
   silence_output(true);

   // ------------
   // constructors

   /// constexpr name()
   REQUIRE_EQUAL( name{}.value, 0ULL )

   /// constexpr explicit name(uint64_t)
   REQUIRE_EQUAL( name{0ULL}.value, 0ULL )
   REQUIRE_EQUAL( name{1ULL}.value, 1ULL )
   REQUIRE_EQUAL( name{u64max}.value, u64max )

   /// constexpr explicit name(name::raw)
   REQUIRE_EQUAL( name{name::raw{0ULL}}.value, 0ULL )
   REQUIRE_EQUAL( name{name::raw{1ULL}}.value, 1ULL )
   REQUIRE_EQUAL( name{name::raw{u64max}}.value, u64max )

   // Note that these are the exact `uint64_t` value representations of the given string
   /// constexpr explicit name(std::string_view)
   REQUIRE_EQUAL( name{"1"}.value, 576460752303423488ULL )
   REQUIRE_EQUAL( name{"5"}.value, 2882303761517117440ULL )
   REQUIRE_EQUAL( name{"a"}.value, 3458764513820540928ULL )
   REQUIRE_EQUAL( name{"z"}.value, 17870283321406128128ULL )

   REQUIRE_EQUAL( name{"abc"}.value, 3589368903014285312ULL )
   REQUIRE_EQUAL( name{"123"}.value, 614178399182651392ULL )

   REQUIRE_EQUAL( name{".abc"}.value, 112167778219196416ULL )
   REQUIRE_EQUAL( name{".........abc"}.value, 102016ULL )
   REQUIRE_EQUAL( name{"123."}.value, 614178399182651392ULL )
   REQUIRE_EQUAL( name{"123........."}.value, 614178399182651392ULL )
   REQUIRE_EQUAL( name{".a.b.c.1.2.3."}.value, 108209673814966320ULL )
   
   REQUIRE_EQUAL( name{"abc.123"}.value, 3589369488740450304ULL )
   REQUIRE_EQUAL( name{"123.abc"}.value, 614181822271586304ULL )

   REQUIRE_EQUAL( name{"12345abcdefgj"}.value, 614251623682315983ULL )
   REQUIRE_EQUAL( name{"hijklmnopqrsj"}.value, 7754926748989239183ULL )
   REQUIRE_EQUAL( name{"tuvwxyz.1234j"}.value, 14895601873741973071ULL )

   REQUIRE_EQUAL( name{"111111111111j"}.value, 595056260442243615ULL )
   REQUIRE_EQUAL( name{"555555555555j"}.value, 2975281302211218015ULL )
   REQUIRE_EQUAL( name{"aaaaaaaaaaaaj"}.value, 3570337562653461615ULL )
   REQUIRE_EQUAL( name{"zzzzzzzzzzzzj"}.value, u64max )

   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"-1"};}) )
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"0"};}) )
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"6"};}) )
   REQUIRE_ASSERT( "thirteenth character in name cannot be a letter that comes after j", ([]() {name{"111111111111k"};}) )
   REQUIRE_ASSERT( "thirteenth character in name cannot be a letter that comes after j", ([]() {name{"zzzzzzzzzzzzk"};}) )
   REQUIRE_ASSERT( "string is too long to be a valid name", ([]() {name{"12345abcdefghj"};}) )

   // --------------------------------------------
   // static constexpr uint8_t char_to_value(char)
   char c{'.'};
   uint8_t expected_value{}; // Will increment to the expected correct value in the set [0,32)
   REQUIRE_EQUAL( name::char_to_value(c), expected_value )
   ++expected_value;

   for(c = '1'; c <= '5'; ++c ) {
      REQUIRE_EQUAL( name::char_to_value(c), expected_value )
      ++expected_value;
   }

   for(c = 'a'; c <= 'z'; ++c ) {
      REQUIRE_EQUAL( name::char_to_value(c), expected_value )
      ++expected_value;
   }
   
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'-'});}) )
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'/'});}) )
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'6'});}) )
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'A'});}) )
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'Z'});}) )
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'`'});}) )
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name::char_to_value(char{'{'});}) );  

   // -------------------------------
   // constexpr uint8_t length()cosnt
   REQUIRE_EQUAL( name{""}.length(), 0 )
   REQUIRE_EQUAL( name{"e"}.length(), 1 )
   REQUIRE_EQUAL( name{"eo"}.length(), 2 )
   REQUIRE_EQUAL( name{"eos"}.length(), 3 )
   REQUIRE_EQUAL( name{"eosi"}.length(), 4 )
   REQUIRE_EQUAL( name{"eosio"}.length(), 5 )
   REQUIRE_EQUAL( name{"eosioa"}.length(), 6 )
   REQUIRE_EQUAL( name{"eosioac"}.length(), 7 )
   REQUIRE_EQUAL( name{"eosioacc"}.length(), 8 )
   REQUIRE_EQUAL( name{"eosioacco"}.length(), 9 )
   REQUIRE_EQUAL( name{"eosioaccou"}.length(), 10 )
   REQUIRE_EQUAL( name{"eosioaccoun"}.length(), 11 )
   REQUIRE_EQUAL( name{"eosioaccount"}.length(), 12 )
   REQUIRE_EQUAL( name{"eosioaccountj"}.length(), 13 )

   REQUIRE_ASSERT( "string is too long to be a valid name", ([]() {name{"12345abcdefghj"}.length();}) )
   
   // ----------------------------
   // constexpr name suffix()const
   REQUIRE_EQUAL( name{".eosioaccounj"}.suffix(), name{"eosioaccounj"} )
   REQUIRE_EQUAL( name{"e.osioaccounj"}.suffix(), name{"osioaccounj"} )
   REQUIRE_EQUAL( name{"eo.sioaccounj"}.suffix(), name{"sioaccounj"} )
   REQUIRE_EQUAL( name{"eos.ioaccounj"}.suffix(), name{"ioaccounj"} )
   REQUIRE_EQUAL( name{"eosi.oaccounj"}.suffix(), name{"oaccounj"} )
   REQUIRE_EQUAL( name{"eosio.accounj"}.suffix(), name{"accounj"} )
   REQUIRE_EQUAL( name{"eosioa.ccounj"}.suffix(), name{"ccounj"} )
   REQUIRE_EQUAL( name{"eosioac.counj"}.suffix(), name{"counj"} )
   REQUIRE_EQUAL( name{"eosioacc.ounj"}.suffix(), name{"ounj"} )
   REQUIRE_EQUAL( name{"eosioacco.unj"}.suffix(), name{"unj"} )
   REQUIRE_EQUAL( name{"eosioaccou.nj"}.suffix(), name{"nj"} )
   REQUIRE_EQUAL( name{"eosioaccoun.j"}.suffix(), name{"j"} )

   // Note that this case is ignored because '.' characters at the end of a name are ignored
   // Make functions perfect mirrors of eachother (01/07/2019)
   // `print_f("Value of suffix:          %\n", name{"eosioaccounj."}.suffix() );`
   // `print_f("Value of expected suffix: \n" );`
   // `eosio_assert( name{"eosioaccounj."}.suffix() == name{""}, "name.suffix() != \"\"" );`

   REQUIRE_EQUAL( name{"e.o.s.i.o.a.c"}.suffix(), name{"c"} )
   REQUIRE_EQUAL( name{"eos.ioa.cco"}.suffix(), name{"cco"} )

   // -----------------------------
   // constexpr operator raw()const
   REQUIRE_EQUAL( name{"1"}.operator name::raw(), static_cast<name::raw>(576460752303423488ULL) )
   REQUIRE_EQUAL( name{"5"}.operator name::raw(), static_cast<name::raw>(2882303761517117440ULL) )
   REQUIRE_EQUAL( name{"a"}.operator name::raw(), static_cast<name::raw>(3458764513820540928ULL) )
   REQUIRE_EQUAL( name{"z"}.operator name::raw(), static_cast<name::raw>(17870283321406128128ULL) )

   REQUIRE_EQUAL( name{"abc"}.operator name::raw(), static_cast<name::raw>(3589368903014285312ULL) )
   REQUIRE_EQUAL( name{"123"}.operator name::raw(), static_cast<name::raw>(614178399182651392ULL) )

   REQUIRE_EQUAL( name{".abc"}.operator name::raw(), static_cast<name::raw>(112167778219196416ULL) )
   REQUIRE_EQUAL( name{".........abc"}.operator name::raw(), static_cast<name::raw>(102016ULL) )
   REQUIRE_EQUAL( name{"123."}.operator name::raw(), static_cast<name::raw>(614178399182651392ULL) )
   REQUIRE_EQUAL( name{"123........."}.operator name::raw(), static_cast<name::raw>(614178399182651392ULL) )
   REQUIRE_EQUAL( name{".a.b.c.1.2.3."}.operator name::raw(), static_cast<name::raw>(108209673814966320ULL) )

   REQUIRE_EQUAL( name{"abc.123"}.operator name::raw(), static_cast<name::raw>(3589369488740450304ULL) )
   REQUIRE_EQUAL( name{"123.abc"}.operator name::raw(), static_cast<name::raw>(614181822271586304ULL) )

   REQUIRE_EQUAL( name{"12345abcdefgj"}.operator name::raw(), static_cast<name::raw>(614251623682315983ULL) )
   REQUIRE_EQUAL( name{"hijklmnopqrsj"}.operator name::raw(), static_cast<name::raw>(7754926748989239183ULL) )
   REQUIRE_EQUAL( name{"tuvwxyz.1234j"}.operator name::raw(), static_cast<name::raw>(14895601873741973071ULL) )

   REQUIRE_EQUAL( name{"111111111111j"}.operator name::raw(), static_cast<name::raw>(595056260442243615ULL) )
   REQUIRE_EQUAL( name{"555555555555j"}.operator name::raw(), static_cast<name::raw>(2975281302211218015ULL) )
   REQUIRE_EQUAL( name{"aaaaaaaaaaaaj"}.operator name::raw(), static_cast<name::raw>(3570337562653461615ULL) )
   REQUIRE_EQUAL( name{"zzzzzzzzzzzzj"}.operator name::raw(), static_cast<name::raw>(u64max) )
     
   // ---------------------------------------
   // constexpr explicit operator bool()const
   // Note that I must be explicit about calling the operator because it is defined as `explicit`
   REQUIRE_EQUAL( name{0}.operator bool(), false )
   REQUIRE_EQUAL( name{1}.operator bool(), true )
   REQUIRE_EQUAL( !name{0}.operator bool(), true )
   REQUIRE_EQUAL( !name{1}.operator bool(), false )

   REQUIRE_EQUAL( name{""}.operator bool(), false )
   REQUIRE_EQUAL( name{"1"}.operator bool(), true )
   REQUIRE_EQUAL( !name{""}.operator bool(), true )
   REQUIRE_EQUAL( !name{"1"}.operator bool(), false )

   // ----------------------------------------
   // char* write_as_string(char*, char*)const
   char buffer[13]{};
   
   std::string test_str{"1"};
   name{test_str}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = "5"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = "a"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = "z"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )

   name{test_str = "abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = "123"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )

   // Note that any '.' characters at the end of a name are ignored
   name{test_str = ".abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = ".........abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = "123."}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp("123", buffer, 3), 0 )
   name{test_str = "123........."}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp("123", buffer, 3), 0 )
   name{test_str = ".a.b.c.1.2.3."}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(".a.b.c.1.2.3", buffer, 12), 0 )

   name{test_str = "abc.123"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = "123.abc"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )

   name{test_str = "12345abcdefgj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = "hijklmnopqrsj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = "tuvwxyz.1234j"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )

   name{test_str = "111111111111j"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = "555555555555j"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = "aaaaaaaaaaaaj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )
   name{test_str = "zzzzzzzzzzzzj"}.write_as_string( buffer, buffer + sizeof(buffer) );
   REQUIRE_EQUAL( memcmp(test_str.c_str(), buffer, strlen(test_str.c_str())), 0 )

   // ----------------------------
   // std::string to_string()const
   REQUIRE_EQUAL( name{"1"}.to_string(), "1" )
   REQUIRE_EQUAL( name{"5"}.to_string(), "5" )
   REQUIRE_EQUAL( name{"a"}.to_string(), "a" )
   REQUIRE_EQUAL( name{"z"}.to_string(), "z" )

   REQUIRE_EQUAL( name{"abc"}.to_string(), "abc" )
   REQUIRE_EQUAL( name{"123"}.to_string(), "123" )

   REQUIRE_EQUAL( name{".abc"}.to_string(), ".abc" )
   REQUIRE_EQUAL( name{".........abc"}.to_string(), ".........abc" )
   REQUIRE_EQUAL( name{"123."}.to_string(), "123" )
   REQUIRE_EQUAL( name{"123........."}.to_string(), "123" )
   REQUIRE_EQUAL( name{".a.b.c.1.2.3."}.to_string(), ".a.b.c.1.2.3" )
   
   REQUIRE_EQUAL( name{"abc.123"}.to_string(), "abc.123" )
   REQUIRE_EQUAL( name{"123.abc"}.to_string(), "123.abc" )

   REQUIRE_EQUAL( name{"12345abcdefgj"}.to_string(), "12345abcdefgj" )
   REQUIRE_EQUAL( name{"hijklmnopqrsj"}.to_string(), "hijklmnopqrsj" )
   REQUIRE_EQUAL( name{"tuvwxyz.1234j"}.to_string(), "tuvwxyz.1234j" )

   REQUIRE_EQUAL( name{"111111111111j"}.to_string(), "111111111111j" )
   REQUIRE_EQUAL( name{"555555555555j"}.to_string(), "555555555555j" )
   REQUIRE_EQUAL( name{"aaaaaaaaaaaaj"}.to_string(), "aaaaaaaaaaaaj" )
   REQUIRE_EQUAL( name{"zzzzzzzzzzzzj"}.to_string(), "zzzzzzzzzzzzj" )

   // ----------------------------------------------------------
   // friend constexpr bool operator==(const name&, const name&)
   REQUIRE_EQUAL( name{"1"} == name{"1"}, true )
   REQUIRE_EQUAL( name{"5"} == name{"5"}, true )
   REQUIRE_EQUAL( name{"a"} == name{"a"}, true )
   REQUIRE_EQUAL( name{"z"} == name{"z"}, true )

   REQUIRE_EQUAL( name{"abc"} == name{"abc"}, true )
   REQUIRE_EQUAL( name{"123"} == name{"123"}, true )

   REQUIRE_EQUAL( name{".abc"} == name{".abc"}, true )
   REQUIRE_EQUAL( name{".........abc"} == name{".........abc"}, true )
   REQUIRE_EQUAL( name{"123."} == name{"123"}, true )
   REQUIRE_EQUAL( name{"123........."} == name{"123"}, true )
   REQUIRE_EQUAL( name{".a.b.c.1.2.3."} == name{".a.b.c.1.2.3"}, true )
   
   REQUIRE_EQUAL( name{"abc.123"} == name{"abc.123"}, true )
   REQUIRE_EQUAL( name{"123.abc"} == name{"123.abc"}, true )

   REQUIRE_EQUAL( name{"12345abcdefgj"} == name{"12345abcdefgj"}, true )
   REQUIRE_EQUAL( name{"hijklmnopqrsj"} == name{"hijklmnopqrsj"}, true )
   REQUIRE_EQUAL( name{"tuvwxyz.1234j"} == name{"tuvwxyz.1234j"}, true )

   REQUIRE_EQUAL( name{"111111111111j"} == name{"111111111111j"}, true )
   REQUIRE_EQUAL( name{"555555555555j"} == name{"555555555555j"}, true )
   REQUIRE_EQUAL( name{"aaaaaaaaaaaaj"} == name{"aaaaaaaaaaaaj"}, true )
   REQUIRE_EQUAL( name{"zzzzzzzzzzzzj"} == name{"zzzzzzzzzzzzj"}, true )

   // -----------------------------------------------------------
   // friend constexpr bool operator!=(const name&, const name&)
   REQUIRE_EQUAL( name{"1"} != name{}, true )
   REQUIRE_EQUAL( name{"5"} != name{}, true )
   REQUIRE_EQUAL( name{"a"} != name{}, true )
   REQUIRE_EQUAL( name{"z"} != name{}, true )

   REQUIRE_EQUAL( name{"abc"} != name{}, true )
   REQUIRE_EQUAL( name{"123"} != name{}, true )

   REQUIRE_EQUAL( name{".abc"} != name{}, true )
   REQUIRE_EQUAL( name{".........abc"} != name{}, true )
   REQUIRE_EQUAL( name{"123."} != name{}, true )
   REQUIRE_EQUAL( name{"123........."} != name{}, true )
   REQUIRE_EQUAL( name{".a.b.c.1.2.3."} != name{}, true )
   
   REQUIRE_EQUAL( name{"abc.123"} != name{}, true )
   REQUIRE_EQUAL( name{"123.abc"} != name{}, true )

   REQUIRE_EQUAL( name{"12345abcdefgj"} != name{}, true )
   REQUIRE_EQUAL( name{"hijklmnopqrsj"} != name{}, true )
   REQUIRE_EQUAL( name{"tuvwxyz.1234j"} != name{}, true )

   REQUIRE_EQUAL( name{"111111111111j"} != name{}, true )
   REQUIRE_EQUAL( name{"555555555555j"} != name{}, true )
   REQUIRE_EQUAL( name{"aaaaaaaaaaaaj"} != name{}, true )
   REQUIRE_EQUAL( name{"zzzzzzzzzzzzj"} != name{}, true )

   // ---------------------------------------------------------
   // friend constexpr bool operator<(const name&, const name&)
   REQUIRE_EQUAL( name{} < name{"1"}, true )
   REQUIRE_EQUAL( name{} < name{"5"}, true )
   REQUIRE_EQUAL( name{} < name{"a"}, true )
   REQUIRE_EQUAL( name{} < name{"z"}, true )

   REQUIRE_EQUAL( name{} < name{"abc"}, true )
   REQUIRE_EQUAL( name{} < name{"123"}, true )

   REQUIRE_EQUAL( name{} < name{".abc"}, true )
   REQUIRE_EQUAL( name{} < name{".........abc"}, true )
   REQUIRE_EQUAL( name{} < name{"123."}, true )
   REQUIRE_EQUAL( name{} < name{"123........."}, true )
   REQUIRE_EQUAL( name{} < name{".a.b.c.1.2.3."}, true )
   
   REQUIRE_EQUAL( name{} < name{"abc.123"}, true )
   REQUIRE_EQUAL( name{} < name{"123.abc"}, true )

   REQUIRE_EQUAL( name{} < name{"12345abcdefgj"}, true )
   REQUIRE_EQUAL( name{} < name{"hijklmnopqrsj"}, true )
   REQUIRE_EQUAL( name{} < name{"tuvwxyz.1234j"}, true )

   REQUIRE_EQUAL( name{} < name{"111111111111j"}, true )
   REQUIRE_EQUAL( name{} < name{"555555555555j"}, true )
   REQUIRE_EQUAL( name{} < name{"aaaaaaaaaaaaj"}, true )
   REQUIRE_EQUAL( name{} < name{"zzzzzzzzzzzzj"}, true )

   // ------------------------------------
   // inline constexpr name operator""_n()
   REQUIRE_EQUAL( name{} == ""_n, true )
   
   REQUIRE_EQUAL( name{"1"} == "1"_n, true )
   REQUIRE_EQUAL( name{"5"} == "5"_n, true )
   REQUIRE_EQUAL( name{"a"} == "a"_n, true )
   REQUIRE_EQUAL( name{"z"} == "z"_n, true )

   REQUIRE_EQUAL( name{"abc"} == "abc"_n, true )
   REQUIRE_EQUAL( name{"123"} == "123"_n, true )

   REQUIRE_EQUAL( name{".abc"} == ".abc"_n, true )
   REQUIRE_EQUAL( name{".........abc"} == ".........abc"_n, true )
   REQUIRE_EQUAL( name{"123."} == "123."_n, true )
   REQUIRE_EQUAL( name{"123........."} == "123........."_n, true )
   REQUIRE_EQUAL( name{".a.b.c.1.2.3."} == ".a.b.c.1.2.3."_n, true )
   
   REQUIRE_EQUAL( name{"abc.123"} == "abc.123"_n, true )
   REQUIRE_EQUAL( name{"123.abc"} == "123.abc"_n, true )

   REQUIRE_EQUAL( name{"12345abcdefgj"} == "12345abcdefgj"_n, true )
   REQUIRE_EQUAL( name{"hijklmnopqrsj"} == "hijklmnopqrsj"_n, true )
   REQUIRE_EQUAL( name{"tuvwxyz.1234j"} == "tuvwxyz.1234j"_n, true )

   REQUIRE_EQUAL( name{"111111111111j"} == "111111111111j"_n, true )
   REQUIRE_EQUAL( name{"555555555555j"} == "555555555555j"_n, true )
   REQUIRE_EQUAL( name{"aaaaaaaaaaaaj"} == "aaaaaaaaaaaaj"_n, true )
   REQUIRE_EQUAL( name{"zzzzzzzzzzzzj"} == "zzzzzzzzzzzzj"_n, true )

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(name_type_test);
   return has_failed();
}
