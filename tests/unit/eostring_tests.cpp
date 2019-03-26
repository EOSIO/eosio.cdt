/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

// TODO:
// test namepstace impl
// test concat
// test datastream
#include <eosio/eosio.hpp>
#include <eosio/tester.hpp>
#include <eosio/eostring.hpp>

#include <eosio/print.hpp> /// remove
using namespace eosio;

// Definitions found in `eosio.cdt/libraries/eosiolib/core/eosio/eostring.hpp`
EOSIO_TEST_BEGIN(eostring_test)
   silence_output(false);

   //// template <size_t N>
   //// eostring(const char (&str)[N])
   {
      static const eosio::eostring eostr0{"a"};
      static const eosio::eostring eostr1{"abcdef"};

      CHECK_EQUAL( eostr0.size(), 1 );
      CHECK_EQUAL( eostr0.capacity(), 1 );
      CHECK_EQUAL( strcmp(eostr0.data(), "a"), 0 );

      CHECK_EQUAL( eostr1.size(), 6 );
      CHECK_EQUAL( eostr1.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr1.data(), "abcdef"), 0 );
   }

   //// eosio::eostring()
   {
      static const eosio::eostring eostr{};

      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( eostr.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr.data(), ""), 0 );
   }

   //// eosio::eostring(const size_t n, const char c)
   {
      static const eosio::eostring eostr0(0, 'c');
      static const eosio::eostring eostr1(1, 'c');
      static const eosio::eostring eostr2(3, 'c');

      CHECK_EQUAL( eostr0.size(), 0 );
      CHECK_EQUAL( eostr0.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0.data(), ""), 0 );

      CHECK_EQUAL( eostr1.size(), 1 );
      CHECK_EQUAL( eostr1.capacity(), 2 );
      CHECK_EQUAL( strcmp(eostr1.data(), "c"), 0 );

      CHECK_EQUAL( eostr2.size(), 3 );
      CHECK_EQUAL( eostr2.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr2.data(), "ccc"), 0 );
   }

   //// eosio::eostring(const eosio::eostring& str, const size_t pos, const size_t n = eosio::eostring::npos)
   {
      static const eosio::eostring eostr{"abcdef"};
      static const eosio::eostring eostr0_sub(eostr, 0, 0);
      static const eosio::eostring eostr1_sub(eostr, 1, 0);
      static const eosio::eostring eostr2_sub(eostr, 0, 1);
      static const eosio::eostring eostr3_sub(eostr, 0, 3);
      static const eosio::eostring eostr4_sub(eostr, 0, 8);
      static const eosio::eostring eostr5_sub(eostr, 0, 7);
      static const eosio::eostring eostr6_sub(eostr, 0, 6);
      static const eosio::eostring eostr7_sub(eostr, 3, 3);
      static const eosio::eostring eostr8_sub(eostr, 3, 2);

      CHECK_EQUAL( eostr0_sub.size(), 0 );
      CHECK_EQUAL( eostr0_sub.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0_sub.data(), ""), 0 );

      CHECK_EQUAL( eostr1_sub.size(), 0 );
      CHECK_EQUAL( eostr1_sub.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr1_sub.data(), ""), 0 );

      CHECK_EQUAL( eostr2_sub.size(), 1 );
      CHECK_EQUAL( eostr2_sub.capacity(), 2 );
      CHECK_EQUAL( strcmp(eostr2_sub.data(), "a"), 0 );

      CHECK_EQUAL( eostr3_sub.size(), 3 );
      CHECK_EQUAL( eostr3_sub.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr3_sub.data(), "abc"), 0 );

      CHECK_EQUAL( eostr4_sub.size(), 6 );
      CHECK_EQUAL( eostr4_sub.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr4_sub.data(), "abcdef"), 0 );

      CHECK_EQUAL( eostr5_sub.size(), 6 );
      CHECK_EQUAL( eostr5_sub.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr5_sub.data(), "abcdef"), 0 );

      CHECK_EQUAL( eostr6_sub.size(), 6 );
      CHECK_EQUAL( eostr6_sub.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr6_sub.data(), "abcdef"), 0 );

      CHECK_EQUAL( eostr7_sub.size(), 3 );
      CHECK_EQUAL( eostr7_sub.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr7_sub.data(), "def"), 0 );

      CHECK_EQUAL( eostr8_sub.size(), 2 );
      CHECK_EQUAL( eostr8_sub.capacity(), 4 );
      CHECK_EQUAL( strcmp(eostr8_sub.data(), "de"), 0 );
   }

   //// constexpr eostring(const eostring& str)
   {
      static const eosio::eostring eostr0{""};
      static const eosio::eostring eostr1{"a"};
      static const eosio::eostring eostr2{"abcdef"};
      static const eosio::eostring eostr0_cpy{eostr0};
      static const eosio::eostring eostr1_cpy{eostr1};
      static const eosio::eostring eostr2_cpy{eostr2};
      
      CHECK_EQUAL( eostr0_cpy.size(), 0 );
      CHECK_EQUAL( eostr0_cpy.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0_cpy.data(), ""), 0 );
      CHECK_EQUAL( eostr0.data() != eostr0_cpy.data(), false );

      CHECK_EQUAL( eostr1_cpy.size(), 1 );
      CHECK_EQUAL( eostr1_cpy.capacity(), 1 );
      CHECK_EQUAL( strcmp(eostr1_cpy.data(), "a"), 0 );
      CHECK_EQUAL( eostr1.data() != eostr1_cpy.data(), false );

      CHECK_EQUAL( eostr2_cpy.size(), 6 );
      CHECK_EQUAL( eostr2_cpy.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr2_cpy.data(), "abcdef"), 0 );
      CHECK_EQUAL( eostr2.data() != eostr2_cpy.data(), false );
   }

   //// constexpr eostring(const eostring&& str)
   {
      static const eosio::eostring eostr0{""};
      static const eosio::eostring eostr1{"a"};
      static const eosio::eostring eostr2{"abcdef"};
      static const eosio::eostring eostr0_mv{eostr0};
      static const eosio::eostring eostr1_mv{eostr1};
      static const eosio::eostring eostr2_mv{eostr2};
      
      CHECK_EQUAL( eostr0_mv.size(), 0 );
      CHECK_EQUAL( eostr0_mv.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0_mv.data(), ""), 0 );
      CHECK_EQUAL( eostr0.data() == eostr0_mv.data(), true );

      CHECK_EQUAL( eostr1_mv.size(), 1 );
      CHECK_EQUAL( eostr1_mv.capacity(), 1 );
      CHECK_EQUAL( strcmp(eostr1_mv.data(), "a"), 0 );
      CHECK_EQUAL( eostr1.data() == eostr1_mv.data(), true );

      CHECK_EQUAL( eostr2_mv.size(), 6 );
      CHECK_EQUAL( eostr2_mv.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr2_mv.data(), "abcdef"), 0 );
      CHECK_EQUAL( eostr2.data() == eostr2_mv.data(), true );
   }

   //// eosio::eostring& operator=(const eosio::eostring& str);
   {
      static const eosio::eostring eostr0{""};
      static const eosio::eostring eostr1{"a"};
      static const eosio::eostring eostr2{"abcdef"};
      static const eosio::eostring eostr0_cpy_assig = eostr0;
      static const eosio::eostring eostr1_cpy_assig = eostr1;
      static const eosio::eostring eostr2_cpy_assig = eostr2;
      
      CHECK_EQUAL( eostr0_cpy_assig.size(), 0 );
      CHECK_EQUAL( eostr0_cpy_assig.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0_cpy_assig.data(), ""), 0 );
      CHECK_EQUAL( eostr0.data() != eostr0_cpy_assig.data(), false );

      CHECK_EQUAL( eostr1_cpy_assig.size(), 1 );
      CHECK_EQUAL( eostr1_cpy_assig.capacity(), 1 );
      CHECK_EQUAL( strcmp(eostr1_cpy_assig.data(), "a"), 0 );
      CHECK_EQUAL( eostr1.data() != eostr1_cpy_assig.data(), false );

      CHECK_EQUAL( eostr2_cpy_assig.size(), 6 );
      CHECK_EQUAL( eostr2_cpy_assig.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr2_cpy_assig.data(), "abcdef"), 0 );
      CHECK_EQUAL( eostr2.data() != eostr2_cpy_assig.data(), false );
   }

   //// eosio::eostring& operator=(eosio::eostring&& str)
   {
      static const eosio::eostring eostr0{""};
      static const eosio::eostring eostr1{"a"};
      static const eosio::eostring eostr2{"abcdef"};
      static const eosio::eostring eostr0_mv_assig = std::move(eostr0);
      static const eosio::eostring eostr1_mv_assig = std::move(eostr1);
      static const eosio::eostring eostr2_mv_assig = std::move(eostr2);
      
      CHECK_EQUAL( eostr0_mv_assig.size(), 0 );
      CHECK_EQUAL( eostr0_mv_assig.capacity(), 0 );
      CHECK_EQUAL( strcmp(eostr0_mv_assig.data(), ""), 0 );
      CHECK_EQUAL( eostr0.data() == eostr0_mv_assig.data(), true );

      CHECK_EQUAL( eostr1_mv_assig.size(), 1 );
      CHECK_EQUAL( eostr1_mv_assig.capacity(), 1 );
      CHECK_EQUAL( strcmp(eostr1_mv_assig.data(), "a"), 0 );
      CHECK_EQUAL( eostr1.data() == eostr1_mv_assig.data(), true );

      CHECK_EQUAL( eostr2_mv_assig.size(), 6 );
      CHECK_EQUAL( eostr2_mv_assig.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr2_mv_assig.data(), "abcdef"), 0 );
      CHECK_EQUAL( eostr2.data() == eostr2_mv_assig.data(), true );
   }

   //// eosio::eostring& operator=(const char* str)
   {
      static eosio::eostring eostr{};
      eostr = "abcdef";
      
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 6 );
      CHECK_EQUAL( strcmp(eostr.data(), "abcdef"), 0 );

      // Note that `_capacity` on a call to `data()` because the allocating
      // of memory is deferred up until said function call.
      
      eostr = eostr;
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( eostr.capacity(), 12 );
      CHECK_EQUAL( strcmp(eostr.data(), "abcdef"), 0 );
   }

   //// char& operator[](const size_t n)
   {
      static eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr[0], 'a' );
      CHECK_EQUAL( eostr[5], 'f' );
   }

   //// const char& operator[](const size_t n) const
   {
      static const eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr[0], 'a' );
      CHECK_EQUAL( eostr[5], 'f' );
   }

   //// char& at(const size_t n)
   {
      static eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.at(0), 'a' );
      CHECK_EQUAL( eostr.at(5), 'f' );
      
      CHECK_ASSERT( "eostring::at", []() {eostr.at(6);} )
   }

   //// const char& at(const size_t n) const
   {
      static const eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.at(0), 'a' );
      CHECK_EQUAL( eostr.at(5), 'f' );

      CHECK_ASSERT( "eostring::at const", []() {eostr.at(6);} )
   }

   //// char& front()
   {
      static eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.front(), 'a' );

      static eosio::eostring empty_str;
      CHECK_EQUAL( eostr.front(), 'a' );
   }

   //// const char& front() const
   {
      static const eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.front(), 'a' );
   }

   //// char& back()
   {
      static eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.back(), 'f' );
   }

   //// const char& back() const
   {
      static const eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.back(), 'f' );
   }

   //// char* data()
   {
      static eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( strcmp(eostr.data(), "abcdef"), 0 );

      eostr = "abc";
      CHECK_EQUAL( strcmp(eostr.data(), "abc"), 0 );
   }

   //// const char* data() const
   {
      static const eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( strcmp(eostr.data(), "abcdef"), 0 );
   }

   //// const char* c_str() const
   {
      static eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 );
   }

   {
      static const eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( strcmp(eostr.c_str(), "abcdef"), 0 );
   }

   //// char* begin()
   {
      static eosio::eostring eostr{"abcdef"};
      char* iter{eostr.begin()};
      CHECK_EQUAL( strcmp(eostr.data(), iter), 0 );
   }

   //// const char* cbegin() const
   {
      static const eosio::eostring eostr{"abcdef"};
      const char* iter{eostr.cbegin()};
      CHECK_EQUAL( strcmp(eostr.data(), iter), 0 );
   }

   //// char* end()
   {
      static eosio::eostring eostr{"abcdef"};
      char* iter{eostr.end()};
      CHECK_EQUAL( strcmp(eostr.data()+eostr.size(), iter), 0 );
   }

   //// const char* cend() const
   {
      static eosio::eostring eostr{"abcdef"};
      const char* iter{eostr.cend()};
      CHECK_EQUAL( strcmp(eostr.data()+eostr.size(), iter), 0 );
   }

   // //// bool eosio::eostring::empty() const
   // {
   //    static eosio::eostring eostr{};
   //    CHECK_EQUAL( eostr.empty(), true );
   //    eostr += 'c';
   //    CHECK_EQUAL( eostr.empty(), false );
   // }

   // //// size_t eosio::eostring::size() const
   // {
   //    static eosio::eostring eostr{"abcdef"};
   //    CHECK_EQUAL( eostr.size(), 6 );
   //    eostr += 'g';
   //    CHECK_EQUAL( eostr.size(), 7 );
   // }

   // //// size_t eosio::eostring::length() const
   // {
   //    static eosio::eostring eostr{"abcdef"};
   //    CHECK_EQUAL( eostr.length(), 6 );
   //    eostr += 'g';
   //    CHECK_EQUAL( eostr.length(), 7 );
   // }

   // //// size_t eosio::eostring::capacity() const
   // {
   //    static eosio::eostring eostr{"abc"};
   //    CHECK_EQUAL( eostr.capacity(), 6 );
   //    eostr += 'd', eostr += 'e', eostr += 'f';
   //    CHECK_EQUAL( eostr.capacity(), 6 );
   //    eostr += 'g';
   //    CHECK_EQUAL( eostr.capacity(), 14 );
   // }

   //// size_t eosio::eostring::max_size() const
   {
      static const eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.max_size(), eosio::eostring::npos );
   }

   //// void reserve(const size_t n)
   {
      static eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.capacity(), 6 );
      eostr.reserve(10);
      CHECK_EQUAL( eostr.capacity(), 10 );
      eostr.reserve(24);
      CHECK_EQUAL( eostr.capacity(), 24 );
   }

   //// void eosio::eostring::shrink_to_fit() const
   {
      static eosio::eostring eostr0{};
      static eosio::eostring eostr1{"a"};
      static eosio::eostring eostr2{"abcdef"};

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

   //// void eosio::eostring::clear()
   {
      static eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( eostr.empty(), false );
      eostr.clear();
      CHECK_EQUAL( eostr.empty(), true );
      CHECK_EQUAL( eostr.size(), 0 );
   }

   //// void resize(size_t n)
   {
      static eosio::eostring eostr{"abcdef"};

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

   //// void swap(eosio::eostring& str)
   {
      static eosio::eostring eostr_swap0{"abc"};
      static eosio::eostring eostr_swap1{"123456"};

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
   //    static eosio::eostring eostr{"abcdef"};
   //    CHECK_EQUAL( eostr.size(), 6 );
   //    eostr.push_back('g');
   //    CHECK_EQUAL( eostr.size(), 7 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "abcdefg"), 0 );
   // }

   //// void pop_back()
   {
      static eosio::eostring eostr{"abcdefg"};
      CHECK_EQUAL( eostr.size(), 7 );
      eostr.pop_back();
      CHECK_EQUAL( eostr.size(), 6 );
      CHECK_EQUAL( strcmp(eostr.data(), "abcdef"), 0 );
   }

   {
      static eosio::eostring eostr{"abc"};
      CHECK_EQUAL( eostr.size(), 3 );
      eostr.pop_back();
      eostr.pop_back();
      eostr.pop_back();
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( strcmp(eostr.data(), ""), 0 );

      eostr.pop_back();
      CHECK_EQUAL( eostr.size(), 0 );
      CHECK_EQUAL( strcmp(eostr.data(), ""), 0 );
   }

   //// eosio::eostring substr(size_t pos = 0, size_t len = npos) const
   {
      static const eosio::eostring eostr{"abcdef"};
      CHECK_EQUAL( strcmp(eostr.substr(0).data(), "abcdef"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,0).data(), ""), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,1).data(), "a"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,2).data(), "ab"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,3).data(), "abc"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,4).data(), "abcd"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,5).data(), "abcde"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(0,6).data(), "abcdef"), 0 );

      CHECK_EQUAL( strcmp(eostr.substr(1,0).data(), ""), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,1).data(), "b"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,2).data(), "bc"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,3).data(), "bcd"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,4).data(), "bcde"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,5).data(), "bcdef"), 0 );
      CHECK_EQUAL( strcmp(eostr.substr(1,6).data(), "bcdef"), 0 );
   }

   // //// size_t copy(char* dest, size_t len, size_t pos = 0) const
   // {
   //    static const eosio::eostring eostr{"abcdef"};
   //    static char str[7]{};

   //    CHECK_EQUAL( eostr.copy(str, 0), 0 );
   //    CHECK_EQUAL( strcmp(str, ""), 0 );

   //    CHECK_EQUAL( eostr.copy(str, 10), 6 );
   //    CHECK_EQUAL( strcmp(str, "abcdef"), 0 );

   //    CHECK_EQUAL( eostr.copy(str, 10, 0), 6 );
   //    CHECK_EQUAL( strcmp(str, "abcdef"), 0 );

   //    CHECK_EQUAL( eostr.copy(str, 10, 1), 5 );
   //    CHECK_EQUAL( strcmp(str, "bcdef"), 0 );

   //    CHECK_EQUAL( eostr.copy(str, 10, 2), 4 );
   //    CHECK_EQUAL( strcmp(str, "cdef"), 0 );

   //    CHECK_EQUAL( eostr.copy(str, 10, 3), 3 );
   //    CHECK_EQUAL( strcmp(str, "def"), 0 );

   //    CHECK_EQUAL( eostr.copy(str, 10, 4), 2 );
   //    CHECK_EQUAL( strcmp(str, "ef"), 0 );

   //    CHECK_EQUAL( eostr.copy(str, 10, 5), 1 );
   //    CHECK_EQUAL( strcmp(str, "f"), 0 );

   //    CHECK_EQUAL( eostr.copy(str, 10, 6), 0 );
   //    CHECK_EQUAL( strcmp(str, ""), 0 );
   // }

   // //// eosio::eostring& insert(const size_t pos, const char* str)
   // {
   //    static eosio::eostring eostr{"iii"};
   //    static const char* str{"ooo"};
   //    eostr.insert(0, str);
   //    CHECK_EQUAL( strcmp(eostr.data(), "oooiii"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"iii"};
   //    static const char* str{"ooo"};
   //    eostr.insert(1, str);
   //    CHECK_EQUAL( strcmp(eostr.data(), "ioooii"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"iii"};
   //    static const char* str{"ooo"};
   //    eostr.insert(2, str);
   //    CHECK_EQUAL( strcmp(eostr.data(), "iioooi"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"iii"};
   //    static const char* str{"ooo"};
   //    eostr.insert(3, str);
   //    CHECK_EQUAL( strcmp(eostr.data(), "iiiooo"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefg"};
   //    static const char* null_man{nullptr};
   //    CHECK_ASSERT( "eostring::insert", []() {eostr.insert(0, null_man, 1);} )
   //    CHECK_ASSERT( "eostring::insert", []() {eostr.insert(-1, "ooo", 1);} )
   // }

   // //// eosio::eostring& insert(const size_t pos, const eosio::eostring& str)
   // {
   //    static eosio::eostring eostr{};
   //    static const eosio::eostring str{"ooo"};
   //    eostr.insert(0, str);
   //    CHECK_EQUAL( eostr.size(), 3 );
   //    CHECK_EQUAL( eostr.capacity(), 6 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "ooo"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abc"};
   //    static const eosio::eostring str{"d"};
   //    eostr.insert(0, str);
   //    CHECK_EQUAL( eostr.size(), 4 );
   //    CHECK_EQUAL( eostr.capacity(), 6 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "dabc"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abc"};
   //    static const eosio::eostring str{"def"};
   //    eostr.insert(0, str);
   //    CHECK_EQUAL( eostr.size(), 6 );
   //    CHECK_EQUAL( eostr.capacity(), 12 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "defabc"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"iii"};
   //    static const eosio::eostring str{"ooo"};
   //    eostr.insert(0, str);
   //    CHECK_EQUAL( eostr.size(), 6 );
   //    CHECK_EQUAL( eostr.capacity(), 12 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "oooiii") , 0 );
   // }

   // {
   //    static eosio::eostring eostr{"iii"};
   //    static const eosio::eostring str{"ooo"};
   //    eostr.insert(1, str);
   //    CHECK_EQUAL( eostr.size(), 6 );
   //    CHECK_EQUAL( eostr.capacity(), 12 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "ioooii") , 0 );
   // }

   // {
   //    static eosio::eostring eostr{"iii"};
   //    static const eosio::eostring str{"ooo"};
   //    eostr.insert(2, str);
   //    CHECK_EQUAL( eostr.size(), 6 );
   //    CHECK_EQUAL( eostr.capacity(), 12 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "iioooi") , 0 );
   // }

   // {
   //    static eosio::eostring eostr{"iii"};
   //    static const eosio::eostring str{"ooo"};
   //    eostr.insert(3, str);
   //    CHECK_EQUAL( eostr.size(), 6 );
   //    CHECK_EQUAL( eostr.capacity(), 12 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "iiiooo") , 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefg"};
   //    static const eosio::eostring str{"ooo"};
   //    CHECK_ASSERT( "eostring::insert", []() {eostr.insert(-1, str);} )
   // }

   // //// eosio::eostring& erase(size_t pos = 0, size_t len = npos)
   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase();
   //    CHECK_EQUAL( eostr.size(), 0 );
   //    CHECK_EQUAL( strcmp(eostr.data(), ""), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase(0);
   //    CHECK_EQUAL( eostr.size(), 0 );
   //    CHECK_EQUAL( strcmp(eostr.data(), ""), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase(0, eosio::eostring::npos);
   //    CHECK_EQUAL( eostr.size(), 0 );
   //    CHECK_EQUAL( strcmp(eostr.data(), ""), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase(1, eosio::eostring::npos);
   //    CHECK_EQUAL( eostr.size(), 1 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "a"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase(2, eosio::eostring::npos);
   //    CHECK_EQUAL( eostr.size(), 2 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "ab"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase(3, eosio::eostring::npos);
   //    CHECK_EQUAL( eostr.size(), 3 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "abc"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase(4, eosio::eostring::npos);
   //    CHECK_EQUAL( eostr.size(), 4 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "abcd"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase(5, eosio::eostring::npos);
   //    CHECK_EQUAL( eostr.size(), 5 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "abcde"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase(6, eosio::eostring::npos);
   //    CHECK_EQUAL( eostr.size(), 6 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "abcdef"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase(7, eosio::eostring::npos);
   //    CHECK_EQUAL( eostr.size(), 7 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "abcdefg"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase(8, eosio::eostring::npos);
   //    CHECK_EQUAL( eostr.size(), 8 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "abcdefgh"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefgh"};
   //    eostr.erase(8, 0);
   //    CHECK_EQUAL( eostr.size(), 8 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "abcdefgh"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefg"};
   //    CHECK_ASSERT( "eostring::erase", []() {eostr.erase(-1, 1);} )
   // }

   // //// eosio::eostring& append(const char* str)
   // {
   //    static eosio::eostring eostr{};
   //    static const char* str{"iii"};
   //    eostr.append(str);
   //    CHECK_EQUAL( eostr.size(), 3 );
   //    CHECK_EQUAL( eostr.capacity(), 6 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "iii"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefg"};
   //    static const char* str{"iii"};
   //    eostr.append(str);
   //    CHECK_EQUAL( eostr.size(), 10 );
   //    CHECK_EQUAL( eostr.capacity(), 14 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "abcdefgiii"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefg"};
   //    static const char* null_man{nullptr};
   //    CHECK_ASSERT( "eostring::append", []() {eostr.append(null_man);} )
   // }

   // //// eosio::eostring& append(const eosio::eostring& str)
   // {
   //    static eosio::eostring eostr{};
   //    static const eosio::eostring str{"iii"};
   //    eostr.append(str);
   //    CHECK_EQUAL( eostr.size(), 3 );
   //    CHECK_EQUAL( eostr.capacity(), 6 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "iii"), 0 );
   // }

   // {
   //    static eosio::eostring eostr{"abcdefg"};
   //    static const eosio::eostring str{"iii"};
   //    eostr.append(str);
   //    CHECK_EQUAL( eostr.size(), 10 );
   //    CHECK_EQUAL( eostr.capacity(), 14 );
   //    CHECK_EQUAL( strcmp(eostr.data(), "abcdefgiii"), 0 );
   // }

   // //// eosio::eostring& operator+=(const char c)
   // {
   //    static eosio::eostring eostr0{};
   //    static eosio::eostring eostr1{"a"};
   //    static eosio::eostring eostr2{"abcdef"};

   //    eostr0 += 'c';
   //    CHECK_EQUAL( eostr0.size(), 2 );
   //    CHECK_EQUAL( eostr0.capacity(), 4 );
   //    CHECK_EQUAL( strcmp(eostr0.data(), "c"), 0 );

   //    eostr1 += 'c';
   //    eostr1 += 'c';
   //    CHECK_EQUAL( eostr1.size(), 3 );
   //    CHECK_EQUAL( eostr1.capacity(), 6 );
   //    CHECK_EQUAL( strcmp(eostr1.data(), "acc"), 0 );

   //    eostr2 += 'c';
   //    CHECK_EQUAL( eostr2.size(), 7 );
   //    CHECK_EQUAL( eostr2.capacity(), 12 );
   //    CHECK_EQUAL( strcmp(eostr2.data(), "abcdefc"), 0 );
   // }

   // //// eosio::eostring& operator+=(const char* rhs)
   // {
   //    static eosio::eostring eostr0{};
   //    static eosio::eostring eostr1{"a"};
   //    static eosio::eostring eostr2{"abcdef"};
   //    static eosio::eostring eostr3{"abcdef"};

   //    eostr0 += "c";
   //    CHECK_EQUAL( eostr0.size(), 2 );
   //    CHECK_EQUAL( eostr0.capacity(), 4 );
   //    CHECK_EQUAL( strcmp(eostr0.data(), "c"), 0 );

   //    eostr1 += "c";
   //    eostr1 += "c";
   //    CHECK_EQUAL( eostr1.size(), 3 );
   //    CHECK_EQUAL( eostr1.capacity(), 6 );
   //    CHECK_EQUAL( strcmp(eostr1.data(), "acc"), 0 );

   //    eostr2 += "c";
   //    CHECK_EQUAL( eostr2.size(), 7 );
   //    CHECK_EQUAL( eostr2.capacity(), 12 );
   //    CHECK_EQUAL( strcmp(eostr2.data(), "abcdefc"), 0 );

   //    eostr3 += "ghijklm";
   //    CHECK_EQUAL( eostr3.size(), 13 );
   //    CHECK_EQUAL( eostr3.capacity(), 26 );
   //    CHECK_EQUAL( strcmp(eostr3.data(), "abcdefghijklm"), 0 );
   // }

   // //// eosio::eostring& operator+=(const eosio::eostring& rhs)
   // {
   //    static eosio::eostring eostr0{};
   //    static eosio::eostring eostr1{"a"};
   //    static eosio::eostring eostr2{"abcdef"};
   //    static eosio::eostring eostr3{"abcdef"};

   //    eostr0 += eosio::eostring{"c"};
   //    CHECK_EQUAL( eostr0.size(), 2 );
   //    CHECK_EQUAL( eostr0.capacity(), 4 );
   //    CHECK_EQUAL( strcmp(eostr0.data(), "c"), 0 );

   //    eostr1 += eosio::eostring{"c"};
   //    eostr1 += eosio::eostring{"c"};
   //    CHECK_EQUAL( eostr1.size(), 3 );
   //    CHECK_EQUAL( eostr1.capacity(), 6 );
   //    CHECK_EQUAL( strcmp(eostr1.data(), "acc"), 0 );

   //    eostr2 += eosio::eostring{"c"};
   //    CHECK_EQUAL( eostr2.size(), 7 );
   //    CHECK_EQUAL( eostr2.capacity(), 12 );
   //    CHECK_EQUAL( strcmp(eostr2.data(), "abcdefc"), 0 );

   //    eostr3 += eosio::eostring{"ghijklm"};
   //    CHECK_EQUAL( eostr3.size(), 13 );
   //    CHECK_EQUAL( eostr3.capacity(), 26 );
   //    CHECK_EQUAL( strcmp(eostr3.data(), "abcdefghijklm"), 0 );
   // }

   // //// friend bool operator< (const eosio::eostring& lhs, const eosio::eostring& rhs)
   // {
   //    static const eosio::eostring eostr0{"abc"};
   //    static const eosio::eostring eostr1{"def"};
   //    CHECK_EQUAL( (eostr0 < eostr0), false );
   //    CHECK_EQUAL( (eostr1 < eostr1), false );
   //    CHECK_EQUAL( (eostr0 < eostr1), true );
   // }

   // //// friend bool operator> (const eosio::eostring& lhs, const eosio::eostring& rhs)
   // {
   //    static const eosio::eostring eostr0{"abc"};
   //    static const eosio::eostring eostr1{"def"};
   //    CHECK_EQUAL( (eostr0 > eostr0), false );
   //    CHECK_EQUAL( (eostr1 > eostr1), false );
   //    CHECK_EQUAL( (eostr0 > eostr1), false );
   // }

   // //// friend bool operator<=(const eosio::eostring& lhs, const eosio::eostring& rhs)
   // {
   //    static const eosio::eostring eostr0{"abc"};
   //    static const eosio::eostring eostr1{"def"};
   //    CHECK_EQUAL( (eostr0 <= eostr0), true );
   //    CHECK_EQUAL( (eostr1 <= eostr1), true );
   //    CHECK_EQUAL( (eostr0 <= eostr1), true );
   // }

   // //// friend bool operator>=(const eosio::eostring& lhs, const eosio::eostring& rhs)
   // {
   //    static const eosio::eostring eostr0{"abc"};
   //    static const eosio::eostring eostr1{"def"};
   //    CHECK_EQUAL( (eostr0 >= eostr0), true );
   //    CHECK_EQUAL( (eostr1 >= eostr1), true );
   //    CHECK_EQUAL( (eostr0 >= eostr1), false );
   // }

   // //// friend bool operator==(const eosio::eostring& lhs, const eosio::eostring& rhs)
   // {
   //    static const eosio::eostring eostr0{"abc"};
   //    static const eosio::eostring eostr1{"def"};
   //    CHECK_EQUAL( (eostr0 == eostr0), true );
   //    CHECK_EQUAL( (eostr1 == eostr1), true );
   //    CHECK_EQUAL( (eostr0 == eostr1), false );
   // }

   // //// friend bool operator!=(const eosio::eostring& lhs, const eosio::eostring& rhs)
   // {
   //    static const eosio::eostring eostr0{"abc"};
   //    static const eosio::eostring eostr1{"def"};
   //    CHECK_EQUAL( (eostr0 != eostr0), false );
   //    CHECK_EQUAL( (eostr1 != eostr1), false );
   //    CHECK_EQUAL( (eostr0 != eostr1), true );
   // }

   // //// eosio::eostring& operator+=(const eosio::eostring& s)
   // {
   //    static eosio::eostring eostr0{"a"};
   //    static eosio::eostring eostr1{"b"};
   //    CHECK_EQUAL( eostr0.size(), 1 );
   //    eostr0 += eostr1;
   //    CHECK_EQUAL( eostr0.size(), 2 );
   //    CHECK_EQUAL( strcmp(eostr0.data(), "ab"), 0 );
   // }

   // {
   //    static eosio::eostring eostr0{"abc"};
   //    static eosio::eostring eostr1{"def"};
   //    CHECK_EQUAL( eostr0.size(), 3 );
   //    eostr0 += eostr1;
   //    CHECK_EQUAL( eostr0.size(), 6 );
   //    CHECK_EQUAL( strcmp(eostr0.data(), "abcdef"), 0 );
   // }

silence_output(false);
EOSIO_TEST_END

int main() {
   EOSIO_TEST(eostring_test);
   return has_failed();
}
