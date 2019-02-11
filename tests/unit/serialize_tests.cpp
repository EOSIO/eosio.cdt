/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/native/tester.hpp>
#include <eosiolib/datastream.hpp>
#include <eosiolib/serialize.hpp>

using std::begin;
using std::end;
using std::fill;
using std::list;
using std::string;
using std::vector;

using eosio::datastream;

struct Base {
   const char   c{'c'};
   const int    i{42};
   const double d{3.14};
   EOSLIB_SERIALIZE( Base, (c) (i) (d) )
};

struct Derived : public Base {
   const string      s{"abcefghi"};
   const list<char>  l{'a','b','c','d','e','f','g','h','i'};
   const vector<int> v{0,1,2,3,4,5,6,7,8,9};
   EOSLIB_SERIALIZE_DERIVED( Derived, Base, (s) (l) (v) )
};

// Definitions in `eosio.cdt/libraries/eosiolib/serialize.hpp`
EOSIO_TEST_BEGIN(serialize_test)
   silence_output(true);

   static constexpr uint16_t buffer_size{256};
   char ds_buffer[buffer_size]{}; // Buffer for the datastream to point to
   char ds_expected_buffer[buffer_size]{};
   datastream<const char*> ds{ds_buffer, buffer_size};
   datastream<const char*> ds_expected{ds_expected_buffer, buffer_size};

   const Base b;
   ds_expected << b.c << b.i << b.d;
   ds << b;
   REQUIRE_EQUAL( memcmp( ds_buffer, ds_expected_buffer, 256), 0 )

   ds.seekp(0);
   fill(begin(ds_buffer), end(ds_buffer), 0);
   ds_expected.seekp(0);
   fill(begin(ds_expected_buffer), end(ds_expected_buffer), 0);

   const Derived d;
   ds_expected << d.c << d.i << d.d << d.s << d.l << d.v;
   ds << d;
   REQUIRE_EQUAL( memcmp( ds_buffer, ds_expected_buffer, 256), 0 )
   
   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(serialize_test)
   return has_failed();
}
