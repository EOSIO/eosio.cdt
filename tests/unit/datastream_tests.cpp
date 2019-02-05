// eosio-cpp -fnative -w -o a.out datastream_tests.cpp; ./a.out
// TODO: come up with more succinct and descriptive variable names
// TODO: tidy up tests

#include <eosio/native/tester.hpp>
#include <eosiolib/binary_extension.hpp>
#include <eosiolib/crypto.hpp>
#include <eosiolib/datastream.hpp>
#include <eosiolib/ignore.hpp>
#include <eosiolib/symbol.hpp>
#include <array>
#include <deque>
#include <list>
#include <vector>

#include <eosiolib/print.hpp>

using eosio::binary_extension;
using eosio::datastream;
using eosio::fixed_bytes;
using eosio::ignore;
using eosio::ignore_wrapper;
using eosio::key256;
using eosio::public_key;
using eosio::signature;
using eosio::symbol;
using eosio::symbol_code;
using std::array;
using std::begin;
using std::end;
using std::fill;
using std::list;

using namespace eosio::native;

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(datastream_test)
   silence_output(false);

   // char datastream_buffer[256]{}; // Buffer for the datastream to point to
   // char buffer[256]; // Buffer to act upon for testing

   // // Fill the char array `datastream_buffer` with all 256 ASCII characters
   // unsigned char j{0};
   // for(int i{0}; i < 256; ++i)
   //    datastream_buffer[i] = j++;

   // // Why can't I initialize a data stream object of type `datastream<uint32_t> ds{0, 256}`??
   // /// datastream(T, size_t)
   // datastream<char*> ds{datastream_buffer, 256};

   // // inline void skip(size_t)
   // ds.skip(1);
   // REQUIRE_EQUAL( ds.pos() == datastream_buffer+1, true )
   // ds.skip(-1);
   // REQUIRE_EQUAL( ds.pos() == datastream_buffer, true )
   
   // // inline bool read(char*, size_t)
   // REQUIRE_EQUAL( ds.read(buffer, 256), true )
   // REQUIRE_EQUAL( memcmp(buffer, datastream_buffer, 256), 0)

   // REQUIRE_ASSERT( "read", ([&]() {ds.read(buffer, 1);}) )

   // // T pos()const
   // // inline bool seekp(size_t)
   // REQUIRE_EQUAL( ds.pos() == datastream_buffer+256, true )
   // REQUIRE_EQUAL( ds.pos() == datastream_buffer, false )
   // ds.seekp(0);
   // REQUIRE_EQUAL( ds.pos() == datastream_buffer, true )
   // REQUIRE_EQUAL( ds.pos() == datastream_buffer+256, false )

   // // inline bool write(const char*, size_t)
   // // Fill `buffer` with a new set of values
   // for(int i{0}, j{1}; i < 256; ++i)
   //    buffer[i] = j;

   // REQUIRE_EQUAL( ds.write(buffer, 256), true )
   // REQUIRE_EQUAL( memcmp(buffer, datastream_buffer, 256), 0 )

   // REQUIRE_ASSERT( "write", ([&]() {ds.write(buffer, 1);}) )

   // // inline bool put(char)
   // ds.seekp(0);
   // REQUIRE_EQUAL( ds.put('c'), true )
   // *buffer = 'c';
   // REQUIRE_EQUAL( memcmp(buffer, datastream_buffer, 256), 0 )

   // ds.seekp(256);
   // REQUIRE_ASSERT( "put", ([&]() {ds.put('c');}) )
  
   // // inline bool get(unsigned char&)
   // unsigned char c0{};

   // ds.seekp(0);
   // REQUIRE_EQUAL( ds.get(c0), true )
   // REQUIRE_EQUAL( c0, 'c' )

   // // inline bool get(char&)
   // char c1{};

   // ds.seekp(0);
   // REQUIRE_EQUAL( ds.get(c1), true )
   // REQUIRE_EQUAL( c1, 'c' )

   // // inline bool valid()const
   // ds.seekp(256);
   // REQUIRE_EQUAL( ds.valid(), true )

   // ds.seekp(257);
   // REQUIRE_EQUAL( ds.valid(), false )

   // // inline size_t tellp()const
   // ds.seekp(0);
   // REQUIRE_EQUAL( ds.tellp(), 0 )
   // ds.seekp(256);
   // REQUIRE_EQUAL( ds.tellp(), 256 )
   // ds.seekp(257);
   // REQUIRE_EQUAL( ds.tellp(), 257 )

   // //inline size_t remaining()const
   // ds.seekp(0);
   // REQUIRE_EQUAL( ds.remaining(), 256 )
   // ds.seekp(256);
   // REQUIRE_EQUAL( ds.remaining(), 0 )
   // // I don't understand:
   // // If the return type is of type `size_t`, how then does `_end - _pos` equate to
   // // -1? Should it be the maximum value of a `size_t`?
   // ds.seekp(257);
   // REQUIRE_EQUAL( ds.remaining(), -1)

   silence_output(false);
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(datastream_specialization_test)
   silence_output(true);

   char datastream_buffer[256]{}; // Buffer for the datastream to point to
   char buffer[256]{}; // Buffer to act upon for testing

   // Is this portable? Is there a better way?
   // Fill the char array `datastream_buffer` with all all 256 ASCII characters
   for(int i{0}, j{-0x80}; i < 256; ++i, ++j)
      datastream_buffer[i] = j;

   // Why can't I initialize a data stream object of type `datastream<uint32_t> ds{0, 256}`??
   /// datastream(T, size_t)
   datastream<size_t> ds{256};

   // inline void skip(size_t)
   // inline size_t tellp()const
   REQUIRE_EQUAL( ds.skip(0), true)
   REQUIRE_EQUAL( ds.tellp(), 256)
   
   REQUIRE_EQUAL( ds.skip(1), true)
   REQUIRE_EQUAL( ds.tellp(), 257)
   
   REQUIRE_EQUAL( ds.skip(255), true)
   REQUIRE_EQUAL( ds.tellp(), 512)
   
   REQUIRE_EQUAL( ds.skip(1028), true)
   REQUIRE_EQUAL( ds.tellp(),1540)

   // inline bool write(const char*,size_t)
   // inline bool seekp(size_t)
   ds.seekp(0);
   REQUIRE_EQUAL( ds.write(buffer, 256), true )
   REQUIRE_EQUAL( ds.tellp(), 256 )

   REQUIRE_EQUAL( ds.write(buffer, 1), true )
   REQUIRE_EQUAL( ds.tellp(), 257 )

   // inline bool put(char)
   char c0{'c'};
   
   ds.seekp(0);
   REQUIRE_EQUAL( ds.put(c0), true )
   REQUIRE_EQUAL( ds.tellp(), 1 )

   // inline bool valid()
   ds.seekp(0);
   REQUIRE_EQUAL( ds.valid(), true )

   ds.seekp(1);
   REQUIRE_EQUAL( ds.valid(), true )

   ds.seekp(256);
   REQUIRE_EQUAL( ds.valid(), true )

   ds.seekp(257);
   REQUIRE_EQUAL( ds.valid(), true )

   // inline size_t remaining()
   ds.seekp(0);
   REQUIRE_EQUAL( ds.remaining(), 0 )

   ds.seekp(1);
   REQUIRE_EQUAL( ds.remaining(), 0 )

   ds.seekp(256);
   REQUIRE_EQUAL( ds.remaining(), 0 )

   ds.seekp(257);
   REQUIRE_EQUAL( ds.remaining(), 0 )

   silence_output(false);
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(datastream_stream_test)
   silence_output(false);

   // TODO: figure out why making this buffer and off number throws a segfault
   static constexpr uint8_t buffer_size{128};
   char datastream_buffer[buffer_size]; // Buffer for the datastream to point to
   datastream<char*> ds{datastream_buffer, buffer_size};

   // ---------
   // std::list
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::list<char> cl{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' };
   std::list<char> l{};
   ds << cl;
   ds.seekp(0);
   ds >> l;
   REQUIRE_EQUAL( cl == l, true )

   // Note: uncomment once issue has been resolved
   // ----------
   // std::deque
   // ds.seekp(0);
   // fill(begin(datastream_buffer), end(datastream_buffer), 0);
   // const std::deque<char> cd{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' };
   // std::deque<char> d{};
   // ds << cd;
   // ds.seekp(0);
   // ds >> d; // Fails here
   // REQUIRE_EQUAL( cd == d, true )

   // ----------------
   // binary_extension
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const binary_extension<char> cbe_char{'c'};
   binary_extension<char> cb_char{};
   ds << cbe_char;
   ds.seekp(0);
   ds >> cb_char;
   REQUIRE_EQUAL( cbe_char.value() == cb_char.value(), true )

   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   binary_extension<int> be_char_vor0{};
   binary_extension<int> cb_char_vor1{};
   ds << be_char_vor0.value_or(42);
   ds.seekp(0);
   ds >> cb_char_vor1;
   REQUIRE_EQUAL( cb_char_vor1.value() == 42, true )

   struct be_test {
      be_test() : val{42} {}
      int val;
   };
   const be_test bet{};
   
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const binary_extension<be_test> cbe{bet};
   binary_extension<be_test> cb{};
   ds << cbe;
   ds.seekp(0);
   ds >> cb;
   REQUIRE_EQUAL( cbe.value().val == cb.value().val, true )
   
   const binary_extension<be_test> cbe_none{};
   REQUIRE_ASSERT( "cannot get value of empty binary_extension", [&](){cbe_none.value();} )

   // ------------
   // std::variant
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::variant<char, int> cv{'c'};
   std::variant<char, int> v{};
   ds << cv;
   ds.seekp(0);
   ds >> v;
   REQUIRE_EQUAL( cv == v, true )

   // ---------
   // std::pair
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::pair<char, int> cp{'c', 0x43};
   std::pair<char, int> p{};
   ds << cp;
   ds.seekp(0);
   ds >> p;
   REQUIRE_EQUAL( cp == p, true )

   // -------------
   // std::optional
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::optional<char> co{'c'};
   std::optional<char> o{};
   ds << co;
   ds.seekp(0);
   ds >> o;
   REQUIRE_EQUAL( co == o, true )
   
   // -----------
   // symbol_code
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const symbol_code csc{"SYMBOLL"};
   symbol_code sc{};
   ds << csc;
   ds.seekp(0);
   ds >> sc;
   REQUIRE_EQUAL( csc == sc, true )

   // ------
   // symbol
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const symbol sym_no_prec{"SYMBOLL", 0};
   symbol sym{};
   ds << sym_no_prec;
   ds.seekp(0);
   ds >> sym;
   REQUIRE_EQUAL( sym_no_prec == sym, true )

   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const symbol sym_prec{"SYMBOLL", 255};
   ds << sym_prec;
   ds.seekp(0);
   ds >> sym;
   REQUIRE_EQUAL( sym_prec == sym, true )

   // --------------
   // ignore_wrapper
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const ignore_wrapper<char> cigw{'c'};
   char c_igw;
   ds << cigw;
   ds.seekp(0);
   ds >> c_igw;
   REQUIRE_EQUAL( cigw.value == c_igw, true )

   // ------
   // ignore
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const ignore<char> cig{};
   ignore<char> c_ig;
   ds << cig;
   REQUIRE_EQUAL( ds.tellp() == 0, true )
   ds >> c_ig;
   REQUIRE_EQUAL( ds.tellp() == 0, true )

   // ---------------
   // capi_public_key
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const capi_public_key c_cpubkey{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
   capi_public_key cpubkey{};
   ds << c_cpubkey;
   ds.seekp(0);
   ds >> cpubkey;
   REQUIRE_EQUAL( memcmp(c_cpubkey.data, cpubkey.data, 32), 0 )

   // ----------
   // public_key
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const public_key c_pubkey{{}, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
   public_key pubkey{};
   ds << c_pubkey;
   ds.seekp(0);
   ds >> pubkey;
   REQUIRE_EQUAL( c_pubkey == pubkey, true )

   // ---------
   // signature
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const signature c_sig{{}, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
   signature sig{};
   ds << c_sig;
   ds.seekp(0);
   ds >> sig;
   REQUIRE_EQUAL( c_sig == sig, true )

   // TODO: Test a more complex `key256`
   // ------
   // key256
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const key256 c_key{};
   key256 key{};
   ds << c_key;
   ds.seekp(0);
   ds >> key;

   REQUIRE_EQUAL( c_key == key, true )

   // TODO: Test a more complex `fixed_bytes`
   // -----------
   // fixed_bytes
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const fixed_bytes<32> c_fb{};
   fixed_bytes<32> fb{};
   ds << c_fb;
   ds.seekp(0);
   ds >> fb;

   REQUIRE_EQUAL( c_fb == fb, true )

   // ----
   // bool
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const bool cboolean{true};
   bool boolean{};
   ds << cboolean;
   ds.seekp(0);
   ds >> boolean;
   REQUIRE_EQUAL( cboolean == boolean, true )

   // ----------
   // std::array
   ds.seekp(0);
   fill(begin(datastream_buffer), end(datastream_buffer), 0);
   const std::array<char, 9> ca{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
   const std::array<char, 9> a{};
   ds << ca;
   ds.seekp(0);
   ds >> a;
   REQUIRE_EQUAL( ca == a, true )

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(datastream_test);
   // EOSIO_TEST(datastream_specialization_test);
   // EOSIO_TEST(datastream_stream_test);
   return has_failed();
}
