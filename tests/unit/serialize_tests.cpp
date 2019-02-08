/**
 *  @file
 *  @copyright defined in eosio.cdt/LICENSE.txt
 */

#include <eosio/native/tester.hpp>
#include <eosiolib/datastream.hpp>
#include <eosiolib/serialize.hpp>

using eosio::datastream;

// Definitions in `eosio.cdt/libraries/eosiolib/serialize.hpp`
EOSIO_TEST_BEGIN(serialize_test)
   silence_output(true);

   static constexpr uint16_t buffer_size{256};
   char ds_expected_buffer[buffer_size]{}; // Buffer for the datastream to point to
   char ds_buffer[buffer_size]{}; // Buffer for the datastream to point to

   datastream<char*> ds_expected{ds_expected_buffer, buffer_size};
   datastream<char*> ds{ds_buffer, buffer_size}; 

struct Base {
   
};
// EOSLIB_SERIALIZE( (BASE), () () () )

struct Derived : public Base {
   
};
// EOSLIB_SERIALIZE( (DERIVED), (BASE), () () () )
   
   silence_output(false);
EOSIO_TEST_END

int main(int argc, char* argv[]) {
   EOSIO_TEST(serialize_test)
   return has_failed();
}
