#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

using namespace eosio::native;

EOSIO_TEST_BEGIN(name_test)
   //silence_output(true);
   eosio_assert( eosio::name{"azaa34"}.value == "azaa34"_n.value, "eosio::name != azaa34" );
   eosio_assert( eosio::name{0}.value == 0, "eosio::name != 0" );
   eosio_assert( eosio::name{"aa11"}.value == "aa11"_n.value, "eosio::name != aa11" );
   eosio_assert( eosio::name{"z11"}.value == "z11"_n.value, "eosio::name != z11" );
   
   auto testa = []() {
      eosio_assert( eosio::name{"bb"}.value == "aa"_n.value, "bb != aa" );
   };
   REQUIRE_ASSERT("bb != aa", testa);
   REQUIRE_ASSERT("character is not in allowed character set for names",
         ([]() {
            eosio::name{"!"}.value;
         }));
   silence_output(false);
EOSIO_TEST_END

EOSIO_TEST_BEGIN(is_account_test)
   intrinsics::set_intrinsic<intrinsics::is_account>([](uint64_t a) { 
         if (a == 3)
            return true;
         if (a == 4)
            return true;
         return false;
         });

   CHECK_ASSERT("is not an account", ([]() {
      eosio_assert(is_account(5), "is not an account");
      }));
   CHECK_EQUAL(is_account(3), true);
   CHECK_EQUAL(is_account(4), true);
   CHECK_EQUAL(is_account(6), true);
   REQUIRE_EQUAL(is_account(7), true);

EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(name_test);
   EOSIO_TEST(is_account_test);
   return has_failed();
}
