/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/tester.hpp>
#include <eosio/binary_extension.hpp>

using std::in_place;
using std::move;

using eosio::binary_extension;

// Definitions in `eosio.cdt/libraries/eosio/binary_extension.hpp`
EOSIO_TEST_BEGIN(binary_extension_test)
   //// constexpr binary_extension()
   // constexpr bool has_value()const
   CHECK_EQUAL( (binary_extension<const char>{}.has_value()), false )

   CHECK_EQUAL( (binary_extension<const char*>{}.has_value()), false )

   //// constexpr binary_extension(const T&)
   CHECK_EQUAL( (binary_extension<const char>{'c'}.has_value()), true )

   CHECK_EQUAL( (binary_extension<const char*>{"abcd"}.has_value()), true )

   //// constexpr binary_extension(T&&)
   CHECK_EQUAL( (binary_extension<const char>{move('c')}.has_value()), true )

   CHECK_EQUAL( (binary_extension<const char*>{move("abcd")}.has_value()), true )

   //// constexpr binary_extension(in_place_t, Args&&...)
   CHECK_EQUAL( (binary_extension<const char>{in_place, 'c'}.has_value()), true )

   CHECK_EQUAL( (binary_extension<const char*>{in_place, "abcd"}.has_value()), true )

   //// constexpr binary_extension(const binary_extension&)
   static const binary_extension<const char> cbe_ctor_char_lval0{'c'};
   static const binary_extension<const char> cbe_ctor_char_lval1{cbe_ctor_char_lval0};
   CHECK_EQUAL( cbe_ctor_char_lval0.value(), cbe_ctor_char_lval1.value() )
   CHECK_EQUAL( cbe_ctor_char_lval0.has_value(), cbe_ctor_char_lval1.has_value() )

   static const binary_extension<const char*> cbe_ctor_str_lval0{"abcd"};
   static const binary_extension<const char*> cbe_ctor_str_lval1{cbe_ctor_str_lval0};
   CHECK_EQUAL( cbe_ctor_str_lval0.value(), cbe_ctor_str_lval1.value() )
   CHECK_EQUAL( cbe_ctor_str_lval0.has_value(), cbe_ctor_str_lval1.has_value() )

   //// constexpr binary_extension(binary_extension&&)
   binary_extension<const char> be_ctor_char_rval0{'c'};
   CHECK_EQUAL( be_ctor_char_rval0.has_value(), true )
   binary_extension<const char> be_ctor_char_rval1{move(be_ctor_char_rval0)};
   CHECK_EQUAL( be_ctor_char_rval0.has_value(), false )
   CHECK_ASSERT( "cannot get value of empty binary_extension", [&](){be_ctor_char_rval0.value();} )

   binary_extension<const char*> be_ctor_str_rval0{"abcd"};
   CHECK_EQUAL( be_ctor_str_rval0.has_value(), true )
   binary_extension<const char*> be_ctor_str_rval1{move(be_ctor_str_rval0)};
   CHECK_EQUAL( be_ctor_str_rval0.has_value(), false )
   CHECK_ASSERT( "cannot get value of empty binary_extension", [&](){be_ctor_str_rval0.value();} )

   // -------------------
   // ~binary_extension()
   static const binary_extension<const char> be_destruct_char{'c'};
   CHECK_EQUAL( be_destruct_char.has_value(), true )
   be_destruct_char.~binary_extension();
   CHECK_EQUAL( be_destruct_char.has_value(), false )

   static const binary_extension<const char*> be_destruct_str{"abcd"};
   CHECK_EQUAL( be_destruct_str.has_value(), true )
   be_destruct_str.~binary_extension();
   CHECK_EQUAL( be_destruct_str.has_value(), false )

   // ---------------------------------------
   // constexpr explicit operator bool()const
   CHECK_EQUAL( (binary_extension<const char>{}.operator bool()), false )
   CHECK_EQUAL( (binary_extension<const char>{0}.operator bool()), true )
   CHECK_EQUAL( (binary_extension<const char>{'c'}.operator bool()), true )

   CHECK_EQUAL( (binary_extension<const char*>{}.operator bool()), false )
   CHECK_EQUAL( (binary_extension<const char*>{nullptr}.operator bool()), true )
   CHECK_EQUAL( (binary_extension<const char*>{"abcd"}.operator bool()), true )

   // ---------------------
   // constexpr T& value()&
   binary_extension<char> be_char_value{'c'};
   CHECK_EQUAL( (be_char_value.value() == 'c'), true  )
   CHECK_EQUAL( (be_char_value.value() == 'd'), false )

   binary_extension<char*> be_str_value{const_cast<char*>("abcd")};
   CHECK_EQUAL( (be_str_value.value() == "abcd"), true  )
   CHECK_EQUAL( (be_str_value.value() == "efgh"), false )

   // --------------------------------
   // constexpr const T& value()const&
   static const binary_extension<const char> cbe_char_value{'c'};
   CHECK_EQUAL( (cbe_char_value.value() == 'c'), true  )
   CHECK_EQUAL( (cbe_char_value.value() == 'd'), false )

   static const binary_extension<const char*> cbe_str_value{"abcd"};
   CHECK_EQUAL( (cbe_str_value.value() == "abcd"), true  )
   CHECK_EQUAL( (cbe_str_value.value() == "efgh"), false )

   // -----------------------------------------------------------------------------
   // constexpr auto value_or(U&&) -> enable_if_t<is_convertible<U, T>::value, T&>&
   binary_extension<uint8_t> be_val_or{};
   uint8_t ui8_val = be_val_or.value_or(uint8_t{0x63});
   CHECK_EQUAL( ui8_val, 0x63 )

   // --------------------------
   // constexpr T&& value_or()&&
   CHECK_EQUAL( binary_extension<char>{'c'}.value_or(), 'c' )

   binary_extension<char*> be_val_or_rval{const_cast<char*>("abcd")};
   CHECK_EQUAL( be_val_or_rval.value_or(), "abcd" )

   // -------------------------------------
   // constexpr const T&& value_or()const&&
   CHECK_EQUAL( binary_extension<const char>{'c'}.value_or(), 'c' )

   static const binary_extension<const char*> cbe_val_or_rval{"abcd"};
   CHECK_EQUAL( cbe_val_or_rval.value_or(), "abcd" )

   // -----------------------
   // constexpr T value_or()&
   binary_extension<char> be_char_val_or{'c'};
   CHECK_EQUAL( be_char_val_or.value_or(), 'c' )

   binary_extension<char*> be_str_val_or{const_cast<char*>("abcd")};
   CHECK_EQUAL( be_str_val_or.value_or(), "abcd" )

   // ----------------------------
   // constexpr T value_or()const&
   static const binary_extension<const char> cbe_char_val_or{'c'};
   CHECK_EQUAL( cbe_char_val_or.value_or(), 'c' )

   static const binary_extension<const char*> cbe_str_val_or{"abcd"};
   CHECK_EQUAL( cbe_str_val_or.value_or(), "abcd" )

   // -------------------------
   // constexpr T* operator->()
   binary_extension<char> be_char_mem_op{'c'};
   CHECK_EQUAL( *be_char_mem_op.operator->() == 'c', true )
   CHECK_EQUAL( *be_char_mem_op.operator->() != 'd', true )

   binary_extension<char*> be_str_mem_op{const_cast<char*>("abcd")};
   CHECK_EQUAL( *be_str_mem_op.operator->() == "abcd", true )
   CHECK_EQUAL( *be_str_mem_op.operator->() != "efgh", true )

   // ------------------------------------
   // constexpr const T* operator->()const
   static const binary_extension<const char> cbe_char_mem_op{'c'};
   CHECK_EQUAL( *cbe_char_mem_op.operator->() == 'c', true )
   CHECK_EQUAL( *cbe_char_mem_op.operator->() != 'd', true )

   static const binary_extension<const char*> cbe_str_arrow_op{"abcd"};
   CHECK_EQUAL( *cbe_str_arrow_op.operator->() == "abcd", true )
   CHECK_EQUAL( *cbe_str_arrow_op.operator->() != "efgh", true )

   // -------------------------
   // constexpr T& operator*()&
   binary_extension<char> be_char_lval_dref_op{'c'};
   CHECK_EQUAL( be_char_lval_dref_op.operator*() == 'c', true )
   CHECK_EQUAL( be_char_lval_dref_op.operator*() != 'd', true )

   binary_extension<char*> be_str_lval_dref_op{const_cast<char*>("abcd")};
   CHECK_EQUAL( be_str_lval_dref_op.operator*() == "abcd", true )
   CHECK_EQUAL( be_str_lval_dref_op.operator*() != "efgh", true )

   // ------------------------------------
   // constexpr const T& operator*()const&
   static const binary_extension<const char> cbe_char_lval_dref_op{'c'};
   CHECK_EQUAL( cbe_char_lval_dref_op.operator*() == 'c', true )
   CHECK_EQUAL( cbe_char_lval_dref_op.operator*() != 'd', true )

   static const binary_extension<const char*> cbe_str_lval_dref_op{"abcd"};
   CHECK_EQUAL( cbe_str_lval_dref_op.operator*() == "abcd", true )
   CHECK_EQUAL( cbe_str_lval_dref_op.operator*() != "efgh", true )

   // ---------------------------
   // constexpr T&& operator*()&&
   binary_extension<char> be_char_rval_dref_op{'c'};
   CHECK_EQUAL( move(be_char_rval_dref_op.operator*()) == 'c', true )
   CHECK_EQUAL( move(be_char_rval_dref_op.operator*()) != 'd', true )

   binary_extension<char*> be_str_rval_dref_op{const_cast<char*>("abcd")};
   CHECK_EQUAL( move(be_str_rval_dref_op.operator*()) == "abcd", true )
   CHECK_EQUAL( move(be_str_rval_dref_op.operator*()) != "efgh", true )

   // --------------------------------------
   // constexpr const T&& operator*()const&&
   static const binary_extension<const char> cbe_char_rval_dref_op{'c'};
   CHECK_EQUAL( move(cbe_char_rval_dref_op.operator*()) == 'c', true )
   CHECK_EQUAL( move(cbe_char_rval_dref_op.operator*()) != 'd', true )

   static const binary_extension<const char*> cbe_str_rval_dref_op{"abcd"};
   CHECK_EQUAL( move(cbe_str_rval_dref_op.operator*()) == "abcd", true )
   CHECK_EQUAL( move(cbe_str_rval_dref_op.operator*()) != "efgh", true )

   // ----------------------------
   // T& emplace(Args&& ... args)&
   binary_extension<const char> be_char_emplace{'c'};
   be_char_emplace.emplace(move('d'));
   CHECK_EQUAL( be_char_emplace.value() == 'd', true )
   CHECK_EQUAL( be_char_emplace.value() != 'c', true )

   binary_extension<const char*> be_str_emplace{"abcd"};
   be_str_emplace.emplace(move("efgh"));
   CHECK_EQUAL( be_str_emplace.value() == "efgh", true )
   CHECK_EQUAL( be_str_emplace.value() != "abcd", true )

   // ------------
   // void reset()
   binary_extension<const char> be_char_reset{'c'};
   CHECK_EQUAL( be_char_reset.has_value(), true )
   be_char_reset.reset();
   CHECK_EQUAL( be_char_reset.has_value(), false )

   binary_extension<const char*> be_str_reset{"abcd"};
   CHECK_EQUAL( be_str_reset.has_value(), true )
   be_str_reset.reset();
   CHECK_EQUAL( be_str_reset.has_value(), false )
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   bool verbose = false;
   if( argc >= 2 && std::strcmp( argv[1], "-v" ) == 0 ) {
      verbose = true;
   }
   silence_output(!verbose);

   EOSIO_TEST(binary_extension_test);
   return has_failed();
}
