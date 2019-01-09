#include <eosiolib/eosio.hpp>
#include <eosiolib/string.hpp>
#include <eosio/native/tester.hpp>
#include <string>

using namespace eosio::native;

EOSIO_TEST_BEGIN(rope_test)
   silence_output(false);
   eosio::rope r("test string 0");
   r += ", test string 1";
   r += ", test string 2";
   r += ", test string 3";
   r += ", test string 4";
   r += ", test string 5";
   r += ", test string 6";
   r += ", test string 7";
   
   std::string s("test string 0");
   s += ", test string 1";
   s += ", test string 2";
   s += ", test string 3";
   s += ", test string 4";
   s += ", test string 5";
   s += ", test string 6";
   s += ", test string 7";

   eosio::rope r2 = r + eosio::rope("lhs") + eosio::rope("rhs") + "some string";
   std::string s2 = s + std::string("lhs") + std::string("rhs") + "some string";

   r2 += eosio::rope("rvalue +=");
   s2 += std::string("rvalue +=");

   r2 = r2 + r2;   
   s2 = s2 + s2;

   r2 += "the end";
   s2 += "the end";
   
   eosio::rope r3(r2);
   std::string s3(s2);

   REQUIRE_EQUAL(s.compare(std::string(r.c_str())), 0);
   REQUIRE_EQUAL(s2.compare(std::string(r2.c_str())), 0);
   REQUIRE_EQUAL(s3.compare(std::string(r3.c_str())), 0);

   REQUIRE_PRINT( "test string 0, test string 1, test string 2, test string 3, test string 4, test string 5, test string 6, test string 7", [&](){eosio::print(r);} );
   REQUIRE_PRINT( "test string 0, test string 1, test string 2, test string 3, test string 4, test string 5, test string 6, test string 7lhsrhssome stringrvalue +=test string 0, test string 1, test string 2, test string 3, test string 4, test string 5, test string 6, test string 7lhsrhssome stringrvalue +=the end", [&](){eosio::print(r2);} );
   REQUIRE_PRINT( "test string 0, test string 1, test string 2, test string 3, test string 4, test string 5, test string 6, test string 7lhsrhssome stringrvalue +=test string 0, test string 1, test string 2, test string 3, test string 4, test string 5, test string 6, test string 7lhsrhssome stringrvalue +=the end", [&](){eosio::print(r3);} );

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(rope_test);
   return has_failed();
}
