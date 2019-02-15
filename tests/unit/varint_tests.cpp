/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/native/tester.hpp>
#include <eosiolib/datastream.hpp>
#include <eosiolib/varint.hpp>

using std::numeric_limits;

using eosio::datastream;

const uint32_t u32min = numeric_limits<uint32_t>::min(); // 0
const uint32_t u32max = numeric_limits<uint32_t>::max(); // 4294967295

const int32_t i32min = numeric_limits<int32_t>::min(); // -2147483648
const int32_t i32max = numeric_limits<int32_t>::max(); //  2147483647

// Defined in `eosio.cdt/libraries/eosiolib/varint.hpp`
EOSIO_TEST_BEGIN(unsigned_int_type_test)
   silence_output(true);

   /// unsigned_int(uint32_t)
   CHECK_EQUAL( unsigned_int{}.value, 0 )
   CHECK_EQUAL( unsigned_int{u32min}.value, 0 )
   CHECK_EQUAL( unsigned_int{u32max}.value, 4294967295 )
   
   /// unsigned_int(T)
   CHECK_EQUAL( unsigned_int{uint8_t{0}}.value,  0 )
   CHECK_EQUAL( unsigned_int{uint16_t{1}}.value, 1 )
   CHECK_EQUAL( unsigned_int{uint32_t{2}}.value, 2 )
   CHECK_EQUAL( unsigned_int{uint64_t{3}}.value, 3 )

   // -----------------
   // operator T()const
   CHECK_EQUAL( unsigned_int{0}.operator bool(), false )
   CHECK_EQUAL( unsigned_int{1}.operator bool(), true  )

   // ---------------------------------
   // unsigned_int& operator=(uint32_t)
   unsigned_int ui0{};
   unsigned_int ui1{42};

   ui0 = ui1;
   CHECK_EQUAL( ui0 == ui1, true )

   // ------------------------------------------------------------
   // friend bool operator==(const unsigned_int&, const uint32_t&)
   CHECK_EQUAL( unsigned_int{42} == uint32_t{42}, true  )
   CHECK_EQUAL( unsigned_int{42} == uint32_t{43}, false )

   // ------------------------------------------------------------
   // friend bool operator==(const uint32_t&, const unsigned_int&)
   CHECK_EQUAL( uint32_t{42} == unsigned_int{42}, true  )
   CHECK_EQUAL( uint32_t{43} == unsigned_int{42}, false )

   // ----------------------------------------------------------------
   // friend bool operator==(const unsigned_int&, const unsigned_int&)
   CHECK_EQUAL( unsigned_int{42} == unsigned_int{42}, true  )
   CHECK_EQUAL( unsigned_int{42} == unsigned_int{43}, false )

   // ------------------------------------------------------------
   // friend bool operator!=(const unsigned_int&, const uint32_t&)
   CHECK_EQUAL( unsigned_int{42} != uint32_t{42}, false )
   CHECK_EQUAL( unsigned_int{42} != uint32_t{43}, true  )

   // ------------------------------------------------------------
   // friend bool operator!=(const uint32_t&, const unsigned_int&)
   CHECK_EQUAL( uint32_t{42} != unsigned_int{42}, false )
   CHECK_EQUAL( uint32_t{43} != unsigned_int{42}, true  )

   // ----------------------------------------------------------------
   // friend bool operator!=(const unsigned_int&, const unsigned_int&)
   CHECK_EQUAL( unsigned_int{42} != unsigned_int{42}, false )
   CHECK_EQUAL( unsigned_int{42} != unsigned_int{43}, true  )

   // ------------------------------------------------------------
   // friend bool operator< (const unsigned_int&, const uint32_t&)
   CHECK_EQUAL( unsigned_int{42} < uint32_t{42}, false )
   CHECK_EQUAL( unsigned_int{42} < uint32_t{43}, true  )

   // ------------------------------------------------------------
   // friend bool operator< (const uint32_t&, const unsigned_int&)
   CHECK_EQUAL( uint32_t{42} < unsigned_int{42}, false )
   CHECK_EQUAL( uint32_t{42} < unsigned_int{43}, true  )

   // ----------------------------------------------------------------
   // friend bool operator< (const unsigned_int&, const unsigned_int&)
   CHECK_EQUAL( unsigned_int{42} < unsigned_int{42}, false )
   CHECK_EQUAL( unsigned_int{42} < unsigned_int{43}, true  )

   // ------------------------------------------------------------
   // friend bool operator>=(const unsigned_int&, const uint32_t&)
   CHECK_EQUAL( unsigned_int{42} >= uint32_t{42}, true  )
   CHECK_EQUAL( unsigned_int{42} >= uint32_t{43}, false )

   // ------------------------------------------------------------
   // friend bool operator>=(const uint32_t&, const unsigned_int&)
   CHECK_EQUAL( uint32_t{42} >= unsigned_int{42}, true  )
   CHECK_EQUAL( uint32_t{42} >= unsigned_int{43}, false )

   // ----------------------------------------------------------------
   // friend bool operator>=(const unsigned_int&, const unsigned_int&)
   CHECK_EQUAL( unsigned_int{42} >= unsigned_int{42}, true  )
   CHECK_EQUAL( unsigned_int{42} >= unsigned_int{43}, false )

   // ---------------------------------------------------------------
   // friend DataStream& operator<<(DataStream&, const unsigned_int&)
   // friend DataStream& operator>>(DataStream&, unsigned_int&)
   static constexpr uint16_t buffer_size{256};
   char datastream_buffer[buffer_size]; // Buffer for the datastream to point to

   datastream<const char*> ds{datastream_buffer, buffer_size};
   
   const unsigned_int cui{42};
   unsigned_int ui{};
   ds << cui;
   ds.seekp(0);
   ds >> ui;
   CHECK_EQUAL( cui == ui, true)
   
   silence_output(false);
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/varint.hpp`
EOSIO_TEST_BEGIN(signed_int_type_test)
   silence_output(true);
   
   /// signed_int(uint32_t)
   CHECK_EQUAL( signed_int{}.value, 0 )
   CHECK_EQUAL( signed_int{i32min}.value, -2147483648 )
   CHECK_EQUAL( signed_int{i32max}.value,  2147483647 )

   // -----------------------
   // operator int32_t()const
   CHECK_EQUAL( signed_int{}.operator int32_t(), 0 )
   CHECK_EQUAL( signed_int{i32min}.operator int32_t(), -2147483648 )
   CHECK_EQUAL( signed_int{i32max}.operator int32_t(),  2147483647 )

   // --------------------------------
   //  signed_int& operator=(const T&)
   signed_int i0{};
   signed_int i1{};
   signed_int i2{};
   signed_int i3{};

   int8_t i8{0};
   int16_t i16{1};
   int32_t i32{2};
   int64_t i64{3};

   i0 = i8;
   i1 = i16;
   i2 = i32;
   i3 = i64;

   CHECK_EQUAL( i0.value, 0 )
   CHECK_EQUAL( i1.value, 1 )
   CHECK_EQUAL( i2.value, 2 )
   CHECK_EQUAL( i3.value, 3 )

   // --------------------------
   // signed_int operator++(int)
   signed_int post_inc0{0};
   signed_int post_inc1{1};
   CHECK_EQUAL( post_inc0++.value, 0 )
   CHECK_EQUAL( post_inc1++.value, 1 )

   // ------------------------
   // signed_int& operator++()
   signed_int pre_inc0{0};
   signed_int pre_inc1{1};
   CHECK_EQUAL( ++pre_inc0.value, 1 )
   CHECK_EQUAL( ++pre_inc1.value, 2 )

   // ------------------------------------------------------------
   // friend bool operator==(const signed_int&, const uint32_t&)
   CHECK_EQUAL( signed_int{42} == int32_t{42}, true  )
   CHECK_EQUAL( signed_int{42} == int32_t{43}, false )

   // ----------------------------------------------------------
   // friend bool operator==(const uint32_t&, const signed_int&)
   CHECK_EQUAL( int32_t{42} == signed_int{42}, true  )
   CHECK_EQUAL( int32_t{43} == signed_int{42}, false )

   // ------------------------------------------------------------
   // friend bool operator==(const signed_int&, const signed_int&)
   CHECK_EQUAL( signed_int{42} == signed_int{42}, true  )
   CHECK_EQUAL( signed_int{42} == signed_int{43}, false )

   // ----------------------------------------------------------
   // friend bool operator!=(const signed_int&, const uint32_t&)
   CHECK_EQUAL( signed_int{42} != int32_t{42}, false )
   CHECK_EQUAL( signed_int{42} != int32_t{43}, true  )

   // ----------------------------------------------------------
   // friend bool operator!=(const uint32_t&, const signed_int&)
   CHECK_EQUAL( int32_t{42} != signed_int{42}, false )
   CHECK_EQUAL( int32_t{43} != signed_int{42}, true  )

   // ------------------------------------------------------------
   // friend bool operator!=(const signed_int&, const signed_int&)
   CHECK_EQUAL( signed_int{42} != signed_int{42}, false )
   CHECK_EQUAL( signed_int{42} != signed_int{43}, true  )

   // ----------------------------------------------------------
   // friend bool operator< (const signed_int&, const uint32_t&)
   CHECK_EQUAL( signed_int{42} < int32_t{42}, false )
   CHECK_EQUAL( signed_int{42} < int32_t{43}, true  )

   // ----------------------------------------------------------
   // friend bool operator< (const uint32_t&, const signed_int&)
   CHECK_EQUAL( int32_t{42} < signed_int{42}, false )
   CHECK_EQUAL( int32_t{42} < signed_int{43}, true  )

   // ------------------------------------------------------------
   // friend bool operator< (const signed_int&, const signed_int&)
   CHECK_EQUAL( signed_int{42} < signed_int{42}, false )
   CHECK_EQUAL( signed_int{42} < signed_int{43}, true  )

   // ----------------------------------------------------------
   // friend bool operator>=(const signed_int&, const uint32_t&)
   CHECK_EQUAL( signed_int{42} >= int32_t{42}, true  )
   CHECK_EQUAL( signed_int{42} >= int32_t{43}, false )

   // ----------------------------------------------------------
   // friend bool operator>=(const uint32_t&, const signed_int&)
   CHECK_EQUAL( int32_t{42} >= signed_int{42}, true  )
   CHECK_EQUAL( int32_t{42} >= signed_int{43}, false )

   // ------------------------------------------------------------
   // friend bool operator>=(const signed_int&, const signed_int&)
   CHECK_EQUAL( signed_int{42} >= signed_int{42}, true  )
   CHECK_EQUAL( signed_int{42} >= signed_int{43}, false )

   // ----------------------------------------------------------------
   // friend DataStream& operator<<(DataStream& ds, const signed_int&)
   // friend DataStream& operator>>(DataStream&, signed_int&)
   static constexpr uint16_t buffer_size{256};
   char datastream_buffer[buffer_size]; // Buffer for the datastream to point to

   datastream<const char*> ds{datastream_buffer, buffer_size};

   const signed_int csi{-42};
   signed_int si{};
   ds << csi;
   ds.seekp(0);
   ds >> si;
   CHECK_EQUAL( csi == si, true)

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(unsigned_int_type_test)
   EOSIO_TEST(signed_int_type_test);
   return has_failed();
}
