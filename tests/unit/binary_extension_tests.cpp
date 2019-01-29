#include <eosio/native/tester.hpp>
#include <eosiolib/binary_extension.hpp>

using eosio::binary_extension;

using namespace eosio::native;

// TODO: come up with more descriptive test variable names

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
   binary_extension<const char> be_destruct_char{'c'};
   REQUIRE_EQUAL( be_destruct_char.has_value(), true )
   be_destruct_char.~binary_extension();
   REQUIRE_EQUAL( be_destruct_char.has_value(), false )

   binary_extension<const char*> be_destruct{"abcd"};
   REQUIRE_EQUAL( be_destruct.has_value(), true )
   be_destruct.~binary_extension();
   REQUIRE_EQUAL( be_destruct.has_value(), false )

   // constexpr binary_extension(const binary_extension&)
   binary_extension<const char> be_copy_char_lval0{'c'};
   binary_extension<const char> be_copy_char_lval1{be_copy_char_lval0};
   REQUIRE_EQUAL( (be_copy_char_lval0.value() == be_copy_char_lval1.value() && be_copy_char_lval0.has_value() == be_copy_char_lval1.has_value()), true)

   binary_extension<const char*> be_copy_lval0{"abcd"};
   binary_extension<const char*> be_copy_lval1{be_copy_lval0};
   REQUIRE_EQUAL( (be_copy_lval0.value() == be_copy_lval1.value() && be_copy_lval0.has_value() == be_copy_lval1.has_value()), true)

   // constexpr binary_extension(binary_extension&&)
   binary_extension<const char> be_copy_char_rval0{'c'};
   REQUIRE_EQUAL( be_copy_char_rval0.has_value(), true )
   binary_extension<const char> be_copy_char_rval1{std::move(be_copy_char_rval0)};
   REQUIRE_EQUAL( be_copy_char_rval0.has_value(), false )
   REQUIRE_ASSERT( "cannot get value of empty binary_extension", [&](){be_copy_char_rval0.value();} )

   binary_extension<const char*> be_copy_rval0{"abcd"};
   REQUIRE_EQUAL( be_copy_rval0.has_value(), true )
   binary_extension<const char*> be_copy_rval1{std::move(be_copy_rval0)};
   REQUIRE_EQUAL( be_copy_rval0.has_value(), false )
   REQUIRE_ASSERT( "cannot get value of empty binary_extension", [&](){be_copy_rval0.value();} )

   // constexpr explicit operator bool()const
   REQUIRE_EQUAL( (binary_extension<const char>{}.operator bool()), false )
   REQUIRE_EQUAL( (binary_extension<const char>{0}.operator bool()), true )
   REQUIRE_EQUAL( (binary_extension<const char>{'c'}.operator bool()), true )

   REQUIRE_EQUAL( (binary_extension<const char*>{}.operator bool()), false )
   REQUIRE_EQUAL( (binary_extension<const char*>{nullptr}.operator bool()), true )
   REQUIRE_EQUAL( (binary_extension<const char*>{"abcd"}.operator bool()), true )

   // constexpr T& value()&
   binary_extension<char> be_char_value0{'c'};
   REQUIRE_EQUAL( (be_char_value0.value() == 'c'), true )
   REQUIRE_EQUAL( (be_char_value0.value() == 'd'), false )

   binary_extension<char*> be_value0{"abcd"};
   REQUIRE_EQUAL( (be_value0.value() == "abcd"), true )
   REQUIRE_EQUAL( (be_value0.value() == "efgh"), false )

   // constexpr const T& value()const&
   binary_extension<const char> be_char_const_value0{'c'};
   REQUIRE_EQUAL( (be_char_const_value0.value() == 'c'), true )
   REQUIRE_EQUAL( (be_char_const_value0.value() == 'd'), false )

   binary_extension<const char*> be_const_value0{"abcd"};
   REQUIRE_EQUAL( (be_const_value0.value() == "abcd"), true )
   REQUIRE_EQUAL( (be_const_value0.value() == "efgh"), false )

   // constexpr auto value_or(U&&) -> std::enable_if_t<std::is_convertible<U, T>::value, T&>&
   binary_extension<uint8_t> be_temp_value_or{};
   // REQUIRE_EQUAL( (be_temp_value_or = be_temp_value_or.value_or(binary_extension<uint16_t>{0x63}) == 0x63), true )

   // constexpr T&& value_or()&&
   REQUIRE_EQUAL( (binary_extension<char>{'c'}.value_or() == 'c'), true )

   binary_extension<char*> be_value_or_rval{"abcd"};
   REQUIRE_EQUAL( (be_value_or_rval.value_or() == "abcd"), true )

   // constexpr const T&& value_or()const&&
   REQUIRE_EQUAL( (binary_extension<const char>{'c'}.value_or() == 'c'), true )

   binary_extension<const char*> be_const_value_or_rval{"abcd"};
   REQUIRE_EQUAL( (be_const_value_or_rval.value_or() == "abcd"), true )

   // constexpr T value_or()&
   binary_extension<char> be_char_value_or_val{'c'};
   REQUIRE_EQUAL( (be_char_value_or_val.value_or() == 'c'), true )

   binary_extension<char*> be_value_or_val{"abcd"};
   REQUIRE_EQUAL( (be_value_or_val.value_or() == "abcd"), true )

   // constexpr T value_or()const&
   binary_extension<const char> be_char_const_value_or_val{'c'};
   REQUIRE_EQUAL( (be_char_const_value_or_val.value_or() == 'c'), true )

   binary_extension<const char*> be_const_value_or_val{"abcd"};
   REQUIRE_EQUAL( (be_const_value_or_val.value_or() == "abcd"), true )

   // constexpr T* operator->()
   binary_extension<char> be_char_arrow_op{'c'};
   REQUIRE_EQUAL( *be_char_arrow_op.operator->() == 'c', true )
   REQUIRE_EQUAL( *be_char_arrow_op.operator->() != 'd', true )

   binary_extension<char*> be_arrow_op{"abcd"};
   REQUIRE_EQUAL( *be_arrow_op.operator->() == "abcd", true )
   REQUIRE_EQUAL( *be_arrow_op.operator->() != "efgh", true )

   // constexpr const T* operator->()const
   binary_extension<const char> be_char_const_arrow_op{'c'};
   REQUIRE_EQUAL( *be_char_const_arrow_op.operator->() == 'c', true )
   REQUIRE_EQUAL( *be_char_const_arrow_op.operator->() != 'd', true )

   binary_extension<const char*> be_const_arrow_op{"abcd"};
   REQUIRE_EQUAL( *be_const_arrow_op.operator->() == "abcd", true )
   REQUIRE_EQUAL( *be_const_arrow_op.operator->() != "efgh", true )

   // constexpr T& operator*()&
   binary_extension<char> be_char_star_op{'c'};
   REQUIRE_EQUAL( be_char_star_op.operator*() == 'c', true )
   REQUIRE_EQUAL( be_char_star_op.operator*() != 'd', true )

   binary_extension<char*> be_star_op{"abcd"};
   REQUIRE_EQUAL( be_star_op.operator*() == "abcd", true )
   REQUIRE_EQUAL( be_star_op.operator*() != "efgh", true )

   // constexpr const T& operator*()const&
   binary_extension<const char> be_char_const_star_op{'c'};
   REQUIRE_EQUAL( be_char_const_star_op.operator*() == 'c', true )
   REQUIRE_EQUAL( be_char_const_star_op.operator*() != 'd', true )

   binary_extension<const char*> be_const_star_op{"abcd"};
   REQUIRE_EQUAL( be_const_star_op.operator*() == "abcd", true )
   REQUIRE_EQUAL( be_const_star_op.operator*() != "efgh", true )

   // constexpr T&& operator*()&&
   binary_extension<char> be_char_rval_star_op{'c'};
   REQUIRE_EQUAL( std::move(be_char_rval_star_op.operator*()) == 'c', true )
   REQUIRE_EQUAL( std::move(be_char_rval_star_op.operator*()) != 'd', true )

   binary_extension<char*> be_rval_star_op{"abcd"};
   REQUIRE_EQUAL( std::move(be_rval_star_op.operator*()) == "abcd", true )
   REQUIRE_EQUAL( std::move(be_rval_star_op.operator*()) != "efgh", true )

   // constexpr const T&& operator*()const&&
   binary_extension<const char> be_char_const_rval_star_op{'c'};
   REQUIRE_EQUAL( std::move(be_char_const_rval_star_op.operator*()) == 'c', true )
   REQUIRE_EQUAL( std::move(be_char_const_rval_star_op.operator*()) != 'd', true )

   binary_extension<const char*> be_const_rval_star_op{"abcd"};
   REQUIRE_EQUAL( std::move(be_const_rval_star_op.operator*()) == "abcd", true )
   REQUIRE_EQUAL( std::move(be_const_rval_star_op.operator*()) != "efgh", true )

   // T& emplace(Args&& ... args)&
   binary_extension<const char> be_char_emplace{'c'};
   be_char_emplace.emplace(std::move('d'));
   REQUIRE_EQUAL( be_char_emplace.value() == 'd', true )
   REQUIRE_EQUAL( be_char_emplace.value() != 'c', true )

   binary_extension<const char*> be_emplace{"abcd"};
   be_emplace.emplace(std::move("efgh"));
   REQUIRE_EQUAL( be_emplace.value() == "efgh", true )
   REQUIRE_EQUAL( be_emplace.value() != "abcd", true )

   // void reset()
   binary_extension<const char> be_char_reset{'c'};
   REQUIRE_EQUAL( be_char_reset.has_value(), true )
   be_char_reset.reset();
   REQUIRE_EQUAL( be_char_reset.has_value(), false )

   binary_extension<const char*> be_reset{"abcd"};
   REQUIRE_EQUAL( be_reset.has_value(), true )
   be_reset.reset();
   REQUIRE_EQUAL( be_reset.has_value(), false )

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(binary_extension_test);
   return has_failed();
}
