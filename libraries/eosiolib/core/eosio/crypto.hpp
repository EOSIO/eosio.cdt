/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include "fixed_bytes.hpp"
#include "varint.hpp"
#include "serialize.hpp"

#include <array>

namespace eosio {

   /**
   *  @defgroup publickeytype Public Key Type
   *  @ingroup core
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
   };

   /**
    *  Serialize an eosio::public_key into a stream
    *
    *  @brief Serialize an eosio::public_key
    *  @param ds - The stream to write
    *  @param pubkey - The value to serialize
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream>
   inline DataStream& operator<<(DataStream& ds, const eosio::public_key& pubkey) {
      ds << pubkey.type;
      ds.write( pubkey.data.data(), pubkey.data.size() );
      return ds;
   }

   /**
    *  Deserialize an eosio::public_key from a stream
    *
    *  @brief Deserialize an eosio::public_key
    *  @param ds - The stream to read
    *  @param pubkey - The destination for deserialized value
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream>
   inline DataStream& operator>>(DataStream& ds, eosio::public_key& pubkey) {
      ds >> pubkey.type;
      ds.read( pubkey.data.data(), pubkey.data.size() );
      return ds;
   }

   /// @} publickeytype

   /**
   *  @defgroup signature Public Key Type
   *  @ingroup core
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
   };

   /**
    *  Serialize an eosio::signature into a stream
    *
    *  @brief Serialize an eosio::signature
    *  @param ds - The stream to write
    *  @param sig - The value to serialize
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream>
   inline DataStream& operator<<(DataStream& ds, const eosio::signature& sig) {
      ds << sig.type;
      ds.write( sig.data.data(), sig.data.size() );
      return ds;
   }

   /**
    *  Deserialize an eosio::signature from a stream
    *
    *  @brief Deserialize an eosio::signature
    *  @param ds - The stream to read
    *  @param sig - The destination for deserialized value
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream>
   inline DataStream& operator>>(DataStream& ds, eosio::signature& sig) {
      ds >> sig.type;
      ds.read( sig.data.data(), sig.data.size() );
      return ds;
   }

   /// @} signaturetype

   /**
    *  @defgroup crypto Chain API
    *  @ingroup core 
    *  @brief Defines API for calculating and checking hashes
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

   /// }@cryptoapi
}
