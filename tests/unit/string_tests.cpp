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

//// template <size_t N>
//// string(const char (&str)[N])
EOSIO_TEST_BEGIN(string_test_1)
   static const string eostr0{"a"};
   static const string eostr1{"abcdef"};

   CHECK_EQUAL( eostr0.size(), 1 )
   CHECK_EQUAL( eostr0.capacity(), 1 )
   CHECK_EQUAL( strcmp(eostr0.c_str(), "a"), 0 )

   CHECK_EQUAL( eostr1.size(), 6 )
   CHECK_EQUAL( eostr1.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr1.c_str(), "abcdef"), 0)
EOSIO_TEST_END

//// string()
EOSIO_TEST_BEGIN(string_test_2)
   static const string eostr{};

   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( eostr.capacity(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0)
EOSIO_TEST_END

//// constexpr string(const char* str, const size_t n)
EOSIO_TEST_BEGIN(string_test_3)
   static const char* str0{""};
   static const char* str1{"abc"};
   static const char* str2{"abcdef"};

   static const string eostr0(str0, 0);
   static const string eostr1(str1, 1);
   static const string eostr2(str2, 6);

   CHECK_EQUAL( eostr0.size(), 0 )
   CHECK_EQUAL( eostr0.capacity(), 0 )
   CHECK_EQUAL( strcmp(eostr0.c_str(), ""), 0)

   CHECK_EQUAL( eostr1.size(), 1 )
   CHECK_EQUAL( eostr1.capacity(), 2 )
   CHECK_EQUAL( strcmp(eostr1.c_str(), "a"), 0)

   CHECK_EQUAL( eostr2.size(), 6 )
   CHECK_EQUAL( eostr2.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr2.c_str(), "abcdef"), 0)
EOSIO_TEST_END

//// string(const size_t n, const char c)
EOSIO_TEST_BEGIN(string_test_4)
   static const string eostr0(0, 'c');
   static const string eostr1(1, 'c');
   static const string eostr2(3, 'c');

   CHECK_EQUAL( eostr0.size(), 0 )
   CHECK_EQUAL( eostr0.capacity(), 0 )
   CHECK_EQUAL( strcmp(eostr0.c_str(), ""), 0)

   CHECK_EQUAL( eostr1.size(), 1 )
   CHECK_EQUAL( eostr1.capacity(), 2 )
   CHECK_EQUAL( strcmp(eostr1.c_str(), "c"), 0)

   CHECK_EQUAL( eostr2.size(), 3 )
   CHECK_EQUAL( eostr2.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr2.c_str(), "ccc"), 0)
EOSIO_TEST_END

//// string(const string& str, const size_t pos, const size_t n = string::npos)
EOSIO_TEST_BEGIN(string_test_5)
   static const string eostr{"abcdef"};
   static const string eostr0_sub(eostr, 0, 0);
   static const string eostr1_sub(eostr, 1, 0);
   static const string eostr2_sub(eostr, 0, 1);
   static const string eostr3_sub(eostr, 0, 3);
   static const string eostr4_sub(eostr, 0, 8);
   static const string eostr5_sub(eostr, 0, 7);
   static const string eostr6_sub(eostr, 0, 6);
   static const string eostr7_sub(eostr, 3, 3);
   static const string eostr8_sub(eostr, 3, 2);

   CHECK_EQUAL( eostr0_sub.size(), 0 )
   CHECK_EQUAL( eostr0_sub.capacity(), 0 )
   CHECK_EQUAL( strcmp(eostr0_sub.c_str(), ""), 0)

   CHECK_EQUAL( eostr1_sub.size(), 0 )
   CHECK_EQUAL( eostr1_sub.capacity(), 0 )
   CHECK_EQUAL( strcmp(eostr1_sub.c_str(), ""), 0)

   CHECK_EQUAL( eostr2_sub.size(), 1 )
   CHECK_EQUAL( eostr2_sub.capacity(), 2 )
   CHECK_EQUAL( strcmp(eostr2_sub.c_str(), "a"), 0)

   CHECK_EQUAL( eostr3_sub.size(), 3 )
   CHECK_EQUAL( eostr3_sub.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr3_sub.c_str(), "abc"), 0)

   CHECK_EQUAL( eostr4_sub.size(), 6 )
   CHECK_EQUAL( eostr4_sub.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr4_sub.c_str(), "abcdef"), 0)

   CHECK_EQUAL( eostr5_sub.size(), 6 )
   CHECK_EQUAL( eostr5_sub.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr5_sub.c_str(), "abcdef"), 0)

   CHECK_EQUAL( eostr6_sub.size(), 6 )
   CHECK_EQUAL( eostr6_sub.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr6_sub.c_str(), "abcdef"), 0 )

   CHECK_EQUAL( eostr7_sub.size(), 3 )
   CHECK_EQUAL( eostr7_sub.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr7_sub.c_str(), "def"), 0 )

   CHECK_EQUAL( eostr8_sub.size(), 2 )
   CHECK_EQUAL( eostr8_sub.capacity(), 4 )
   CHECK_EQUAL( strcmp(eostr8_sub.c_str(), "de"), 0)
EOSIO_TEST_END

//// constexpr string(const string& str)
EOSIO_TEST_BEGIN(string_test_6)
   static const string eostr0{""};
   static const string eostr1{"a"};
   static const string eostr2{"abcdef"};
   static const string eostr0_cpy{eostr0};
   static const string eostr1_cpy{eostr1};
   static const string eostr2_cpy{eostr2};

   CHECK_EQUAL( eostr0_cpy.size(), 0 )
   CHECK_EQUAL( eostr0_cpy.capacity(), 0 )
   CHECK_EQUAL( strcmp(eostr0_cpy.c_str(), ""), 0)

   CHECK_EQUAL( eostr1_cpy.size(), 1 )
   CHECK_EQUAL( eostr1_cpy.capacity(), 1 )
   CHECK_EQUAL( strcmp(eostr1_cpy.c_str(), "a"), 0)

   CHECK_EQUAL( eostr2_cpy.size(), 6 )
   CHECK_EQUAL( eostr2_cpy.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr2_cpy.c_str(), "abcdef"), 0)
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_7)
   static string eostr0{""};
   eostr0 += "a";
   static string eostr1{"abc"};
   eostr1 += "def";
   static string eostr0_cpy{eostr0};
   static string eostr1_cpy{eostr1};

   CHECK_EQUAL( eostr0_cpy.size(), 1 )
   CHECK_EQUAL( eostr0_cpy.capacity(), 2 )
   CHECK_EQUAL( strcmp(eostr0_cpy.c_str(), "a"), 0)

   CHECK_EQUAL( eostr1_cpy.size(), 6 )
   CHECK_EQUAL( eostr1_cpy.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr1_cpy.c_str(), "abcdef"), 0)
EOSIO_TEST_END

//// constexpr string(const string&& str)
EOSIO_TEST_BEGIN(string_test_8)
   static string eostr0{""};
   static string eostr1{"a"};
   static string eostr2{"abcdef"};
   static const string eostr0_mv{move(eostr0)};
   static const string eostr1_mv{move(eostr1)};
   static const string eostr2_mv{move(eostr2)};

   CHECK_EQUAL( eostr0_mv.size(), 0 )
   CHECK_EQUAL( eostr0_mv.capacity(), 0 )
   CHECK_EQUAL( strcmp(eostr0.c_str(), ""), 0)

   CHECK_EQUAL( eostr1_mv.size(), 1 )
   CHECK_EQUAL( eostr1_mv.capacity(), 1 )
   CHECK_EQUAL( strcmp(eostr1.c_str(), "a"), 0)

   CHECK_EQUAL( eostr2_mv.size(), 6 )
   CHECK_EQUAL( eostr2_mv.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr2.c_str(), "abcdef"), 0)
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_9)
   static string eostr0{""};
   eostr0 += "a";
   static string eostr1{"abc"};
   eostr1 += "def";
   static string eostr0_cpy{move(eostr0)};
   static string eostr1_cpy{move(eostr1)};

   CHECK_EQUAL( eostr0_cpy.size(), 1 )
   CHECK_EQUAL( eostr0_cpy.capacity(), 2 )
   CHECK_EQUAL( strcmp(eostr0_cpy.c_str(), "a"), 0)

   CHECK_EQUAL( eostr1_cpy.size(), 6 )
   CHECK_EQUAL( eostr1_cpy.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr1_cpy.c_str(), "abcdef"), 0)
EOSIO_TEST_END

//// string& operator=(const string& str);
EOSIO_TEST_BEGIN(string_test_10)
   static const string eostr0{""};
   static const string eostr1{"a"};
   static const string eostr2{"abcdef"};
   static string eostr0_cpy_assig{};
   static string eostr1_cpy_assig{};
   static string eostr2_cpy_assig{};
   eostr0_cpy_assig = eostr0;
   eostr1_cpy_assig = eostr1;
   eostr2_cpy_assig = eostr2;

   CHECK_EQUAL( eostr0_cpy_assig.size(), 0 )
   CHECK_EQUAL( eostr0_cpy_assig.capacity(), 0 )
   CHECK_EQUAL( strcmp(eostr0_cpy_assig.c_str(), ""), 0)

   CHECK_EQUAL( eostr1_cpy_assig.size(), 1 )
   CHECK_EQUAL( eostr1_cpy_assig.capacity(), 1 )
   CHECK_EQUAL( strcmp(eostr1_cpy_assig.c_str(), "a"), 0)

   CHECK_EQUAL( eostr2_cpy_assig.size(), 6 )
   CHECK_EQUAL( eostr2_cpy_assig.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr2_cpy_assig.c_str(), "abcdef"), 0)
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_11)
   static string eostr0{""};
   eostr0 += "a";
   static string eostr1{"abc"};
   eostr1 += "def";
   static string eostr0_cpy_assig{};
   static string eostr1_cpy_assig{};
   eostr0_cpy_assig = eostr0;
   eostr1_cpy_assig = eostr1;

   CHECK_EQUAL( eostr0_cpy_assig.size(), 1 )
   CHECK_EQUAL( eostr0_cpy_assig.capacity(), 2 )
   CHECK_EQUAL( strcmp(eostr0_cpy_assig.c_str(), "a"), 0)

   CHECK_EQUAL( eostr1_cpy_assig.size(), 6 )
   CHECK_EQUAL( eostr1_cpy_assig.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr1_cpy_assig.c_str(), "abcdef"), 0)
EOSIO_TEST_END

//// string& operator=(string&& str)
EOSIO_TEST_BEGIN(string_test_12)
   static string eostr0{""};
   static string eostr1{"a"};
   static string eostr2{"abcdef"};
   static string eostr0_mv_assig{};
   static string eostr1_mv_assig{};
   static string eostr2_mv_assig{};
   eostr0_mv_assig = move(eostr0);
   eostr1_mv_assig = move(eostr1);
   eostr2_mv_assig = move(eostr2);

   CHECK_EQUAL( eostr0_mv_assig.size(), 0 )
   CHECK_EQUAL( eostr0_mv_assig.capacity(), 0 )
   CHECK_EQUAL( strcmp(eostr0_mv_assig.c_str(), ""), 0)

   CHECK_EQUAL( eostr1_mv_assig.size(), 1 )
   CHECK_EQUAL( eostr1_mv_assig.capacity(), 1 )
   CHECK_EQUAL( strcmp(eostr1_mv_assig.c_str(), "a"), 0)

   CHECK_EQUAL( eostr2_mv_assig.size(), 6 )
   CHECK_EQUAL( eostr2_mv_assig.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr2_mv_assig.c_str(), "abcdef"), 0)
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_13)
   static string eostr0{""};
   eostr0 += "a";
   static string eostr1{"abc"};
   eostr1 += "def";
   static string eostr0_mv_assig{};
   static string eostr1_mv_assig{};
   eostr0_mv_assig = move(eostr0);
   eostr1_mv_assig = move(eostr1);

   CHECK_EQUAL( eostr0_mv_assig.size(), 1 )
   CHECK_EQUAL( eostr0_mv_assig.capacity(), 2 )
   CHECK_EQUAL( strcmp(eostr0_mv_assig.c_str(), "a"), 0)

   CHECK_EQUAL( eostr1_mv_assig.size(), 6 )
   CHECK_EQUAL( eostr1_mv_assig.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr1_mv_assig.c_str(), "abcdef"), 0)
EOSIO_TEST_END

//// string& operator=(const char* str)
EOSIO_TEST_BEGIN(string_test_14)
   static string eostr{};
   eostr = "abcdef";

   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 )

   eostr = eostr;
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_15)
   static string eostr{};
   eostr = "";
   eostr += "abcdef";

   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 )

   eostr = eostr;
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 )
EOSIO_TEST_END

//// char& operator[](const size_t n)
EOSIO_TEST_BEGIN(string_test_16)
   static string eostr{"abcdef"};
   CHECK_EQUAL( eostr[0], 'a' )
   CHECK_EQUAL( eostr[5], 'f' )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_17)
   static string eostr{"abc"};
   eostr += "def";
   CHECK_EQUAL( eostr[0], 'a' )
   CHECK_EQUAL( eostr[5], 'f' )
EOSIO_TEST_END

//// const char& operator[](const size_t n) const
EOSIO_TEST_BEGIN(string_test_18)
   static const string eostr{"abcdef"};
   CHECK_EQUAL( eostr[0], 'a' )
   CHECK_EQUAL( eostr[5], 'f' )
EOSIO_TEST_END

//// char& at(const size_t n)
EOSIO_TEST_BEGIN(string_test_19)
   static string eostr{"abcdef"};
   CHECK_EQUAL( eostr.at(0), 'a' )
   CHECK_EQUAL( eostr.at(5), 'f' )

   CHECK_ASSERT( "eosio::string::at", []() {eostr.at(6);} )
EOSIO_TEST_END

//// const char& at(const size_t n) const
EOSIO_TEST_BEGIN(string_test_20)
   static const string eostr{"abcdef"};
   CHECK_EQUAL( eostr.at(0), 'a' )
   CHECK_EQUAL( eostr.at(5), 'f' )

   CHECK_ASSERT( "eosio::string::at const", []() {eostr.at(6);} )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_21)
   static string eostr{""};
   eostr += "abcdef";
   const char c0{eostr.at(0)};
   const char c1{eostr.at(5)};
   CHECK_EQUAL( c0, 'a' )
   CHECK_EQUAL( c1, 'f' )
EOSIO_TEST_END

//// char& front()
EOSIO_TEST_BEGIN(string_test_22)
   static string eostr{"abcdef"};
   CHECK_EQUAL( eostr.front(), 'a' )

   static string empty_str;
   CHECK_EQUAL( eostr.front(), 'a' )
EOSIO_TEST_END

//// const char& front() const
EOSIO_TEST_BEGIN(string_test_23)
   static const string eostr{"abcdef"};
   CHECK_EQUAL( eostr.front(), 'a' )
EOSIO_TEST_END

//// char& back()
EOSIO_TEST_BEGIN(string_test_24)
   static string eostr{"abcdef"};
   CHECK_EQUAL( eostr.back(), 'f' )
EOSIO_TEST_END

//// const char& back() const
EOSIO_TEST_BEGIN(string_test_25)
   static const string eostr{"abcdef"};
   CHECK_EQUAL( eostr.back(), 'f' )
EOSIO_TEST_END

//// char* data()
EOSIO_TEST_BEGIN(string_test_26)
   static string eostr{"abcdef"};
   CHECK_EQUAL( strcmp(eostr.data(), "abcdef"), 0 )

   eostr = "abc";
   CHECK_EQUAL( strcmp(eostr.data(), "abc"), 0 )
EOSIO_TEST_END

//// const char* data() const
EOSIO_TEST_BEGIN(string_test_27)
   static const string eostr{"abcdef"};
   CHECK_EQUAL( strcmp(eostr.data(), "abcdef"), 0 )
EOSIO_TEST_END

//// const char* c_str() const
EOSIO_TEST_BEGIN(string_test_28)
   static string eostr{"abcdef"};
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 )
   CHECK_EQUAL( eostr.c_str()[eostr.size()], '\0' )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_29)
   static string eostr{""};
   eostr += "abcdef";
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 )
   CHECK_EQUAL( eostr.c_str()[eostr.size()], '\0' )
EOSIO_TEST_END

//// char* begin()
EOSIO_TEST_BEGIN(string_test_30)
   static string eostr{"abcdef"};
   char* iter{eostr.begin()};
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), iter), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_31)
   static string eostr{""};
   eostr += "abcdef";
   char* iter{eostr.begin()};
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), iter), 0 )
EOSIO_TEST_END

//// const char* cbegin() const
EOSIO_TEST_BEGIN(string_test_32)
   static const string eostr{"abcdef"};
   const char* iter{eostr.cbegin()};
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), iter), 0 )
EOSIO_TEST_END

//// char* end()
EOSIO_TEST_BEGIN(string_test_33)
   static string eostr{"abcdef"};
   char* iter{eostr.end()};
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str()+eostr.size(), iter), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_34)
   static string eostr{""};
   eostr += "abcdef";
   char* iter{eostr.end()};
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str()+eostr.size(), iter), 0 )
EOSIO_TEST_END

//// const char* cend() const
EOSIO_TEST_BEGIN(string_test_35)
   static const string eostr{"abcdef"};
   const char* iter{eostr.cend()};
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.data()+eostr.size(), iter), 0 )
EOSIO_TEST_END

//// bool string::empty() const
EOSIO_TEST_BEGIN(string_test_36)
   static string eostr{};
   CHECK_EQUAL( eostr.empty(), true )
   eostr += 'c';
   CHECK_EQUAL( eostr.empty(), false )
EOSIO_TEST_END

//// size_t string::size() const
EOSIO_TEST_BEGIN(string_test_37)
   static string eostr{"abcdef"};
   CHECK_EQUAL( eostr.size(), 6 )
   eostr += 'g';
   CHECK_EQUAL( eostr.size(), 7 )
EOSIO_TEST_END

//// size_t string::length() const
EOSIO_TEST_BEGIN(string_test_38)
   static string eostr{"abcdef"};
   CHECK_EQUAL( eostr.length(), 6 )
   eostr += 'g';
   CHECK_EQUAL( eostr.length(), 7 )
EOSIO_TEST_END

//// size_t string::capacity() const
EOSIO_TEST_BEGIN(string_test_39)
   static string eostr{"abc"};
   CHECK_EQUAL( eostr.capacity(), 3 )
   eostr += 'd', eostr += 'e', eostr += 'f';
   CHECK_EQUAL( eostr.capacity(), 8 )
   eostr += 'g';
   CHECK_EQUAL( eostr.capacity(), 8 )
EOSIO_TEST_END

//// size_t string::max_size() const
EOSIO_TEST_BEGIN(string_test_40)
   static const string eostr{"abcdef"};
   CHECK_EQUAL( eostr.max_size(), string::npos )
EOSIO_TEST_END

//// void reserve(const size_t n)
EOSIO_TEST_BEGIN(string_test_41)
   static string eostr{"abcdef"};
   CHECK_EQUAL( eostr.capacity(), 6 )
   eostr.reserve(10);
   CHECK_EQUAL( eostr.capacity(), 10 )
   eostr.reserve(24);
   CHECK_EQUAL( eostr.capacity(), 24 )
   eostr.reserve(1);
   CHECK_EQUAL( eostr.capacity(), 24 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_42)
   static string eostr{""};
   eostr += "abcdef";
   CHECK_EQUAL( eostr.capacity(), 12 )
   eostr.reserve(10);
   CHECK_EQUAL( eostr.capacity(), 12 )
   eostr.reserve(24);
   CHECK_EQUAL( eostr.capacity(), 24 )
   eostr.reserve(1);
   CHECK_EQUAL( eostr.capacity(), 24 )
EOSIO_TEST_END

//// void string::shrink_to_fit() const
EOSIO_TEST_BEGIN(string_test_43)
   static string eostr0{};
   static string eostr1{"a"};
   static string eostr2{"abcdef"};

   CHECK_EQUAL( eostr0.capacity(), 0 )
   eostr0.reserve(100);
   CHECK_EQUAL( eostr0.capacity(), 100 )
   eostr0.shrink_to_fit();
   CHECK_EQUAL( eostr0.capacity(), 0 )

   CHECK_EQUAL( eostr1.capacity(), 1 )
   eostr1.reserve(100);
   CHECK_EQUAL( eostr1.capacity(), 100 )
   eostr1.shrink_to_fit();
   CHECK_EQUAL( eostr1.capacity(), 1 )

   CHECK_EQUAL( eostr2.capacity(), 6 )
   eostr2.reserve(100);
   CHECK_EQUAL( eostr2.capacity(), 100 )
   eostr2.shrink_to_fit();
   CHECK_EQUAL( eostr2.capacity(), 6 )
EOSIO_TEST_END

//// void string::clear()
EOSIO_TEST_BEGIN(string_test_44)
   static string eostr{"abcdef"};
   CHECK_EQUAL( eostr.empty(), false )
   eostr.clear();
   CHECK_EQUAL( eostr.empty(), true )
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( eostr.data()[0], '\0' )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_45)
   static string eostr{""};
   eostr += "abcdef";
   CHECK_EQUAL( eostr.empty(), false )
   eostr.clear();
   CHECK_EQUAL( eostr.empty(), true )
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( eostr.data()[0], '\0' )
EOSIO_TEST_END

//// void resize(size_t n)
EOSIO_TEST_BEGIN(string_test_46)
   static string eostr{"abcdef"};

   eostr.resize(3);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 )

   eostr.resize(5);
   CHECK_EQUAL( eostr.size(), 5 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 )

   eostr.resize(13);
   CHECK_EQUAL( eostr.size(), 13 )
   CHECK_EQUAL( eostr.capacity(), 26 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_47)
   static string eostr{""};
   eostr += "abcdef";

   eostr.resize(3);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 )

   eostr.resize(5);
   CHECK_EQUAL( eostr.size(), 5 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 )

   eostr.resize(13);
   CHECK_EQUAL( eostr.size(), 13 )
   CHECK_EQUAL( eostr.capacity(), 26 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 )
EOSIO_TEST_END

//// void swap(string& str)
EOSIO_TEST_BEGIN(string_test_48)
   static string eostr_swap0{"abc"};
   static string eostr_swap1{"123456"};

   eostr_swap0.swap(eostr_swap1);

   CHECK_EQUAL( eostr_swap0.size(), 6 )
   CHECK_EQUAL( eostr_swap0.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr_swap0.c_str(), "123456"), 0 )

   CHECK_EQUAL( eostr_swap1.size(), 3 )
   CHECK_EQUAL( eostr_swap1.capacity(), 3 )
   CHECK_EQUAL( strcmp(eostr_swap1.c_str(), "abc"), 0 )
EOSIO_TEST_END

//// void push_back(char c)
EOSIO_TEST_BEGIN(string_test_49)
   static string eostr{"abcdef"};
   CHECK_EQUAL( eostr.size(), 6 )
   eostr.push_back('g');
   CHECK_EQUAL( eostr.size(), 7 )
   CHECK_EQUAL( eostr.capacity(), 14 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefg"), 0 )
EOSIO_TEST_END

//// void pop_back()
EOSIO_TEST_BEGIN(string_test_50)
   static string eostr{"abcdefg"};
   CHECK_EQUAL( eostr.size(), 7 )
   eostr.pop_back();
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_51)
   static string eostr{"abc"};
   CHECK_EQUAL( eostr.size(), 3 )
   eostr.pop_back();
   eostr.pop_back();
   eostr.pop_back();
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )

   eostr.pop_back();
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

//// string substr(size_t pos = 0, size_t len = npos) const
EOSIO_TEST_BEGIN(string_test_52)
   static const string eostr{"abcdef"};
   CHECK_EQUAL( strcmp(eostr.substr(0).c_str(), "abcdef"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,0).c_str(), ""), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,1).c_str(), "a"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,2).c_str(), "ab"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,3).c_str(), "abc"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,4).c_str(), "abcd"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,5).c_str(), "abcde"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,6).c_str(), "abcdef"), 0 )

   CHECK_EQUAL( strcmp(eostr.substr(1,0).c_str(), ""), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,1).c_str(), "b"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,2).c_str(), "bc"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,3).c_str(), "bcd"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,4).c_str(), "bcde"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,5).c_str(), "bcdef"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,6).c_str(), "bcdef"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_53)
   static string eostr{""};
   eostr += "abcdef";
   CHECK_EQUAL( strcmp(eostr.substr(0).c_str(), "abcdef"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,0).c_str(), ""), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,1).c_str(), "a"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,2).c_str(), "ab"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,3).c_str(), "abc"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,4).c_str(), "abcd"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,5).c_str(), "abcde"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(0,6).c_str(), "abcdef"), 0 )

   CHECK_EQUAL( strcmp(eostr.substr(1,0).c_str(), ""), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,1).c_str(), "b"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,2).c_str(), "bc"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,3).c_str(), "bcd"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,4).c_str(), "bcde"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,5).c_str(), "bcdef"), 0 )
   CHECK_EQUAL( strcmp(eostr.substr(1,6).c_str(), "bcdef"), 0 )
EOSIO_TEST_END

//// size_t copy(char* dest, size_t len, size_t pos = 0) const
EOSIO_TEST_BEGIN(string_test_54)
   static const string eostr{"abcdef"};
   static char str[7]{};

   CHECK_EQUAL( eostr.copy(str, 0), 0 )
   CHECK_EQUAL( strcmp(str, ""), 0 )

   CHECK_EQUAL( eostr.copy(str, 10), 6 )
   CHECK_EQUAL( strcmp(str, "abcdef"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 0), 6 )
   CHECK_EQUAL( strcmp(str, "abcdef"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 1), 5 )
   CHECK_EQUAL( strcmp(str, "bcdef"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 2), 4 )
   CHECK_EQUAL( strcmp(str, "cdef"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 3), 3 )
   CHECK_EQUAL( strcmp(str, "def"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 4), 2 )
   CHECK_EQUAL( strcmp(str, "ef"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 5), 1 )
   CHECK_EQUAL( strcmp(str, "f"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 6), 0 )
   CHECK_EQUAL( strcmp(str, ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_55)
   static string eostr{""};
   eostr += "abcdef";
   static char str[7]{};

   CHECK_EQUAL( eostr.copy(str, 0), 0 )
   CHECK_EQUAL( strcmp(str, ""), 0 )

   CHECK_EQUAL( eostr.copy(str, 10), 6 )
   CHECK_EQUAL( strcmp(str, "abcdef"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 0), 6 )
   CHECK_EQUAL( strcmp(str, "abcdef"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 1), 5 )
   CHECK_EQUAL( strcmp(str, "bcdef"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 2), 4 )
   CHECK_EQUAL( strcmp(str, "cdef"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 3), 3 )
   CHECK_EQUAL( strcmp(str, "def"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 4), 2 )
   CHECK_EQUAL( strcmp(str, "ef"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 5), 1 )
   CHECK_EQUAL( strcmp(str, "f"), 0 )

   CHECK_EQUAL( eostr.copy(str, 10, 6), 0 )
   CHECK_EQUAL( strcmp(str, ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_56)
   static const string eostr{"abcdef"};
   static char str[7]{};
   CHECK_ASSERT( "eosio::string::copy", []() {eostr.copy(str, 1, eostr.size()+1);} )
EOSIO_TEST_END

//// string& insert(const size_t pos, const char* str)
EOSIO_TEST_BEGIN(string_test_57)
   static string eostr{"iii"};
   static const char* str{"ooo"};
   eostr.insert(0, str);
   CHECK_EQUAL( strcmp(eostr.c_str(), "oooiii"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_58)
   static string eostr{"iii"};
   static const char* str{"ooo"};
   eostr.insert(1, str);
   CHECK_EQUAL( strcmp(eostr.c_str(), "ioooii"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_59)
   static string eostr{"iii"};
   static const char* str{"ooo"};
   eostr.insert(2, str);
   CHECK_EQUAL( strcmp(eostr.c_str(), "iioooi"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_60)
   static string eostr{"iii"};
   static const char* str{"ooo"};
   eostr.insert(3, str);
   CHECK_EQUAL( strcmp(eostr.c_str(), "iiiooo"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_61)
   static string eostr{""};
   eostr += "iii";
   static const char* str{"ooo"};
   eostr.insert(0, str);
   CHECK_EQUAL( strcmp(eostr.c_str(), "oooiii"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_62)
   static string eostr{""};
   eostr += "iii";
   static const char* str{"ooo"};
   eostr.insert(1, str);
   CHECK_EQUAL( strcmp(eostr.c_str(), "ioooii"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_63)
   static string eostr{""};
   eostr += "iii";
   static const char* str{"ooo"};
   eostr.insert(2, str);
   CHECK_EQUAL( strcmp(eostr.c_str(), "iioooi"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_64)
   static string eostr{""};
   eostr += "iii";
   static const char* str{"ooo"};
   eostr.insert(3, str);
   CHECK_EQUAL( strcmp(eostr.c_str(), "iiiooo"), 0 )
EOSIO_TEST_END

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

   EOSIO_TEST(string_test_1)
   EOSIO_TEST(string_test_2)
   EOSIO_TEST(string_test_3)
   EOSIO_TEST(string_test_4)
   EOSIO_TEST(string_test_5)
   EOSIO_TEST(string_test_6)
   EOSIO_TEST(string_test_7)
   EOSIO_TEST(string_test_8)
   EOSIO_TEST(string_test_9)
   EOSIO_TEST(string_test_10)
   EOSIO_TEST(string_test_11)
   EOSIO_TEST(string_test_12)
   EOSIO_TEST(string_test_13)
   EOSIO_TEST(string_test_14)
   EOSIO_TEST(string_test_15)
   EOSIO_TEST(string_test_16)
   EOSIO_TEST(string_test_17)
   EOSIO_TEST(string_test_18)
   EOSIO_TEST(string_test_19)
   EOSIO_TEST(string_test_20)
   EOSIO_TEST(string_test_21)
   EOSIO_TEST(string_test_22)
   EOSIO_TEST(string_test_23)
   EOSIO_TEST(string_test_24)
   EOSIO_TEST(string_test_25)
   EOSIO_TEST(string_test_26)
   EOSIO_TEST(string_test_27)
   EOSIO_TEST(string_test_28)
   EOSIO_TEST(string_test_29)
   EOSIO_TEST(string_test_30)
   EOSIO_TEST(string_test_31)
   EOSIO_TEST(string_test_32)
   EOSIO_TEST(string_test_33)
   EOSIO_TEST(string_test_34)
   EOSIO_TEST(string_test_35)
   EOSIO_TEST(string_test_36)
   EOSIO_TEST(string_test_37)
   EOSIO_TEST(string_test_38)
   EOSIO_TEST(string_test_39)
   EOSIO_TEST(string_test_40)
   EOSIO_TEST(string_test_41)
   EOSIO_TEST(string_test_42)
   EOSIO_TEST(string_test_43)
   EOSIO_TEST(string_test_44)
   EOSIO_TEST(string_test_45)
   EOSIO_TEST(string_test_46)
   EOSIO_TEST(string_test_47)
   EOSIO_TEST(string_test_48)
   EOSIO_TEST(string_test_49)
   EOSIO_TEST(string_test_50)
   EOSIO_TEST(string_test_51)
   EOSIO_TEST(string_test_52)
   EOSIO_TEST(string_test_53)
   EOSIO_TEST(string_test_54)
   EOSIO_TEST(string_test_55)
   EOSIO_TEST(string_test_56)
   EOSIO_TEST(string_test_57)
   EOSIO_TEST(string_test_58)
   EOSIO_TEST(string_test_59)
   EOSIO_TEST(string_test_60)
   EOSIO_TEST(string_test_61)
   EOSIO_TEST(string_test_62)
   EOSIO_TEST(string_test_63)
   EOSIO_TEST(string_test_64)
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
