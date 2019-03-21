#include <eosio/eosio.hpp>
#include <eosio/tester.hpp>

using namespace eosio::native;

EOSIO_TEST_BEGIN(print_test)
   silence_output(false);
   CHECK_PRINT("27", [](){ eosio::print((uint8_t)27); });
   CHECK_PRINT("34", [](){ eosio::print((int)34); });
   CHECK_PRINT([](std::string s){return s[0] == 'a';},  [](){ eosio::print((char)'a'); });
   CHECK_PRINT([](std::string s){return s[0] == 'b';},  [](){ eosio::print((int8_t)'b'); });
   CHECK_PRINT("202", [](){ eosio::print((unsigned int)202); });
   CHECK_PRINT("-202", [](){ eosio::print((int)-202); });
   CHECK_PRINT("707", [](){ eosio::print((unsigned long)707); });
   CHECK_PRINT("-707", [](){ eosio::print((long)-707); });
   CHECK_PRINT("909", [](){ eosio::print((unsigned long long)909); });
   CHECK_PRINT("-909", [](){ eosio::print((long long)-909); });
   CHECK_PRINT("404", [](){ eosio::print((uint32_t)404); });
   CHECK_PRINT("-404", [](){ eosio::print((int32_t)-404); });
   CHECK_PRINT("404000000", [](){ eosio::print((uint64_t)404000000); });
   CHECK_PRINT("-404000000", [](){ eosio::print((int64_t)-404000000); });
   CHECK_PRINT("0x0066000000000000", [](){ eosio::print((uint128_t)102); });
   CHECK_PRINT("0xffffff9affffffffffffffffffffffff", [](){ eosio::print((int128_t)-102); });
   silence_output(false);
EOSIO_TEST_END

int main(int argc, char** argv) {
   EOSIO_TEST(print_test);
   return has_failed();
}
