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

EOSIO_TEST_BEGIN(string_test_ins_null)
   string eostr{"abcdefg"};
   const char* null_man{nullptr};
   CHECK_ASSERT( "eosio::string::insert", [&]() {eostr.insert(0, null_man, 1);} )
   CHECK_ASSERT( "eosio::string::insert", [&]() {eostr.insert(-1, "ooo", 1);} )
EOSIO_TEST_END

//// string& insert(const size_t pos, const string& str)
EOSIO_TEST_BEGIN(string_test_ins_to_blank)
   string eostr{};
   const string str{"ooo"};
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ooo"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_at_bgn_single)
   string eostr{"abc"};
   const string str{"d"};
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 4 )
   CHECK_EQUAL( eostr.capacity(), 8 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "dabc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_at_bgn_mul_1)
   string eostr{"abc"};
   const string str{"def"};
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "defabc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_at_bgn_mul_2)
   string eostr{"iii"};
   const string str{"ooo"};
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "oooiii") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_in_middle_1)
   string eostr{"iii"};
   const string str{"ooo"};
   eostr.insert(1, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ioooii") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_in_middle_2)
   string eostr{"iii"};
   const string str{"ooo"};
   eostr.insert(2, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iioooi") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_at_end)
   string eostr{"iii"};
   const string str{"ooo"};
   eostr.insert(3, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 12 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iiiooo") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_neg_index_1)
   string eostr{"abcdefg"};
   const string str{"ooo"};
   CHECK_ASSERT( "eosio::string::insert", [&]() {eostr.insert(-1, str);} )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_op_pl_1)
   string eostr{""};
   string str{""};
   str += "ooo";
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ooo"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_op_pl_2)
   string eostr{""};
   eostr += "abc";
   string str{""};
   str += "d";
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 4 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "dabc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_op_pl_3)
   string eostr{""};
   eostr += "abc";
   string str{""};
   str += "def";
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "defabc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_op_pl_4)
   string eostr{""};
   eostr += "iii";
   string str{""};
   str += "ooo";
   eostr.insert(0, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "oooiii") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_op_pl_5)
   string eostr{""};
   eostr += "iii";
   string str{""};
   str += "ooo";
   eostr.insert(1, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ioooii") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_op_pl_6)
   string eostr{""};
   eostr += "iii";
   string str{""};
   str += "ooo";
   eostr.insert(2, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iioooi") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_op_pl_7)
   string eostr{""};
   eostr += "iii";
   string str{""};
   str += "ooo";
   eostr.insert(3, str);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iiiooo") , 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_neg_index_2)
   string eostr{"abcdefg"};
   string str{"ooo"};
   CHECK_ASSERT( "eosio::string::insert", [&]() {eostr.insert(-1, str);} )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_ins_capacity)
   string eostr = "hello";
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
EOSIO_TEST_BEGIN(string_test_erase)
   string eostr{"abcdefgh"};
   eostr.erase();
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_at_zero)
   string eostr{"abcdefgh"};
   eostr.erase(0);
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_to_npos)
   string eostr{"abcdefgh"};
   eostr.erase(0, string::npos);
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_1)
   string eostr{"abcdefgh"};
   eostr.erase(1, string::npos);
   CHECK_EQUAL( eostr.size(), 1 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "a"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_2)
   string eostr{"abcdefgh"};
   eostr.erase(2, string::npos);
   CHECK_EQUAL( eostr.size(), 2 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ab"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_3)
   string eostr{"abcdefgh"};
   eostr.erase(3, string::npos);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_4)
   string eostr{"abcdefgh"};
   eostr.erase(4, string::npos);
   CHECK_EQUAL( eostr.size(), 4 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcd"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_5)
   string eostr{"abcdefgh"};
   eostr.erase(5, string::npos);
   CHECK_EQUAL( eostr.size(), 5 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcde"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_6)
   string eostr{"abcdefgh"};
   eostr.erase(6, string::npos);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_7)
   string eostr{"abcdefgh"};
   eostr.erase(7, string::npos);
   CHECK_EQUAL( eostr.size(), 7 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefg"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_8)
   string eostr{"abcdefgh"};
   eostr.erase(8, string::npos);
   CHECK_EQUAL( eostr.size(), 8 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_8_len_0)
   string eostr{"abcdefgh"};
   eostr.erase(8, 0);
   CHECK_EQUAL( eostr.size(), 8 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_neg_index_1)
   string eostr{"abcdefg"};
   CHECK_ASSERT( "eosio::string::erase", [&]() {eostr.erase(-1, 1);} )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_op_pl)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase();
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_at_0_op_pl)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(0);
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_at_0_op_pl_npos)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(0, string::npos);
   CHECK_EQUAL( eostr.size(), 0 )
   CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_1_op_pl)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(1, string::npos);
   CHECK_EQUAL( eostr.size(), 1 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "a"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_2_op_pl)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(2, string::npos);
   CHECK_EQUAL( eostr.size(), 2 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "ab"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_3_op_pl)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(3, string::npos);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_4_op_pl)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(4, string::npos);
   CHECK_EQUAL( eostr.size(), 4 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcd"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_5_op_pl)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(5, string::npos);
   CHECK_EQUAL( eostr.size(), 5 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcde"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_6_op_pl)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(6, string::npos);
   CHECK_EQUAL( eostr.size(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_7_op_pl)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(7, string::npos);
   CHECK_EQUAL( eostr.size(), 7 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefg"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_8_op_pl)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(8, string::npos);
   CHECK_EQUAL( eostr.size(), 8 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_at_8_op_pl)
   string eostr{""};
   eostr += "abcdefgh";

   eostr.erase(8, 0);
   CHECK_EQUAL( eostr.size(), 8 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_erase_neg_index_2)
   string eostr{"abcdefg"};
   CHECK_ASSERT( "eosio::string::erase", [&]() {eostr.erase(-1, 1);} )
EOSIO_TEST_END

//// string& append(const char* str)
EOSIO_TEST_BEGIN(string_test_append_to_blank_1)
   string eostr{};
   const char* str{"iii"};
   eostr.append(str);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iii"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_append_1)
   string eostr{"abcdefg"};
   const char* str{"iii"};
   eostr.append(str);
   CHECK_EQUAL( eostr.size(), 10 )
   CHECK_EQUAL( eostr.capacity(), 20 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgiii"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_append_null)
   string eostr{"abcdefg"};
   const char* null_man{nullptr};
   CHECK_ASSERT( "eosio::string::append", [&]() {eostr.append(null_man);} )
EOSIO_TEST_END

//// string& append(const string& str)
EOSIO_TEST_BEGIN(string_test_append_to_blank_2)
   string eostr{};
   const string str{"iii"};
   eostr.append(str);
   CHECK_EQUAL( eostr.size(), 3 )
   CHECK_EQUAL( eostr.capacity(), 6 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "iii"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_append_2)
   string eostr{"abcdefg"};
   const string str{"iii"};
   eostr.append(str);
   CHECK_EQUAL( eostr.size(), 10 )
   CHECK_EQUAL( eostr.capacity(), 20 )
   CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgiii"), 0 )
EOSIO_TEST_END

//// string& operator+=(const char c)
EOSIO_TEST_BEGIN(string_test_append_3)
   string eostr0{};
   string eostr1{"a"};
   string eostr2{"abcdef"};

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
EOSIO_TEST_BEGIN(string_test_append_op_pl_1)
   string eostr0{};
   string eostr1{"a"};
   string eostr2{"abcdef"};
   string eostr3{"abcdef"};

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
EOSIO_TEST_BEGIN(string_test_append_op_pl_2)
   string eostr0{};
   string eostr1{"a"};
   string eostr2{"abcdef"};
   string eostr3{"abcdef"};

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
EOSIO_TEST_BEGIN(string_test_append_op_pl_3)
   string eostr0{"a"};
   string eostr1{"b"};
   CHECK_EQUAL( eostr0.size(), 1 )
   eostr0 += eostr1;
   CHECK_EQUAL( eostr0.size(), 2 )
   CHECK_EQUAL( strcmp(eostr0.c_str(), "ab"), 0 )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_append_op_pl_4)
   string eostr0{"abc"};
   string eostr1{"def"};
   CHECK_EQUAL( eostr0.size(), 3 )
   eostr0 += eostr1;
   CHECK_EQUAL( eostr0.size(), 6 )
   CHECK_EQUAL( strcmp(eostr0.c_str(), "abcdef"), 0 )
EOSIO_TEST_END

//// inline void print(eosio::string str)
EOSIO_TEST_BEGIN(string_test_print)
   const string eostr0{""};
   const string eostr1{"abc"};
   const string eostr2{"abcdef"};

   CHECK_PRINT( "", [&](){ print(eostr0); } )
   CHECK_PRINT( "abc", [&](){ print(eostr1); } )
   CHECK_PRINT( "abcdef", [&](){ print(eostr2); } )
EOSIO_TEST_END

//// friend bool operator< (const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_less_than)
   const string eostr0{"abc"};
   const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 < eostr0), false )
   CHECK_EQUAL( (eostr1 < eostr1), false )
   CHECK_EQUAL( (eostr0 < eostr1), true )
EOSIO_TEST_END

//// friend bool operator> (const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_gt)
   const string eostr0{"abc"};
   const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 > eostr0), false )
   CHECK_EQUAL( (eostr1 > eostr1), false )
   CHECK_EQUAL( (eostr0 > eostr1), false )
EOSIO_TEST_END

//// friend bool operator<=(const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_lt_or_eq)
   const string eostr0{"abc"};
   const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 <= eostr0), true )
   CHECK_EQUAL( (eostr1 <= eostr1), true )
   CHECK_EQUAL( (eostr0 <= eostr1), true )
EOSIO_TEST_END

//// friend bool operator>=(const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_gt_or_eq)
   const string eostr0{"abc"};
   const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 >= eostr0), true )
   CHECK_EQUAL( (eostr1 >= eostr1), true )
   CHECK_EQUAL( (eostr0 >= eostr1), false )
EOSIO_TEST_END

//// friend bool operator==(const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_equal)
   const string eostr0{"abc"};
   const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 == eostr0), true )
   CHECK_EQUAL( (eostr1 == eostr1), true )
   CHECK_EQUAL( (eostr0 == eostr1), false )
EOSIO_TEST_END

//// friend bool operator!=(const string& lhs, const string& rhs)
EOSIO_TEST_BEGIN(string_test_not_equal)
   const string eostr0{"abc"};
   const string eostr1{"def"};
   CHECK_EQUAL( (eostr0 != eostr0), false )
   CHECK_EQUAL( (eostr1 != eostr1), false )
   CHECK_EQUAL( (eostr0 != eostr1), true )
EOSIO_TEST_END

//// template<typename DataStream>
//// DataStream& operator<<(DataStream& ds, const string& str)
//// DataStream& operator>>(DataStream& ds, string& str)
EOSIO_TEST_BEGIN(string_test_stream_io_1)
   constexpr uint16_t buffer_size{256};
   char datastream_buffer[buffer_size]{}; // Buffer for the datastream to point to
   char buffer[buffer_size]; // Buffer to compare `datastream_buffer` with
   datastream<char*> ds{datastream_buffer, buffer_size};

   ds.seekp(0);
   fill(std::begin(datastream_buffer), std::end(datastream_buffer), 0);
   const string cstr {""};
   string str{};
   ds << cstr;
   ds.seekp(0);
   ds >> str;
   CHECK_EQUAL( cstr, str )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_stream_io_2)
   constexpr uint16_t buffer_size{256};
   char datastream_buffer[buffer_size]{};
   char buffer[buffer_size];
   datastream<char*> ds{datastream_buffer, buffer_size};

   ds.seekp(0);
   fill(std::begin(datastream_buffer), std::end(datastream_buffer), 0);
   const string cstr {"a"};
   string str{};
   ds << cstr;
   ds.seekp(0);
   ds >> str;
   CHECK_EQUAL( cstr, str )
EOSIO_TEST_END

EOSIO_TEST_BEGIN(string_test_stream_io_3)
   constexpr uint16_t buffer_size{256};
   char datastream_buffer[buffer_size]{};
   char buffer[buffer_size];
   datastream<char*> ds{datastream_buffer, buffer_size};

   ds.seekp(0);
   fill(std::begin(datastream_buffer), std::end(datastream_buffer), 0);
   const string cstr {"abcdefghi"};
   string str{};
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

   EOSIO_TEST(string_test_ins_null)
   EOSIO_TEST(string_test_ins_to_blank)
   EOSIO_TEST(string_test_ins_at_bgn_single)
   EOSIO_TEST(string_test_ins_at_bgn_mul_1)
   EOSIO_TEST(string_test_ins_at_bgn_mul_2)
   EOSIO_TEST(string_test_ins_in_middle_1)
   EOSIO_TEST(string_test_ins_in_middle_2)
   EOSIO_TEST(string_test_ins_at_end)
   EOSIO_TEST(string_test_ins_neg_index_1)
   EOSIO_TEST(string_test_ins_op_pl_1)
   EOSIO_TEST(string_test_ins_op_pl_2)
   EOSIO_TEST(string_test_ins_op_pl_3)
   EOSIO_TEST(string_test_ins_op_pl_4)
   EOSIO_TEST(string_test_ins_op_pl_5)
   EOSIO_TEST(string_test_ins_op_pl_6)
   EOSIO_TEST(string_test_ins_op_pl_7)
   EOSIO_TEST(string_test_ins_neg_index_2)
   EOSIO_TEST(string_test_ins_capacity)
   EOSIO_TEST(string_test_erase)
   EOSIO_TEST(string_test_erase_at_zero)
   EOSIO_TEST(string_test_erase_to_npos)
   EOSIO_TEST(string_test_erase_1)
   EOSIO_TEST(string_test_erase_2)
   EOSIO_TEST(string_test_erase_3)
   EOSIO_TEST(string_test_erase_4)
   EOSIO_TEST(string_test_erase_5)
   EOSIO_TEST(string_test_erase_6)
   EOSIO_TEST(string_test_erase_7)
   EOSIO_TEST(string_test_erase_8)
   EOSIO_TEST(string_test_erase_8_len_0)
   EOSIO_TEST(string_test_erase_neg_index_1)
   EOSIO_TEST(string_test_erase_op_pl)
   EOSIO_TEST(string_test_erase_at_0_op_pl)
   EOSIO_TEST(string_test_erase_at_0_op_pl_npos)
   EOSIO_TEST(string_test_erase_1_op_pl)
   EOSIO_TEST(string_test_erase_2_op_pl)
   EOSIO_TEST(string_test_erase_3_op_pl)
   EOSIO_TEST(string_test_erase_4_op_pl)
   EOSIO_TEST(string_test_erase_5_op_pl)
   EOSIO_TEST(string_test_erase_6_op_pl)
   EOSIO_TEST(string_test_erase_7_op_pl)
   EOSIO_TEST(string_test_erase_8_op_pl)
   EOSIO_TEST(string_test_erase_at_8_op_pl)
   EOSIO_TEST(string_test_erase_neg_index_2)
   EOSIO_TEST(string_test_append_to_blank_1)
   EOSIO_TEST(string_test_append_1)
   EOSIO_TEST(string_test_append_null)
   EOSIO_TEST(string_test_append_to_blank_2)
   EOSIO_TEST(string_test_append_2)
   EOSIO_TEST(string_test_append_3)
   EOSIO_TEST(string_test_append_op_pl_1)
   EOSIO_TEST(string_test_append_op_pl_2)
   EOSIO_TEST(string_test_append_op_pl_3)
   EOSIO_TEST(string_test_append_op_pl_4)
   EOSIO_TEST(string_test_print)
   EOSIO_TEST(string_test_less_than)
   EOSIO_TEST(string_test_gt)
   EOSIO_TEST(string_test_lt_or_eq)
   EOSIO_TEST(string_test_gt_or_eq)
   EOSIO_TEST(string_test_equal)
   EOSIO_TEST(string_test_not_equal)
   EOSIO_TEST(string_test_stream_io_1)
   EOSIO_TEST(string_test_stream_io_2)
   EOSIO_TEST(string_test_stream_io_3)

   return has_failed();
}
