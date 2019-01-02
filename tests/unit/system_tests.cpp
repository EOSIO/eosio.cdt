#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

using namespace eosio::native;

EOSIO_TEST_BEGIN(system_test)
   silence_output(true);
   auto test_check1 = [](bool b) { eosio::check(b, "asserted"); }; 
   test_check1(true);
   CHECK_ASSERT("asserted", test_check1, false);

   auto test_check2 = [](bool b) { std::string s="asserted"; eosio::check(b, s); }; 
   test_check2(true);
   REQUIRE_ASSERT("asserted", test_check2, false);

   auto test_check3 = [](bool b) { eosio::check(b, "asserted with more text", 8); }; 
   test_check3(true);
   REQUIRE_ASSERT("asserted", test_check3, false);

   auto test_check4 = [](bool b) { std::string s="asserted with more text"; eosio::check(b, s, 8); }; 
   test_check4(true);
   REQUIRE_ASSERT("asserted", test_check4, false);

   auto test_check5 = [](bool b) { eosio::check(b, 13); }; 
   test_check5(true);
   REQUIRE_ASSERT("13", test_check5, false);
   silence_output(false);
EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(system_test);
   return has_failed();
}
