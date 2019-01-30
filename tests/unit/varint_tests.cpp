#include <eosio/native/tester.hpp>
#include <eosiolib/datastream.hpp>
#include <eosiolib/varint.hpp>

#include <eosiolib/print.hpp>

using eosio::datastream;

using namespace eosio::native;

const uint32_t u32min = std::numeric_limits<uint32_t>::min(); // 0
const uint32_t u32max = std::numeric_limits<uint32_t>::max(); // 18446744073709551615

const int32_t i32min = std::numeric_limits<int32_t>::min(); // -9223372036854775808
const int32_t i32max = std::numeric_limits<int32_t>::max(); //  9223372036854775807

// Defined in `eosio.cdt/libraries/eosiolib/varint.hpp`
EOSIO_TEST_BEGIN(unsigned_int_type_test)
   silence_output(false);

   // ------------
   // constructors
   
   /// unsigned_int(uint32_t)
   REQUIRE_EQUAL( unsigned_int{}.value, 0 )
   REQUIRE_EQUAL( unsigned_int{u32min}.value, 0 )
   REQUIRE_EQUAL( unsigned_int{u32max}.value, 18446744073709551615 )

   /// template<typename T>
   /// unsigned_int(T)
   REQUIRE_EQUAL( unsigned_int{uint8_t{0}}.value, 0 )
   REQUIRE_EQUAL( unsigned_int{uint16_t{1}}.value, 1 )
   REQUIRE_EQUAL( unsigned_int{uint32_t{2}}.value, 2 )
   REQUIRE_EQUAL( unsigned_int{uint64_t{3}}.value, 3 )

   //  template<typename T>
   //  operator T()const
   REQUIRE_EQUAL( unsigned_int{0}.operator bool(), false )
   REQUIRE_EQUAL( unsigned_int{1}.operator bool(), true )

   // ---------------------------------
   // unsigned_int& operator=(uint32_t)
   unsigned_int ui0{};
   unsigned_int ui1{42};

   ui0 = ui1;
   REQUIRE_EQUAL( ui0 == ui1, true )

   // ------------------------------------------------------------
   // friend bool operator==(const unsigned_int&, const uint32_t&)
   REQUIRE_EQUAL( unsigned_int{42} == uint32_t{42}, true )
   REQUIRE_EQUAL( unsigned_int{42} == uint32_t{43}, false )

   // ------------------------------------------------------------
   // friend bool operator==(const uint32_t&, const unsigned_int&)
   REQUIRE_EQUAL( uint32_t{42} == unsigned_int{42}, true )
   REQUIRE_EQUAL( uint32_t{43} == unsigned_int{42}, false )

   // ----------------------------------------------------------------
   // friend bool operator==(const unsigned_int&, const unsigned_int&)
   REQUIRE_EQUAL( unsigned_int{42} == unsigned_int{42}, true )
   REQUIRE_EQUAL( unsigned_int{42} == unsigned_int{43}, false )

   // ------------------------------------------------------------
   // friend bool operator!=(const unsigned_int&, const uint32_t&)
   REQUIRE_EQUAL( unsigned_int{42} != uint32_t{42}, false )
   REQUIRE_EQUAL( unsigned_int{42} != uint32_t{43}, true )

   // ------------------------------------------------------------
   // friend bool operator!=(const uint32_t&, const unsigned_int&)
   REQUIRE_EQUAL( uint32_t{42} != unsigned_int{42}, false )
   REQUIRE_EQUAL( uint32_t{43} != unsigned_int{42}, true )

   // ----------------------------------------------------------------
   // friend bool operator!=(const unsigned_int&, const unsigned_int&)
   REQUIRE_EQUAL( unsigned_int{42} != unsigned_int{42}, false )
   REQUIRE_EQUAL( unsigned_int{42} != unsigned_int{43}, true )

   // ------------------------------------------------------------
   // friend bool operator< (const unsigned_int&, const uint32_t&)
   REQUIRE_EQUAL( unsigned_int{42} < uint32_t{42}, false )
   REQUIRE_EQUAL( unsigned_int{42} < uint32_t{43}, true )

   // ------------------------------------------------------------
   // friend bool operator< (const uint32_t&, const unsigned_int&)
   REQUIRE_EQUAL( uint32_t{42} < unsigned_int{42}, false )
   REQUIRE_EQUAL( uint32_t{42} < unsigned_int{43}, true )

   // ----------------------------------------------------------------
   // friend bool operator< (const unsigned_int&, const unsigned_int&)
   REQUIRE_EQUAL( unsigned_int{42} < unsigned_int{42}, false )
   REQUIRE_EQUAL( unsigned_int{42} < unsigned_int{43}, true )

   // ------------------------------------------------------------
   // friend bool operator>=(const unsigned_int&, const uint32_t&)
   REQUIRE_EQUAL( unsigned_int{42} >= uint32_t{42}, true )
   REQUIRE_EQUAL( unsigned_int{42} >= uint32_t{43}, false )

   // ------------------------------------------------------------
   // friend bool operator>=(const uint32_t&, const unsigned_int&)
   REQUIRE_EQUAL( uint32_t{42} >= unsigned_int{42}, true )
   REQUIRE_EQUAL( uint32_t{42} >= unsigned_int{43}, false )

   // ----------------------------------------------------------------
   // friend bool operator>=(const unsigned_int&, const unsigned_int&)
   REQUIRE_EQUAL( unsigned_int{42} >= unsigned_int{42}, true )
   REQUIRE_EQUAL( unsigned_int{42} >= unsigned_int{43}, false )

   // -----------------------------
   // template<typename DataStream>
   // friend DataStream& operator<<(DataStream&, const unsigned_int&)
   const unsigned_int cui{0x7F};
   const char expected_buffer0[1]{0x7F};

   char buffer0[1]{};
   datastream<char*> ds0{buffer0, 1};

   ds0 << cui;

   REQUIRE_EQUAL( memcmp(expected_buffer0, buffer0, 1), 0 )

   // -----------------------------
   // template<typename DataStream>
   // friend DataStream& operator>>(DataStream&, unsigned_int&)
   unsigned_int ui{};
   const char expected_buffer1[1]{0x7F};

   char buffer1[1]{0x7F};
   datastream<char*> ds1{buffer1, 1};

   ds1 >> ui;

   REQUIRE_EQUAL( memcmp(expected_buffer1, buffer1, 1), 0 )

   silence_output(false);
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/varint.hpp`
EOSIO_TEST_BEGIN(signed_int_type_test)
   silence_output(false);

   // ------------
   // constructors
   
   /// signed_int(uint32_t)
   REQUIRE_EQUAL( signed_int{}.value, 0 )
   REQUIRE_EQUAL( signed_int{i32min}.value, -9223372036854775808 )
   REQUIRE_EQUAL( signed_int{i32max}.value,  9223372036854775807 )

   // -----------------------
   // operator int32_t()const
   REQUIRE_EQUAL( signed_int{}.operator int32_t(), 0 )
   REQUIRE_EQUAL( signed_int{i32min}.operator int32_t(), -9223372036854775808 )
   REQUIRE_EQUAL( signed_int{i32max}.operator int32_t(),  9223372036854775807 )

   // ---------------------
   //  template<typename T>
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

   REQUIRE_EQUAL( i0.value, 0 )
   REQUIRE_EQUAL( i1.value, 1 )
   REQUIRE_EQUAL( i2.value, 2 )
   REQUIRE_EQUAL( i3.value, 3 )

   // --------------------------
   // signed_int operator++(int)
   signed_int post_inc0{0};
   signed_int post_inc1{1};
   REQUIRE_EQUAL( post_inc0++.value, 0 )
   REQUIRE_EQUAL( post_inc1++.value, 1 )

   // ------------------------
   // signed_int& operator++()
   signed_int pre_inc0{0};
   signed_int pre_inc1{1};
   REQUIRE_EQUAL( ++pre_inc0.value, 1 )
   REQUIRE_EQUAL( ++pre_inc1.value, 2 )

   // ------------------------------------------------------------
   // friend bool operator==(const signed_int&, const uint32_t&)
   REQUIRE_EQUAL( signed_int{42} == int32_t{42}, true )
   REQUIRE_EQUAL( signed_int{42} == int32_t{43}, false )

   // ------------------------------------------------------------
   // friend bool operator==(const uint32_t&, const signed_int&)
   REQUIRE_EQUAL( int32_t{42} == signed_int{42}, true )
   REQUIRE_EQUAL( int32_t{43} == signed_int{42}, false )

   // ----------------------------------------------------------------
   // friend bool operator==(const signed_int&, const signed_int&)
   REQUIRE_EQUAL( signed_int{42} == signed_int{42}, true )
   REQUIRE_EQUAL( signed_int{42} == signed_int{43}, false )

   // ------------------------------------------------------------
   // friend bool operator!=(const signed_int&, const uint32_t&)
   REQUIRE_EQUAL( signed_int{42} != int32_t{42}, false )
   REQUIRE_EQUAL( signed_int{42} != int32_t{43}, true )

   // ------------------------------------------------------------
   // friend bool operator!=(const uint32_t&, const signed_int&)
   REQUIRE_EQUAL( int32_t{42} != signed_int{42}, false )
   REQUIRE_EQUAL( int32_t{43} != signed_int{42}, true )

   // ----------------------------------------------------------------
   // friend bool operator!=(const signed_int&, const signed_int&)
   REQUIRE_EQUAL( signed_int{42} != signed_int{42}, false )
   REQUIRE_EQUAL( signed_int{42} != signed_int{43}, true )

   // ------------------------------------------------------------
   // friend bool operator< (const signed_int&, const uint32_t&)
   REQUIRE_EQUAL( signed_int{42} < int32_t{42}, false )
   REQUIRE_EQUAL( signed_int{42} < int32_t{43}, true )

   // ------------------------------------------------------------
   // friend bool operator< (const uint32_t&, const signed_int&)
   REQUIRE_EQUAL( int32_t{42} < signed_int{42}, false )
   REQUIRE_EQUAL( int32_t{42} < signed_int{43}, true )

   // ----------------------------------------------------------------
   // friend bool operator< (const signed_int&, const signed_int&)
   REQUIRE_EQUAL( signed_int{42} < signed_int{42}, false )
   REQUIRE_EQUAL( signed_int{42} < signed_int{43}, true )

   // ------------------------------------------------------------
   // friend bool operator>=(const signed_int&, const uint32_t&)
   REQUIRE_EQUAL( signed_int{42} >= int32_t{42}, true )
   REQUIRE_EQUAL( signed_int{42} >= int32_t{43}, false )

   // ------------------------------------------------------------
   // friend bool operator>=(const uint32_t&, const signed_int&)
   REQUIRE_EQUAL( int32_t{42} >= signed_int{42}, true )
   REQUIRE_EQUAL( int32_t{42} >= signed_int{43}, false )

   // ----------------------------------------------------------------
   // friend bool operator>=(const signed_int&, const signed_int&)
   REQUIRE_EQUAL( signed_int{42} >= signed_int{42}, true )
   REQUIRE_EQUAL( signed_int{42} >= signed_int{43}, false )

   // -----------------------------
   // template<typename DataStream>
   // friend DataStream& operator<<(DataStream&, const signed_int&)
   const signed_int ci{0x7F};

   char buffer0[10]{};
   datastream<char*> ds0{buffer0, 10};

eosio::print(ci.value, "\n");
   ds0 << ci;
eosio::print(static_cast<int>(buffer0[0]), " ");
eosio::print(static_cast<int>(buffer0[1]), "\n");
   signed_int temp{};
   ds0 >> temp;
eosio::print(static_cast<int>(buffer0[0]), " ");
eosio::print(static_cast<int>(buffer0[1]), "\n");
eosio::print(temp.value, "\n");

   REQUIRE_EQUAL( ci == temp, true )
   
   // REQUIRE_EQUAL( memcmp(expected_buffer0, buffer0, 2), 0 )

   // -----------------------------
   // template<typename DataStream>
   // friend DataStream& operator>>(DataStream&, signed_int&)
   signed_int i{};
   const char expected_buffer1[1]{0x7F};

   char buffer1[1]{0x7F};
   datastream<char*> ds1{buffer1, 1};

   ds1 >> i;

   REQUIRE_EQUAL( memcmp(expected_buffer1, buffer1, 1), 0 )

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(unsigned_int_type_test)
   EOSIO_TEST(signed_int_type_test);
   return has_failed();
}
