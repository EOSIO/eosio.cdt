/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosiolib/crypto.h>
#include <eosiolib/public_key.hpp>
#include <eosiolib/fixed_bytes.hpp>

namespace eosio {

   /**
    *  @defgroup cryptoapi Chain API
    *  @brief Defines API for calculating and checking hashes
    *  @ingroup contractdev
    */

   /**
    *  @defgroup cryptocppapi Chain C API
    *  @brief Defines type-safe C++ wrapers for calculating and checking hashes
    *  @ingroup chainapi
    *  @{
    */

   /**
    *  Tests if the SHA256 hash generated from data matches the provided digest.
    *  This method is optimized to a NO-OP when in fast evaluation mode.
    *  @brief Tests if the sha256 hash generated from data matches the provided digest.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - hash to compare to
    */
   void assert_sha256( const char* data, uint32_t length, const eosio::digest256& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_sha256( data, length, reinterpret_cast<const capi_checksum256*>(hash_data.data()) );
   }

   /**
    *  Tests if the SHA1 hash generated from data matches the provided digest.
    *  This method is optimized to a NO-OP when in fast evaluation mode.
    *  @brief Tests if the sha1 hash generated from data matches the provided digest.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - hash to compare to
    */
   void assert_sha1( const char* data, uint32_t length, const eosio::digest160& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_sha1( data, length, reinterpret_cast<const capi_checksum160*>(hash_data.data()) );
   }

   /**
    *  Tests if the SHA512 hash generated from data matches the provided digest.
    *  This method is optimized to a NO-OP when in fast evaluation mode.
    *  @brief Tests if the sha512 hash generated from data matches the provided digest.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - hash to compare to
    */
   void assert_sha512( const char* data, uint32_t length, const eosio::digest512& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_sha512( data, length, reinterpret_cast<const capi_checksum512*>(hash_data.data()) );
   }

   /**
    *  Tests if the RIPEMD160 hash generated from data matches the provided digest.
    *  @brief Tests if the ripemd160 hash generated from data matches the provided digest.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - hash to compare to
    */
   void assert_ripemd160( const char* data, uint32_t length, const eosio::digest160& hash ) {
      auto hash_data = hash.extract_as_byte_array();
      ::assert_ripemd160( data, length, reinterpret_cast<const capi_checksum160*>(hash_data.data()) );
   }

   /**
    *  Hashes `data` using SHA256.
    *  @brief Hashes `data` using SHA256.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::digest256 - Computed hash
    */
   eosio::digest256 sha256( const char* data, uint32_t length ) {
      capi_checksum256 hash;
      ::sha256( data, length, &hash );
      return {hash.hash};
   }

   /**
    *  Hashes `data` using SHA1.
    *  @brief Hashes `data` using SHA1.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::digest160 - Computed hash
    */
   eosio::digest160 sha1( const char* data, uint32_t length ) {
      capi_checksum160 hash;
      ::sha1( data, length, &hash );
      return {hash.hash};
   }

   /**
    *  Hashes `data` using SHA512.
    *  @brief Hashes `data` using SHA512.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::digest512 - Computed hash
    */
   eosio::digest512 sha512( const char* data, uint32_t length ) {
      capi_checksum512 hash;
      ::sha512( data, length, &hash );
      return {hash.hash};
   }

   /**
    *  Hashes `data` using RIPEMD160.
    *  @brief Hashes `data` using RIPEMD160.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::digest160 - Computed hash
    */
   eosio::digest160 ripemd160( const char* data, uint32_t length ) {
      capi_checksum160 hash;
      ::ripemd160( data, length, &hash );
      return {hash.hash};
   }

   /**
    *  Calculates the public key used for a given signature and hash used to create a message.
    *  @brief Calculates the public key used for a given signature and hash used to create a message.
    *
    *  @param digest - Hash used to create a message
    *  @param sig - Signature
    *  @param siglen - Signature length
    *  @param pub - Public key
    *  @param publen - Public key length
    *  @return eosio::public_key - Recovered public key
    */
   eosio::public_key recover_key( const eosio::digest256& digest, const char* sig, size_t siglen ) {
      auto digest_data = digest.extract_as_byte_array();
      char pubkey_data[38];
      size_t pubkey_size = ::recover_key( reinterpret_cast<const capi_checksum256*>(digest_data.data()), sig, siglen, pubkey_data, sizeof(pubkey_data) );
      eosio::datastream<char*> ds( pubkey_data, pubkey_size );
      eosio::public_key pubkey;
      ds >> pubkey;
      return pubkey;
   }

   /**
    *  Tests a given public key with the generated key from digest and the signature.
    *  @brief Tests a given public key with the generated key from digest and the signature.
    *
    *  @param digest - What the key will be generated from
    *  @param sig - Signature
    *  @param siglen - Signature length
    *  @param pubkey - Public key
    */
   void assert_recover_key( const eosio::digest256& digest, const char* sig, size_t siglen, const eosio::public_key& pubkey ) {
      auto digest_data = digest.extract_as_byte_array();
      char pubkey_data[38];
      eosio::datastream<char*> ds( pubkey_data, sizeof(pubkey_data) );
      auto begin = ds.pos();
      ds << pubkey;
      ::assert_recover_key( reinterpret_cast<const capi_checksum256*>(digest_data.data()), sig, siglen, begin, (ds.pos() - begin) );
   }

   /// }@cryptocppapi
}
