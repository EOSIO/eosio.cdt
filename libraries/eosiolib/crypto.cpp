/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */

#include "crypto.hpp"
#include "datastream.hpp"

namespace eosio {

   void assert_sha256( const char* data, uint32_t length, const eosio::checksum256& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_sha256( data, length, reinterpret_cast<const ::capi_checksum256*>(hash_data.data()) );
   }

   void assert_sha1( const char* data, uint32_t length, const eosio::checksum160& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_sha1( data, length, reinterpret_cast<const ::capi_checksum160*>(hash_data.data()) );
   }

   void assert_sha512( const char* data, uint32_t length, const eosio::checksum512& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_sha512( data, length, reinterpret_cast<const ::capi_checksum512*>(hash_data.data()) );
   }

   void assert_ripemd160( const char* data, uint32_t length, const eosio::checksum160& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_ripemd160( data, length, reinterpret_cast<const ::capi_checksum160*>(hash_data.data()) );
   }

   eosio::checksum256 sha256( const char* data, uint32_t length ) {
      ::capi_checksum256 hash;
      ::sha256( data, length, &hash );
      return {hash.hash};
   }

   eosio::checksum160 sha1( const char* data, uint32_t length ) {
      ::capi_checksum160 hash;
      ::sha1( data, length, &hash );
      return {hash.hash};
   }

   eosio::checksum512 sha512( const char* data, uint32_t length ) {
      ::capi_checksum512 hash;
      ::sha512( data, length, &hash );
      return {hash.hash};
   }

   eosio::checksum160 ripemd160( const char* data, uint32_t length ) {
      ::capi_checksum160 hash;
      ::ripemd160( data, length, &hash );
      return {hash.hash};
   }

   eosio::public_key recover_key( const eosio::checksum256& digest, const eosio::signature& sig ) {
      auto digest_data = digest.extract_as_byte_array();

      char sig_data[70];
      eosio::datastream<char*> sig_ds( sig_data, sizeof(sig_data) );
      auto sig_begin = sig_ds.pos();
      sig_ds << sig;

      char pubkey_data[38];
      size_t pubkey_size = ::recover_key( reinterpret_cast<const capi_checksum256*>(digest_data.data()),
                                          sig_begin, (sig_ds.pos() - sig_begin),
                                          pubkey_data, sizeof(pubkey_data) );
      eosio::datastream<char*> pubkey_ds( pubkey_data, pubkey_size );
      eosio::public_key pubkey;
      pubkey_ds >> pubkey;
      return pubkey;
   }

   void assert_recover_key( const eosio::checksum256& digest, const eosio::signature& sig, const eosio::public_key& pubkey ) {
      auto digest_data = digest.extract_as_byte_array();

      char sig_data[70];
      eosio::datastream<char*> sig_ds( sig_data, sizeof(sig_data) );
      auto sig_begin = sig_ds.pos();
      sig_ds << sig;

      char pubkey_data[38];
      eosio::datastream<char*> pubkey_ds( pubkey_data, sizeof(pubkey_data) );
      auto pubkey_begin = pubkey_ds.pos();
      pubkey_ds << pubkey;

      ::assert_recover_key( reinterpret_cast<const capi_checksum256*>(digest_data.data()),
                            sig_begin, (sig_ds.pos() - sig_begin),
                            pubkey_begin, (pubkey_ds.pos() - pubkey_begin) );
   }

}
