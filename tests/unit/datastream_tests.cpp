#include <eosio/native/tester.hpp>
#include <eosiolib/datastream.hpp>
#include <list>
#include <vector>

#include <eosiolib/print.hpp>

using eosio::datastream;
using std::list;

using namespace eosio::native;

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(datastream_test)
   silence_output(true);

   char datastream_buffer[256]{}; // Buffer for the datastream to point to
   char buffer[256]{}; // Buffer to act upon for testing

   // Fill the char array `datastream_buffer` with all all 256 ASCII characters
   for(int i{0}, j{-0x80}; i < 256; ++i, ++j)
      datastream_buffer[i] = j;

   // Why can't I initialize a data stream object of type `datastream<uint32_t> ds{0, 256}`??
   /// datastream(T, size_t)
   datastream<char*> ds{datastream_buffer, 256};

   // inline void skip(size_t)
   ds.skip(1);
   REQUIRE_EQUAL( ds.pos() == datastream_buffer+1, true )
   ds.skip(-1);
   REQUIRE_EQUAL( ds.pos() == datastream_buffer, true )

   // inline bool read(char*, size_t)
   REQUIRE_EQUAL( ds.read(buffer, 256), true )
   REQUIRE_EQUAL( memcmp(buffer, datastream_buffer, 256), 0 )

   REQUIRE_ASSERT( "read", ([&]() {ds.read(buffer, 1);}) )

   // T pos()const
   // inline bool seekp(size_t)
   REQUIRE_EQUAL( ds.pos() == datastream_buffer+256, true )
   REQUIRE_EQUAL( ds.pos() == datastream_buffer, false )
   ds.seekp(0);
   REQUIRE_EQUAL( ds.pos() == datastream_buffer, true )
   REQUIRE_EQUAL( ds.pos() == datastream_buffer+256, false )

   // inline bool write(const char*, size_t)
   // Fill `buffer` with a new set of values
   for(int i{0}, j{1}; i < 256; ++i)
      buffer[i] = j;

   REQUIRE_EQUAL( ds.write(buffer, 256), true )
   REQUIRE_EQUAL( memcmp(buffer, datastream_buffer, 256), 0 )

   REQUIRE_ASSERT( "write", ([&]() {ds.write(buffer, 1);}) )

   // I'm getting the error: `read-only variable is not assignable *_pos = c;`
   // inline bool put(char)
   ds.seekp(0);
   REQUIRE_EQUAL( ds.put('c'), true )
   *buffer = 'c';
   REQUIRE_EQUAL( memcmp(buffer, datastream_buffer, 256), 0 )

   ds.seekp(256);
   REQUIRE_ASSERT( "put", ([&]() {ds.put('c');}) )
  
   // inline bool get(unsigned char&)
   unsigned char c0{};

   ds.seekp(0);
   REQUIRE_EQUAL( ds.get(c0), true )
   REQUIRE_EQUAL( c0, 'c' )

   // inline bool get(char&)
   char c1{};

   ds.seekp(0);
   REQUIRE_EQUAL( ds.get(c1), true )
   REQUIRE_EQUAL( c1, 'c' )

   // inline bool valid()const
   ds.seekp(256);
   REQUIRE_EQUAL( ds.valid(), true )

   ds.seekp(257);
   REQUIRE_EQUAL( ds.valid(), false )

   // inline size_t tellp()const
   ds.seekp(0);
   REQUIRE_EQUAL( ds.tellp(), 0 )
   ds.seekp(256);
   REQUIRE_EQUAL( ds.tellp(), 256 )
   ds.seekp(257);
   REQUIRE_EQUAL( ds.tellp(), 257 )

   //inline size_t remaining()const
   ds.seekp(0);
   REQUIRE_EQUAL( ds.remaining(), 256 )
   ds.seekp(256);
   REQUIRE_EQUAL( ds.remaining(), 0 )
   // I don't understand:
   // If the return type is of type `size_t`, how then does `_end - _pos` equate to
   // -1? Should it be the maximum value of a `size_t`?
   ds.seekp(257);
   REQUIRE_EQUAL( ds.remaining(), -1)

   // eosio::print("_start: ", (uint64_t)ds._start, "_pos: ", (uint64_t)ds._pos, "_end: ", (uint64_t)ds._end);

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

   char datastream_buffer[8+sizeof(unsigned_int)]{}; // Buffer for the datastream to point to
   char const buffer[8]{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}; // Buffer to act upon for testing

   // // Fill the char array `buffer` with all all 256 ASCII characters
   // for(int i{0}, j{-0x80}; i < 256; ++i, ++j)
   //    buffer[i] = j;

   // ---------
   // std::list

const std::list<char> l{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', };
eosio::print(sizeof(unsigned_int));

   datastream<char*> ds{datastream_buffer, 8+sizeof(unsigned_int)};

   for(int i{0}; i < 8; ++i)
       eosio::print(datastream_buffer[i]);
   eosio::print("\n");
   for(int i{0}; i < 8; ++i)
       eosio::print(buffer[i]);
   eosio::print("\n");
   for(auto& x : l)
       eosio::print(x);
   eosio::print("\n");

   eosio::print("\n");
   
   ds << l;

   for(int i{0}; i < 8+sizeof(unsigned_int); ++i)
       eosio::print(datastream_buffer[i]);
   eosio::print("\n");
   for(int i{0}; i < 8; ++i)
       eosio::print(buffer[i]);
   eosio::print("\n");
   for(auto& x : l)
       eosio::print(x);
   eosio::print("\n");

   // std::vector<char> um_ds(std::begin(datastream_buffer), std::end(datastream_buffer));
   // std::vector<char> um_list(std::begin(datastream_buffer), std::end(datastream_buffer));

   // for(uint16_t i{0}; i < 256; ++i)
   //     eosio::print(datastream_buffer);

   // for(uint16_t i{0}; i < 256; ++i)
   //     eosio::print(datastream_buffer);

   // REQUIRE_EQUAL( um_ds ==  um_list, true )

   // ----------
   // std::deque

   // ----------------
   // binary_extension

   // ------------
   // std::variant

   // ---------
   // std::pair

   // ---------
   // std::optional

   // ---------
   // std::pair

   // ---------
   // symbol_code

   // ---------
   // symbol

   // ---------
   // ignore_wrapper

   // ---------
   // ignore

   // ---------
   // capi_public_key

   // ---------
   // public_key

   // ---------
   // signature

   // ---------
   // public_keykey256

   // -----------
   // fixed_bytes

   // -----------
   // bool

   // -----------
   // std::array

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
    //EOSIO_TEST(datastream_test);
   // EOSIO_TEST(datastream_specialization_test);
   EOSIO_TEST(datastream_stream_test);
   return has_failed();
}
