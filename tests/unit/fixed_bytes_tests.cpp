/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

using eosio::fixed_bytes;
using std::array;

using namespace eosio::native;

// Definitions in `eosio.cdt/libraries/eosiolib/fixed_bytes.hpp`
EOSIO_TEST_BEGIN(fixed_bytes_test)
   silence_output(false);

/// constexpr fixed_bytes()
// static constexpr size_t padded_bytes()
REQUIRE_EQUAL( fixed_bytes<20>{}.padded_bytes(), 12 )
REQUIRE_EQUAL( fixed_bytes<32>{}.padded_bytes(),  0 )
REQUIRE_EQUAL( fixed_bytes<64>{}.padded_bytes(),  0 )

// eosio::print(fixed_bytes<20>{}.num_words());
// eosio::print(fixed_bytes<32>{}.num_words());
// eosio::print(fixed_bytes<64>{}.num_words(), "\n");

// eosio::print(fixed_bytes<20>{array<uint128_t,2>{}}.num_words());
// eosio::print(fixed_bytes<32>{array<uint128_t,2>{}}.num_words());
// eosio::print(fixed_bytes<64>{array<uint128_t,4>{}}.num_words(), "\n");

/// fixed_bytes(const std::array<word_t, num_words()>&)
// static constexpr size_t num_words()
REQUIRE_EQUAL( (fixed_bytes<20>{array<uint128_t,2>{}}.num_words()), 2 )
REQUIRE_EQUAL( (fixed_bytes<32>{array<uint128_t,2>{}}.num_words()), 2 )
REQUIRE_EQUAL( (fixed_bytes<64>{array<uint128_t,4>{}}.num_words()), 4 )

// /// fixed_bytes(const std::array<Word, NumWords>&)
REQUIRE_EQUAL( (fixed_bytes<20> {array<uint8_t,0>{}}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<32> {array<uint8_t,0>{}}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<64> {array<uint8_t,0>{}}.num_words()),  4 )
REQUIRE_EQUAL( (fixed_bytes<128>{array<uint8_t,0>{}}.num_words()),  8 )
REQUIRE_EQUAL( (fixed_bytes<256>{array<uint8_t,0>{}}.num_words()), 16 )

REQUIRE_EQUAL( (fixed_bytes<20> {array<uint16_t,0>{}}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<32> {array<uint16_t,0>{}}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<64> {array<uint16_t,0>{}}.num_words()),  4 )
REQUIRE_EQUAL( (fixed_bytes<128>{array<uint16_t,0>{}}.num_words()),  8 )
REQUIRE_EQUAL( (fixed_bytes<256>{array<uint16_t,0>{}}.num_words()), 16 )

REQUIRE_EQUAL( (fixed_bytes<20> {array<uint32_t,0>{}}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<32> {array<uint32_t,0>{}}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<64> {array<uint32_t,0>{}}.num_words()),  4 )
REQUIRE_EQUAL( (fixed_bytes<128>{array<uint32_t,0>{}}.num_words()),  8 )
REQUIRE_EQUAL( (fixed_bytes<256>{array<uint32_t,0>{}}.num_words()), 16 )

REQUIRE_EQUAL( (fixed_bytes<20> {array<uint64_t,0>{}}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<32> {array<uint64_t,0>{}}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<64> {array<uint64_t,0>{}}.num_words()),  4 )
REQUIRE_EQUAL( (fixed_bytes<128>{array<uint64_t,0>{}}.num_words()),  8 )
REQUIRE_EQUAL( (fixed_bytes<256>{array<uint64_t,0>{}}.num_words()), 16 )

// eosio::print(fixed_bytes<20>{std::array<uint8_t, 0>{}}.num_words());
// eosio::print(fixed_bytes<32>{std::array<uint16_t,0>{}}.num_words());
// eosio::print(fixed_bytes<64>{std::array<uint32_t,0>{}}.num_words());
// eosio::print(fixed_bytes<128>{std::array<uint64_t,0>{}}.num_words());
// eosio::print(fixed_bytes<256>{std::array<uint64_t,0>{}}.num_words());
// REQUIRE_EQUAL( (fixed_bytes<32>{std::array<uint16_t,1>{}}.num_words()), 1 )
// REQUIRE_EQUAL( (fixed_bytes<64>{std::array<uint32_t,1>{}}.num_words()), 1 )
// REQUIRE_EQUAL( (fixed_bytes<128>{std::array<uint64_t,1>{}}.num_words()), 1 )

/// fixed_bytes(const Word(&arr)[NumWords])
const uint8_t   arr8[1]{};
const uint16_t  arr16[1]{};
const uint32_t  arr32[1]{};
const uint64_t  arr64[1]{};

REQUIRE_EQUAL( (fixed_bytes<20> {arr8}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<32> {arr8}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<64> {arr8}.num_words()),  4 )
REQUIRE_EQUAL( (fixed_bytes<128>{arr8}.num_words()),  8 )
REQUIRE_EQUAL( (fixed_bytes<256>{arr8}.num_words()), 16 )

REQUIRE_EQUAL( (fixed_bytes<20> {arr16}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<32> {arr16}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<64> {arr16}.num_words()),  4 )
REQUIRE_EQUAL( (fixed_bytes<128>{arr16}.num_words()),  8 )
REQUIRE_EQUAL( (fixed_bytes<256>{arr16}.num_words()), 16 )

REQUIRE_EQUAL( (fixed_bytes<20> {arr32}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<32> {arr32}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<64> {arr32}.num_words()),  4 )
REQUIRE_EQUAL( (fixed_bytes<128>{arr32}.num_words()),  8 )
REQUIRE_EQUAL( (fixed_bytes<256>{arr32}.num_words()), 16 )

REQUIRE_EQUAL( (fixed_bytes<20> {arr64}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<32> {arr64}.num_words()),  2 )
REQUIRE_EQUAL( (fixed_bytes<64> {arr64}.num_words()),  4 )
REQUIRE_EQUAL( (fixed_bytes<128>{arr64}.num_words()),  8 )
REQUIRE_EQUAL( (fixed_bytes<256>{arr64}.num_words()), 16 )

// make_from_word_sequence(FirstWord, Rest...)
array<uint64_t,4> arr{1ULL,2ULL,3ULL,4ULL};
fixed_bytes<32> fb1{fixed_bytes<32>::make_from_word_sequence<uint64_t>(1ULL,2ULL,3ULL,4ULL)};
fixed_bytes<32> fb2{arr};

REQUIRE_EQUAL( (fixed_bytes<20>::make_from_word_sequence<uint64_t>(1ULL)),
               (fixed_bytes<20>{array<uint64_t,1>{1ULL}}) )
REQUIRE_EQUAL( (fixed_bytes<32>::make_from_word_sequence<uint64_t>(1ULL,2ULL,3ULL,4ULL)),
               (fixed_bytes<32>{array<uint64_t,4>{1ULL,2ULL,3ULL,4ULL}}) )

// // const auto& get_array()const
// REQUIRE_EQUAL( (fixed_bytes<0>{}.get_array()), (std::array<uint128_t, 0>{}) )
// REQUIRE_EQUAL( (fixed_bytes<1>{}.get_array()), (std::array<uint128_t, 1>{}) )
// REQUIRE_EQUAL( (fixed_bytes<32>{}.get_array()), (std::array<uint128_t, 2>{}) )
// REQUIRE_EQUAL( (fixed_bytes<512>{}.get_array()), (std::array<uint128_t, 32>{}) )

// eosio::print((fixed_bytes<2>{1}.data()));
// eosio::print("\n");
// eosio::print(*((uint128_t*){}));
// eosio::print("\n");

// auto data()
// std::array<uint128_t,0> arr0{};
// std::array<uint128_t,1> arr1{};
// std::array<uint128_t,2> arr2{};
// std::array<uint128_t,32> arr3{};
// fixed_bytes<0> fb0(arr0);
// fixed_bytes<1> fb1{arr1};
// fixed_bytes<32> fb2{arr2};
// fixed_bytes<512> fb3{arr3};

// REQUIRE_EQUAL( fb0.data(), arr0.data() )
// REQUIRE_EQUAL( (fixed_bytes<0>{}.data()), uint128_t*(std::array<uint128_t, 0>{0}) )
// REQUIRE_EQUAL( fixed_bytes<1>{}.data(), 1 )
// REQUIRE_EQUAL( fixed_bytes<32>{}.data(), 2 )
// REQUIRE_EQUAL( fixed_bytes<512>{}.data(), 32 )

// // auto data()const
// REQUIRE_EQUAL( fixed_bytes<0>{}.data(), 0 )
// REQUIRE_EQUAL( fixed_bytes<1>{}.data(), 1 )
// REQUIRE_EQUAL( fixed_bytes<32>{}.data(), 2 )
// REQUIRE_EQUAL( fixed_bytes<512>{}.data(), 32 )

// // auto size()const
// REQUIRE_EQUAL( fixed_bytes<0>{}.size(), 0 )
// REQUIRE_EQUAL( fixed_bytes<1>{}.size(), 1 )
// REQUIRE_EQUAL( fixed_bytes<32>{}.size(), 2 )
// REQUIRE_EQUAL( fixed_bytes<512>{}.size(), 32 )

// // std::array<uint8_t, Size> extract_as_byte_array()const
// REQUIRE_EQUAL( fixed_bytes<0>{}.extract_as_byte_array(), 0 )
// REQUIRE_EQUAL( fixed_bytes<1>{}.extract_as_byte_array(), 1 )
// REQUIRE_EQUAL( fixed_bytes<32>{}.extract_as_byte_array(), 2 )
// REQUIRE_EQUAL( fixed_bytes<512>{}.extract_as_byte_array(), 32 )

// // template<size_t Size>
// // bool operator==(const fixed_bytes<Size>, const fixed_bytes<Size>)
// REQUIRE_EQUAL( fixed_bytes<0>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<1>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<32>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<512>{}.num_words(), true )

// // template<size_t Size>
// // bool operator!=(const fixed_bytes<Size>, const fixed_bytes<Size>)
// REQUIRE_EQUAL( fixed_bytes<0>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<1>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<32>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<512>{}.num_words(), true )

// // template<size_t Size>
// // bool operator>(const fixed_bytes<Size>, const fixed_bytes<Size>)
// REQUIRE_EQUAL( fixed_bytes<0>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<1>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<32>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<512>{}.num_words(), true )

// // template<size_t Size>
// // bool operator<(const fixed_bytes<Size>, const fixed_bytes<Size>)
// REQUIRE_EQUAL( fixed_bytes<0>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<1>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<32>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<512>{}.num_words(), true )

// // template<size_t Size>
// // bool operator>=(const fixed_bytes<Size>, const fixed_bytes<Size>)
// REQUIRE_EQUAL( fixed_bytes<0>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<1>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<32>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<512>{}.num_words(), true )

// // template<size_t Size>
// // bool operator<=(const fixed_bytes<Size>, const fixed_bytes<Size>)
// REQUIRE_EQUAL( fixed_bytes<0>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<1>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<32>{}.num_words(), true )
// REQUIRE_EQUAL( fixed_bytes<512>{}.num_words(), true )

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(fixed_bytes_test);
   return has_failed();
}
