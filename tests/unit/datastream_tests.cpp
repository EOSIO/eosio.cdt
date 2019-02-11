// TODO: Make costructor comments across all files consistant
// TODO: Fix line 275 comment
/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <array>
#include <deque>
#include <list>
#include <set>
#include <string>
#include <vector>

#include <eosio/native/tester.hpp>
#include <eosiolib/binary_extension.hpp>
#include <eosiolib/crypto.hpp>
#include <eosiolib/datastream.hpp>
#include <eosiolib/ignore.hpp>
#include <eosiolib/symbol.hpp>

#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>

using std::array;
using std::begin;
using std::end;
using std::fill;
using std::list;
using std::map;
using std::set;
using std::string;
using std::tuple;
using std::vector;

using eosio::binary_extension;
using eosio::datastream;
using eosio::fixed_bytes;
using eosio::ignore;
using eosio::ignore_wrapper;
using eosio::key256;
using eosio::pack;
using eosio::pack_size;
using eosio::public_key;
using eosio::signature;
using eosio::symbol;
using eosio::symbol_code;
using eosio::unpack;

using boost::container::flat_map;
using boost::container::flat_set;

// This data structure (which cannot be defined within a test macro block) needs both a default and a
// user-defined constructor for a specific `binary extension` test
struct be_test {
   be_test() : val{42} {}
   be_test(int i) : val{i} {}
   int val;
      
   EOSLIB_SERIALIZE( be_test, (val) )
};

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(datastream_test)
   silence_output(true);

   static constexpr uint16_t buffer_size{256};
   char datastream_buffer[buffer_size]{}; // Buffer for the datastream to point to
   char buffer[buffer_size]; // Buffer to compare `datastream_buffer` with

   unsigned char j{0};
   for(int i{0}; i < buffer_size; ++i) // Fill the char array `datastream_buffer` with all 256 ASCII characters
      datastream_buffer[i] = j++;

   /// datastream(T, size_t)
   datastream<char*> ds{datastream_buffer, buffer_size};

   // inline void skip(size_t)
   ds.skip(1);
   CHECK_EQUAL( ds.pos(), datastream_buffer+1 )
   ds.skip(-1);
   CHECK_EQUAL( ds.pos(), datastream_buffer )
   
   // inline bool read(char*, size_t)
   CHECK_EQUAL( ds.read(buffer, 256), true )
   CHECK_EQUAL( memcmp(buffer, datastream_buffer, 256), 0)

   CHECK_ASSERT( "read", ([&]() {ds.read(buffer, 1);}) )

   // T pos()const
   CHECK_EQUAL( ds.pos(), datastream_buffer+256 )
   CHECK_EQUAL( ds.pos(), datastream_buffer )

   // inline bool seekp(size_t)
   ds.seekp(0);
   CHECK_EQUAL( ds.pos(), datastream_buffer )
   CHECK_EQUAL( ds.pos(), datastream_buffer+256 )

   // inline bool write(const char*, size_t)
   fill(begin(buffer), end(buffer), 1); // Fill `buffer` with a new set of values

   CHECK_EQUAL( ds.write(buffer, 256), true )
   CHECK_EQUAL( memcmp(buffer, datastream_buffer, 256), 0 )

   CHECK_ASSERT( "write", ([&]() {ds.write(buffer, 1);}) )

   // inline bool put(char)
   ds.seekp(0);
   CHECK_EQUAL( ds.put('c'), true )
   *buffer = 'c';
   CHECK_EQUAL( memcmp(buffer, datastream_buffer, 256), 0 )

   ds.seekp(256);
   CHECK_ASSERT( "put", ([&]() {ds.put('c');}) )
  
   // inline bool get(unsigned char&)
   unsigned char uch{};

   ds.seekp(0);
   CHECK_EQUAL( ds.get(uch), true )
   CHECK_EQUAL( uch, 'c' )

   // inline bool get(char&)
   char ch{};

   ds.seekp(0);
   CHECK_EQUAL( ds.get(ch), true )
   CHECK_EQUAL( ch, 'c' )

   // inline bool valid()const
   ds.seekp(256);
   CHECK_EQUAL( ds.valid(), true )

   ds.seekp(257);
   CHECK_EQUAL( ds.valid(), false )

   // inline size_t tellp()const
   ds.seekp(0);
   CHECK_EQUAL( ds.tellp(), 0 )
   ds.seekp(256);
   CHECK_EQUAL( ds.tellp(), 256 )
   ds.seekp(257);
   CHECK_EQUAL( ds.tellp(), 257 )

   //inline size_t remaining()const
   ds.seekp(0);
   CHECK_EQUAL( ds.remaining(), 256 )
   ds.seekp(256);
   CHECK_EQUAL( ds.remaining(), 0 )
   ds.seekp(257);
   CHECK_EQUAL( ds.remaining(), -1)

   silence_output(false);
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(datastream_specialization_test)
   silence_output(true);

   static constexpr uint16_t buffer_size{256};
   char datastream_buffer[buffer_size]{}; // Buffer for the datastream to point to
   char buffer[buffer_size]; // Buffer to compare `datastream_buffer` with

   unsigned char j{0};
   for(int i{0}; i < buffer_size; ++i) // Fill the char array `datastream_buffer` with all all 256 ASCII characters
      datastream_buffer[i] = j++;

   /// datastream(T, size_t)
   datastream<size_t> ds{buffer_size};

   // inline void skip(size_t)
   // inline size_t tellp()const
   CHECK_EQUAL( ds.skip(0), true)
   CHECK_EQUAL( ds.tellp(), 256)
   
   CHECK_EQUAL( ds.skip(1), true)
   CHECK_EQUAL( ds.tellp(), 257)
   
   CHECK_EQUAL( ds.skip(255), true)
   CHECK_EQUAL( ds.tellp(), 512)
   
   CHECK_EQUAL( ds.skip(1028), true)
   CHECK_EQUAL( ds.tellp(), 1540)
   
   // inline bool seekp(size_t)
   ds.seekp(0);
   CHECK_EQUAL( ds.tellp(), 0)

   // inline bool write(const char*,size_t)
   CHECK_EQUAL( ds.write(buffer, 256), true )
   CHECK_EQUAL( ds.tellp(), 256 )

   CHECK_EQUAL( ds.write(buffer, 1), true )
   CHECK_EQUAL( ds.tellp(), 257 )

   // inline bool put(char)
   char ch{'c'};
   
   ds.seekp(0);
   CHECK_EQUAL( ds.put(ch), true )
   CHECK_EQUAL( ds.tellp(), 1 )

   // inline bool valid()
   ds.seekp(0);
   CHECK_EQUAL( ds.valid(), true )

   ds.seekp(1);
   CHECK_EQUAL( ds.valid(), true )

   ds.seekp(256);
   CHECK_EQUAL( ds.valid(), true )

   ds.seekp(257);
   CHECK_EQUAL( ds.valid(), true )

   // inline size_t remaining()
   ds.seekp(0);
   CHECK_EQUAL( ds.remaining(), 0 )

   ds.seekp(1);
   CHECK_EQUAL( ds.remaining(), 0 )

   ds.seekp(256);
   CHECK_EQUAL( ds.remaining(), 0 )

   ds.seekp(257);
   CHECK_EQUAL( ds.remaining(), 0 )

   silence_output(false);
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(datastream_stream_test)
   silence_output(true);

   static constexpr uint16_t buffer_size{256};
   char datastream_buffer[buffer_size]; // Buffer for the datastream to point to

   datastream<const char*> ds{datastream_buffer, buffer_size};

   // -------------
   // T (primitive)
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const int cprim{10};
   int prim{};
   ds << cprim;
   ds.seekp(0);
   ds >> prim;
   CHECK_EQUAL( cprim, prim )

   // -----------------
   // T (non-primitive)
   struct non_prim_test {
      int val;
   };

   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const non_prim_test cnonprim{10};
   non_prim_test nonprim{};
   ds << cnonprim;
   ds.seekp(0);
   ds >> nonprim;
   CHECK_EQUAL( cnonprim.val, nonprim.val )

   // ---------------
   // T[] (primitive)
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const int cprim_array[10]{0,1,2,3,4,5,6,7,8,9};
   int prim_array[10]{};
   ds << cprim_array;
   CHECK_EQUAL( ds.tellp(), 41 )

   ds.seekp(0); // Next three lines are testing an implementation detail ???
   ds >> prim_array;
   CHECK_EQUAL( memcmp(cprim_array, prim_array, 10), 0 )

   ds.seekp(1);
   fill(begin(prim_array), end(prim_array), 0);
   REQUIRE_ASSERT( "T[] size and unpacked size don't match", [&](){ds >> prim_array;} )

   // -------------------
   // T[] (non-primitive)
   struct non_prim_array_test {
      int val;
   };

   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const non_prim_array_test cnon_prim_array[10]{0,1,2,3,4,5,6,7,8,9};
   non_prim_array_test non_prim_array[10]{};
   ds << cnon_prim_array;
   CHECK_EQUAL( ds.tellp(), 41 )

   ds.seekp(0); // Next three lines are testing an implementation detail ???
   ds >> non_prim_array;
   CHECK_EQUAL( memcmp(cnon_prim_array, non_prim_array, 10), 0 )

   ds.seekp(1);
   fill(begin(non_prim_array), end(non_prim_array), non_prim_array_test{});
   REQUIRE_ASSERT( "T[] size and unpacked size don't match", [&](){ds >> non_prim_array;} )

   // ----
   // bool
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const bool cboolean{true};
   bool boolean{};
   ds << cboolean;
   ds.seekp(0);
   ds >> boolean;
   CHECK_EQUAL( cboolean, boolean )

   // ----------
   // std::array
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const array<char, 9> ca{'a','b','c','d','e','f','g','h','i'};
   std::array<char, 9> a{};
   ds << ca;
   ds.seekp(0);
   ds >> a;
   CHECK_EQUAL( ca, a )

   ////////////////////////////////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////// Note: uncomment once issue has been resolved ////////////////////////////////
   ////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // ----------
   // std::deque
   // ds.seekp(0);
   // fill(begin(datastream_buffer), end(datastream_buffer), 0);
   // const std::deque<char> cd{'a','b','c','d','e','f','g','h','i' };
   // std::deque<char> d{};
   // ds << cd;
   // ds.seekp(0);
   // ds >> d; // Fails here
   // CHECK_EQUAL( cd, d )

   // ---------
   // std::list
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::list<char> cl{'a','b','c','d','e','f','g','h','i' };
   std::list<char> l{};
   ds << cl;
   ds.seekp(0);
   ds >> l;
   CHECK_EQUAL( cl, l )

   // --------
   // std::map
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const map<char,int> cchar_map{{'a',97}, {'b',98}, {'c',99}, {'d',100}};
   map<char,int> char_map{};
   ds << cchar_map;
   ds.seekp(0);
   ds >> char_map;
   CHECK_EQUAL( cchar_map, char_map )

   // -------------
   // std::optional
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::optional<char> co{'c'};
   std::optional<char> o{};
   ds << co;
   ds.seekp(0);
   ds >> o;
   CHECK_EQUAL( co, o )

   // ---------
   // std::pair
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::pair<char, int> cp{'c', 42};
   std::pair<char, int> p{};
   ds << cp;
   ds.seekp(0);
   ds >> p;
   CHECK_EQUAL( cp, p )

   // --------
   // std::set
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const set<char> cchar_set{'a','b','c','d','e','f','g','h','i'};
   set<char> char_set{};
   ds << cchar_set;
   ds.seekp(0);
   ds >> char_set;
   CHECK_EQUAL( cchar_set, char_set )

   // -----------
   // std::string
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const string cstr {"abcdefghi"};
   string str{};
   ds << cstr;
   ds.seekp(0);
   ds >> str;
   CHECK_EQUAL( cstr, str )

   // ----------
   // std::tuple
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const tuple<string,int,char> ctup{"abcefghi",42,'a'};
   tuple<string,int,char> tup{};
   ds << ctup;
   ds.seekp(0);
   ds >> tup;
   CHECK_EQUAL( ctup, tup )

   // ------------
   // std::variant
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::variant<int, char> v0{1024};
   std::variant<int, char> v1{};
   ds << v0;
   ds.seekp(0);
   ds >> v1;
   CHECK_EQUAL( v0, v1 )
   
   // -----------
   // std::vector
   struct vec_test {
      char val;
   };

   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const vector<vec_test> ctest_vec{{'a'},{'b'},{'c'},{'d'},{'e'},{'f'},{'g'},{'h'},{'i'}};
   vector<vec_test> test_vec{};
   ds << ctest_vec;
   ds.seekp(0);
   ds >> test_vec;

   for (int i{0}; i < ctest_vec.size(); ++i) {
      CHECK_EQUAL( ctest_vec[i].val, test_vec[i].val )
   }

   // -----------------
   // std::vector<char>
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const vector<char> cchar_vec{'a','b','c','d','e','f','g','h','i'};
   vector<char> char_vec{};
   ds << cchar_vec;
   ds.seekp(0);
   ds >> char_vec;
   CHECK_EQUAL( cchar_vec, char_vec )

   // ----------------
   // capi_checksum160
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const capi_checksum160 ccheck160 {0x01,0x02,0x03,0x04};
   capi_checksum160 check160{};
   ds << ccheck160;
   ds.seekp(0);
   ds >> check160;
   CHECK_EQUAL( memcmp(ccheck160.hash, check160.hash, 20), 0 )

   // ----------------
   // capi_checksum256
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const capi_checksum256 ccheck256 {0x01,0x02,0x03,0x04};
   capi_checksum256 check256{};
   ds << ccheck256;
   ds.seekp(0);
   ds >> check256;
   CHECK_EQUAL( memcmp(ccheck256.hash, check256.hash, 32), 0 )

   // ----------------
   // capi_checksum512
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const capi_checksum512 ccheck512 {0x01,0x02,0x03,0x04};
   capi_checksum512 check512{};
   ds << ccheck512;
   ds.seekp(0);
   ds >> check512;
   CHECK_EQUAL( memcmp(ccheck512.hash, check512.hash, 64), 0 )

   // ---------------
   // capi_public_key
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const capi_public_key c_cpubkey{'a','b','c','d','e','f','g','h','i'};
   capi_public_key c_pubkey{};
   ds << c_cpubkey;
   ds.seekp(0);
   ds >> c_pubkey;
   CHECK_EQUAL( memcmp(c_cpubkey.data, c_cpubkey.data, 32), 0 )

   // -----------------------
   // eosio::binary_extension
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const binary_extension<char> cbe_char{'c'};
   binary_extension<char> cb_char{};
   ds << cbe_char;
   ds.seekp(0);
   ds >> cb_char;
   CHECK_EQUAL( cbe_char.value(), cb_char.value() )

   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   binary_extension<int> be_int0{};
   binary_extension<int> be_int1{};
   ds << be_int0.value_or(42);
   ds.seekp(0);
   ds >> be_int1;
   CHECK_EQUAL( be_int1.value(), 42 )

   const be_test bet{}; // For default value
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const binary_extension<be_test> cbe_btest{bet};
   binary_extension<be_test> cb_btest{};
   ds << cbe_btest;
   ds.seekp(0);
   ds >> cb_btest;
   CHECK_EQUAL( cbe_btest.value().val, cb_btest.value().val )

   ds.seekp(256);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   binary_extension<be_test> cb_default_test{};
   ds >> cb_default_test;
   CHECK_EQUAL( 42, cb_default_test.value_or().val )
   
   const binary_extension<be_test> cbe_none{};
   CHECK_ASSERT( "cannot get value of empty binary_extension", [&](){cbe_none.value();} )

   // ------------------
   // eosio::fixed_bytes
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const fixed_bytes<32> cfb{fixed_bytes<32>::make_from_word_sequence<uint64_t>(1ULL,2ULL,3ULL,4ULL)};
   fixed_bytes<32> fb{};
   ds << cfb;
   ds.seekp(0);
   ds >> fb;
   CHECK_EQUAL( cfb, fb )

   // -------------
   // eosio::ignore
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const ignore<char> cig{};
   ignore<char> ig;
   ds << cig;
   CHECK_EQUAL( ds.tellp(), 0 )
   ds >> ig;
   CHECK_EQUAL( ds.tellp(), 0 )

   // ---------------------
   // eosio::ignore_wrapper
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const ignore_wrapper<char> ciw{'c'};
   char iw;
   ds << ciw;
   ds.seekp(0);
   ds >> iw;
   CHECK_EQUAL( ciw.value, iw )

   // -----------------
   // eosio::public_key
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const public_key cpubkey{{},'a','b','c','d','e','f','g','h','i'};
   public_key pubkey{};
   ds << cpubkey;
   ds.seekp(0);
   ds >> pubkey;
   CHECK_EQUAL( cpubkey, pubkey )

   // ----------------
   // eosio::signature
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const signature csig{{},'a','b','c','d','e','f','g','h','i'};
   signature sig{};
   ds << csig;
   ds.seekp(0);
   ds >> sig;
   CHECK_EQUAL( csig, sig )

   // -------------
   // eosio::symbol
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const symbol sym_no_prec{"SYMBOLL", 0};
   symbol sym{};
   ds << sym_no_prec;
   ds.seekp(0);
   ds >> sym;
   CHECK_EQUAL( sym_no_prec, sym )

   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const symbol sym_prec{"SYMBOLL", 255};
   ds << sym_prec;
   ds.seekp(0);
   ds >> sym;
   CHECK_EQUAL( sym_prec, sym )
   
   // ------------------
   // eosio::symbol_code
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const symbol_code csc{"SYMBOLL"};
   symbol_code sc{};
   ds << csc;
   ds.seekp(0);
   ds >> sc;
   CHECK_EQUAL( csc, sc )

   // --------------------------
   // boost::container::flat_map
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const flat_map<char,int> cchar_flat_map{{'a',97}, {'b',98}, {'c',99}, {'d',100}};
   flat_map<char,int> char_flat_map{};
   ds << cchar_flat_map;
   ds.seekp(0);
   ds >> char_flat_map;
   CHECK_EQUAL( cchar_flat_map, char_flat_map )

   // --------------------------
   // boost::container::flat_set
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const flat_set<char> cchar_flat_set{'a','b','c','d','e','f','g','h','i'};
   flat_set<char> char_flat_set{};
   ds << cchar_flat_set;
   ds.seekp(0);
   ds >> char_flat_set;
   CHECK_EQUAL( cchar_flat_set, char_flat_set )
   
   silence_output(false);
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(misc_datastream_test)
   silence_output(true);

   // ---------------------------
   // vector<char> pack(const T&)
   const string pack_s{"abcdefghi"};
   std::vector<char> pack_res{};
   pack_res = pack(pack_s);
   CHECK_EQUAL( memcmp(pack_s.data(), pack_res.data()+1, pack_s.size()), 0 )

   // --------------------------
   // size_t pack_size(const T&)
   int pack_size_i{42};
   double pack_size_d{3.14};
   string pack_size_s{"abcdefghi"};;
   CHECK_EQUAL( pack_size(pack_size_i), 4 )
   CHECK_EQUAL( pack_size(pack_size_d), 8 )
   CHECK_EQUAL( pack_size(pack_size_s), 10 )

   // -----------------------------
   // T unpack(const char*, size_t)
   const char unpack_source_buffer[9]{'a','b','c','d','e','f','g','h','i'};
   char unpack_ch{};
   for (uint8_t i = 0; i < 9; ++i) {
      unpack_ch = unpack<char>(unpack_source_buffer+i, 9);
      CHECK_EQUAL( unpack_source_buffer[i], unpack_ch )
   }

   // -----------------------------
   // T unpack(const vector<char>&)
   const vector<char> unpack_source_vec{'a','b','c','d','e','f','g','h','i'};
   for (uint8_t i = 0; i < 9; ++i) {
      unpack_ch = unpack<char>(unpack_source_buffer+i, 9);
      CHECK_EQUAL( unpack_source_buffer[i], unpack_ch )
   }
   
   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   // Note: uncomment once segfaulting during `memcpy` has been resolved
   // EOSIO_TEST(datastream_test);
   EOSIO_TEST(datastream_specialization_test);
   EOSIO_TEST(datastream_stream_test);
   EOSIO_TEST(misc_datastream_test);
   return has_failed();
}
