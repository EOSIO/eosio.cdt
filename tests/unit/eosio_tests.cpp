/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/native/tester.hpp>
#include <eosiolib/eosio.hpp>

namespace using_no_macros {
class [[eosio::contract]] con {

   struct [[eosio::table]] tab {};
    
   [[eosio::action]] void act() {}
};
}

namespace using_macros {
CONTRACT con {

   TABLE tab {};

   ACTION act() {}
};
}

// Definitions in `eosio.cdt/libraries/eosiolib/eosio.hpp`
EOSIO_TEST_BEGIN(eosio_test)
   silence_output(false);

// REQUIRE_EQUAL( typeid(using_no_macros::con).name() == typeid(using_macros::con).name(), true )

   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(eosio_test);
   return has_failed();
}
