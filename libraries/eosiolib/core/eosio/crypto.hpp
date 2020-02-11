/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include_next <eosio/crypto.hpp>
#include <eosio/fixed_bytes.hpp>

namespace eosio {

   /**
    *  @defgroup crypto Crypto
    *  @ingroup core
    *  @brief Defines API for calculating and checking hashes
    */

   /**
    *  Tests if the SHA256 hash generated from data matches the provided digest.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    *  @note This method is optimized to a NO-OP when in fast evaluation mode.
    */
   void assert_sha256( const char* data, uint32_t length, const eosio::checksum256& hash );

   /**
    *  Tests if the SHA1 hash generated from data matches the provided digest.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    *  @note This method is optimized to a NO-OP when in fast evaluation mode.
    */
   void assert_sha1( const char* data, uint32_t length, const eosio::checksum160& hash );

   /**
    *  Tests if the SHA512 hash generated from data matches the provided digest.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    *  @note This method is optimized to a NO-OP when in fast evaluation mode.
    */
   void assert_sha512( const char* data, uint32_t length, const eosio::checksum512& hash );

   /**
    *  Tests if the RIPEMD160 hash generated from data matches the provided digest.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    */
   void assert_ripemd160( const char* data, uint32_t length, const eosio::checksum160& hash );

   /**
    *  Hashes `data` using SHA256.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum256 - Computed digest
    */
   eosio::checksum256 sha256( const char* data, uint32_t length );

   /**
    *  Hashes `data` using SHA1.
    *
    *  @ingroup crypto
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum160 - Computed digest
    */
   eosio::checksum160 sha1( const char* data, uint32_t length );

   /**
    *  Hashes `data` using SHA512.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum512 - Computed digest
    */
   eosio::checksum512 sha512( const char* data, uint32_t length );

   /**
    *  Hashes `data` using RIPEMD160.
    *
    *  @ingroup crypto
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum160 - Computed digest
    */
   eosio::checksum160 ripemd160( const char* data, uint32_t length );

   /**
    *  Calculates the public key used for a given signature on a given digest.
    *
    *  @ingroup crypto
    *  @param digest - Digest of the message that was signed
    *  @param sig - Signature
    *  @return eosio::public_key - Recovered public key
    */
   eosio::public_key recover_key( const eosio::checksum256& digest, const eosio::signature& sig );

   /**
    *  Tests a given public key with the recovered public key from digest and signature.
    *
    *  @ingroup crypto
    *  @param digest - Digest of the message that was signed
    *  @param sig - Signature
    *  @param pubkey - Public key
    */
   void assert_recover_key( const eosio::checksum256& digest, const eosio::signature& sig, const eosio::public_key& pubkey );
}
