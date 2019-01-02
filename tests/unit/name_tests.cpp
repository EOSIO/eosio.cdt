#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

using namespace eosio;
using namespace eosio::native;

// Defined in `eosio.cdt/libraries/eosiolib/name.hpp`
EOSIO_TEST_BEGIN(name_type_constructors_test)
   eosio_assert( name{}.value == 0ULL, "name != 0ULL" );

   eosio_assert( name{0ULL}.value == 0ULL, "name != 0ULL" );
   eosio_assert( name{1ULL}.value == 1ULL, "name != 1ULL" );
   eosio_assert( name{18446744073709551615ULL}.value == 18446744073709551615ULL, "name != 18446744073709551615ULL" );

   eosio_assert( name{"1"}.value == 576460752303423488ULL, "name != 1" );
   eosio_assert( name{"5"}.value == 2882303761517117440ULL, "name != 5" );
   eosio_assert( name{"a"}.value == 3458764513820540928ULL, "name != a" );
   eosio_assert( name{"z"}.value == 17870283321406128128ULL, "name != z" );

   eosio_assert( name{"abc"}.value == 3589368903014285312ULL, "name != abc" );
   eosio_assert( name{"123"}.value == 614178399182651392ULL, "name != 123" );

   eosio_assert( name{".abc"}.value == 112167778219196416ULL, "name != .abc" );
   eosio_assert( name{"123."}.value == 614178399182651392ULL, "name != 123." );
   
   eosio_assert( name{"abc.123"}.value == 3589369488740450304ULL, "name != abc.123" );
   eosio_assert( name{"123.abc"}.value == 614181822271586304ULL, "name != 123.abc" );

   eosio_assert( name{"12345abcdefgj"}.value == 614251623682315983ULL, "name != 12345abcdefgj" );
   eosio_assert( name{"hijklmnopqrsj"}.value == 7754926748989239183ULL, "name != hijklmnopqrsj" );
   eosio_assert( name{"tuvwxyz.1234j"}.value == 14895601873741973071ULL, "name != tuvwxyz.1234j" );

   eosio_assert( name{"111111111111j"}.value == 595056260442243615ULL, "name != 111111111111j" );
   eosio_assert( name{"555555555555j"}.value == 2975281302211218015ULL, "name != 555555555555j" );
   eosio_assert( name{"aaaaaaaaaaaaj"}.value == 3570337562653461615ULL, "name != aaaaaaaaaaaaj" );
   eosio_assert( name{"zzzzzzzzzzzzj"}.value == 18446744073709551615ULL, "name != zzzzzzzzzzzzj" );

   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"-1"}.value;}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"0"}.value;}) );
   REQUIRE_ASSERT( "character is not in allowed character set for names", ([]() {name{"6"}.value;}) );
   REQUIRE_ASSERT( "thirteenth character in name cannot be a letter that comes after j", ([]() {name{"111111111111k"}.value;}) );
   REQUIRE_ASSERT( "thirteenth character in name cannot be a letter that comes after j", ([]() {name{"zzzzzzzzzzzzk"}.value;}) );
   REQUIRE_ASSERT( "the length must not be longer than 13 characters in an `eosio::name`", ([]() {name{"12345abcdefghj"}.value;}) );

   silence_output(false);
EOSIO_TEST_END

EOSIO_TEST_BEGIN(name_type_methods_test)
EOSIO_TEST_END

// // Intrinsics defined in `eos/libraries/chain/wasm_interface.cpp` that involve the `name` type
// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(set_resource_limits_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::set_resource_limits>([](name account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(get_resource_limits_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::get_resource_limits>([](name account, int64_t ram_bytes, int64_t net_weight, int64_t cpu_weight) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(set_proposed_producers_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::set_proposed_producers>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(is_privileged_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::is_privileged>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(set_privileged_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::set_privileged>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(get_active_producers_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::get_active_producers>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(check_permission_authorization_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::check_permission_authorization>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(get_permission_last_used_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::get_permission_last_used>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(get_account_creation_time_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::get_account_creation_time>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(get_permission_last_used_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::get_permission_last_used>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(require_authorization_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::require_authorization>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(has_authorization_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::has_authorization>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(require_recipient_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::require_recipient>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(is_account_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::is_account>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // Defined in `eos/libraries/chain/wasm_interface.cpp`
// EOSIO_TEST_BEGIN(send_deferred_intrinsic_test)
//    intrinsics::set_intrinsic<intrinsics::send_deferred>([](uint64_t a) {
      
//    });
// EOSIO_TEST_END

// // EOSIO_TEST_BEGIN(is_account_test)
// // intrinsics::set_intrinsic<intrinsics::is_account>([](uint64_t a) { 
// //       if (a == 3)
// //          return true;
// //       if (a == 4)
// //          return true;
// //       return false;
// //       });

// //    CHECK_ASSERT("is not an account", ([]() {
// //       eosio_assert(is_account(5), "is not an account");
// //       }));
// //    CHECK_EQUAL(is_account(3), true);
// //    CHECK_EQUAL(is_account(4), true);
// //    CHECK_EQUAL(is_account(6), true);
// //    REQUIRE_EQUAL(is_account(7), true);

// // EOSIO_TEST_END

int main(int argc, char** argv) {
   //EOSIO_TEST(name_type_test);
   // EOSIO_TEST(is_account_test);
   return has_failed();
}
