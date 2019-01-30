#include <eosio/native/tester.hpp>
#include <eosiolib/binary_extension.hpp>

using eosio::binary_extension;

using namespace eosio::native;

// Definitions in `eosio.cdt/libraries/eosiolib/binary_extension.hpp`
EOSIO_TEST_BEGIN(binary_extension_test)
   silence_output(false);

   /// constexpr binary_extension()
   // constexpr bool has_value()const
   REQUIRE_EQUAL( (binary_extension<const char>{}.has_value()), false ) ///// Should I test more than just the `const char*` /////

   REQUIRE_EQUAL( (binary_extension<const char*>{}.has_value()), false ) ///// Should I test more than just the `const char*` /////

   /// constexpr binary_extension(const T&)
   REQUIRE_EQUAL( (binary_extension<const char>{'c'}.has_value()), true )

   REQUIRE_EQUAL( (binary_extension<const char*>{"abcd"}.has_value()), true )

   /// constexpr binary_extension(T&&)
   REQUIRE_EQUAL( (binary_extension<const char>{std::move('c')}.has_value()), true )

   REQUIRE_EQUAL( (binary_extension<const char*>{std::move("abcd")}.has_value()), true )

   /// constexpr binary_extension(std::in_place_t, Args&&...)
   REQUIRE_EQUAL( (binary_extension<const char>{std::in_place, 'c'}.has_value()), true ) ///// How do I pack more args into this?? /////

   REQUIRE_EQUAL( (binary_extension<const char*>{std::in_place, "abcd"}.has_value()), true ) ///// How do I pack more args into this?? /////

   // ~binary_extension()
   binary_extension<const char> bin_destruct_char{'c'};
   REQUIRE_EQUAL( bin_destruct_char.has_value(), true )
   bin_destruct_char.~binary_extension();
   REQUIRE_EQUAL( bin_destruct_char.has_value(), false )

   binary_extension<const char*> bin_destruct_str{"abcd"};
   REQUIRE_EQUAL( bin_destruct_str.has_value(), true )
   bin_destruct_str.~binary_extension();
   REQUIRE_EQUAL( bin_destruct_str.has_value(), false )

   // constexpr binary_extension(const binary_extension&)
   binary_extension<const char> bin_copy_char_lval0{'c'};
   binary_extension<const char> bin_copy_char_lval1{bin_copy_char_lval0};
   REQUIRE_EQUAL( (bin_copy_char_lval0.value() == bin_copy_char_lval1.value() &&
                   bin_copy_char_lval0.has_value() == bin_copy_char_lval1.has_value()), true)

   binary_extension<const char*> bin_copy_str_lval0{"abcd"};
   binary_extension<const char*> bin_copy_str_lval1{bin_copy_str_lval0};
   REQUIRE_EQUAL( (bin_copy_str_lval0.value() == bin_copy_str_lval1.value() && bin_copy_str_lval0.has_value() == bin_copy_str_lval1.has_value()), true)

   // constexpr binary_extension(binary_extension&&)
   binary_extension<const char> bin_copy_char_rval0{'c'};
   REQUIRE_EQUAL( bin_copy_char_rval0.has_value(), true )
   binary_extension<const char> bin_copy_char_rval1{std::move(bin_copy_char_rval0)};
   REQUIRE_EQUAL( bin_copy_char_rval0.has_value(), false )
   REQUIRE_ASSERT( "cannot get value of empty binary_extension", [&](){bin_copy_char_rval0.value();} )

   binary_extension<const char*> bin_copy_str_rval0{"abcd"};
   REQUIRE_EQUAL( bin_copy_str_rval0.has_value(), true )
   binary_extension<const char*> bin_copy_str_rval1{std::move(bin_copy_str_rval0)};
   REQUIRE_EQUAL( bin_copy_str_rval0.has_value(), false )
   REQUIRE_ASSERT( "cannot get value of empty binary_extension", [&](){bin_copy_str_rval0.value();} )

   // constexpr explicit operator bool()const
   REQUIRE_EQUAL( (binary_extension<const char>{}.operator bool()), false )
   REQUIRE_EQUAL( (binary_extension<const char>{0}.operator bool()), true )
   REQUIRE_EQUAL( (binary_extension<const char>{'c'}.operator bool()), true )

   REQUIRE_EQUAL( (binary_extension<const char*>{}.operator bool()), false )
   REQUIRE_EQUAL( (binary_extension<const char*>{nullptr}.operator bool()), true )
   REQUIRE_EQUAL( (binary_extension<const char*>{"abcd"}.operator bool()), true )

   // constexpr T& value()&
   binary_extension<char> bin_char_value0{'c'};
   REQUIRE_EQUAL( (bin_char_value0.value() == 'c'), true )
   REQUIRE_EQUAL( (bin_char_value0.value() == 'd'), false )

   binary_extension<char*> bin_str_value0{"abcd"};
   REQUIRE_EQUAL( (bin_str_value0.value() == "abcd"), true )
   REQUIRE_EQUAL( (bin_str_value0.value() == "efgh"), false )

   // constexpr const T& value()const&
   binary_extension<const char> bin_const_char_value0{'c'};
   REQUIRE_EQUAL( (bin_const_char_value0.value() == 'c'), true )
   REQUIRE_EQUAL( (bin_const_char_value0.value() == 'd'), false )

   binary_extension<const char*> bin_const_str_value0{"abcd"};
   REQUIRE_EQUAL( (bin_const_str_value0.value() == "abcd"), true )
   REQUIRE_EQUAL( (bin_const_str_value0.value() == "efgh"), false )

   // constexpr auto value_or(U&&) -> std::enable_if_t<std::is_convertible<U, T>::value, T&>&
   binary_extension<uint8_t> bin_temp_value_or{};
   uint8_t some_value = bin_temp_value_or.value_or(uint8_t{0x63});
   REQUIRE_EQUAL( (some_value == 0x63), true )

   // constexpr T&& value_or()&&
   REQUIRE_EQUAL( (binary_extension<char>{'c'}.value_or() == 'c'), true )

   binary_extension<char*> bin_value_or_rval{"abcd"};
   REQUIRE_EQUAL( (bin_value_or_rval.value_or() == "abcd"), true )

   // constexpr const T&& value_or()const&&
   REQUIRE_EQUAL( (binary_extension<const char>{'c'}.value_or() == 'c'), true )

   binary_extension<const char*> bin_const_value_or_rval{"abcd"};
   REQUIRE_EQUAL( (bin_const_value_or_rval.value_or() == "abcd"), true )

   // constexpr T value_or()&
   binary_extension<char> bin_char_value_or_val{'c'};
   REQUIRE_EQUAL( (bin_char_value_or_val.value_or() == 'c'), true )

   binary_extension<char*> bin_value_or_val{"abcd"};
   REQUIRE_EQUAL( (bin_value_or_val.value_or() == "abcd"), true )

   // constexpr T value_or()const&
   binary_extension<const char> bin_char_const_value_or_val{'c'};
   REQUIRE_EQUAL( (bin_char_const_value_or_val.value_or() == 'c'), true )

   binary_extension<const char*> bin_const_value_or_val{"abcd"};
   REQUIRE_EQUAL( (bin_const_value_or_val.value_or() == "abcd"), true )

   // constexpr T* operator->()
   binary_extension<char> bin_char_arrow_op{'c'};
   REQUIRE_EQUAL( *bin_char_arrow_op.operator->() == 'c', true )
   REQUIRE_EQUAL( *bin_char_arrow_op.operator->() != 'd', true )

   binary_extension<char*> bin_arrow_op{"abcd"};
   REQUIRE_EQUAL( *bin_arrow_op.operator->() == "abcd", true )
   REQUIRE_EQUAL( *bin_arrow_op.operator->() != "efgh", true )

   // constexpr const T* operator->()const
   binary_extension<const char> bin_char_const_arrow_op{'c'};
   REQUIRE_EQUAL( *bin_char_const_arrow_op.operator->() == 'c', true )
   REQUIRE_EQUAL( *bin_char_const_arrow_op.operator->() != 'd', true )

   binary_extension<const char*> bin_const_arrow_op{"abcd"};
   REQUIRE_EQUAL( *bin_const_arrow_op.operator->() == "abcd", true )
   REQUIRE_EQUAL( *bin_const_arrow_op.operator->() != "efgh", true )

   // constexpr T& operator*()&
   binary_extension<char> bin_char_star_op{'c'};
   REQUIRE_EQUAL( bin_char_star_op.operator*() == 'c', true )
   REQUIRE_EQUAL( bin_char_star_op.operator*() != 'd', true )

   binary_extension<char*> bin_star_op{"abcd"};
   REQUIRE_EQUAL( bin_star_op.operator*() == "abcd", true )
   REQUIRE_EQUAL( bin_star_op.operator*() != "efgh", true )

   // constexpr const T& operator*()const&
   binary_extension<const char> bin_char_const_star_op{'c'};
   REQUIRE_EQUAL( bin_char_const_star_op.operator*() == 'c', true )
   REQUIRE_EQUAL( bin_char_const_star_op.operator*() != 'd', true )

   binary_extension<const char*> bin_const_star_op{"abcd"};
   REQUIRE_EQUAL( bin_const_star_op.operator*() == "abcd", true )
   REQUIRE_EQUAL( bin_const_star_op.operator*() != "efgh", true )

   // constexpr T&& operator*()&&
   binary_extension<char> bin_char_rval_star_op{'c'};
   REQUIRE_EQUAL( std::move(bin_char_rval_star_op.operator*()) == 'c', true )
   REQUIRE_EQUAL( std::move(bin_char_rval_star_op.operator*()) != 'd', true )

   binary_extension<char*> bin_str_rval_star_op{"abcd"};
   REQUIRE_EQUAL( std::move(bin_str_rval_star_op.operator*()) == "abcd", true )
   REQUIRE_EQUAL( std::move(bin_str_rval_star_op.operator*()) != "efgh", true )

   // constexpr const T&& operator*()const&&
   binary_extension<const char> bin_const_char_rval_star_op{'c'};
   REQUIRE_EQUAL( std::move(bin_const_char_rval_star_op.operator*()) == 'c', true )
   REQUIRE_EQUAL( std::move(bin_const_char_rval_star_op.operator*()) != 'd', true )

   binary_extension<const char*> bin_const_str_rval_star_op{"abcd"};
   REQUIRE_EQUAL( std::move(bin_const_str_rval_star_op.operator*()) == "abcd", true )
   REQUIRE_EQUAL( std::move(bin_const_str_rval_star_op.operator*()) != "efgh", true )
   
   // T& emplace(Args&& ... args)&
   binary_extension<const char> bin_char_emplace{'c'};
   bin_char_emplace.emplace(std::move('d'));
   REQUIRE_EQUAL( bin_char_emplace.value() == 'd', true )
   REQUIRE_EQUAL( bin_char_emplace.value() != 'c', true )

   binary_extension<const char*> bin_str_emplace{"abcd"};
   bin_str_emplace.emplace(std::move("efgh"));
   REQUIRE_EQUAL( bin_str_emplace.value() == "efgh", true )
   REQUIRE_EQUAL( bin_str_emplace.value() != "abcd", true ) 

   // void reset()
   binary_extension<const char> bin_char_reset{'c'};
   REQUIRE_EQUAL( bin_char_reset.has_value(), true )
   bin_char_reset.reset();
   REQUIRE_EQUAL( bin_char_reset.has_value(), false )

   binary_extension<const char*> bin_str_reset{"abcd"};
   REQUIRE_EQUAL( bin_str_reset.has_value(), true )
   bin_str_reset.reset();
   REQUIRE_EQUAL( bin_str_reset.has_value(), false )

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(binary_extension_test);
   return has_failed();
}
