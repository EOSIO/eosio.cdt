/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosiolib/crypto.h>
#include <eosiolib/fixed_bytes.hpp>
#include <eosiolib/varint.hpp>
#include <eosiolib/serialize.hpp>

#include <array>

namespace eosio {

   /**
   *  @defgroup publickeytype Public Key Type
   *  @ingroup types
   *  @brief Specifies public key type
   *
   *  @{
   */

   /**
    * EOSIO Public Key
    * @brief EOSIO Public Key
    */
   struct public_key {
      /**
       * Type of the public key, could be either K1 or R1
       * @brief Type of the public key
       */
      unsigned_int        type;

      /**
       * Bytes of the public key
       *
       * @brief Bytes of the public key
       */
      std::array<char,33> data;

      friend bool operator == ( const public_key& a, const public_key& b ) {
        return std::tie(a.type,a.data) == std::tie(b.type,b.data);
      }
      friend bool operator != ( const public_key& a, const public_key& b ) {
        return std::tie(a.type,a.data) != std::tie(b.type,b.data);
      }
      EOSLIB_SERIALIZE( public_key, (type)(data) )
   };

   /// @} publickeytype

   /**
   *  @defgroup signaturetype Public Key Type
   *  @ingroup types
   *  @brief Specifies signature type
   *
   *  @{
   */

   /**
    * EOSIO Signature
    * @brief EOSIO Signature
    */
   struct signature {
      /**
       * Type of the signature, could be either K1 or R1
       * @brief Type of the signature
       */
      unsigned_int        type;

      /**
       * Bytes of the signature
       *
       * @brief Bytes of the signature
       */
      std::array<char,65> data;

      friend bool operator == ( const signature& a, const signature& b ) {
        return std::tie(a.type,a.data) == std::tie(b.type,b.data);
      }
      friend bool operator != ( const signature& a, const signature& b ) {
        return std::tie(a.type,a.data) != std::tie(b.type,b.data);
      }
      EOSLIB_SERIALIZE( signature, (type)(data) )
   };

   /// @} signaturetype

   /**
    *  @defgroup cryptoapi Chain API
    *  @brief Defines API for calculating and checking hashes
    *  @ingroup contractdev
    *  @{
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
    *  @param hash - digest to compare to
    */
   void assert_sha256( const char* data, uint32_t length, const eosio::checksum256& hash );

   /**
    *  Tests if the SHA1 hash generated from data matches the provided digest.
    *  This method is optimized to a NO-OP when in fast evaluation mode.
    *  @brief Tests if the sha1 hash generated from data matches the provided digest.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    */
   void assert_sha1( const char* data, uint32_t length, const eosio::checksum160& hash );

   /**
    *  Tests if the SHA512 hash generated from data matches the provided digest.
    *  This method is optimized to a NO-OP when in fast evaluation mode.
    *  @brief Tests if the sha512 hash generated from data matches the provided digest.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    */
   void assert_sha512( const char* data, uint32_t length, const eosio::checksum512& hash );

   /**
    *  Tests if the RIPEMD160 hash generated from data matches the provided digest.
    *  @brief Tests if the ripemd160 hash generated from data matches the provided digest.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @param hash - digest to compare to
    */
   void assert_ripemd160( const char* data, uint32_t length, const eosio::checksum160& hash );

   /**
    *  Hashes `data` using SHA256.
    *  @brief Hashes `data` using SHA256.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum256 - Computed digest
    */
   eosio::checksum256 sha256( const char* data, uint32_t length );

   /**
    *  Hashes `data` using SHA1.
    *  @brief Hashes `data` using SHA1.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum160 - Computed digest
    */
   eosio::checksum160 sha1( const char* data, uint32_t length );

   /**
    *  Hashes `data` using SHA512.
    *  @brief Hashes `data` using SHA512.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum512 - Computed digest
    */
   eosio::checksum512 sha512( const char* data, uint32_t length );

   /**
    *  Hashes `data` using RIPEMD160.
    *  @brief Hashes `data` using RIPEMD160.
    *
    *  @param data - Data you want to hash
    *  @param length - Data length
    *  @return eosio::checksum160 - Computed digest
    */
   eosio::checksum160 ripemd160( const char* data, uint32_t length );

   /**
    *  Calculates the public key used for a given signature on a given digest.
    *  @brief Calculates the public key used for a given signature on a given digest.
    *
    *  @param digest - Digest of the message that was signed
    *  @param sig - Signature
    *  @return eosio::public_key - Recovered public key
    */
   eosio::public_key recover_key( const eosio::checksum256& digest, const eosio::signature& sig );

   /**
    *  Tests a given public key with the recovered public key from digest and signature.
    *  @brief Tests a given public key with the recovered public key from digest and signature.
    *
    *  @param digest - Digest of the message that was signed
    *  @param sig - Signature
    *  @param pubkey - Public key
    */
   void assert_recover_key( const eosio::checksum256& digest, const eosio::signature& sig, const eosio::public_key& pubkey );

   /// }@cryptocppapi
   /// }@cryptoapi
}
