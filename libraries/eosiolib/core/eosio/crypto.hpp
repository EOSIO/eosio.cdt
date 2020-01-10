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
    *  @defgroup public_key Public Key Type
    *  @ingroup core
    *  @ingroup types
    *  @brief Specifies public key type
    */

   /**
    *  EOSIO ECC public key data
    *
    *  Fixed size representation of either a K1 or R1 compressed public key

    *  @ingroup public_key
    */
   using ecc_public_key = std::array<char, 33>;

   /**
    *  EOSIO WebAuthN public key
    *
    *  @ingroup public_key
    */
   struct webauthn_public_key {
      /**
       * Enumeration of the various results of a Test of User Presence
       * @see https://w3c.github.io/webauthn/#test-of-user-presence
       */
      enum class user_presence_t : uint8_t {
         USER_PRESENCE_NONE,
         USER_PRESENCE_PRESENT,
         USER_PRESENCE_VERIFIED
      };

      /**
       * The ECC key material
       */
      ecc_public_key     key;

      /**
       * expected result of the test of user presence for a valid signature
       * @see https://w3c.github.io/webauthn/#test-of-user-presence
       */
      user_presence_t    user_presence;

      /**
       * the Relying Party Identifier for WebAuthN
       * @see https://w3c.github.io/webauthn/#relying-party-identifier
       */
      std::string        rpid;

      /// @cond OPERATORS

      friend bool operator == ( const webauthn_public_key& a, const webauthn_public_key& b ) {
         return std::tie(a.key,a.user_presence,a.rpid) == std::tie(b.key,b.user_presence,b.rpid);
      }
      friend bool operator != ( const webauthn_public_key& a, const webauthn_public_key& b ) {
         return std::tie(a.key,a.user_presence,a.rpid) != std::tie(b.key,b.user_presence,b.rpid);
      }
      friend bool operator < ( const webauthn_public_key& a, const webauthn_public_key& b ) {
         return std::tie(a.key,a.user_presence,a.rpid) < std::tie(b.key,b.user_presence,b.rpid);
      }
      friend bool operator <= ( const webauthn_public_key& a, const webauthn_public_key& b ) {
         return std::tie(a.key,a.user_presence,a.rpid) <= std::tie(b.key,b.user_presence,b.rpid);
      }
      friend bool operator > ( const webauthn_public_key& a, const webauthn_public_key& b ) {
         return std::tie(a.key,a.user_presence,a.rpid) > std::tie(b.key,b.user_presence,b.rpid);
      }
      friend bool operator >= ( const webauthn_public_key& a, const webauthn_public_key& b ) {
         return std::tie(a.key,a.user_presence,a.rpid) >= std::tie(b.key,b.user_presence,b.rpid);
      }

      /// @cond
   };

   /**
    *  EOSIO Public Key
    *
    *  A public key is a variant of
    *   0 : a ECC K1 public key
    *   1 : a ECC R1 public key
    *   2 : a WebAuthN public key (requires the host chain to activate the WEBAUTHN_KEY consensus upgrade)
    *
    *  @ingroup public_key
    */
   using public_key = std::variant<ecc_public_key, ecc_public_key, webauthn_public_key>;


   /// @cond IMPLEMENTATIONS

   /**
    *  Serialize an eosio::webauthn_public_key into a stream
    *
    *  @ingroup public_key
    *  @param ds - The stream to write
    *  @param pubkey - The value to serialize
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream>
   inline DataStream& operator<<(DataStream& ds, const eosio::webauthn_public_key& pubkey) {
      ds << pubkey.key << pubkey.user_presence << pubkey.rpid;
      return ds;
   }

   /**
    *  Deserialize an eosio::webauthn_public_key from a stream
    *
    *  @ingroup public_key
    *  @param ds - The stream to read
    *  @param pubkey - The destination for deserialized value
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream>
   inline DataStream& operator>>(DataStream& ds, eosio::webauthn_public_key& pubkey) {
      ds >> pubkey.key >> pubkey.user_presence >> pubkey.rpid;
      return ds;
   }

   /// @endcond

   /**
    *  @defgroup signature Signature
    *  @ingroup core
    *  @ingroup types
    *  @brief Specifies signature type
    */

   /**
    *  EOSIO ECC signature data
    *
    *  Fixed size representation of either a K1 or R1 ECC compact signature

    *  @ingroup signature
    */
   using ecc_signature = std::array<char, 65>;

   /**
    *  EOSIO WebAuthN signature
    *
    *  @ingroup signature
    */
   struct webauthn_signature {
      /**
       * The ECC signature data
       */
      ecc_signature                     compact_signature;

      /**
       * The Encoded Authenticator Data returned from WebAuthN ceremony
       * @see https://w3c.github.io/webauthn/#sctn-authenticator-data
       */
      std::vector<uint8_t>              auth_data;

      /**
       * the JSON encoded Collected Client Data from a WebAuthN ceremony
       * @see https://w3c.github.io/webauthn/#dictdef-collectedclientdata
       */
      std::string                       client_json;

      /// @cond OPERATORS

      friend bool operator == ( const webauthn_signature& a, const webauthn_signature& b ) {
         return std::tie(a.compact_signature,a.auth_data,a.client_json) == std::tie(b.compact_signature,b.auth_data,b.client_json);
      }
      friend bool operator != ( const webauthn_signature& a, const webauthn_signature& b ) {
         return std::tie(a.compact_signature,a.auth_data,a.client_json) != std::tie(b.compact_signature,b.auth_data,b.client_json);
      }

      /// @cond
   };

   /**
    *  EOSIO Signature
    *
    *  A signature is a variant of
    *   0 : a ECC K1 signature
    *   1 : a ECC R1 signatre
    *   2 : a WebAuthN signature (requires the host chain to activate the WEBAUTHN_KEY consensus upgrade)
    *
    *  @ingroup signature
    */
   using signature = std::variant<ecc_signature, ecc_signature, webauthn_signature>;

   /// @cond IMPLEMENTATIONS

   /**
    *  Serialize an eosio::webauthn_signature into a stream
    *
    *  @param ds - The stream to write
    *  @param sig - The value to serialize
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream>
   inline DataStream& operator<<(DataStream& ds, const eosio::webauthn_signature& sig) {
      ds << sig.compact_signature << sig.auth_data << sig.client_json;
      return ds;
   }

   /**
    *  Deserialize an eosio::webauthn_signature from a stream
    *
    *  @param ds - The stream to read
    *  @param sig - The destination for deserialized value
    *  @tparam DataStream - Type of datastream buffer
    *  @return DataStream& - Reference to the datastream
    */
   template<typename DataStream>
   inline DataStream& operator>>(DataStream& ds, eosio::webauthn_signature& sig) {
      ds >> sig.compact_signature >> sig.auth_data >> sig.client_json;
      return ds;
   }

   /// @endcond

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
