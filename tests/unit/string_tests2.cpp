/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/tester.hpp>
#include <eosio/datastream.hpp>
#include <eosio/string.hpp>

using std::fill;
using std::move;

using eosio::datastream;
using eosio::string;

// Definitions found in `eosio.cdt/libraries/eosiolib/core/eosio/string.hpp`

EOSIO_TEST_BEGIN(string_test_65)
   static string eostr{"abcdefg"};
   static const char* null_man{nullptr};
   CHECK_ASSERT( "eosio::string::insert", []() {eostr.insert(0, null_man, 1);} )
   CHECK_ASSERT( "eosio::string::insert", []() {eostr.insert(-1, "ooo", 1);} )
EOSIO_TEST_END

//// string& insert(const size_t pos, const string& str)
EOSIO_TEST_BEGIN(string_test_66)
   static string eostr{};
   static const string str{"ooo"};
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ooo"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_67)
   static string eostr{"abc"};
   static const string str{"d"};
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 4 )
   CHECK_EQUAL( eostr.capacity(), 8 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "dabc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_68)
   static string eostr{"abc"};
   static const string str{"def"};
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "defabc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_69)
   static string eostr{"iii"};
   static const string str{"ooo"};
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "oooiii") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_70)
   static string eostr{"iii"};
   static const string str{"ooo"};
   eostr.insert(1, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ioooii") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_71)
   static string eostr{"iii"};
   static const string str{"ooo"};
   eostr.insert(2, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iioooi") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_72)
   static string eostr{"iii"};
   static const string str{"ooo"};
   eostr.insert(3, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iiiooo") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_73)
   static string eostr{"abcdefg"};
   static const string str{"ooo"};
   CHECK_ASSERT( "eosio::string::insert", []() {eostr.insert(-1, str);} )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_74)
   static string eostr{""};
   static string str{""};
   str += "ooo";
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ooo"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_75)
   static string eostr{""};
   eostr += "abc";
   static string str{""};
   str += "d";
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 4 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "dabc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_76)
   static string eostr{""};
   eostr += "abc";
   static string str{""};
   str += "def";
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "defabc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_77)
   static string eostr{""};
   eostr += "iii";
   static string str{""};
   str += "ooo";
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "oooiii") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_78)
   static string eostr{""};
   eostr += "iii";
   static string str{""};
   str += "ooo";
   eostr.insert(1, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ioooii") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_79)
   static string eostr{""};
   eostr += "iii";
   static string str{""};
   str += "ooo";
   eostr.insert(2, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iioooi") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_80)
   static string eostr{""};
   eostr += "iii";
   static string str{""};
   str += "ooo";
   eostr.insert(3, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iiiooo") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_81)
   static string eostr{"abcdefg"};
   static string str{"ooo"};
   CHECK_ASSERT( "eosio::string::insert", []() {eostr.insert(-1, str);} )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_82)
   static string eostr = "hello";
   eostr.insert(0, "0", 1); /// `_capacity` is now 12; `_begin` now holds `std::unique_ptr<char[]>`
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "0hello") , 0 )

   eostr.insert(0, "h", 1);
   CHECK_EQUAL( eostr.size(), 7 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "h0hello") , 0 )
EOSIO_TEST_END

//// string& erase(size_t pos = 0, size_t len = npos)
EOSIO_TEST_BEGIN(string_test_83)
   static string eostr{"abcdefgh"};
   eostr.erase();
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_84)
   static string eostr{"abcdefgh"};
   eostr.erase(0);
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_85)
   static string eostr{"abcdefgh"};
   eostr.erase(0, string::npos);
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_86)
   static string eostr{"abcdefgh"};
   eostr.erase(1, string::npos);
   CHECK_EQUAL( eostr.size(), 1 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "a"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_87)
   static string eostr{"abcdefgh"};
   eostr.erase(2, string::npos);
   CHECK_EQUAL( eostr.size(), 2 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ab"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_88)
   static string eostr{"abcdefgh"};
   eostr.erase(3, string::npos);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_89)
   static string eostr{"abcdefgh"};
   eostr.erase(4, string::npos);
   CHECK_EQUAL( eostr.size(), 4 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcd"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_90)
   static string eostr{"abcdefgh"};
   eostr.erase(5, string::npos);
   CHECK_EQUAL( eostr.size(), 5 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcde"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_91)
   static string eostr{"abcdefgh"};
   eostr.erase(6, string::npos);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_92)
   static string eostr{"abcdefgh"};
   eostr.erase(7, string::npos);
   CHECK_EQUAL( eostr.size(), 7 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefg"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_93)
   static string eostr{"abcdefgh"};
   eostr.erase(8, string::npos);
   CHECK_EQUAL( eostr.size(), 8 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_94)
   static string eostr{"abcdefgh"};
   eostr.erase(8, 0);
   CHECK_EQUAL( eostr.size(), 8 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_95)
   static string eostr{"abcdefg"};
   CHECK_ASSERT( "eosio::string::erase", []() {eostr.erase(-1, 1);} )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_96)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase();
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_97)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(0);
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_98)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(0, string::npos);
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_99)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(1, string::npos);
   CHECK_EQUAL( eostr.size(), 1 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "a"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_100)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(2, string::npos);
   CHECK_EQUAL( eostr.size(), 2 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ab"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_101)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(3, string::npos);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_102)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(4, string::npos);
   CHECK_EQUAL( eostr.size(), 4 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcd"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_103)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(5, string::npos);
   CHECK_EQUAL( eostr.size(), 5 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcde"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_104)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(6, string::npos);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_105)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(7, string::npos);
   CHECK_EQUAL( eostr.size(), 7 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefg"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_106)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(8, string::npos);
   CHECK_EQUAL( eostr.size(), 8 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_107)
   static string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(8, 0);
   CHECK_EQUAL( eostr.size(), 8 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_108)
   static string eostr{"abcdefg"};
   CHECK_ASSERT( "eosio::string::erase", []() {eostr.erase(-1, 1);} )
EOSIO_TEST_END

//// string& append(const char* str)
EOSIO_TEST_BEGIN(string_test_109)
   static string eostr{};
   static const char* str{"iii"};
   eostr.append(str);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iii"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_110)
   static string eostr{"abcdefg"};
   static const char* str{"iii"};
   eostr.append(str);
   CHECK_EQUAL( eostr.size(), 10 )
   CHECK_EQUAL( eostr.capacity(), 20 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgiii"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_111)
   static string eostr{"abcdefg"};
   static const char* null_man{nullptr};
   CHECK_ASSERT( "eosio::string::append", []() {eostr.append(null_man);} )
EOSIO_TEST_END

//// string& append(const string& str)
EOSIO_TEST_BEGIN(string_test_112)
   static string eostr{};
   static const string str{"iii"};
   eostr.append(str);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iii"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_113)
   static string eostr{"abcdefg"};
   static const string str{"iii"};
   eostr.append(str);
   CHECK_EQUAL( eostr.size(), 10 )
   CHECK_EQUAL( eostr.capacity(), 20 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgiii"), 0 )
EOSIO_TEST_END

//// string& operator+=(const char c)
EOSIO_TEST_BEGIN(string_test_114)
   static string eostr0{};
   static string eostr1{"a"};
   static string eostr2{"abcdef"};

   eostr0 += 'c';
   CHECK_EQUAL( eostr0.size(), 1 )
   CHECK_EQUAL( eostr0.capacity(), 2 )
   CHECK_EQUAL( strcmp(eostr0.c_str(), "c"), 0 )

   eostr1 += 'c';
   eostr1 += 'c';
   CHECK_EQUAL( eostr1.size(), 3 )
   CHECK_EQUAL( eostr1.capacity(), 4 )
   CHECK_EQUAL( strcmp(eostr1.c_str(), "acc"), 0 )

   eostr2 += 'c';
   CHECK_EQUAL( eostr2.size(), 7 )
   CHECK_EQUAL( eostr2.capacity(), 14 )
   CHECK_EQUAL( strcmp(eostr2.c_str(), "abcdefc"), 0 )
EOSIO_TEST_END

//// string& operator+=(const char* rhs)
EOSIO_TEST_BEGIN(string_test_115)
   static string eostr0{};
   static string eostr1{"a"};
   static string eostr2{"abcdef"};
   static string eostr3{"abcdef"};

   eostr0 += "c";
   CHECK_EQUAL( eostr0.size(), 1 )
   CHECK_EQUAL( eostr0.capacity(), 2 )
   CHECK_EQUAL( strcmp(eostr0.c_str(), "c"), 0 )

   eostr1 += "c";
   eostr1 += "c";
   CHECK_EQUAL( eostr1.size(), 3 )
   CHECK_EQUAL( eostr1.capacity(), 4 )
   CHECK_EQUAL( strcmp(eostr1.c_str(), "acc"), 0 )

   eostr2 += "c";
   CHECK_EQUAL( eostr2.size(), 7 )
   CHECK_EQUAL( eostr2.capacity(), 14 )
   CHECK_EQUAL( strcmp(eostr2.c_str(), "abcdefc"), 0 )

   eostr3 += "ghijklm";
   CHECK_EQUAL( eostr3.size(), 13 )
   CHECK_EQUAL( eostr3.capacity(), 26 )
   CHECK_EQUAL( strcmp(eostr3.c_str(), "abcdefghijklm"), 0 )
EOSIO_TEST_END

//// string& operator+=(const string& rhs)
EOSIO_TEST_BEGIN(string_test_116)
   static string eostr0{};
   static string eostr1{"a"};
   static string eostr2{"abcdef"};
   static string eostr3{"abcdef"};

   eostr0 += string{"c"};
   CHECK_EQUAL( eostr0.size(), 1 )
   CHECK_EQUAL( eostr0.capacity(), 2 )
   CHECK_EQUAL( strcmp(eostr0.c_str(), "c"), 0 )

   eostr1 += string{"c"};
   eostr1 += string{"c"};
   CHECK_EQUAL( eostr1.size(), 3 )
   CHECK_EQUAL( eostr1.capacity(), 4 )
   CHECK_EQUAL( strcmp(eostr1.c_str(), "acc"), 0 )

   eostr2 += string{"c"};
   CHECK_EQUAL( eostr2.size(), 7 )
   CHECK_EQUAL( eostr2.capacity(), 14 )
   CHECK_EQUAL( strcmp(eostr2.c_str(), "abcdefc"), 0 )

   eostr3 += string{"ghijklm"};
   CHECK_EQUAL( eostr3.size(), 13 )
   CHECK_EQUAL( eostr3.capacity(), 26 )
   CHECK_EQUAL( strcmp(eostr3.c_str(), "abcdefghijklm"), 0 )
EOSIO_TEST_END

//// string& operator+=(const string& s)
EOSIO_TEST_BEGIN(string_test_117)
   static string eostr0{"a"};
   static string eostr1{"b"};
   CHECK_EQUAL( eostr0.size(), 1 )
   eostr0 += eostr1;
   CHECK_EQUAL( eostr0.size(), 2 )
   CHECK_EQUAL( strcmp(eostr0.c_str(), "ab"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_118)
   static string eostr0{"abc"};
   static string eostr1{"def"};
   CHECK_EQUAL( eostr0.size(), 3 )
   eostr0 += eostr1;
   CHECK_EQUAL( eostr0.size(), 6 )
   CHECK_EQUAL( strcmp(eostr0.c_str(), "abcdef"), 0 )
EOSIO_TEST_END

//// inline void print(eosio::string str)
EOSIO_TEST_BEGIN(string_test_119)
   static const string eostr0{""};
   static const string eostr1{"abc"};
   static const string eostr2{"abcdef"};

   CHECK_PRINT( "", [](){ print(eostr0); } )
   CHECK_PRINT( "abc", [](){ print(eostr1); } )
   CHECK_PRINT( "abcdef", [](){ print(eostr2); } )
EOSIO_TEST_END

//// friend bool operator< (const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_120)
   static const string eostr0{"abc"};
   static const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 < eostr0), false )
   CHECK_EQUAL( (eostr1 < eostr1), false )
   CHECK_EQUAL( (eostr0 < eostr1), true )
EOSIO_TEST_END

//// friend bool operator> (const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_121)
   static const string eostr0{"abc"};
   static const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 > eostr0), false )
   CHECK_EQUAL( (eostr1 > eostr1), false )
   CHECK_EQUAL( (eostr0 > eostr1), false )
EOSIO_TEST_END

//// friend bool operator<=(const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_122)
   static const string eostr0{"abc"};
   static const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 <= eostr0), true )
   CHECK_EQUAL( (eostr1 <= eostr1), true )
   CHECK_EQUAL( (eostr0 <= eostr1), true )
EOSIO_TEST_END

//// friend bool operator>=(const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_123)
   static const string eostr0{"abc"};
   static const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 >= eostr0), true )
   CHECK_EQUAL( (eostr1 >= eostr1), true )
   CHECK_EQUAL( (eostr0 >= eostr1), false )
EOSIO_TEST_END

//// friend bool operator==(const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_124)
   static const string eostr0{"abc"};
   static const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 == eostr0), true )
   CHECK_EQUAL( (eostr1 == eostr1), true )
   CHECK_EQUAL( (eostr0 == eostr1), false )
EOSIO_TEST_END

//// friend bool operator!=(const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_125)
   static const string eostr0{"abc"};
   static const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 != eostr0), false )
   CHECK_EQUAL( (eostr1 != eostr1), false )
   CHECK_EQUAL( (eostr0 != eostr1), true )
EOSIO_TEST_END

//// template<typename DataStream>
//// DataStream& operator<<(DataStream& ds, const string& str)
//// DataStream& operator>>(DataStream& ds, string& str)
EOSIO_TEST_BEGIN(string_test_126)
   static constexpr uint16_t buffer_size{256};
   static char datastream_buffer[buffer_size]{}; // Buffer for the datastream to point to
   static char buffer[buffer_size]; // Buffer to compare `datastream_buffer` with
   static datastream<char*> ds{datastream_buffer, buffer_size};

   ds.seekp(0);
   fill(std::begin(datastream_buffer), std::end(datastream_buffer), 0);
   static const string cstr {""};
   static string str{};
   ds << cstr;
   ds.seekp(0);
   ds >> str;
   CHECK_EQUAL( cstr, str )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_127)
   static constexpr uint16_t buffer_size{256};
   static char datastream_buffer[buffer_size]{};
   static char buffer[buffer_size];
   static datastream<char*> ds{datastream_buffer, buffer_size};

   ds.seekp(0);
   fill(std::begin(datastream_buffer), std::end(datastream_buffer), 0);
   static const string cstr {"a"};
   static string str{};
   ds << cstr;
   ds.seekp(0);
   ds >> str;
   CHECK_EQUAL( cstr, str )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_128)
   static constexpr uint16_t buffer_size{256};
   static char datastream_buffer[buffer_size]{};
   static char buffer[buffer_size];
   static datastream<char*> ds{datastream_buffer, buffer_size};

   ds.seekp(0);
   fill(std::begin(datastream_buffer), std::end(datastream_buffer), 0);
   static const string cstr {"abcdefghi"};
   static string str{};
   ds << cstr;
   ds.seekp(0);
   ds >> str;
   CHECK_EQUAL( cstr, str )
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   bool verbose = false;
   if( argc >= 2 && std::strcmp( argv[1], "-v" ) == 0 ) {
      verbose = true;
   }
   silence_output(!verbose);

   EOSIO_TEST(string_test_65)
   EOSIO_TEST(string_test_66)
   EOSIO_TEST(string_test_67)
   EOSIO_TEST(string_test_68)
   EOSIO_TEST(string_test_69)
   EOSIO_TEST(string_test_70)
   EOSIO_TEST(string_test_71)
   EOSIO_TEST(string_test_72)
   EOSIO_TEST(string_test_73)
   EOSIO_TEST(string_test_74)
   EOSIO_TEST(string_test_75)
   EOSIO_TEST(string_test_76)
   EOSIO_TEST(string_test_77)
   EOSIO_TEST(string_test_78)
   EOSIO_TEST(string_test_79)
   EOSIO_TEST(string_test_80)
   EOSIO_TEST(string_test_81)
   EOSIO_TEST(string_test_82)
   EOSIO_TEST(string_test_83)
   EOSIO_TEST(string_test_84)
   EOSIO_TEST(string_test_85)
   EOSIO_TEST(string_test_86)
   EOSIO_TEST(string_test_87)
   EOSIO_TEST(string_test_88)
   EOSIO_TEST(string_test_89)
   EOSIO_TEST(string_test_90)
   EOSIO_TEST(string_test_91)
   EOSIO_TEST(string_test_92)
   EOSIO_TEST(string_test_93)
   EOSIO_TEST(string_test_94)
   EOSIO_TEST(string_test_95)
   EOSIO_TEST(string_test_96)
   EOSIO_TEST(string_test_97)
   EOSIO_TEST(string_test_98)
   EOSIO_TEST(string_test_99)
   EOSIO_TEST(string_test_100)
   EOSIO_TEST(string_test_101)
   EOSIO_TEST(string_test_102)
   EOSIO_TEST(string_test_103)
   EOSIO_TEST(string_test_104)
   EOSIO_TEST(string_test_105)
   EOSIO_TEST(string_test_106)
   EOSIO_TEST(string_test_107)
   EOSIO_TEST(string_test_108)
   EOSIO_TEST(string_test_109)
   EOSIO_TEST(string_test_110)
   EOSIO_TEST(string_test_111)
   EOSIO_TEST(string_test_112)
   EOSIO_TEST(string_test_113)
   EOSIO_TEST(string_test_114)
   EOSIO_TEST(string_test_115)
   EOSIO_TEST(string_test_116)
   EOSIO_TEST(string_test_117)
   EOSIO_TEST(string_test_118)
   EOSIO_TEST(string_test_119)
   EOSIO_TEST(string_test_120)
   EOSIO_TEST(string_test_121)
   EOSIO_TEST(string_test_122)
   EOSIO_TEST(string_test_123)
   EOSIO_TEST(string_test_124)
   EOSIO_TEST(string_test_125)
   EOSIO_TEST(string_test_126)
   EOSIO_TEST(string_test_127)
   EOSIO_TEST(string_test_128)
   return has_failed();
}
