#include <eosio/native/tester.hpp>
#include <eosiolib/datastream.hpp>
#include <vector>

#include <eosiolib/print.hpp>

using std::vector;
using eosio::datastream;

using namespace eosio::native;

// Definitions in `eosio.cdt/libraries/eosiolib/datastream.hpp`
EOSIO_TEST_BEGIN(datastream_test)
   silence_output(false);

char test_stream_bytes[256]{}; // Buffer for the datastream to point to
char buffer[256]{}; // Buffer to act upon for testing

// Fill the char array `test_stream_bytes` with all all 256 ASCII characters
for(int i{0}, j{-0x80}; i < 256; ++i, ++j)
    test_stream_bytes[i] = j;

// Why can't I initialize a data stream object of type `datastream<uint32_t> ds{0, 256}`??
/// datastream(T, size_t)
datastream<const char*> ds{test_stream_bytes, 256};

// inline void skip(size_t)
ds.skip(1);
REQUIRE_EQUAL( ds.pos() == test_stream_bytes+1, true )
ds.skip(-1);
REQUIRE_EQUAL( ds.pos() == test_stream_bytes, true )

// inline bool read(char*, size_t)
REQUIRE_EQUAL( ds.read(buffer, 256), true )
REQUIRE_EQUAL( strcmp(buffer, test_stream_bytes), 0 )

REQUIRE_ASSERT( "read", ([&]() {ds.read(buffer, 1);}) )

// T pos()const
// inline bool seekp(size_t)
REQUIRE_EQUAL( ds.pos() == test_stream_bytes+255, true )
REQUIRE_EQUAL( ds.pos() == test_stream_bytes, false )
ds.seekp(0);
REQUIRE_EQUAL( ds.pos() == test_stream_bytes, true )
REQUIRE_EQUAL( ds.pos() == test_stream_bytes+255, false )

// inline bool write(const char*, size_t)
// REQUIRE_EQUAL( ds.write(buffer, 256), true )
// REQUIRE_EQUAL( strcmp(buffer, test_stream_bytes), 0 )

// inline bool put(char)
  
// inline bool get(unsigned char&)
// eosio::print("_start: ", (uint64_t)ds._start, "_pos: ", (uint64_t)ds._pos, "_end: ", (uint64_t)ds._end);

// inline bool get(char&)
     
// REQUIRE_EQUAL( ds.pos() == 0, true )
// REQUIRE_EQUAL( ds.pos() == 1, false )

// inline bool valid()const

// inline size_t tellp()const

//inline size_t remaining()const




// Note that if the size given to the buffer (which is of type `size_t`) is
// the value of the maximum underlying type (which is a generic type `T`), then
// the `_pos` will always be valid because `_pos` will overflow before it exceeds
// the  size of the buffer.
// The solution, I see is to make the type of `_pos` type `size_t` with an
// invariant maximum value of `numeric_limits<size_t>::max()-1`
// REQUIRE_EQUAL( ds.valid() == true, true )
// ds.seekp(257);
// REQUIRE_EQUAL( ds.valid() == true, false )
// ds.seekp(0);
// REQUIRE_EQUAL( ds.tellp() == 0, true )
// REQUIRE_EQUAL( ds.tellp() == 1, false )
// REQUIRE_EQUAL( ds.remaining() == 256, true )
// REQUIRE_EQUAL( ds.remaining() == 255, false )

// ds.seekp(10);
// REQUIRE_EQUAL( ds.pos() == 10, true )
// REQUIRE_EQUAL( ds.valid() == true, true )
// REQUIRE_EQUAL( ds.tellp() == 10, true )
// REQUIRE_EQUAL( ds.remaining() == 246, true )




// ds.skip(257);
// eosio::print("_start: ", ds._start, "_pos: ", ds._pos, "_end: ", ds._end);
// REQUIRE_EQUAL( ds.valid() == true, true )

// eosio::print("_start: ", (uint64_t)ds._start, "_pos: ", (uint64_t)ds._pos, "_end: ", (uint64_t)ds._end);
// eosio::print((uint64_t)ds._end - (uint64_t)ds._start);
// eosio::print(strcmp(buffer, test_stream_bytes));

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(datastream_test);
   return has_failed();
}
