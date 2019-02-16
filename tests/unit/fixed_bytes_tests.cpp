/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <array>

#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

using std::array;

using eosio::fixed_bytes;

// Definitions in `eosio.cdt/libraries/eosiolib/fixed_bytes.hpp`
EOSIO_TEST_BEGIN(fixed_bytes_test)
   silence_output(false);

/// constexpr fixed_bytes()
// static constexpr size_t padded_bytes()
CHECK_EQUAL( fixed_bytes<20>{}.padded_bytes(), 12 )
CHECK_EQUAL( fixed_bytes<32>{}.padded_bytes(),  0 )
CHECK_EQUAL( fixed_bytes<64>{}.padded_bytes(),  0 )

/// fixed_bytes(const std::array<word_t, num_words()>&)
// static constexpr size_t num_words()
CHECK_EQUAL( (fixed_bytes<20>{array<uint128_t,2>{}}.num_words()), 2 )
CHECK_EQUAL( (fixed_bytes<32>{array<uint128_t,2>{}}.num_words()), 2 )
CHECK_EQUAL( (fixed_bytes<64>{array<uint128_t,4>{}}.num_words()), 4 )

/// fixed_bytes(const std::array<Word, NumWords>&)
CHECK_EQUAL( (fixed_bytes<20> {array<uint8_t,0>{}}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<32> {array<uint8_t,0>{}}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<64> {array<uint8_t,0>{}}.num_words()),  4 )
CHECK_EQUAL( (fixed_bytes<128>{array<uint8_t,0>{}}.num_words()),  8 )
CHECK_EQUAL( (fixed_bytes<256>{array<uint8_t,0>{}}.num_words()), 16 )

CHECK_EQUAL( (fixed_bytes<20> {array<uint16_t,0>{}}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<32> {array<uint16_t,0>{}}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<64> {array<uint16_t,0>{}}.num_words()),  4 )
CHECK_EQUAL( (fixed_bytes<128>{array<uint16_t,0>{}}.num_words()),  8 )
CHECK_EQUAL( (fixed_bytes<256>{array<uint16_t,0>{}}.num_words()), 16 )

CHECK_EQUAL( (fixed_bytes<20> {array<uint32_t,0>{}}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<32> {array<uint32_t,0>{}}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<64> {array<uint32_t,0>{}}.num_words()),  4 )
CHECK_EQUAL( (fixed_bytes<128>{array<uint32_t,0>{}}.num_words()),  8 )
CHECK_EQUAL( (fixed_bytes<256>{array<uint32_t,0>{}}.num_words()), 16 )

CHECK_EQUAL( (fixed_bytes<20> {array<uint64_t,0>{}}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<32> {array<uint64_t,0>{}}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<64> {array<uint64_t,0>{}}.num_words()),  4 )
CHECK_EQUAL( (fixed_bytes<128>{array<uint64_t,0>{}}.num_words()),  8 )
CHECK_EQUAL( (fixed_bytes<256>{array<uint64_t,0>{}}.num_words()), 16 )

/// fixed_bytes(const Word(&arr)[NumWords])
const uint8_t   arr8[1]{};
const uint16_t  arr16[1]{};
const uint32_t  arr32[1]{};
const uint64_t  arr64[1]{};

CHECK_EQUAL( (fixed_bytes<20> {arr8}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<32> {arr8}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<64> {arr8}.num_words()),  4 )
CHECK_EQUAL( (fixed_bytes<128>{arr8}.num_words()),  8 )
CHECK_EQUAL( (fixed_bytes<256>{arr8}.num_words()), 16 )

CHECK_EQUAL( (fixed_bytes<20> {arr16}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<32> {arr16}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<64> {arr16}.num_words()),  4 )
CHECK_EQUAL( (fixed_bytes<128>{arr16}.num_words()),  8 )
CHECK_EQUAL( (fixed_bytes<256>{arr16}.num_words()), 16 )

CHECK_EQUAL( (fixed_bytes<20> {arr32}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<32> {arr32}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<64> {arr32}.num_words()),  4 )
CHECK_EQUAL( (fixed_bytes<128>{arr32}.num_words()),  8 )
CHECK_EQUAL( (fixed_bytes<256>{arr32}.num_words()), 16 )

CHECK_EQUAL( (fixed_bytes<20> {arr64}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<32> {arr64}.num_words()),  2 )
CHECK_EQUAL( (fixed_bytes<64> {arr64}.num_words()),  4 )
CHECK_EQUAL( (fixed_bytes<128>{arr64}.num_words()),  8 )
CHECK_EQUAL( (fixed_bytes<256>{arr64}.num_words()), 16 )

// -------------------------------------------
// make_from_word_sequence(FirstWord, Rest...)
array<uint64_t,4> arr{1ULL,2ULL,3ULL,4ULL};
fixed_bytes<32> fb1{fixed_bytes<32>::make_from_word_sequence<uint64_t>(1ULL,2ULL,3ULL,4ULL)};
fixed_bytes<32> fb2{arr};

CHECK_EQUAL( (fixed_bytes<20>::make_from_word_sequence<uint64_t>(1ULL, 2ULL)),
               (fixed_bytes<20>{array<uint64_t,2>{1ULL, 2ULL}}) )

CHECK_EQUAL( (fixed_bytes<32>::make_from_word_sequence<uint64_t>(1ULL,2ULL,3ULL,4ULL)),
               (fixed_bytes<32>{array<uint64_t,4>{1ULL,2ULL,3ULL,4ULL}}) )

// ----------------------------
// const auto& get_array()const
// CHECK_EQUAL( (fixed_bytes<0>{}.get_array()), (std::array<uint128_t, 0>{}) )
// CHECK_EQUAL( (fixed_bytes<1>{}.get_array()), (std::array<uint128_t, 1>{}) )
// CHECK_EQUAL( (fixed_bytes<32>{}.get_array()), (std::array<uint128_t, 2>{}) )
// CHECK_EQUAL( (fixed_bytes<512>{}.get_array()), (std::array<uint128_t, 32>{}) )

// -----------
// auto data()
// std::array<uint128_t,0> arr0{};
// std::array<uint128_t,1> arr1{};
// std::array<uint128_t,2> arr2{};
// std::array<uint128_t,32> arr3{};
// fixed_bytes<0> fb0(arr0);
// fixed_bytes<1> fb1{arr1};
// fixed_bytes<32> fb2{arr2};
// fixed_bytes<512> fb3{arr3};

// CHECK_EQUAL( fb0.data(), arr0.data() )
// CHECK_EQUAL( (fixed_bytes<0>{}.data()), uint128_t*(std::array<uint128_t, 0>{0}) )
// CHECK_EQUAL( fixed_bytes<1>{}.data(), 1 )
// CHECK_EQUAL( fixed_bytes<32>{}.data(), 2 )
// CHECK_EQUAL( fixed_bytes<512>{}.data(), 32 )

// ----------------
// auto data()const
// CHECK_EQUAL( fixed_bytes<0>{}.data(), 0 )
// CHECK_EQUAL( fixed_bytes<1>{}.data(), 1 )
// CHECK_EQUAL( fixed_bytes<32>{}.data(), 2 )
// CHECK_EQUAL( fixed_bytes<512>{}.data(), 32 )

// ----------------
// auto size()const
// CHECK_EQUAL( fixed_bytes<0>{}.size(), 0 )
// CHECK_EQUAL( fixed_bytes<1>{}.size(), 1 )
// CHECK_EQUAL( fixed_bytes<32>{}.size(), 2 )
// CHECK_EQUAL( fixed_bytes<512>{}.size(), 32 )

// ------------------------------------------------------
// std::array<uint8_t, Size> extract_as_byte_array()const
// CHECK_EQUAL( fixed_bytes<0>{}.extract_as_byte_array(), 0 )
// CHECK_EQUAL( fixed_bytes<1>{}.extract_as_byte_array(), 1 )
// CHECK_EQUAL( fixed_bytes<32>{}.extract_as_byte_array(), 2 )
// CHECK_EQUAL( fixed_bytes<512>{}.extract_as_byte_array(), 32 )

// ----------
// template<>
// const fixed_bytes<Size>
// bool operator==(const fixed_bytes<Size>, const fixed_bytes<Size>)
// CHECK_EQUAL( fixed_bytes<1>{1ULL} == fixed_bytes<1>{1ULL}, true  )
// CHECK_EQUAL( fixed_bytes<1>{} == fixed_bytes<1>{}, false )

// // ----------
// // template<>
// // const fixed_bytes<Size>
// // bool operator!=(const fixed_bytes<Size>, const fixed_bytes<Size>)
// CHECK_EQUAL( fixed_bytes<1>{} != fixed_bytes<1>{}, true  )
// CHECK_EQUAL( fixed_bytes<1>{} != fixed_bytes<1>{}, false )

// // ----------
// // template<>
// // const fixed_bytes<Size>
// // bool operator<(const fixed_bytes<Size>, const fixed_bytes<Size>)
// CHECK_EQUAL( fixed_bytes<1>{} < fixed_bytes<1>{}, true  )
// CHECK_EQUAL( fixed_bytes<1>{} < fixed_bytes<1>{}, false )

// // ----------
// // template<>
// // const fixed_bytes<Size>
// // bool operator<=(const fixed_bytes<Size>, const fixed_bytes<Size>)
// CHECK_EQUAL( fixed_bytes<1>{} <= fixed_bytes<1>{}, true  )
// CHECK_EQUAL( fixed_bytes<1>{} <= fixed_bytes<1>{}, false )

// // ----------
// // template<>
// // const fixed_bytes<Size>
// // bool operator>(const fixed_bytes<Size>, const fixed_bytes<Size>)
// CHECK_EQUAL( fixed_bytes<1>{} > fixed_bytes<1>{}, true  )
// CHECK_EQUAL( fixed_bytes<1>{} > fixed_bytes<1>{}, false )

// // ----------
// // template<>
// // const fixed_bytes<Size>
// // bool operator>=(const fixed_bytes<Size>, const fixed_bytes<Size>)
// CHECK_EQUAL( fixed_bytes<1>{} >= fixed_bytes<1>{}, true  )
// CHECK_EQUAL( fixed_bytes<1>{} >= fixed_bytes<1>{}, false )

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(fixed_bytes_test);
   return has_failed();
}
