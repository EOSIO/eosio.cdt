#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

using eosio::fixed_bytes;

using namespace eosio::native;

EOSIO_TEST_BEGIN(fixed_bytes_test)
   silence_output(false);

// constructors

/// fixed_bytes(const std::array<word_t, num_words()>&)d
// static constexpr size_t num_words()
REQUIRE_EQUAL( (fixed_bytes<0>{std::array<uint128_t,0>{}}.num_words()), 0 )
REQUIRE_EQUAL( (fixed_bytes<1>{std::array<uint128_t,1>{}}.num_words()), 1 )
REQUIRE_EQUAL( (fixed_bytes<32>{std::array<uint128_t,2>{}}.num_words()), 2 )
REQUIRE_EQUAL( (fixed_bytes<512>{std::array<uint128_t,32>{}}.num_words()), 32 )

/// constexpr fixed_bytes()
REQUIRE_EQUAL( fixed_bytes<0>{}.num_words(), 0 )
REQUIRE_EQUAL( fixed_bytes<1>{}.num_words(), 1 )
REQUIRE_EQUAL( fixed_bytes<32>{}.num_words(), 2 )
REQUIRE_EQUAL( fixed_bytes<512>{}.num_words(), 32 )

/// fixed_bytes(const Word(&arr)[NumWords])
// const uint128_t arr0[0]{};
// const uint128_t arr1[1]{};
// const uint128_t arr2[2]{};
// const uint128_t arr3[32]{};

// static constexpr size_t padded_bytes()
REQUIRE_EQUAL( fixed_bytes<0>{}.padded_bytes(), 0 )
REQUIRE_EQUAL( fixed_bytes<1>{}.padded_bytes(), 15 )
REQUIRE_EQUAL( fixed_bytes<32>{}.padded_bytes(), 0 )
REQUIRE_EQUAL( fixed_bytes<512>{}.padded_bytes(), 0 )

// const auto& get_array()const
REQUIRE_EQUAL( (fixed_bytes<0>{}.get_array()), (std::array<uint128_t, 0>{}) )
REQUIRE_EQUAL( (fixed_bytes<1>{}.get_array()), (std::array<uint128_t, 1>{}) )
REQUIRE_EQUAL( (fixed_bytes<32>{}.get_array()), (std::array<uint128_t, 2>{}) )
REQUIRE_EQUAL( (fixed_bytes<512>{}.get_array()), (std::array<uint128_t, 32>{}) )

// eosio::print((fixed_bytes<2>{1}.data()));
// eosio::print("\n");
// eosio::print(*((uint128_t*){}));
// eosio::print("\n");

// auto data()
std::array<uint128_t,0> arr0{};
std::array<uint128_t,1> arr1{};
std::array<uint128_t,2> arr2{};
std::array<uint128_t,32> arr3{};
fixed_bytes<0> fb0(arr0);
fixed_bytes<1> fb1{arr1};
fixed_bytes<32> fb2{arr2};
fixed_bytes<512> fb3{arr3};

REQUIRE_EQUAL( fb0.data(), arr0.data() )
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
