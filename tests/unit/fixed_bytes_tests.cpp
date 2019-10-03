/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <array>

#include <eosio/tester.hpp>
#include <eosio/fixed_bytes.hpp>

using std::array;

using eosio::fixed_bytes;

// Definitions in `eosio.cdt/libraries/eosio/fixed_bytes.hpp`
EOSIO_TEST_BEGIN(fixed_bytes_test)
   //// constexpr fixed_bytes()
   // static constexpr size_t padded_bytes()
   CHECK_EQUAL( fixed_bytes<20>{}.padded_bytes(), 12 )
   CHECK_EQUAL( fixed_bytes<32>{}.padded_bytes(),  0 )
   CHECK_EQUAL( fixed_bytes<64>{}.padded_bytes(),  0 )
   CHECK_EQUAL( fixed_bytes<128>{}.padded_bytes(), 0 )
   CHECK_EQUAL( fixed_bytes<256>{}.padded_bytes(), 0 )

   //// fixed_bytes(const array<word_t, num_words()>&)
   // static constexpr size_t num_words()
   CHECK_EQUAL( (fixed_bytes<20>{array<fixed_bytes<20>::word_t, 2>{}}.num_words()), 2 )
   CHECK_EQUAL( (fixed_bytes<32>{array<fixed_bytes<32>::word_t, 2>{}}.num_words()), 2 )
   CHECK_EQUAL( (fixed_bytes<64>{array<fixed_bytes<64>::word_t, 4>{}}.num_words()), 4 )

   //// fixed_bytes(const std::array<Word, NumWords>&)
   CHECK_EQUAL( (fixed_bytes<20> {array<uint8_t, 0>{}}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<32> {array<uint8_t, 0>{}}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<64> {array<uint8_t, 0>{}}.num_words()),  4 )
   CHECK_EQUAL( (fixed_bytes<128>{array<uint8_t, 0>{}}.num_words()),  8 )
   CHECK_EQUAL( (fixed_bytes<256>{array<uint8_t, 0>{}}.num_words()), 16 )

   CHECK_EQUAL( (fixed_bytes<20> {array<uint16_t, 0>{}}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<32> {array<uint16_t, 0>{}}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<64> {array<uint16_t, 0>{}}.num_words()),  4 )
   CHECK_EQUAL( (fixed_bytes<128>{array<uint16_t, 0>{}}.num_words()),  8 )
   CHECK_EQUAL( (fixed_bytes<256>{array<uint16_t, 0>{}}.num_words()), 16 )

   CHECK_EQUAL( (fixed_bytes<20> {array<uint32_t, 0>{}}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<32> {array<uint32_t, 0>{}}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<64> {array<uint32_t, 0>{}}.num_words()),  4 )
   CHECK_EQUAL( (fixed_bytes<128>{array<uint32_t, 0>{}}.num_words()),  8 )
   CHECK_EQUAL( (fixed_bytes<256>{array<uint32_t, 0>{}}.num_words()), 16 )

   CHECK_EQUAL( (fixed_bytes<20> {array<uint64_t, 0>{}}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<32> {array<uint64_t, 0>{}}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<64> {array<uint64_t, 0>{}}.num_words()),  4 )
   CHECK_EQUAL( (fixed_bytes<128>{array<uint64_t, 0>{}}.num_words()),  8 )
   CHECK_EQUAL( (fixed_bytes<256>{array<uint64_t, 0>{}}.num_words()), 16 )

   //// fixed_bytes(const Word(&arr)[NumWords])
   static constexpr uint8_t   ctor_arr8[1]{};
   static constexpr uint16_t  ctor_arr16[1]{};
   static constexpr uint32_t  ctor_arr32[1]{};
   static constexpr uint64_t  ctor_arr64[1]{};

   CHECK_EQUAL( (fixed_bytes<20> {ctor_arr8}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<32> {ctor_arr8}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<64> {ctor_arr8}.num_words()),  4 )
   CHECK_EQUAL( (fixed_bytes<128>{ctor_arr8}.num_words()),  8 )
   CHECK_EQUAL( (fixed_bytes<256>{ctor_arr8}.num_words()), 16 )

   CHECK_EQUAL( (fixed_bytes<20> {ctor_arr16}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<32> {ctor_arr16}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<64> {ctor_arr16}.num_words()),  4 )
   CHECK_EQUAL( (fixed_bytes<128>{ctor_arr16}.num_words()),  8 )
   CHECK_EQUAL( (fixed_bytes<256>{ctor_arr16}.num_words()), 16 )

   CHECK_EQUAL( (fixed_bytes<20> {ctor_arr32}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<32> {ctor_arr32}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<64> {ctor_arr32}.num_words()),  4 )
   CHECK_EQUAL( (fixed_bytes<128>{ctor_arr32}.num_words()),  8 )
   CHECK_EQUAL( (fixed_bytes<256>{ctor_arr32}.num_words()), 16 )

   CHECK_EQUAL( (fixed_bytes<20> {ctor_arr64}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<32> {ctor_arr64}.num_words()),  2 )
   CHECK_EQUAL( (fixed_bytes<64> {ctor_arr64}.num_words()),  4 )
   CHECK_EQUAL( (fixed_bytes<128>{ctor_arr64}.num_words()),  8 )
   CHECK_EQUAL( (fixed_bytes<256>{ctor_arr64}.num_words()), 16 )

   // -------------------------------------------
   // make_from_word_sequence(FirstWord, Rest...)

   // Possible discrepancy between two `make_from_word_sequence` and array constructor
   // CHECK_EQUAL( (fixed_bytes<20>::make_from_word_sequence<uint64_t>(1ULL, 2ULL)),
   //              (fixed_bytes<20>{array<uint64_t,2>{1ULL, 2ULL}}) )

   CHECK_EQUAL( (fixed_bytes<32>::make_from_word_sequence<uint64_t>(1ULL,2ULL,3ULL,4ULL)),
                (fixed_bytes<32>{array<uint64_t, 4>{1ULL,2ULL,3ULL,4ULL}}) )

   // -------------------------------------------------
   // array<uint8_t, Size> extract_as_byte_array()const
   static constexpr array<uint8_t, 32> extract_arr{ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
                                                    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04 };

   CHECK_EQUAL( (fixed_bytes<32>{array<uint64_t, 4>{1ULL,2ULL,3ULL,4ULL}}.extract_as_byte_array()), extract_arr )

   // ----------------------------
   // const auto& get_array()const
   CHECK_EQUAL( (fixed_bytes<0>{}.get_array()), (array<uint128_t, 0>{}) )
   CHECK_EQUAL( (fixed_bytes<1>{}.get_array()), (array<uint128_t, 1>{}) )
   CHECK_EQUAL( (fixed_bytes<32>{}.get_array()), (array<uint128_t, 2>{}) )
   CHECK_EQUAL( (fixed_bytes<512>{}.get_array()), (array<uint128_t, 32>{}) )

   // -----------
   // auto data()
   static constexpr fixed_bytes<0> fb_data0{};
   static constexpr fixed_bytes<1> fb_data1{};
   static constexpr fixed_bytes<32> fb_data2{};
   static constexpr fixed_bytes<512> fb_data3{};

   CHECK_EQUAL( fb_data0.data(), fb_data0.get_array().data() )
   CHECK_EQUAL( fb_data1.data(), fb_data1.get_array().data() )
   CHECK_EQUAL( fb_data2.data(), fb_data2.get_array().data() )
   CHECK_EQUAL( fb_data3.data(), fb_data3.get_array().data() )

   // ----------------
   // auto data()const
   static constexpr fixed_bytes<0> cfb_data0{};
   static constexpr fixed_bytes<1> cfb_data1{};
   static constexpr fixed_bytes<32> cfb_data2{};
   static constexpr fixed_bytes<512> cfb_data3{};

   CHECK_EQUAL( cfb_data0.data(), cfb_data0.get_array().data() )
   CHECK_EQUAL( cfb_data1.data(), cfb_data1.get_array().data() )
   CHECK_EQUAL( cfb_data2.data(), cfb_data2.get_array().data() )
   CHECK_EQUAL( cfb_data3.data(), cfb_data3.get_array().data() )

   // ----------------
   // auto size()const
   CHECK_EQUAL( fixed_bytes<0>{}.size(), 0 )
   CHECK_EQUAL( fixed_bytes<1>{}.size(), 1 )
   CHECK_EQUAL( fixed_bytes<32>{}.size(), 2 )
   CHECK_EQUAL( fixed_bytes<512>{}.size(), 32 )

   // ---------------------------------------------------------------------------
   // friend bool operator== <>(const fixed_bytes<Size>, const fixed_bytes<Size>)
   static const fixed_bytes<32> fb_cmp0{fixed_bytes<32>::make_from_word_sequence<uint64_t>(0ULL,0ULL,0ULL,0ULL)};
   static const fixed_bytes<32> fb_cmp1{fixed_bytes<32>::make_from_word_sequence<uint64_t>(1ULL,1ULL,1ULL,1ULL)};
   static const fixed_bytes<32> fb_cmp2{fixed_bytes<32>::make_from_word_sequence<uint64_t>(2ULL,2ULL,2ULL,2ULL)};

   CHECK_EQUAL( fb_cmp1 == fb_cmp1, true  )
   CHECK_EQUAL( fb_cmp0 == fb_cmp1, false )

   // ---------------------------------------------------------------------------
   // friend bool operator!= <>(const fixed_bytes<Size>, const fixed_bytes<Size>)
   CHECK_EQUAL( fb_cmp1 != fb_cmp1, false )
   CHECK_EQUAL( fb_cmp0 != fb_cmp1, true  )

   // ---------------------------------------------------------------------------
   // friend bool operator<  <>(const fixed_bytes<Size>, const fixed_bytes<Size>)
   CHECK_EQUAL( fb_cmp0 <  fb_cmp1, true  )
   CHECK_EQUAL( fb_cmp1 <  fb_cmp1, false )

   // ---------------------------------------------------------------------------
   // friend bool operator<= <>(const fixed_bytes<Size>, const fixed_bytes<Size>)
   CHECK_EQUAL( fb_cmp1 <= fb_cmp1, true  )
   CHECK_EQUAL( fb_cmp2 <= fb_cmp1, false )

   // ---------------------------------------------------------------------------
   // friend bool operator>  <>(const fixed_bytes<Size>, const fixed_bytes<Size>)
   CHECK_EQUAL( fb_cmp1 >  fb_cmp0, true  )
   CHECK_EQUAL( fb_cmp1 >  fb_cmp1, false )

   // ---------------------------------------------------------------------------
   // friend bool operator>= <>(const fixed_bytes<Size>, const fixed_bytes<Size>)
   CHECK_EQUAL( fb_cmp1 >= fb_cmp1, true  )
   CHECK_EQUAL( fb_cmp1 >= fb_cmp2, false )
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   bool verbose = false;
   if( argc >= 2 && std::strcmp( argv[1], "-v" ) == 0 ) {
      verbose = true;
   }
   silence_output(!verbose);

   EOSIO_TEST(fixed_bytes_test);
   return has_failed();
}
