#include <eosiolib/eosio.hpp>
#include <eosio/native/tester.hpp>

#include <eosiolib/crypto.hpp>

using eosio::public_key;
using eosio::signature;

using namespace eosio::native;

// Defined in `eosio.cdt/libraries/eosiolib/crypto.hpp`
EOSIO_TEST_BEGIN(public_key_type_test)
   silence_output(false);

   // bool operator==(const public_key&, const public_key&)
   REQUIRE_EQUAL( (public_key{0, std::array<char,33>{}}  == public_key{0, std::array<char,33>{}}), true )
   REQUIRE_EQUAL( (public_key{0, std::array<char,33>{1}} == public_key{0, std::array<char,33>{}}), false )

   // bool operator!=(const public_key&, const public_key&)
   REQUIRE_EQUAL( (public_key{0, std::array<char,33>{}}  != public_key{0, std::array<char,33>{}}), false )
   REQUIRE_EQUAL( (public_key{0, std::array<char,33>{1}} != public_key{0, std::array<char,33>{}}), true )
   
   silence_output(false);
EOSIO_TEST_END

// Defined in `eosio.cdt/libraries/eosiolib/crypto.hpp`
EOSIO_TEST_BEGIN(signature_type_test)
   silence_output(false);

   // bool operator==(const signature&, const signature&)
   REQUIRE_EQUAL( (signature{0, std::array<char,65>{}}  == signature{0, std::array<char,65>{}}), true )
   REQUIRE_EQUAL( (signature{0, std::array<char,65>{1}} == signature{0, std::array<char,65>{}}), false )

   // bool operator!=(const signature&, const signature&)
   REQUIRE_EQUAL( (signature{0, std::array<char,65>{}}  != signature{0, std::array<char,65>{}}), false )
   REQUIRE_EQUAL( (signature{0, std::array<char,65>{1}} != signature{0, std::array<char,65>{}}), true )
   
   silence_output(false);
EOSIO_TEST_END

// Currently, these intrinsics are not supported
// Defined in `eosio.cdt/libraries/eosiolib/crypto.hpp`
// EOSIO_TEST_BEGIN(crypto_test)
//    silence_output(false);

// void assert_sha256( const char*, uint32_t, const eosio::checksum256&)

// void assert_sha1(const char*, uint32_t, const eosio::checksum160&)

// void assert_sha512(const char*, uint32_t, const eosio::checksum512&)

// void assert_ripemd160(const char*, uint32_t, const eosio::checksum160&)

// eosio::checksum256 sha256(const char*, uint32_t)

// eosio::checksum160 sha1(const char*, uint32_t)

// eosio::checksum512 sha512(const char*, uint32_t)

// eosio::checksum160 ripemd160(const char*, uint32_t)

// eosio::public_key recover_key(const eosio::checksum256&, const eosio::signature&)

// void assert_recover_key(const eosio::checksum256&, const eosio::signature&, const eosio::public_key&)
   
//    silence_output(false);
// EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(public_key_type_test)
   EOSIO_TEST(signature_type_test)
   // EOSIO_TEST(crypto_test)
   return has_failed();
}
