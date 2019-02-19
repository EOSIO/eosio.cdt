/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/tester.hpp>
#include <eosio/crypto.hpp>

using eosio::public_key;
using eosio::signature;

// Definitions in `eosio.cdt/libraries/eosio/crypto.hpp`
EOSIO_TEST_BEGIN(public_key_type_test)
   silence_output(true);

   // -----------------------------------------------------
   // bool operator==(const public_key&, const public_key&)
   CHECK_EQUAL( (public_key{0, std::array<char, 33>{}}  == public_key{0, std::array<char, 33>{}}), true  )
   CHECK_EQUAL( (public_key{0, std::array<char, 33>{1}} == public_key{0, std::array<char, 33>{}}), false )

   // -----------------------------------------------------
   // bool operator!=(const public_key&, const public_key&)
   CHECK_EQUAL( (public_key{0, std::array<char, 33>{}}  != public_key{0, std::array<char, 33>{}}), false )
   CHECK_EQUAL( (public_key{0, std::array<char, 33>{1}} != public_key{0, std::array<char, 33>{}}), true  )
   
   silence_output(false);
EOSIO_TEST_END

// Definitions in `eosio.cdt/libraries/eosio/crypto.hpp`
EOSIO_TEST_BEGIN(signature_type_test)
   silence_output(true);

   // ---------------------------------------------------
   // bool operator==(const signature&, const signature&)
   CHECK_EQUAL( (signature{0, std::array<char, 65>{}}  == signature{0, std::array<char, 65>{}}), true  )
   CHECK_EQUAL( (signature{0, std::array<char, 65>{1}} == signature{0, std::array<char, 65>{}}), false )

   // ---------------------------------------------------
   // bool operator!=(const signature&, const signature&)
   CHECK_EQUAL( (signature{0, std::array<char, 65>{1}} != signature{0, std::array<char, 65>{}}), true  )
   CHECK_EQUAL( (signature{0, std::array<char, 65>{}}  != signature{0, std::array<char, 65>{}}), false )
   
   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(public_key_type_test)
   EOSIO_TEST(signature_type_test)
   return has_failed();
}
