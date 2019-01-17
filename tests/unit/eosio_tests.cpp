#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

using namespace eosio;
using namespace eosio::native;

// Defined in `eosio.cdt/libraries/eosiolib/eosio.hpp`
EOSIO_TEST_BEGIN(eosio_test)
   silence_output(false);

   

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(eosio_test);
   return has_failed();
}
