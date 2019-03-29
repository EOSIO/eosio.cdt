/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

// TODO:
// test datastream
// TODO:
// take out prints
// TODO:
// teset print
// TODO:
// print libraries/core/print.hpp; starts at line 102
// TODO:
// change all `.data()` compares with memcmp or strcmp or c_str
// TODO:
// change all `=` to `{}` when initializing
// TODO:
// remove all semi-colons from the test macros
// TODO:
// Remove trailing whitespace from both files
// TODO:
// refactor to use ternary operator thorughout code
// TODO:
// Make sure to change public to private

// #include <eosio/eosio.hpp>
#include <eosio/tester.hpp>
#include <eosio/datastream.hpp>
#include <eosio/eostring.hpp>

// eosio::print("\n\n\n>>>>",eostr.capacity(),"<<<<\n\n\n");

using std::fill;
using std::move;

using eosio::datastream;
using eosio::eostring;

// Definitions found in `eosio.cdt/libraries/eosiolib/core/eosio/eostring.hpp`
EOSIO_TEST_BEGIN(eostring_test)
   silence_output(false);

   //// template <size_t N>
   //// eostring(const char (&str)[N])
   {
      static const eostring eostr0{"a"};
      static const eostring eostr1{"abcdef"};

      CHECK_EQUAL( eostr0.size(), 1 );
      CHECK_EQUAL( eostr0.capacity(), 1 );
      CHECK_EQUAL( strcmp(eostr0.c_str(), "a"), 0 );

      CHECK_EQUAL( eostr1.size(), 6 );
      CHECK_EQUAL( eostr1.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr1.c_str(), "abcdef"), 0);
   }

   //// eostring()
   {
      static const eostring eostr{};

      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( eostr.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0);
   }

   //// eostring(const size_t n, const char c)
   {
      static const eostring eostr0(0, 'c');
      static const eostring eostr1(1, 'c');
      static const eostring eostr2(3, 'c');

      CHECK_EQUAL( eostr0.size(), 0 );
      CHECK_EQUAL( eostr0.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0.c_str(), ""), 0);

      CHECK_EQUAL( eostr1.size(), 1 );
      CHECK_EQUAL( eostr1.capacity(), 2 );
      CHECK_EQUAL( strcmp(eostr1.c_str(), "c"), 0);

      CHECK_EQUAL( eostr2.size(), 3 );
      CHECK_EQUAL( eostr2.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr2.c_str(), "ccc"), 0);
   }

   //// eostring(const eostring& str, const size_t pos, const size_t n = eostring::npos)
   {
      static const eostring eostr{"abcdef"};
      static const eostring eostr0_sub(eostr, 0, 0);
      static const eostring eostr1_sub(eostr, 1, 0);
      static const eostring eostr2_sub(eostr, 0, 1);
      static const eostring eostr3_sub(eostr, 0, 3);
      static const eostring eostr4_sub(eostr, 0, 8);
      static const eostring eostr5_sub(eostr, 0, 7);
      static const eostring eostr6_sub(eostr, 0, 6);
      static const eostring eostr7_sub(eostr, 3, 3);
      static const eostring eostr8_sub(eostr, 3, 2);

      CHECK_EQUAL( eostr0_sub.size(), 0 );
      CHECK_EQUAL( eostr0_sub.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0_sub.c_str(), ""), 0);

      CHECK_EQUAL( eostr1_sub.size(), 0 );
      CHECK_EQUAL( eostr1_sub.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr1_sub.c_str(), ""), 0);

      CHECK_EQUAL( eostr2_sub.size(), 1 );
      CHECK_EQUAL( eostr2_sub.capacity(), 2 );
      CHECK_EQUAL( strcmp(eostr2_sub.c_str(), "a"), 0);

      CHECK_EQUAL( eostr3_sub.size(), 3 );
      CHECK_EQUAL( eostr3_sub.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr3_sub.c_str(), "abc"), 0);

      CHECK_EQUAL( eostr4_sub.size(), 6 );
      CHECK_EQUAL( eostr4_sub.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr4_sub.c_str(), "abcdef"), 0);

      CHECK_EQUAL( eostr5_sub.size(), 6 );
      CHECK_EQUAL( eostr5_sub.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr5_sub.c_str(), "abcdef"), 0);

      CHECK_EQUAL( eostr6_sub.size(), 6 );
      CHECK_EQUAL( eostr6_sub.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr6_sub.c_str(), "abcdef"), 0 )

      CHECK_EQUAL( eostr7_sub.size(), 3 );
      CHECK_EQUAL( eostr7_sub.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr7_sub.c_str(), "def"), 0 )

      CHECK_EQUAL( eostr8_sub.size(), 2 );
      CHECK_EQUAL( eostr8_sub.capacity(), 4 );
      CHECK_EQUAL( strcmp(eostr8_sub.c_str(), "de"), 0);
   }

   //// constexpr eostring(const eostring& str)
   {
      static const eostring eostr0{""};
      static const eostring eostr1{"a"};
      static const eostring eostr2{"abcdef"};
      static const eostring eostr0_cpy{eostr0};
      static const eostring eostr1_cpy{eostr1};
      static const eostring eostr2_cpy{eostr2};
      
      CHECK_EQUAL( eostr0_cpy.size(), 0 );
      CHECK_EQUAL( eostr0_cpy.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0_cpy.c_str(), ""), 0);

      CHECK_EQUAL( eostr1_cpy.size(), 1 );
      CHECK_EQUAL( eostr1_cpy.capacity(), 1 );
      CHECK_EQUAL( strcmp(eostr1_cpy.c_str(), "a"), 0);

      CHECK_EQUAL( eostr2_cpy.size(), 6 );
      CHECK_EQUAL( eostr2_cpy.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr2_cpy.c_str(), "abcdef"), 0);
   }

   {
      static eostring eostr0{""};
      eostr0 += "a";
      static eostring eostr1{"abc"};
      eostr1 += "def";
      static eostring eostr0_cpy{eostr0};
      static eostring eostr1_cpy{eostr1};

      CHECK_EQUAL( eostr0_cpy.size(), 1 );
      CHECK_EQUAL( eostr0_cpy.capacity(), 2 );
      CHECK_EQUAL( strcmp(eostr0_cpy.c_str(), "a"), 0);

      CHECK_EQUAL( eostr1_cpy.size(), 6 );
      CHECK_EQUAL( eostr1_cpy.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr1_cpy.c_str(), "abcdef"), 0);
   }

   //// constexpr eostring(const eostring&& str)
   {
      static eostring eostr0{""};
      static eostring eostr1{"a"};
      static eostring eostr2{"abcdef"};
      static const eostring eostr0_mv{move(eostr0)};
      static const eostring eostr1_mv{move(eostr1)};
      static const eostring eostr2_mv{move(eostr2)};
      
      CHECK_EQUAL( eostr0_mv.size(), 0 );
      CHECK_EQUAL( eostr0_mv.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0.c_str(), ""), 0);

      CHECK_EQUAL( eostr1_mv.size(), 1 );
      CHECK_EQUAL( eostr1_mv.capacity(), 1 );
      CHECK_EQUAL( strcmp(eostr1.c_str(), "a"), 0);

      CHECK_EQUAL( eostr2_mv.size(), 6 );
      CHECK_EQUAL( eostr2_mv.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr2.c_str(), "abcdef"), 0);
   }

   {
      static eostring eostr0{""};
      eostr0 += "a";
      static eostring eostr1{"abc"};
      eostr1 += "def";
      static eostring eostr0_cpy{move(eostr0)};
      static eostring eostr1_cpy{move(eostr1)};

      CHECK_EQUAL( eostr0_cpy.size(), 1 );
      CHECK_EQUAL( eostr0_cpy.capacity(), 2 );
      CHECK_EQUAL( strcmp(eostr0_cpy.c_str(), "a"), 0);

      CHECK_EQUAL( eostr1_cpy.size(), 6 );
      CHECK_EQUAL( eostr1_cpy.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr1_cpy.c_str(), "abcdef"), 0);
   }

   //// eostring& operator=(const eostring& str);
   {
      static const eostring eostr0{""};
      static const eostring eostr1{"a"};
      static const eostring eostr2{"abcdef"};
      static eostring eostr0_cpy_assig{};
      static eostring eostr1_cpy_assig{};
      static eostring eostr2_cpy_assig{};
      eostr0_cpy_assig = eostr0;
      eostr1_cpy_assig = eostr1;
      eostr2_cpy_assig = eostr2;
      
      CHECK_EQUAL( eostr0_cpy_assig.size(), 0 );
      CHECK_EQUAL( eostr0_cpy_assig.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0_cpy_assig.c_str(), ""), 0);

      CHECK_EQUAL( eostr1_cpy_assig.size(), 1 );
      CHECK_EQUAL( eostr1_cpy_assig.capacity(), 1 );
      CHECK_EQUAL( strcmp(eostr1_cpy_assig.c_str(), "a"), 0);

      CHECK_EQUAL( eostr2_cpy_assig.size(), 6 );
      CHECK_EQUAL( eostr2_cpy_assig.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr2_cpy_assig.c_str(), "abcdef"), 0);
   }

   {
      static eostring eostr0{""};
      eostr0 += "a";
      static eostring eostr1{"abc"};
      eostr1 += "def";
      static eostring eostr0_cpy_assig{};
      static eostring eostr1_cpy_assig{};
      eostr0_cpy_assig = eostr0;
      eostr1_cpy_assig = eostr1;
      
      CHECK_EQUAL( eostr0_cpy_assig.size(), 1 );
      CHECK_EQUAL( eostr0_cpy_assig.capacity(), 2 );
      CHECK_EQUAL( strcmp(eostr0_cpy_assig.c_str(), "a"), 0);

      CHECK_EQUAL( eostr1_cpy_assig.size(), 6 );
      CHECK_EQUAL( eostr1_cpy_assig.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr1_cpy_assig.c_str(), "abcdef"), 0);
   }

   //// eostring& operator=(eostring&& str)
   {
      static eostring eostr0{""};
      static eostring eostr1{"a"};
      static eostring eostr2{"abcdef"};
      static eostring eostr0_mv_assig{};
      static eostring eostr1_mv_assig{};
      static eostring eostr2_mv_assig{};
      eostr0_mv_assig = move(eostr0);
      eostr1_mv_assig = move(eostr1);
      eostr2_mv_assig = move(eostr2);
      
      CHECK_EQUAL( eostr0_mv_assig.size(), 0 );
      CHECK_EQUAL( eostr0_mv_assig.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0_mv_assig.c_str(), ""), 0);

      CHECK_EQUAL( eostr1_mv_assig.size(), 1 );
      CHECK_EQUAL( eostr1_mv_assig.capacity(), 1 );
      CHECK_EQUAL( strcmp(eostr1_mv_assig.c_str(), "a"), 0);

      CHECK_EQUAL( eostr2_mv_assig.size(), 6 );
      CHECK_EQUAL( eostr2_mv_assig.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr2_mv_assig.c_str(), "abcdef"), 0);
   }

   {
      static eostring eostr0{""};
      eostr0 += "a";
      static eostring eostr1{"abc"};
      eostr1 += "def";
      static eostring eostr0_mv_assig{};
      static eostring eostr1_mv_assig{};
      eostr0_mv_assig = move(eostr0);
      eostr1_mv_assig = move(eostr1);
      
      CHECK_EQUAL( eostr0_mv_assig.size(), 1 );
      CHECK_EQUAL( eostr0_mv_assig.capacity(), 2 );
      CHECK_EQUAL( strcmp(eostr0_mv_assig.c_str(), "a"), 0);

      CHECK_EQUAL( eostr1_mv_assig.size(), 6 );
      CHECK_EQUAL( eostr1_mv_assig.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr1_mv_assig.c_str(), "abcdef"), 0);
   }

   //// eostring& operator=(const char* str)
   {
      static eostring eostr{};
      eostr = "abcdef";
      
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 );
      
      eostr = eostr;
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 );
   }

   {
      static eostring eostr{};
      eostr = "";
      eostr += "abcdef";
      
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 );
      
      eostr = eostr;
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 );
   }

   //// char& operator[](const size_t n)
   {
      static eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr[0], 'a' );
      CHECK_EQUAL( eostr[5], 'f' );
   }

   {
      static eostring eostr{"abc"};
      eostr += "def";
      CHECK_EQUAL( eostr[0], 'a' );
      CHECK_EQUAL( eostr[5], 'f' );
   }

   //// const char& operator[](const size_t n) const
   {
      static const eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr[0], 'a' );
      CHECK_EQUAL( eostr[5], 'f' );
   }

   //// char& at(const size_t n)
   {
      static eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.at(0), 'a' );
      CHECK_EQUAL( eostr.at(5), 'f' );
      
      CHECK_ASSERT( "eostring::at", []() {eostr.at(6);} )
   }

   //// const char& at(const size_t n) const
   {
      static const eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.at(0), 'a' );
      CHECK_EQUAL( eostr.at(5), 'f' );

      CHECK_ASSERT( "eostring::at const", []() {eostr.at(6);} )
   }

   {
      static eostring eostr{""};
      eostr += "abcdef";
      const char c0{eostr.at(0)};
      const char c1{eostr.at(5)};
      CHECK_EQUAL( c0, 'a' );
      CHECK_EQUAL( c1, 'f' );
   }

   //// char& front()
   {
      static eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.front(), 'a' );

      static eostring empty_str;
      CHECK_EQUAL( eostr.front(), 'a' );
   }

   //// const char& front() const
   {
      static const eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.front(), 'a' );
   }

   //// char& back()
   {
      static eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.back(), 'f' );
   }

   //// const char& back() const
   {
      static const eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.back(), 'f' );
   }

   //// char* data()
   {
      static eostring eostr{"abcdef"};
      CHECK_EQUAL( strcmp(eostr.data(), "abcdef"), 0 );

      eostr = "abc";
      CHECK_EQUAL( strcmp(eostr.data(), "abc"), 0 );
   }

   //// const char* data() const
   {
      static const eostring eostr{"abcdef"};
      CHECK_EQUAL( strcmp(eostr.data(), "abcdef"), 0 );
   }

   //// const char* c_str() const
   {
      static eostring eostr{"abcdef"};
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 );
      CHECK_EQUAL( eostr.c_str()[eostr.size()], '\0' );
   }

   {
      static eostring eostr{""};
      eostr += "abcdef";
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 );
      CHECK_EQUAL( eostr.c_str()[eostr.size()], '\0' );
   }

   //// char* begin()
   {
      static eostring eostr{"abcdef"};
      char* iter{eostr.begin()};
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.c_str(), iter), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdef";
      char* iter{eostr.begin()};
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.c_str(), iter), 0 );
   }

   //// const char* cbegin() const
   {
      static const eostring eostr{"abcdef"};
      const char* iter{eostr.cbegin()};
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.data(), iter), 0 );
   }

   //// char* end()
   {
      static eostring eostr{"abcdef"};
      char* iter{eostr.end()};
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.c_str()+eostr.size(), iter), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdef";
      char* iter{eostr.end()};
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.c_str()+eostr.size(), iter), 0 );
   }

   //// const char* cend() const
   {
      static const eostring eostr{"abcdef"};
      const char* iter{eostr.cend()};
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.data()+eostr.size(), iter), 0 );
   }

   //// bool eostring::empty() const
   {
      static eostring eostr{};
      CHECK_EQUAL( eostr.empty(), true );
      eostr += 'c';
      CHECK_EQUAL( eostr.empty(), false );
   }

   //// size_t eostring::size() const
   {
      static eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.size(), 6 );
      eostr += 'g';
      CHECK_EQUAL( eostr.size(), 7 );
   }

   //// size_t eostring::length() const
   {
      static eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.length(), 6 );
      eostr += 'g';
      CHECK_EQUAL( eostr.length(), 7 );
   }

   //// size_t eostring::capacity() const
   {
      static eostring eostr{"abc"};
      CHECK_EQUAL( eostr.capacity(), 3 );
      eostr += 'd', eostr += 'e', eostr += 'f';
      CHECK_EQUAL( eostr.capacity(), 8 );
      eostr += 'g';
      CHECK_EQUAL( eostr.capacity(), 8 );
   }

   //// size_t eostring::max_size() const
   {
      static const eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.max_size(), eostring::npos );
   }

   //// void reserve(const size_t n)
   {
      static eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.capacity(), 6 );
      eostr.reserve(10);
      CHECK_EQUAL( eostr.capacity(), 10 );
      eostr.reserve(24);
      CHECK_EQUAL( eostr.capacity(), 24 );
      eostr.reserve(1);
      CHECK_EQUAL( eostr.capacity(), 24 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdef";
      CHECK_EQUAL( eostr.capacity(), 12 );
      eostr.reserve(10);
      CHECK_EQUAL( eostr.capacity(), 12 );
      eostr.reserve(24);
      CHECK_EQUAL( eostr.capacity(), 24 );
      eostr.reserve(1);
      CHECK_EQUAL( eostr.capacity(), 24 );
   }

   //// void eostring::shrink_to_fit() const
   {
      static eostring eostr0{};
      static eostring eostr1{"a"};
      static eostring eostr2{"abcdef"};

      CHECK_EQUAL( eostr0.capacity(), 0 );
      eostr0.reserve(100);
      CHECK_EQUAL( eostr0.capacity(), 100 );
      eostr0.shrink_to_fit();
      CHECK_EQUAL( eostr0.capacity(), 0 );

      CHECK_EQUAL( eostr1.capacity(), 1 );
      eostr1.reserve(100);
      CHECK_EQUAL( eostr1.capacity(), 100 );
      eostr1.shrink_to_fit();
      CHECK_EQUAL( eostr1.capacity(), 1 );

      CHECK_EQUAL( eostr2.capacity(), 6 );
      eostr2.reserve(100);
      CHECK_EQUAL( eostr2.capacity(), 100 );
      eostr2.shrink_to_fit();
      CHECK_EQUAL( eostr2.capacity(), 6 );
   }

   //// void eostring::clear()
   {
      static eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.empty(), false );
      eostr.clear();
      CHECK_EQUAL( eostr.empty(), true );
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( eostr.data()[0], '\0' );
   }

   {
      static eostring eostr{""};
      eostr += "abcdef";
      CHECK_EQUAL( eostr.empty(), false );
      eostr.clear();
      CHECK_EQUAL( eostr.empty(), true );
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( eostr.data()[0], '\0' );
   }

   //// void resize(size_t n)
   {
      static eostring eostr{"abcdef"};

      eostr.resize(3);
      CHECK_EQUAL( eostr.size(), 3 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.data(), "abc"), 0 );

      eostr.resize(5);
      CHECK_EQUAL( eostr.size(), 5 );
      CHECK_EQUAL( eostr.capacity(), 10 );
      CHECK_EQUAL( strcmp(eostr.data(), "abc"), 0 );

      eostr.resize(13);
      CHECK_EQUAL( eostr.size(), 13 );
      CHECK_EQUAL( eostr.capacity(), 26 );
      CHECK_EQUAL( strcmp(eostr.data(), "abc"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdef";

      eostr.resize(3);
      CHECK_EQUAL( eostr.size(), 3 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.data(), "abc"), 0 );

      eostr.resize(5);
      CHECK_EQUAL( eostr.size(), 5 );
      CHECK_EQUAL( eostr.capacity(), 10 );
      CHECK_EQUAL( strcmp(eostr.data(), "abc"), 0 );

      eostr.resize(13);
      CHECK_EQUAL( eostr.size(), 13 );
      CHECK_EQUAL( eostr.capacity(), 26 );
      CHECK_EQUAL( strcmp(eostr.data(), "abc"), 0 );
   }

   //// void swap(eostring& str)
   {
      static eostring eostr_swap0{"abc"};
      static eostring eostr_swap1{"123456"};

      eostr_swap0.swap(eostr_swap1);

      CHECK_EQUAL( eostr_swap0.size(), 6 );
      CHECK_EQUAL( eostr_swap0.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr_swap0.data(), "123456"), 0 );

      CHECK_EQUAL( eostr_swap1.size(), 3 );
      CHECK_EQUAL( eostr_swap1.capacity(), 3 );
      CHECK_EQUAL( strcmp(eostr_swap1.data(), "abc"), 0 );
   }

   // //// void push_back(char c)
   // {
   //    static eostring eostr{"abcdef"};
   //    CHECK_EQUAL( eostr.size(), 6 );
   //    eostr.push_back('g');
   //    CHECK_EQUAL( eostr.size(), 7 );
   //    CHECK_EQUAL( eostr.capacity(), 14 );
   //    CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefg"), 0 );
   //    eosio::print("\n\n\n>>>>",eostr.c_str(),"<<<<\n\n\n");
   // }

   //// void pop_back()
   {
      static eostring eostr{"abcdefg"};
      CHECK_EQUAL( eostr.size(), 7 );
      eostr.pop_back();
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( strcmp(eostr.data(), "abcdef"), 0 );
   }

   {
      static eostring eostr{"abc"};
      CHECK_EQUAL( eostr.size(), 3 );
      eostr.pop_back();
      eostr.pop_back();
      eostr.pop_back();
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 );

      eostr.pop_back();
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 );
   }

   //// eostring substr(size_t pos = 0, size_t len = npos) const
   {
      static const eostring eostr{"abcdef"};
      CHECK_EQUAL( strcmp(eostr.substr(0).c_str(), "abcdef"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,0).c_str(), ""), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,1).c_str(), "a"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,2).c_str(), "ab"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,3).c_str(), "abc"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,4).c_str(), "abcd"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,5).c_str(), "abcde"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,6).c_str(), "abcdef"), 0 );

      CHECK_EQUAL( strcmp(eostr.substr(1,0).c_str(), ""), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,1).c_str(), "b"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,2).c_str(), "bc"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,3).c_str(), "bcd"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,4).c_str(), "bcde"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,5).c_str(), "bcdef"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,6).c_str(), "bcdef"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdef";
      CHECK_EQUAL( strcmp(eostr.substr(0).c_str(), "abcdef"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,0).c_str(), ""), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,1).c_str(), "a"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,2).c_str(), "ab"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,3).c_str(), "abc"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,4).c_str(), "abcd"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,5).c_str(), "abcde"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,6).c_str(), "abcdef"), 0 );

      CHECK_EQUAL( strcmp(eostr.substr(1,0).c_str(), ""), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,1).c_str(), "b"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,2).c_str(), "bc"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,3).c_str(), "bcd"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,4).c_str(), "bcde"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,5).c_str(), "bcdef"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,6).c_str(), "bcdef"), 0 );
   }

   //// size_t copy(char* dest, size_t len, size_t pos = 0) const
   {
      static const eostring eostr{"abcdef"};
      static char str[7]{};
      
      CHECK_EQUAL( eostr.copy(str, 0), 0 );
      CHECK_EQUAL( strcmp(str, ""), 0 );

      CHECK_EQUAL( eostr.copy(str, 10), 6 );
      CHECK_EQUAL( strcmp(str, "abcdef"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 0), 6 );
      CHECK_EQUAL( strcmp(str, "abcdef"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 1), 5 );
      CHECK_EQUAL( strcmp(str, "bcdef"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 2), 4 );
      CHECK_EQUAL( strcmp(str, "cdef"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 3), 3 );
      CHECK_EQUAL( strcmp(str, "def"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 4), 2 );
      CHECK_EQUAL( strcmp(str, "ef"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 5), 1 );
      CHECK_EQUAL( strcmp(str, "f"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 6), 0 );
      CHECK_EQUAL( strcmp(str, ""), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdef";
      static char str[7]{};
      
      CHECK_EQUAL( eostr.copy(str, 0), 0 );
      CHECK_EQUAL( strcmp(str, ""), 0 );

      CHECK_EQUAL( eostr.copy(str, 10), 6 );
      CHECK_EQUAL( strcmp(str, "abcdef"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 0), 6 );
      CHECK_EQUAL( strcmp(str, "abcdef"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 1), 5 );
      CHECK_EQUAL( strcmp(str, "bcdef"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 2), 4 );
      CHECK_EQUAL( strcmp(str, "cdef"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 3), 3 );
      CHECK_EQUAL( strcmp(str, "def"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 4), 2 );
      CHECK_EQUAL( strcmp(str, "ef"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 5), 1 );
      CHECK_EQUAL( strcmp(str, "f"), 0 );

      CHECK_EQUAL( eostr.copy(str, 10, 6), 0 );
      CHECK_EQUAL( strcmp(str, ""), 0 );
   }

   {
      static const eostring eostr{"abcdef"};
      static char str[7]{};
      CHECK_ASSERT( "eostring::copy", []() {eostr.copy(str, 1, eostr.size()+1);} )
   }

   //// eostring& insert(const size_t pos, const char* str)
   {
      static eostring eostr{"iii"};
      static const char* str{"ooo"};
      eostr.insert(0, str);
      CHECK_EQUAL( strcmp(eostr.c_str(), "oooiii"), 0 );
   }

   {
      static eostring eostr{"iii"};
      static const char* str{"ooo"};
      eostr.insert(1, str);
      CHECK_EQUAL( strcmp(eostr.c_str(), "ioooii"), 0 );
   }

   {
      static eostring eostr{"iii"};
      static const char* str{"ooo"};
      eostr.insert(2, str);
      CHECK_EQUAL( strcmp(eostr.c_str(), "iioooi"), 0 );
   }

   {
      static eostring eostr{"iii"};
      static const char* str{"ooo"};
      eostr.insert(3, str);
      CHECK_EQUAL( strcmp(eostr.c_str(), "iiiooo"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "iii";
      static const char* str{"ooo"};
      eostr.insert(0, str);
      CHECK_EQUAL( strcmp(eostr.c_str(), "oooiii"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "iii";
      static const char* str{"ooo"};
      eostr.insert(1, str);
      CHECK_EQUAL( strcmp(eostr.c_str(), "ioooii"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "iii";
      static const char* str{"ooo"};
      eostr.insert(2, str);
      CHECK_EQUAL( strcmp(eostr.c_str(), "iioooi"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "iii";
      static const char* str{"ooo"};
      eostr.insert(3, str);
      CHECK_EQUAL( strcmp(eostr.c_str(), "iiiooo"), 0 );
   }

   {
      static eostring eostr{"abcdefg"};
      static const char* null_man{nullptr};
      CHECK_ASSERT( "eostring::insert", []() {eostr.insert(0, null_man, 1);} )
      CHECK_ASSERT( "eostring::insert", []() {eostr.insert(-1, "ooo", 1);} )
   }

   //// eostring& insert(const size_t pos, const eostring& str)
   {
      static eostring eostr{};
      static const eostring str{"ooo"};
      eostr.insert(0, str);
      CHECK_EQUAL( eostr.size(), 3 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "ooo"), 0 );
   }

   {
      static eostring eostr{"abc"};
      static const eostring str{"d"};
      eostr.insert(0, str);
      CHECK_EQUAL( eostr.size(), 4 );
      CHECK_EQUAL( eostr.capacity(), 8 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "dabc"), 0 );
   }

   {
      static eostring eostr{"abc"};
      static const eostring str{"def"};
      eostr.insert(0, str);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "defabc"), 0 );
   }

   {
      static eostring eostr{"iii"};
      static const eostring str{"ooo"};
      eostr.insert(0, str);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "oooiii") , 0 );
   }

   {
      static eostring eostr{"iii"};
      static const eostring str{"ooo"};
      eostr.insert(1, str);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "ioooii") , 0 );
   }

   {
      static eostring eostr{"iii"};
      static const eostring str{"ooo"};
      eostr.insert(2, str);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "iioooi") , 0 );
   }

   {
      static eostring eostr{"iii"};
      static const eostring str{"ooo"};
      eostr.insert(3, str);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "iiiooo") , 0 );
   }

   {
      static eostring eostr{"abcdefg"};
      static const eostring str{"ooo"};
      CHECK_ASSERT( "eostring::insert", []() {eostr.insert(-1, str);} )
   }

   {
      static eostring eostr{""};
      static eostring str{""};
      str += "ooo";
      eostr.insert(0, str);
      CHECK_EQUAL( eostr.size(), 3 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "ooo"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abc";
      static eostring str{""};
      str += "d";
      eostr.insert(0, str);
      CHECK_EQUAL( eostr.size(), 4 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "dabc"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abc";
      static eostring str{""};
      str += "def";
      eostr.insert(0, str);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "defabc"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "iii";
      static eostring str{""};
      str += "ooo";
      eostr.insert(0, str);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "oooiii") , 0 );
   }

   {
      static eostring eostr{""};
      eostr += "iii";
      static eostring str{""};
      str += "ooo";
      eostr.insert(1, str);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "ioooii") , 0 );
   }

   {
      static eostring eostr{""};
      eostr += "iii";
      static eostring str{""};
      str += "ooo";
      eostr.insert(2, str);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "iioooi") , 0 );
   }

   {
      static eostring eostr{""};
      eostr += "iii";
      static eostring str{""};
      str += "ooo";
      eostr.insert(3, str);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "iiiooo") , 0 );
   }

   {
      static eostring eostr{"abcdefg"};
      static eostring str{"ooo"};
      CHECK_ASSERT( "eostring::insert", []() {eostr.insert(-1, str);} )
   }

   //// eostring& erase(size_t pos = 0, size_t len = npos)
   {
      static eostring eostr{"abcdefgh"};
      eostr.erase();
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 );
   }

   {
      static eostring eostr{"abcdefgh"};
      eostr.erase(0);
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 );
   }

   {
      static eostring eostr{"abcdefgh"};
      eostr.erase(0, eostring::npos);
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 );
   }

   {
      static eostring eostr{"abcdefgh"};
      eostr.erase(1, eostring::npos);
      CHECK_EQUAL( eostr.size(), 1 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "a"), 0 );
   }

   {
      static eostring eostr{"abcdefgh"};
      eostr.erase(2, eostring::npos);
      CHECK_EQUAL( eostr.size(), 2 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "ab"), 0 );
   }

   {
      static eostring eostr{"abcdefgh"};
      eostr.erase(3, eostring::npos);
      CHECK_EQUAL( eostr.size(), 3 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 );
   }

   {
      static eostring eostr{"abcdefgh"};
      eostr.erase(4, eostring::npos);
      CHECK_EQUAL( eostr.size(), 4 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcd"), 0 );
   }

   {
      static eostring eostr{"abcdefgh"};
      eostr.erase(5, eostring::npos);
      CHECK_EQUAL( eostr.size(), 5 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcde"), 0 );
   }

   {
      static eostring eostr{"abcdefgh"};
      eostr.erase(6, eostring::npos);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 );
   }

   {
      static eostring eostr{"abcdefgh"};
      eostr.erase(7, eostring::npos);
      CHECK_EQUAL( eostr.size(), 7 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefg"), 0 );
   }

   {
      static eostring eostr{"abcdefgh"};
      eostr.erase(8, eostring::npos);
      CHECK_EQUAL( eostr.size(), 8 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 );
   }

   {
      static eostring eostr{"abcdefgh"};
      eostr.erase(8, 0);
      CHECK_EQUAL( eostr.size(), 8 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 );
   }

   {
      static eostring eostr{"abcdefg"};
      CHECK_ASSERT( "eostring::erase", []() {eostr.erase(-1, 1);} )
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase();
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase(0);
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase(0, eostring::npos);
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( strcmp(eostr.c_str(), ""), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase(1, eostring::npos);
      CHECK_EQUAL( eostr.size(), 1 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "a"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase(2, eostring::npos);
      CHECK_EQUAL( eostr.size(), 2 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "ab"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase(3, eostring::npos);
      CHECK_EQUAL( eostr.size(), 3 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abc"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase(4, eostring::npos);
      CHECK_EQUAL( eostr.size(), 4 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcd"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase(5, eostring::npos);
      CHECK_EQUAL( eostr.size(), 5 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcde"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase(6, eostring::npos);
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase(7, eostring::npos);
      CHECK_EQUAL( eostr.size(), 7 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefg"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase(8, eostring::npos);
      CHECK_EQUAL( eostr.size(), 8 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 );
   }

   {
      static eostring eostr{""};
      eostr += "abcdefgh";
      
      eostr.erase(8, 0);
      CHECK_EQUAL( eostr.size(), 8 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgh"), 0 );
   }

   {
      static eostring eostr{"abcdefg"};
      CHECK_ASSERT( "eostring::erase", []() {eostr.erase(-1, 1);} )
   }

   //// eostring& append(const char* str)
   {
      static eostring eostr{};
      static const char* str{"iii"};
      eostr.append(str);
      CHECK_EQUAL( eostr.size(), 3 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "iii"), 0 );
   }

   {
      static eostring eostr{"abcdefg"};
      static const char* str{"iii"};
      eostr.append(str);
      CHECK_EQUAL( eostr.size(), 10 );
      CHECK_EQUAL( eostr.capacity(), 20 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgiii"), 0 );
   }

   {
      static eostring eostr{"abcdefg"};
      static const char* null_man{nullptr};
      CHECK_ASSERT( "eostring::append", []() {eostr.append(null_man);} )
   }

   //// eostring& append(const eostring& str)
   {
      static eostring eostr{};
      static const eostring str{"iii"};
      eostr.append(str);
      CHECK_EQUAL( eostr.size(), 3 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "iii"), 0 );
   }

   {
      static eostring eostr{"abcdefg"};
      static const eostring str{"iii"};
      eostr.append(str);
      CHECK_EQUAL( eostr.size(), 10 );
      CHECK_EQUAL( eostr.capacity(), 20 );
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdefgiii"), 0 );
   }

   //// eostring& operator+=(const char c)
   {
      static eostring eostr0{};
      static eostring eostr1{"a"};
      static eostring eostr2{"abcdef"};

      eostr0 += 'c';
      CHECK_EQUAL( eostr0.size(), 1 );
      CHECK_EQUAL( eostr0.capacity(), 2 );
      CHECK_EQUAL( strcmp(eostr0.c_str(), "c"), 0 );
      
      eostr1 += 'c';
      eostr1 += 'c';
      CHECK_EQUAL( eostr1.size(), 3 );
      CHECK_EQUAL( eostr1.capacity(), 4 );
      CHECK_EQUAL( strcmp(eostr1.c_str(), "acc"), 0 );

      eostr2 += 'c';
      CHECK_EQUAL( eostr2.size(), 7 );
      CHECK_EQUAL( eostr2.capacity(), 14 );
      CHECK_EQUAL( strcmp(eostr2.c_str(), "abcdefc"), 0 );
   }

   //// eostring& operator+=(const char* rhs)
   {
      static eostring eostr0{};
      static eostring eostr1{"a"};
      static eostring eostr2{"abcdef"};
      static eostring eostr3{"abcdef"};

      eostr0 += "c";
      CHECK_EQUAL( eostr0.size(), 1 );
      CHECK_EQUAL( eostr0.capacity(), 2 );
      CHECK_EQUAL( strcmp(eostr0.c_str(), "c"), 0 );

      eostr1 += "c";
      eostr1 += "c";
      CHECK_EQUAL( eostr1.size(), 3 );
      CHECK_EQUAL( eostr1.capacity(), 4 );
      CHECK_EQUAL( strcmp(eostr1.c_str(), "acc"), 0 );

      eostr2 += "c";
      CHECK_EQUAL( eostr2.size(), 7 );
      CHECK_EQUAL( eostr2.capacity(), 14 );
      CHECK_EQUAL( strcmp(eostr2.c_str(), "abcdefc"), 0 );

      eostr3 += "ghijklm";
      CHECK_EQUAL( eostr3.size(), 13 );
      eosio::print(eostr3.capacity());
      CHECK_EQUAL( eostr3.capacity(), 26 );
      CHECK_EQUAL( strcmp(eostr3.c_str(), "abcdefghijklm"), 0 );
   }

   //// eostring& operator+=(const eostring& rhs)
   {
      static eostring eostr0{};
      static eostring eostr1{"a"};
      static eostring eostr2{"abcdef"};
      static eostring eostr3{"abcdef"};

      eostr0 += eostring{"c"};
      CHECK_EQUAL( eostr0.size(), 1 );
      CHECK_EQUAL( eostr0.capacity(), 2 );
      CHECK_EQUAL( strcmp(eostr0.c_str(), "c"), 0 );

      eostr1 += eostring{"c"};
      eostr1 += eostring{"c"};
      CHECK_EQUAL( eostr1.size(), 3 );
      CHECK_EQUAL( eostr1.capacity(), 4 );
      CHECK_EQUAL( strcmp(eostr1.c_str(), "acc"), 0 );

      eostr2 += eostring{"c"};
      CHECK_EQUAL( eostr2.size(), 7 );
      CHECK_EQUAL( eostr2.capacity(), 14 );
      CHECK_EQUAL( strcmp(eostr2.c_str(), "abcdefc"), 0 );

      eostr3 += eostring{"ghijklm"};
      CHECK_EQUAL( eostr3.size(), 13 );
      CHECK_EQUAL( eostr3.capacity(), 26 );
      CHECK_EQUAL( strcmp(eostr3.c_str(), "abcdefghijklm"), 0 );
   }

   //// eostring& operator+=(const eostring& s)
   {
      static eostring eostr0{"a"};
      static eostring eostr1{"b"};
      CHECK_EQUAL( eostr0.size(), 1 );
      eostr0 += eostr1;
      CHECK_EQUAL( eostr0.size(), 2 );
      CHECK_EQUAL( strcmp(eostr0.data(), "ab"), 0 );
   }

   {
      static eostring eostr0{"abc"};
      static eostring eostr1{"def"};
      CHECK_EQUAL( eostr0.size(), 3 );
      eostr0 += eostr1;
      CHECK_EQUAL( eostr0.size(), 6 );
      CHECK_EQUAL( strcmp(eostr0.data(), "abcdef"), 0 );
   }

   //// inline void print(eosio::eostring str)
   {
      static const eostring eostr0{""};
      static const eostring eostr1{"abc"};
      static const eostring eostr2{"abcdef"};

      CHECK_PRINT( "", [](){ print(eostr0); } );
      CHECK_PRINT( "abc", [](){ print(eostr1); } );
      CHECK_PRINT( "abcdef", [](){ print(eostr2); } );
   }

   //// friend bool operator< (const eostring& lhs, const eostring& rhs)
   {
      static const eostring eostr0{"abc"};
      static const eostring eostr1{"def"};
      CHECK_EQUAL( (eostr0 < eostr0), false );
      CHECK_EQUAL( (eostr1 < eostr1), false );
      CHECK_EQUAL( (eostr0 < eostr1), true );
   }

   //// friend bool operator> (const eostring& lhs, const eostring& rhs)
   {
      static const eostring eostr0{"abc"};
      static const eostring eostr1{"def"};
      CHECK_EQUAL( (eostr0 > eostr0), false );
      CHECK_EQUAL( (eostr1 > eostr1), false );
      CHECK_EQUAL( (eostr0 > eostr1), false );
   }

   //// friend bool operator<=(const eostring& lhs, const eostring& rhs)
   {
      static const eostring eostr0{"abc"};
      static const eostring eostr1{"def"};
      CHECK_EQUAL( (eostr0 <= eostr0), true );
      CHECK_EQUAL( (eostr1 <= eostr1), true );
      CHECK_EQUAL( (eostr0 <= eostr1), true );
   }

   //// friend bool operator>=(const eostring& lhs, const eostring& rhs)
   {
      static const eostring eostr0{"abc"};
      static const eostring eostr1{"def"};
      CHECK_EQUAL( (eostr0 >= eostr0), true );
      CHECK_EQUAL( (eostr1 >= eostr1), true );
      CHECK_EQUAL( (eostr0 >= eostr1), false );
   }

   //// friend bool operator==(const eostring& lhs, const eostring& rhs)
   {
      static const eostring eostr0{"abc"};
      static const eostring eostr1{"def"};
      CHECK_EQUAL( (eostr0 == eostr0), true );
      CHECK_EQUAL( (eostr1 == eostr1), true );
      CHECK_EQUAL( (eostr0 == eostr1), false );
   }

   //// friend bool operator!=(const eostring& lhs, const eostring& rhs)
   {
      static const eostring eostr0{"abc"};
      static const eostring eostr1{"def"};
      CHECK_EQUAL( (eostr0 != eostr0), false );
      CHECK_EQUAL( (eostr1 != eostr1), false );
      CHECK_EQUAL( (eostr0 != eostr1), true );
   }

   //// template<typename DataStream>
   //// DataStream& operator<<(DataStream& ds, const eostring& str)
   //// DataStream& operator>>(DataStream& ds, eostring& str)
   {
      static constexpr uint16_t buffer_size{256};
      static char datastream_buffer[buffer_size]{}; // Buffer for the datastream to point to
      static char buffer[buffer_size]; // Buffer to compare `datastream_buffer` with
      static datastream<char*> ds{datastream_buffer, buffer_size};
      
      ds.seekp(0);
      fill(std::begin(datastream_buffer), std::end(datastream_buffer), 0);
      static const eostring cstr {""};
      static eostring str{};
      ds << cstr;
      ds.seekp(0);
      ds >> str;
      CHECK_EQUAL( cstr, str )
   }

   {
      static constexpr uint16_t buffer_size{256};
      static char datastream_buffer[buffer_size]{};
      static char buffer[buffer_size];
      static datastream<char*> ds{datastream_buffer, buffer_size};
      
      ds.seekp(0);
      fill(std::begin(datastream_buffer), std::end(datastream_buffer), 0);
      static const eostring cstr {"a"};
      static eostring str{};
      ds << cstr;
      ds.seekp(0);
      ds >> str;
      CHECK_EQUAL( cstr, str )
   }

   {
      static constexpr uint16_t buffer_size{256};
      static char datastream_buffer[buffer_size]{};
      static char buffer[buffer_size];
      static datastream<char*> ds{datastream_buffer, buffer_size};
      
      ds.seekp(0);
      fill(std::begin(datastream_buffer), std::end(datastream_buffer), 0);
      static const eostring cstr {"abcdefghi"};
      static eostring str{};
      ds << cstr;
      ds.seekp(0);
      ds >> str;
      CHECK_EQUAL( cstr, str )
   }

silence_output(false);
EOSIO_TEST_END

int main() {
   EOSIO_TEST(eostring_test);
   return has_failed();
}
